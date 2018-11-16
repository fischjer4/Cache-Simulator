
#include "LRUList.h"

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
  while (tmp != NULL && tmp->next != NULL) {
    tmp = tmp->next;
  }
  /*tmp is null. This should never happen */
  if (tmp == NULL) return NULL;

  CacheBlock *result = tmp->next->block;
  this->map.erase(result);
  delete tmp->next;
  tmp->next = NULL;

  return result;
}


void LRUList::hookUp(Node *back, Node *ahead) {
  if (back == NULL)
    return;

  Node *tmp = back;
  /* hook up back */
  back->next = ahead;
  if (ahead != NULL) {
    back->prev = ahead->prev;
    /* hook up ahead */
    ahead->prev = back;
  }
  /* hook up aheads old prev */
  if (back->prev)
    back->prev->next = back;

  /* reconnect link where back used to be */
  if (tmp != NULL && tmp->next != NULL)
    tmp->next->prev = tmp->prev;
  if (tmp != NULL && tmp->prev != NULL)
    tmp->prev->next = tmp->next;
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
    this->hookUp(newNode, head); //move newNode to front of list
  }
  /* Case 3: Block is in list */
  else {
    this->hookUp(foundBlock->second, head);
  }
}
