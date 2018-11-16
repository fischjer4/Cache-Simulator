#ifndef _CACHESET_H_
#define _CACHESET_H_

#include <vector>

#include "CacheBlock.h"
#include "LRUList.h"
#include "CacheStuff.h"

class CacheSet {
  private:
    unsigned int index;          //sets index in cache
    unsigned int associativity;  //level of associativity
    ReplacementPolicy rp;        //replacement policy
    WritePolicy wp;              //write policy
    /*if rp is LRU, then LruList keeps linked list where head is Most recently used */
    LRUList *lru;
    std::vector<CacheBlock> blocks;
    void removeBlock();          //used when set is full
  public:
    CacheBlock* getAvailableBlock();
    CacheBlock* getBlockWithTag(unsigned int tag);
    void writeToMemory();
    void storeBlockFromCPU(unsigned int tag);
    void loadBlockIntoCPU(unsigned int tag);
    void modifyBlockFromCPU(unsigned int tag);
    void loadBlockIntoCache(unsigned int tag);  //loading block from memory into cache
    CacheSet(unsigned int idx, unsigned int asoc, ReplacementPolicy rp, WritePolicy wp);
};

#endif
