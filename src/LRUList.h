#ifndef _LRULIST_H_
#define _LRULIST_H_

#include <unordered_map>
#include "CacheBlock.h"

struct Node {
  CacheBlock *block;
  Node *next;
  Node *prev;
  Node() : block(NULL), next(NULL), prev(NULL) {}
};

class LRUList {
  private:
    Node *head;
    std::unordered_map<CacheBlock*, Node*> map; //map block address to their spot in linked list
  public:
    void print();  //used to print LRU
    void hookUp(Node *, Node *);
    CacheBlock* getEvictedBlock();
    void addInteraction(CacheBlock *);
    LRUList();
};

#endif
