#include "CacheBlock.h"

/*
  * CacheBlock Constuctor
*/
CacheBlock::CacheBlock() {
  this->tag = -1;
  this->dirty = 0;
  this->valid = 0;
}

/*
  * Sets the tag
*/
void CacheBlock::setTag(unsigned int tag) {
  this->tag = tag;
}
/*
  * Get tag
*/
unsigned int CacheBlock::getTag() {
  return this->tag;
} 


/*
  * Set dirty bit
*/
void CacheBlock::setDirty() {
  this->dirty = 1;
}
/*
  * Unset dirty bit
*/
void CacheBlock::unsetDirty() {
  this->dirty = 0;
}
/*
  * Get dirty bit
*/
unsigned int CacheBlock::getDirty() {
  return this->dirty;
}


/*
  * Set valid bit
*/
void CacheBlock::setValid() {
  this->valid = 1;
}
/*
  * Unset valid bit
*/
void CacheBlock::unsetValid() {
  this->valid = 0;
}
/*
  * Get valid bit
*/
unsigned int CacheBlock::getValid() {
  return this->valid;
}
