#include "skip_list.h"
#include <string.h>


Node* HelpDelete(Node* prev, uint32_t level) {

    return NULL;
}

int CompareKey(KEY key1, KEY key2)
{
    uint32_t k1 = (uint32_t)key1;
    uint32_t k2 = (uint32_t)key2;
    if (k1 < k2) {
        return -1;
    }
    if (k1 == k2) {
        return 0;
    }
    return 1;
}

// consider if *prev is marked del
int32_t ReadNext(Node** prev, Node** next, uint32_t level)
{
// consider later
//    if (IS_MARK_DEL(*prev)) {
//        *prev = HelpDelete(*prev, level);
//    } 
    VLink nRef = atomic_load_explicit(&(*prev)->next[level], memory_order_seq_cst); 
    if (nRef.ptr == (uintptr_t)NULL)
    {
        *next = NULL; 
        return nRef.isDel ? ERROR_DELETING : OK;
    }
    *next = COPY_NODE((Node*)nRef.ptr);  
    return OK;
}

// **start [IN, OUT] must be copyed already
// **find  [OUT]     must be copyed before return
int32_t ScanKey(KEY key, uint32_t level, Node** start, Node** found)
{
    int32_t ret = OK;
    Node* node1 = *start; 
    Node* node2 = NULL;
    while (true) {
        ret = ReadNext(&node1, &node2, level);
        if (node2 == NULL) {
            // to the end 
            // meet deleting node
            break;   
        } 
        if (CompareKey(key, node2->key) < 0) {
            RELEASE_NODE(node1);
            node1 = node2;
        } else {
            // found
            break;    
        }
    }
    *found = node2;
    return ret;
}

int32_t FindByKey(SkipList* skipList, KEY key, Node** foundPrev, Node** foundNode)
{
    int32_t ret = OK;
    Node* node1 = COPY_NODE(skipList->head);
    Node* node2 = NULL;
    uint8_t maxLevel = GetMaxLevel(skipList) - 1;
    uint8_t level = maxLevel;
    while (true) {
        ret = ScanKey(key, level, &node1, &node2);
        if (ret == ERROR_DELETING) {
            if (level < maxLevel) {      
            // back 2 upper lvl and retry
                ++level;
                node1 = foundPrev[level];
                continue;
            } else {
            // 2 the top already, return fail
                break;
            }
        } 
        if (foundPrev != NULL) {
            foundPrev[level] = node1;
        }
        if (level == 0) {
            *foundNode = node2;
            break; 
        } else {
            level--;
        }
        if (node2 != NULL) {
            RELEASE_NODE(node2);
        }
    }
    return ret;
}

Node* CreateNode(uint32_t level, KEY key, void* value)
{
    uint32_t nodeSize = sizeof(Node) + sizeof(VLink) * level;
    Node* node = (Node*)malloc(nodeSize);
    if (node == NULL) {
        return NULL;
    }
    memcpy((void*)&node->key, (void*)&key, sizeof(KEY));
    {
        VLink pValue = {0};
        pValue.isDel = false;
        pValue.ptr = (uintptr_t)value;
        atomic_store_explicit(&node->value, pValue, memory_order_release);
    }
    atomic_store_explicit(&node->refCnt, 1, memory_order_release);
    return node;
}

int32_t Delete(SkipList* skipList, KEY key)
{
    // find key node

    // set mark as del

    // unlink level 0

    // unlink level-top 2 level-1

    return OK;
}

int32_t Insert(SkipList* skipList, KEY key, VALUE value)
{
    int32_t ret = OK;
    uint8_t i = 0;
    uint8_t level = RandomLenvel(skipList);
    uint8_t maxHeight = GetMaxLevel(skipList);
    Node* prev[SKIP_LIST_MAX_HEIGHT] = {NULL};
    Node* node1 = NULL;
    Node* node2 = NULL;
    Node* nodeNew = NULL;
    
    ret = FindByKey(skipList, key, prev, &node2);
    if (ret != OK) {
        // TODO handle error here
    }
    RELEASE_NODE(node2);
    // set level 0
    node1 = prev[0];
    while (true) {
        ret = ScanKey(key, 0, &node1, &node2);
        if (ret == ERROR_DELETING) {
            // TODO handle error here
        }
        if (!IS_MARK_DEL(node2) && (node2->key == key)) {
            // swap value
        }
        BarrierSetNext(nodeNew, 0, node2);
        VLink linkOld = GET_VLINK(node1,0); // todo
        VLink linkNew = {false, nodeNew};
        if (atomic_compare_exchange_strong(&node1->next[0], &linkOld, linkNew)) {
            break; 
        }
        // return fail
        // retry
    }
    // set upper levels
    uint8_t safeLevel = 1;
    for (i = 1; i < level - 1; ++i) {
        node1 = prev[i];
        while (true) {
            ret = ScanKey(key, i, &node1, &node2);
            if (ret == ERROR_DELETING) {
                // retry with up level key
                safeLevel = i + 1;
                if (safeLevel >= level - 1) {
                    // error  
                }
                node1 = prev[safeLevel]; 
                continue;
            }
            SET_VLINK(nodeNew, i, false, node2);
            VLink linkOld = GET_VLINK(node1, i);
            VLink linkNew = {false, nodeNew};
            if (atomic_compare_exchange_strong(&node1->next[i], &linkOld, linkNew)) {
                atomic_store(&nodeNew->validLevel, i);
                break;
            } 
        }
    }
    return 0; 
}

SkipList* CreateSkipList()
{
    int32_t result = ERROR;
    SkipList* sl = NULL;
    Node* head = NULL;
    do {
        sl = malloc(sizeof(SkipList));
        if (sl == NULL) {
            break;
        }
        atomic_store_explicit(&sl->length, 1, memory_order_relaxed);
        atomic_store_explicit(&sl->maxHeight, 1, memory_order_relaxed);
        atomic_store_explicit(&sl->randomSeed, 1, memory_order_relaxed);
        head = CreateNode(SKIP_LIST_MAX_HEIGHT, 0, NULL);
        if (head == NULL) { 
            break;
        }
        sl->head = head;
        for (uint32_t i = 0; i < SKIP_LIST_MAX_HEIGHT; ++i) {
            SET_VLINK(head, i, false, NULL); 
        }
        result = OK;
    } while (0);
    if (result != OK && sl != NULL) {
        free(sl); 
    }
    return sl;
}



int main() 
{
    SkipList* sl = CreateSkipList();

     
    return 0;
}
