#include "chain.h"
#include "chain_given.cpp"

// PA1 functions

//length = 0 corresonds to a chain with just a sentinel

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain(){
  //We need to delete each node;
  
  clear();
  delete head_;
  //delete this; //not sure abouot this line either
  //maybe we need to delete more stuff too?? the chain itself??

}

/**
 * Inserts a new node at the end of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block & ndata){
  Node* insertNode = new Node(ndata);
  Node* oldLast = head_->prev;
  head_->prev = insertNode;
  insertNode->next = head_;
  insertNode->prev = oldLast;
  oldLast->next = insertNode;
  length_ ++;
}

/**
 * Modifies the Chain by moving the subchain of len Nodes,
 * starting at position startPos, dist positions toward the
 * end of the chain.  If startPos + len - 1 + dist > length
 * then dist = length - startPos - len + 1 (to prevent gaps
 * in the new chain).
 * The subchain occupies positions (startPos + dist) through
 * (startPos + dist + len - 1) of the resulting chain.
 * The order of subchain nodes is not changed in the move.
 * You may assume that: 1 <= startPos <= length - len + 1,
 * 0 <= dist <= length, and 0 <= len <= length. 
 */
  //BP basically just moves chunk of len nodes back in the chain by dist
void Chain::moveBack(int startPos, int len, int dist){
  //should use walk fxn
  if(dist == 0) return;
  Node* start = walk(head_, startPos); //first node in the subchain;
  Node* prevBound = start->prev; //the left side of the gap we produce
  Node* end = walk(start, len-1); //last node in subchain
  Node* nextBound = end->next; //the right side of the gap we produce

  if(startPos + len - 1 + dist > length_){
    //if subchain would be moved around end of list then change dist to just move it to end
    dist = length_ - startPos - len + 1;
  }

  //take care of moving the chunk
  start->prev = walk(start, len+dist-1);
  end->next = start->prev->next;
  start->prev->next = start;
  end->next->prev = end;

  //now need to take care of holes we left
  prevBound->next = nextBound;
  nextBound->prev = prevBound;
}

/**
 * Rolls the current Chain by k nodes: reorders the current list
 * so that the first k nodes are the (n-k+1)th, (n-k+2)th, ... , nth
 * nodes of the original list followed by the 1st, 2nd, ..., (n-k)th
 * nodes of the original list where n is the length.
 */
void Chain::roll(int k){
  if(k == length_) return;
  //head should still be first
  //moves k nodes from end of chain to start
  Node * startRoll = walk(head_, k - 1);
  Node * newLast = startRoll->prev;
  Node * endRoll = head_->prev; //we don't need to define nextBound cuz is always head_
  Node * oldFirst = head_->next;

  startRoll->prev = head_;
  head_->next = startRoll;
  endRoll->next = oldFirst;
  oldFirst->prev = endRoll;
  newLast->next = head_;
  head_->prev = newLast;
}

/**
 * Modifies the current chain by reversing the sequence
 * of nodes from pos1 up to and including pos2. You may
 * assume that pos1 and pos2 exist in the given chain,
 * and pos1 <= pos2.
 * The positions are 1-based.
 */
void Chain::reverseSub(int pos1, int pos2){ //this works
  Node * start = walk(head_, pos1);
  Node * end = walk(head_, pos2);
  Node * beforeSwap = walk(head_, pos1-1);
  Node * afterSwap = walk(head_, pos2+1);

  Node * curr = start->next;
  Node * temp;
  while(curr != end){
    temp = curr->next;
    curr->next = curr->prev;
    curr->prev = temp;
    curr = temp;
  }
  start->prev = start->next;
  start->next = afterSwap;

  end->next = end->prev;
  end->prev = beforeSwap;

  beforeSwap->next = end;
  afterSwap->prev = start;
}

/*
* Modifies both the current chain and the "other" chain by removing
* nodes from the other chain and adding them between the nodes
* of the current chain -- one "other" node between two current nodes --
* until one of the two chains is exhausted.  Then the rest of the
* nodes in the non-empty chain follow.
* The length of the resulting chain should be the sum of the lengths
* of current and other. The other chain should have only
* the head_ sentinel at the end of the operation.
* The weave fails and the original
* chains should be unchanged if block sizes are different.
* In that case, the result of the function should be:
* cout << "Block sizes differ." << endl;
*/
void Chain::weave(Chain & other) { // leaves other empty.
  //checking block width
  if(width_ != other.width_ || height_ != other.height_){
    cout << "Block sizes differ." << endl;
    return;
  }

  //if other chain is empty, return original chain
  if(other.length_ == 0) return;
  //if current chain is empty, connect other to head
  if(length_ == 0){
    head_->prev = other.head_->prev;
    head_->next = other.head_->next;
    other.head_->next = other.head_;
    other.head_->prev = other.head_;
    length_ = other.length_;
    other.length_ = 0;
    return;
  }

  Node * curr1 = head_->next->next;
  Node * curr2 = other.head_->next;
  bool first = true;
  while(curr1 != head_ && curr2 != other.head_){
    if(first){
      curr1->prev->next = curr2;
      curr2->prev = curr1->prev;
      curr2 = curr2->next;
    }else{
      curr2->prev->next = curr1;
      curr1->prev = curr2->prev;
      curr1 = curr1->next;
    }
    first = !first;
  }

  if(curr1 == head_){
    curr1->prev->next = curr2;
    curr2->prev = curr1->prev;

    curr2 = other.head_->prev;

    head_->prev = curr2;
    curr2 ->next = head_;
  }

  if(curr2 == other.head_){
    curr2->prev->next = curr1;
    curr1->prev = curr2->prev;
  }

  other.head_->next = other.head_;
  other.head_->prev = other.head_;

  length_ += other.length_;
  other.length_ = 0;
}


/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class except for the sentinel head_. Sets length_
 * to zero.  After clear() the chain represents an empty chain.
 */
void Chain::clear() {
  while(empty() == false){
    Node* curr = head_->prev;
    curr->prev->next = head_;
    head_->prev = curr->prev;
    delete curr;
    length_ --;
  }
}


/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const& other) {
  //make current chain (this) into a copy of other
  cout << "in copy" << endl;

  clear(); //clears this chain
  height_ = other.height_;
  width_ = other.width_;
 
  for(int i = 1; i <= other.length_; i++){
    //cout << i << endl;
    insertBack(walk(other.head_, i)->data);
  }
  //cout << other.length_ << endl;
}

