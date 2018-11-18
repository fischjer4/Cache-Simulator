
#include "LRUList.h"

#include <iostream>
using namespace std;

/*
  * LRUList constructor
*/
LRUList::LRUList() {
  this->head = NULL;
}

/*
  * This function should ONLY be called when all blocks in the set are full
  * Returns the address of the block that is least recently used
    (i.e the block to replace). The tail of the list
*/
CacheBlock* LRUList::getEvictedBlock() {
  Node *tmp = this->head;

  /* move tmp to the end of the list */
  while (tmp != NULL && tmp->next != NULL) {
    tmp = tmp->next;
  }
  /*tmp is null. This should never happen */
  if (tmp == NULL) return NULL;

  CacheBlock *result;
  result = tmp->block;
  if (tmp->prev) {
    tmp->prev->next = NULL;
  }
  delete tmp; tmp = NULL;
  this->map.erase(result);

  return result;
}


void LRUList::hookUp(Node *back, Node *ahead) {
  if (back == NULL)
    return;

  /* hook up back->prev up to back->next since back is leaving */
  if (back->prev) {
    back->prev->next = back->next;
    if (back->next) {
      back->next->prev = back->prev;
    }
  }

  /* hook up back */
  back->next = ahead;
  if (ahead != NULL) {
    back->prev = ahead->prev;
    /* hook up ahead */
    ahead->prev = back;
  }
}

/*
  * Given the address of the block that was just interacted with,
    move it to the head of the list.
*/
void LRUList::addInteraction(CacheBlock *block) {
  auto foundBlock = this->map.find(block);
  /* case 1: Head is NULL. First init of list */
  if (this->head == NULL) {
    this->head = new Node();
    this->head->block = block;
    map[block] = this->head;
  }

  /* Case 2: Not all blocks in list yet and this block
     isnt in the list yet, add it to the front */
  else if (foundBlock == this->map.end()) {
    Node *newNode = new Node();
    newNode->block = block;
    map[block] = newNode;
    this->hookUp(newNode, this->head); //move newNode to front of list
    this->head = newNode;
  }
  /* Case 3: Block is in list */
  else {
    if (foundBlock->second != this->head) {
      this->hookUp(foundBlock->second, this->head);
      this->head = foundBlock->second;
    }
  }
}
