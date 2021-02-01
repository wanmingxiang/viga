#include <stdint.h>
#include <stdbool.h>

#define MY_ASSERT(expr) do {  } while(0)

typedef void* KeyIndexMap;

typedef struct {
    char    buf[24];
    uint8_t len;
} Key;

typedef union {
    uint32_t raw;
    struct {
        uint32_t hashValue : 27;
        uint32_t conflict : 2;
        uint32_t inUse : 1;
    };
} KeyIndex;

typedef struct {
    KeyIndex  key;
    Key       value;
} KvPair;

typedef struct {
    uint8_t  hashValueBits;
    uint8_t  segmentBits;
    uint8_t  bucketBits;
    uint8_t  slotBits;
    uint32_t hashValueMask;
    uint32_t segmentMask;
    uint32_t bucketMask;
    uint32_t slotMask;
    uint32_t segmentCnt;
    uint32_t bucketCnt;
    uint32_t slotCnt;
} HashTableConf;

typedef struct {
    uint64_t lock : 1;
    uint64_t kvCnt : 5; 
    uint64_t kvBitMap : 16;
    KvPair kvArr[0];
} NormalBucket;

typedef struct OverFlowBucket {
    NormalBucket bsc;
    struct OverFlowBucket* next;
} OverFlowBucket;

typedef struct {
    uint64_t lock : 1;
    uint64_t bucketCnt: 63;
    NormalBucket*   array;
    OverFlowBucket* first;
} Segment;

typedef enum {
    UNINIT,
    NORMAL,
    REHASH,
    PURGE
} HT_STATUS;

typedef enum {
    CREATE,
    UPGRADE,
    DEGRADE,
} HT_ACTION;

typedef uint32_t (*HASH_FUNC)(char* buf, uint32_t len);

typedef struct {
    int32_t       ref;
    bool          active;
    HashTableConf conf;
    HASH_FUNC     hashFunc;
    Segment       directory[];
} HashTable;

typedef struct {
    HashTable*  ht[2];      // two hash table
    uint32_t    cur;
    uint32_t    status;
} KeyIndexMapImpl; 



