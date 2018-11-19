#include "CacheSet.h"
#include "CacheStuff.h"
#include <iostream>
#include <string>
/*
  * Constuctor
  * Set the sets index within the cache, replacement policy (random or LRU),
    and associativity level
  * Note that an associativity level of 1 indicates a Direct-Mapped cache.
    In this situation replacement policy has no meaning.
*/
CacheSet::CacheSet(unsigned int idx, ConfigInfo ci) {
  this->index = idx;
  this->associativity = ci.associativity;
  this->rp = ci.rp;
  this->wp = ci.wp;

  for(int i = 0; i < (int)this->associativity; i++) {
    this->blocks.push_back( new CacheBlock() );
  }
  this->lru = (this->rp == ReplacementPolicy::LRU ? new LRUList() : NULL);
}

/*
  * Prints the status of the LRU if valid, otherwise NULL
*/
void CacheSet::printLRU() {
  if (this->lru != NULL) {
    this->lru->print();
  }
}

 /*
  * Returns the address of block with the matching Tag
  * NULL if tag not in set
*/
CacheBlock* CacheSet::getBlockWithTag(unsigned int tag) {
  for(size_t i = 0; i < this->blocks.size(); i++) {
    if (this->blocks[i]->getTag() == tag){
      return this->blocks[i];
    }
  }
  return NULL;
}

/*
  * Simulates writing the block back to memory
*/
void CacheSet::writeToMemory(CacheResponse *response, unsigned int tag) {
  std::string typeOfWrite = (response->dirtyEviction ? "dirty block" : "block");
  std::cout << "Writing " << typeOfWrite << " (" << tag << ") back to memory" << std::endl;
}
/*
  * Only used when the set is full
  * Block is written back to memory if write-back mode and
    dirty bit is set
  * Removes the block from the LRU and unsets
    the blocks valid bit
*/
void CacheSet::removeBlock(CacheResponse *response) {
  CacheBlock *evictMe = (this->rp == ReplacementPolicy::LRU ? this->lru->getEvictedBlock() : this->blocks[0]);
  if (evictMe == NULL){ //error. May be due to func being called when set is not full
    std::cerr << "Error in getEvictedBlock" << std::endl;
    exit(1);
  }
  /* eviction took place */
  response->eviction = true;
  /* default dirtyEviction to false, will update if true */
  response->dirtyEviction = false;
  /* if write-back mode and block is dirty */
  if (this->wp == WritePolicy::WriteBack && evictMe->getDirty() == 1) {
    response->dirtyEviction = true;  //evicted block written back to memory
    this->writeToMemory(response, evictMe->getTag());
  }
  /* invalidate the block */
  evictMe->unsetValid();
}

/*
  * Cycles through the blocks in the set and returns the address
    to one that is invalid. Meaning it is free to use
*/
CacheBlock* CacheSet::getAvailableBlock() {
  for (size_t i = 0; i < this->blocks.size(); i++) {
    /* if the block is not valid, meaning free to use */
    if (this->blocks[i]->getValid() == 0)
      return this->blocks[i];  //return address of available block
  }
  return NULL;
}

/*
  * Load a block from memory into the cache
  * This is done when a block needed is not in the cache
*/
void CacheSet::loadBlockIntoCache(CacheResponse *response, unsigned int tag) {
  CacheBlock *available = NULL;
  /* default eviction to false, will update if true later */
  response->eviction = false;
  /* check if available spots in set */
  do {
    available = this->getAvailableBlock();
    if (available == NULL) {
      /* no room in set, need to evict one */
      this->removeBlock(response);
    }
  } while(available == NULL);
  /* load block into available slot */

  available->setTag(tag);
  available->setValid();
}

/*
  * Store data.
  * Data needs to be written from the CPU into the cache or
    main memory (possibly both) depending on the write policy.
  * The block to be written to is determined by the tag passed in
  * Update LRU
*/
void CacheSet::storeBlockFromCPU(CacheResponse *response, unsigned int tag) {
  CacheBlock *block = getBlockWithTag(tag);
  /* default hit to true, will update to false if it is not */
  response->hit = true;  //tag in cache, hit
  /* if block is NULL it is not in the cache, we need to load
     it then store the data */
  if (block == NULL) {
    response->hit = false;  //tag not in cache, hit = false = miss
    this->loadBlockIntoCache(response, tag);  //load the block into the cache
    block = getBlockWithTag(tag);  //get a pointer to the newly loaded block
  }
  /* write-through mode, write to memory */
  if (this->wp == WritePolicy::WriteThrough) {
       this->writeToMemory(response, block->getTag());
  } else {
    /* write-back mode, set dirty bit */
    block->setDirty();
  }
  /* let the lru know we used this block */
  this->lru->addInteraction(block);
}

/*
  * Load data.
  * Data is loaded from the cache into the CPU
  * The block to be loaded is determined by the tag passed in
  * Update LRU
*/
void CacheSet::loadBlockIntoCPU(CacheResponse *response, unsigned int tag) {
  CacheBlock *block = getBlockWithTag(tag);
  /* default hit to true, will update to false if it is not */
  response->hit = true;  //tag in cache, hit
  /* if block is NULL it is not in the cache, we need to get it*/
  if (block == NULL) {
    response->hit = false;  //tag not in cache, hit = false = miss
    this->loadBlockIntoCache(response, tag);
    block = getBlockWithTag(tag);
  }
  /* let the lru know we used this block */
  this->lru->addInteraction(block);
}
