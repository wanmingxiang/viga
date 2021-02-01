#include <stdatomic.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define SKIP_LIST_MAX_HEIGHT (12)

typedef enum {
    ERROR           = -1,
    OK              = 0,
    ERROR_DELETING,
} ERR_CODE;

typedef uint32_t KEY;

typedef void* VALUE;

typedef union {
    uint64_t raw;
    struct {
        uint64_t isDel: 1;
        uint64_t ptr: 48;
    };
} VLink;

typedef struct tagNode {
    _Atomic uint32_t            refCnt;         // var
    _Atomic uint8_t             level;          // fixed
    _Atomic uint8_t             validLevel;     // var
    KEY                         key;            // fixed
    _Atomic VLink               value;          // var
    struct tagNode*             prev;           // var
    _Atomic VLink               next[];         // var
} Node;
    
typedef struct tagSkipList {
    Node* head;
    _Atomic uint32_t randomSeed;
    _Atomic uint32_t maxHeight;
    _Atomic uint64_t length;
} SkipList;

static inline void SET_VLINK(Node* node, uint8_t lvl, bool isDel, Node* ptr)
{
    VLink tmp = {0};
    tmp.isDel = isDel;
    tmp.ptr = (uintptr_t) ptr;
    atomic_store_explicit(&node->next[lvl], tmp, memory_order_seq_cst);
}

static inline VLink GET_VLINK(Node* node, uint8_t lvl) 
{
    return atomic_load_explicit(&node->next[lvl], memory_order_seq_cst);
}

static inline bool IS_MARK_DEL(Node* node)
{
    VLink vRef;
    vRef= atomic_load_explicit(&node->value, memory_order_seq_cst);  
    return (vRef.isDel == 0);
}

static inline void RELEASE_NODE(Node* node)
{
    uint32_t old = atomic_fetch_sub_explicit(&node->refCnt, 1, memory_order_seq_cst); 
    if (old == 1) {
        free(node);
    }
}

static inline Node* COPY_NODE(Node* node)
{
    while (true) {
        uint32_t ref = atomic_load_explicit(&node->refCnt, memory_order_seq_cst); 
        if (ref == 0) {
            return NULL;
        }
        if (atomic_compare_exchange_weak(&node->refCnt, &ref, ref + 1)) {
            break;
        }
    }
    return node;
}

static inline Node* READ_NODE(_Atomic VLink* link)
{
    VLink nRef = atomic_load_explicit(link, memory_order_seq_cst); 

    if (nRef.isDel && (nRef.ptr == (uintptr_t)NULL)) {
        return NULL;
    }
    return COPY_NODE((Node*)nRef.ptr);  
}

static inline uint32_t RandomLenvel(SkipList* skipList)
{
    // TODO
    return 3;
}

static inline uint8_t GetMaxLevel(SkipList* skipList)
{
   return atomic_load(&skipList->maxHeight); 
}

static inline void BarrierSetNext(Node* cur, uint8_t lvl, Node* next)
{
    SET_VLINK(cur, lvl, false, next);
}



