#include "CacheSet.h"
#include "CacheStuff.h"
#include <iostream>

/*
  * Constuctor
  * Set the sets index within the cache, replacement policy (random or LRU),
    and associativity level
  * Note that an associativity level of 1 indicates a Direct-Mapped cache.
    In this situation replacement policy has no meaning.
*/
CacheSet::CacheSet(unsigned int idx, unsigned int asoc, ReplacementPolicy rp,
                  WritePolicy wp) {
  this->index = idx;
  this->associativity = asoc;
  this->rp = rp;
  this->wp = wp;

  for(unsigned int i = 0; i < this->associativity; i++) {
    this->blocks.push_back( CacheBlock() );
  }

  this->lru = (rp == ReplacementPolicy::LRU ? new LRUList() : NULL);
}

/*
  * Returns the address of block with the matching Tag
  * NULL if tag not in set
*/
CacheBlock* CacheSet::getBlockWithTag(unsigned int tag) {
  for(size_t i = 0; i < this->blocks.size(); i++) {
    if (this->blocks[i].getTag() == tag)
      return &this->blocks[i];
  }
  return NULL;
}

/*
  * Simulates writing the block back to memory
*/
void CacheSet::writeToMemory() {
  std::cout << "Writing dirty block back to memory" << std::endl;
}

/*
  * Only used when the set is full
  * Block is written back to memory if write-back mode and
    dirty bit is set
  * Removes the block from the LRU and unsets
    the blocks valid bit
*/
void CacheSet::removeBlock() {
  CacheBlock *evictMe = this->lru->getEvictedBlock();
  if (evictMe == NULL){ //error
    std::cerr << "Error in getEvictedBlock" << std::endl;
    exit(1);
  }

  /* if write-back mode and block is dirty */
  if (this->wp == WritePolicy::WriteBack && evictMe->getDirty() == 1) {
    this->writeToMemory();
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
    if (this->blocks[i].getValid() == 0)
      return &this->blocks[i];
  }
  return NULL;
}

/*
  * Load a block from memory into the cache
  * This is done when a block needed is not in the cache
*/
void CacheSet::loadBlockIntoCache(unsigned int tag) {
  CacheBlock *available = NULL;
  /* check if available spots in set */
  do {
    available = this->getAvailableBlock();
    if (available == NULL) {
      /* no room in set, need to evict one */
      this->removeBlock();
    }
  } while(available == NULL);
  /* successfully loaded the block into the cache */
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
void CacheSet::storeBlockFromCPU(unsigned int tag) {
  CacheBlock *block = getBlockWithTag(tag);
  /* if block is NULL it is not in the cache, we need to load
     it then store the data. Sounds just like a modify */
  if (block == NULL) {
    this->modifyBlockFromCPU(tag);
  } else {
    /* write-through mode or write-back mode and block is dirty */
    if (this->wp == WritePolicy::WriteThrough ||
       (this->wp == WritePolicy::WriteBack && block->getDirty() == 1)) {
         this->writeToMemory();
    }
    /* let the lru know we used this block */
    this->lru->addInteraction(block);
  }
}

/*
  * Load data.
  * Data is loaded from the cache into the CPU
  * The block to be loaded is determined by the tag passed in
  * Update LRU
*/
void CacheSet::loadBlockIntoCPU(unsigned int tag) {
  CacheBlock *block = getBlockWithTag(tag);
  /* if block is NULL it is not in the cache, we need to get it*/
  if (block == NULL) {
    this->loadBlockIntoCache(tag);
  }
  /* let the lru know we used this block */
  this->lru->addInteraction(block);
}

/*
  * Modify data.
  * Implies a special case of a data load, followed
    immediately by a data store
  * The block to be loaded is determined by the tag passed in
  * Update LRU
*/
void CacheSet::modifyBlockFromCPU(unsigned int tag) {
  this->loadBlockIntoCPU(tag);
  this->storeBlockFromCPU(tag);
}
