
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MEMPOOL_MAX_PAGE_N (512)
#define MEMPOOL_MAX_CORE_N (256)
#define MEMPOOL_MAGIC_N    (73)
#define MEMPOOL_SLAB_N     (16)
#define MEMPOOL_PAGE_SIZE  (1 << 20)

typedef struct Element {
    struct Element* next;
} Element;

typedef struct NormalMemoryPool {
    uint32_t pageSize;  
    uint16_t elementSize; 
    uint32_t allocNum;
    uint32_t freeNum;
    uint16_t pageNum;
    Element* freeList;
    void*    pageVec[MEMPOOL_MAX_PAGE_N];
} NormalMemoryPool;

typedef struct MultiCoreNormalMemPool {
    uint32_t pageSize;
    uint32_t elementSize;
    NormalMemoryPool* arr[MEMPOOL_MAX_CORE_N];
} MultiCoreNormalMemPool;
 
typedef struct MultiMemoryPoolMgr {
    uint8_t bscSize;
    uint8_t slabId;
    uint8_t size2IdMap[256];            // map desc <size, slabId>
    MultiCoreNormalMemPool* slabs[MEMPOOL_SLAB_N];
} MultiMemoryPoolMgr;

int32_t NormalMemoryPoolInit(NormalMemoryPool* memPool, uint32_t elementSize, uint32_t pageSize) {
    if (elementSize == 0 || pageSize == 0 || elementSize > pageSize) {
        return -1;
    }
    memPool->elementSize = (elementSize + sizeof(Element) - 1) / sizeof(Element) * sizeof(Element);
    memPool->pageSize = pageSize;
    memPool->allocNum = 0;
    memPool->freeNum = 0;
    memPool->pageNum = 0;
    memPool->freeList = NULL;
    return 0;
}

void NormalMemoryPoolDestroy(NormalMemoryPool* memPool) {
    for (int i = 0; i < memPool->pageNum; ++i) {
        free(memPool->pageVec[i]);
    }
}

void* NormalMemoryPoolAlloc(NormalMemoryPool* memPool) {
    if (memPool->freeList) {
        void* ptr = memPool->freeList;
        memPool->freeList = memPool->freeList->next;
        ++memPool->allocNum;
        --memPool->freeNum;
        return ptr;
    }
    if (memPool->pageNum >= MEMPOOL_MAX_PAGE_N) {
        return NULL;
    }
    void* page = malloc(memPool->pageSize);
    if (page == NULL) {
        return NULL;
    }
    memPool->pageVec[memPool->pageNum] = page;
    ++memPool->pageNum;
    int num = memPool->pageSize / memPool->elementSize;
    for (int i = 0; i < num; ++i) {
        Element* ele = (Element*)(((char*)page) + memPool->elementSize * i);
        ele->next = memPool->freeList;
        memPool->freeList = ele;
    }
    memPool->freeNum += num;
    return NormalMemoryPoolAlloc(memPool);
}

void NormalMemoryPoolFree(void* ptr, NormalMemoryPool* memPool) {
    if (ptr == NULL) {
        return;
    }
    Element* ele = (Element*)ptr;
    ele->next = memPool->freeList;
    memPool->freeList = ele;
    --memPool->allocNum;
    ++memPool->freeNum;
}

int32_t MultiCoreNormalMemPoolInit(MultiCoreNormalMemPool* multiCorePool, uint32_t elementSize, uint32_t pageSize) {
    if (elementSize == 0 || pageSize == 0 || elementSize > pageSize) {
        return -1;
    }
    memset((void*)multiCorePool, 0, sizeof(MultiCoreNormalMemPool));
    multiCorePool->pageSize = pageSize;
    multiCorePool->elementSize = elementSize;
    return 0;
}

void MultiCoreNormalMemPoolDestroy(MultiCoreNormalMemPool* multiCorePool) {
    for (int i = 0; i < MEMPOOL_MAX_CORE_N; ++i) {
        if (multiCorePool->arr[i] != NULL) {
            NormalMemoryPoolDestroy(multiCorePool->arr[i]);
        }
    }
}

void* MultiCoreNormalMemPoolAlloc(MultiCoreNormalMemPool* multiCorePool, uint16_t coreId) {
    if (multiCorePool->arr[coreId] != NULL) {
        return NormalMemoryPoolAlloc(multiCorePool->arr[coreId]);
    }
    multiCorePool->arr[coreId] = malloc(sizeof(NormalMemoryPool));
    if (multiCorePool->arr[coreId] == NULL) {
        return NULL;
    }
    return NormalMemoryPoolAlloc(multiCorePool->arr[coreId]);
}

void MultiCoreNormalMemPoolFree(void* ptr, MultiCoreNormalMemPool* multiCorePool, uint16_t coreId) {
    if (ptr == NULL || multiCorePool == NULL) {
        return;
    }
    NormalMemoryPoolFree(ptr, multiCorePool->arr[coreId]);
}

int32_t MultiMemoryPoolMgrInit(MultiMemoryPoolMgr* mgr) {
    memset(mgr, 0, sizeof(MultiMemoryPoolMgr));
    mgr->bscSize = sizeof(uint64_t);
    return 0;
}

void MultiMemoryPoolMgrDestroy(MultiMemoryPoolMgr* mgr) {

}

int32_t MultiMemoryPoolMgrRegister(MultiMemoryPoolMgr* mgr, uint32_t size) {
    uint8_t idx = (size + mgr->bscSize - 1) / mgr->bscSize;
    mgr->size2IdMap[idx] = mgr->slabId;
    mgr->slabs[mgr->slabId] = malloc(sizeof(MultiCoreNormalMemPool));
    return MultiCoreNormalMemPoolInit(mgr->slabs[mgr->slabId], size, MEMPOOL_PAGE_SIZE);   
}

void* MultiMemoryPoolMgrAlloc(MultiMemoryPoolMgr* mgr, int size) {
    uint16_t coreId = 0;
    uint8_t idx = (size + mgr->bscSize - 1) / mgr->bscSize;
    uint32_t slabId = mgr->size2IdMap[idx];
    return MultiCoreNormalMemPoolAlloc(mgr->slabs[slabId], coreId);
}

void MultiMemoryPoolMgrFree(void* ptr, MultiMemoryPoolMgr* mgr, int size) {
    uint16_t coreId = 0;
    uint8_t idx = (size + mgr->bscSize - 1) / mgr->bscSize;
    uint32_t slabId = mgr->size2IdMap[idx];
    MultiCoreNormalMemPoolFree(ptr, mgr->slabs[slabId], coreId);
}

typedef struct TestItem{
    uint8_t  a;
    uint32_t b;
    uint64_t c;
    struct TestItem* next; 
} TestItem;

int main() {
    MultiMemoryPoolMgr mgr;
    MultiMemoryPoolMgrInit(&mgr);
    MultiMemoryPoolMgrRegister(&mgr, sizeof(TestItem));

    TestItem head;
    TestItem* cur = &head;
    uint32_t count = 10240;
    while (count > 0) {
        TestItem* item = MultiMemoryPoolMgrAlloc(&mgr, sizeof(TestItem));
        item->next = NULL;
        item->c = count;
        count--;
        cur->next = item;
        cur = item;
    }
    cur = &head;
    while (cur) {
        printf(" %llu \n", cur->c);
        cur = cur->next;
    }
    return 0;
}



