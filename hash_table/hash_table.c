#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//   -----------------------------------
//              hashValuebits
//   -----------------------------------
//   segbits|       |bucketbits|slotbis|
//   -----------------------------------
int32_t CreateHashTable(HashTableConf* conf, HashTable** ht)
{
    conf->hashValueMask = (1 << conf->segmentBits) - 1;
    conf->slotMask = (1 << conf->slotBits) - 1;
    conf->bucketMask = conf->slotMask ^ ((1 << (conf->slotBits + conf->bucketBits)) - 1);
    conf->segmentMask = conf->hashValueMask ^ ((1 << (conf->hashValueBits - conf->segmentBits)) - 1);
    conf->segmentCnt = 1 << conf->segmentBits;
    uint64_t htSize = sizeof(HashTable) + sizeof(Segment) * conf->segmentCnt;
    // TODO log
    printf("slot mask: %x  \n", conf->slotMask);
    printf("bkt mask:  %x  \n", conf->bucketMask);
    printf("seg mask:  %x  \n", conf->segmentMask);
    printf("ht size:   %uK \n", htSize >> 10);
    HashTable* hashTable = (HashTable*)malloc(htSize);
    if (hashTable == NULL)  {
        return -1;
    }
    memset(hashTable, 0, htSize);
    hashTable->conf = *conf;
    (void)__atomic_add_fetch(&hashTable->ref, 1, __ATOMIC_SEQ_CST);
    return 0;
}

void ReleaseHashTable(HashTable* hashTable)
{
    int32_t ref = __atomic_sub_fetch(&hashTable->ref, 1, __ATOMIC_SEQ_CST);
    if (ref == 0) {
        free(hashTable);
    }
}

HashTable* AcquireHashTable(HashTable* ht)
{
    (void)__atomic_add_fetch(&ht->ref, 1, __ATOMIC_SEQ_CST);
    bool isActive = __atomic_load_n(&ht->active, __ATOMIC_SEQ_CST);
    if (!isActive) {
       ReleaseHashTable(ht); 
       return NULL;
    } else {
        return ht;
    }
}

int32_t GenerateHashTableConf(const HashTableConf* old, HashTableConf* new, uint32_t action)
{
    int32_t ret = 0;
    switch (action) {  
        case CREATE: {
            new->hashValueBits = 27;
            new->segmentBits = 10;
            new->bucketBits = 4;
            new->slotBits = 4;
            break;
        }
        case UPGRADE: {
            *new = *old;
            new->segmentBits = old->segmentBits + 1; 
            break;
        }
        case DEGRADE:
        default:
            break;
    }
    return ret;
}

KeyIndexMap CreateKeyIndexMap()
{
    KeyIndexMapImpl* impl = malloc(sizeof(KeyIndexMapImpl)); 
    if (impl == NULL) {
        return NULL;
    }
    memset(impl, 0, sizeof(KeyIndexMapImpl));
    HashTableConf old;
    HashTableConf new;
    int ret = GenerateHashTableConf(&old, &new, CREATE);
    if (ret != 0) {
        free(impl);
        return NULL;
    }
    ret = CreateHashTable(&new, &impl->ht[0]) ;
    if (ret != 0) {
        free(impl);
        return NULL;
    }
    return impl; 
}

void SwitchHashTable(KeyIndexMapImpl* keyIndexMap)
{
    uint32_t curIdx = keyIndexMap->cur;
    uint32_t newIdx = (curIdx + 1) % 2;
    __atomic_store_n(&keyIndexMap->ht[newIdx]->active, true, __ATOMIC_RELEASE);
    __atomic_store_n(&keyIndexMap->ht[curIdx]->active, false, __ATOMIC_RELEASE);
    __atomic_store_n(&keyIndexMap->cur, newIdx, __ATOMIC_RELEASE);
}


int32_t rehash(KeyIndexMapImpl* keyIndexMap)
{
    uint32_t curIdx = keyIndexMap->cur;
    uint32_t newIdx = (curIdx + 1) % 2;
    MY_ASSERT(keyIndexMap->ht[newIdx] != NULL);
     
     

}

HashTable* GetCurrentHashTable(KeyIndexMapImpl* keyIndexMap)
{
    uint32_t cur = __atomic_load_n(&keyIndexMap->cur, __ATOMIC_ACQUIRE);
    HashTable* ht = AcquireHashTable(keyIndexMap->ht[cur]);
    while (true) {
        
    };
}

int32_t InsertKeyIndexMap(KeyIndexMap map, const Key* key, uint32_t* idx)
{
    int32_t ret = 0; 
    KeyIndexMapImpl* impl = (KeyIndexMapImpl*)map;
    HashTable* ht = NULL;
    KeyIndex keyIndex = {0};
    keyIndex.hashValue = ht->hashFunc(key->buf, key->len);
}


int main() {

    KeyIndexMap map = CreateKeyIndexMap();
    if (map == NULL) {
        
    }
    return 0;
}
