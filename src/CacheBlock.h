#ifndef _CACHEBLOCK_H_
#define _CACHEBLOCK_H_

class CacheBlock {
  private:
    unsigned int tag;    //tag that this block represents
    unsigned int dirty;  //set if needs to be written to cache
    unsigned int valid;  //set when filled with data
  public:
    CacheBlock();
    unsigned int getTag();
    unsigned int getDirty(); 
    unsigned int getValid();

    void setTag(unsigned int);
    void setDirty();
    void setValid();

    void unsetDirty();
    void unsetValid();
};

#endif
