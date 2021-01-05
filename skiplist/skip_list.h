#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define VMEM_ALLOC(size, type) malloc(size)
#define VMEM_FREE(size, type)  free(size)

/*  LINK define
*   -----------------------------------
*   reserved |delFlag| ptr            |
*   15       |1      | 48             |
*   -----------------------------------
*/

typedef uint32_t Key;
typedef uint64_t LINK;
#define IS_DELETE_BIT (1 << 48)
#define SET_LINK_PTR(LINK, intPtr)  (LINK & (intPtr & 0xFFFFFFFFFFFF)) 
#define GET_LINK_PTR(LINK)  (LINK & 0xFFFFFFFFFFFF) 
#define SET_LINK_DEL(LINK) (LINK | IS_DELETE_BIT)
#define GET_LINK_DEL(LINK) (LINK & IS_DELETE_BIT)

typedef struct SNode {
    Key key;  
    uint8_t level;
    LINK next[];
} SNode;

#define PAYLOAD_OFFSET(level) (sizeof(SNode) + (sizeof(LINK) * level))
#define PAYLOAD_SIZE(level, payloadSize) (((payloadSize + sizeof(LINK) - 1) / sizeof(LINK)) * sizeof(LINK))
#define SNODE_SIZE(level, payloadSize)  (PAYLOAD_OFFSET(level) + PAYLOAD_SIZE(level, payloadSize)) 

typedef struct SkipList {
    SNode* head;
    uint8_t maxHeight;
} SkipList;


