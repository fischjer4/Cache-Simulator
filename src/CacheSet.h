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
    unsigned int memoryAccessCycles;  //number of cycles required to W/R a block from main memory
    unsigned int cacheAccessCycles;  //number of cycles required to W/R a block from the cache
    /*if rp is LRU, then LruList keeps linked list where head is Most recently used */
    LRUList *lru;
    std::vector<CacheBlock*> blocks;
    void removeBlock(CacheResponse *response);          //used when set is full
  public:
    void printLRU();
    CacheBlock* getAvailableBlock();
    CacheBlock* getBlockWithTag(unsigned int tag);
    void writeToMemory(CacheResponse *response, unsigned int tag);
    void storeBlockFromCPU(CacheResponse *resp, unsigned int tag);
    void loadBlockIntoCPU(CacheResponse *resp, unsigned int tag);
    void loadBlockIntoCache(CacheResponse *resp, unsigned int tag);  //loading block from memory into cache
    CacheSet(unsigned int idx, ConfigInfo ci);
};

#endif
