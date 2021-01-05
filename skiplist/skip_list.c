#include "skip_list.h"

typedef struct SLIterator {
    SkipList* list;
    LINK curNode;
} SLIterator;

int32_t SLIterInit(SkipList* list, SLIterator* iter)
{

}

int32_t SLIterSeek(SLIterator* iter, Key key)
{

}

int32_t SLIterNext(SLIterator* iter)
{

}

int32_t SLIterValid(SLIterator* iter)
{

}

Key SLIterKey(SLIterator* iter)
{

}


int32_t Put(SkipList* skipList, Key key, void* payload, uint32_t payloadLen)
{
    

    return 0;
}

SNode* ReadNext(SkipList* skipList, SNode* prev, uint32_t level)
{

    return NULL;
}

void SearchKey(SkipList* skipList, Key key, LINK* prev)
{


{

void Remove(SkipList* skipList, Key key)
{


{


