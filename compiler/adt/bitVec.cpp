#include "chpl.h"
#include "bitVec.h"

#define TYPE unsigned

BitVec::BitVec(int in_size) {
  if (in_size == 0) {
    ndata = 0;
    this->in_size = 0;
    data = NULL;
  } else {
    ndata = 1 + (in_size-1) / (sizeof(TYPE)<<3);
    this->in_size = in_size;
    data = (TYPE*)calloc(ndata, sizeof(TYPE));
  }
}


BitVec::~BitVec() {
  free(data);
}


void BitVec::clear() {
  for (int i = 0; i < ndata; i++)
    data[i] = 0;
}


bool BitVec::get(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j*(sizeof(TYPE)<<3);
  return data[j] & (1 << k);
}


void BitVec::unset(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j*(sizeof(TYPE)<<3);
  data[j] &= ((TYPE)-1) - (1 << k);
}


void BitVec::disjunction(BitVec& other) {
  for (int i = 0; i < ndata; i++)
    data[i] |= other.data[i];
}


void BitVec::intersection(BitVec& other) {
  for (int i = 0; i < ndata; i++)
    data[i] &= other.data[i];
}




/*
 * Added functionality to make this compatible with std::bitset and thus 
 * boosts dynamic bitset if that gets into the STL 
 * I would also like to implement operator overloading and a copy constructor 
 * if nobody is against that
 */ 


bool BitVec::equals(BitVec& other) {
  for(int i = 0; i < ndata; i++) {
    if(data[i] != other.data[i]) {
      return false;
    }
  }
  return true;
}


void BitVec::set() {
  for (int i = 0; i < ndata; i++)
    data[i] = ~0;
}


void BitVec::set(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j*(sizeof(TYPE)<<3);
  data[j] |= 1 << k;
}


void BitVec::reset() {
  for (int i = 0; i < ndata; i++)
    data[i] = 0;
}
      
        
void BitVec::reset(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j*(sizeof(TYPE)<<3);
  data[j] &= ((int)-1) - (1 << k);
}


void BitVec::flip() {
  for (int i = 0; i < ndata; i++)
    data[i] = ~data[i];
}


void BitVec::flip(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j*(sizeof(TYPE)<<3);
  data[j] ^= 1 << k;
}


/*
 * The current use of this function expect there to be almost no 1's
 * in which case this algorithm is the most efficient instead of an
 * implementation that uses divide and conquer shifting and counting with
 * hex manipulation (I have no idea what the name of that bit counting 
 * algorithm is called)
 */
int BitVec::count() {
  int count = 0;
  for (int i = 0; i < ndata; i++) {
    int localCount ;
    int x = data[i]; 
    for (localCount=0; x; localCount++) {
      x &= x-1;
    }
    count += localCount;
  }
  return count;
}


int BitVec::size() {
  return (int)in_size; 
}


bool BitVec::test(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j*(sizeof(TYPE)<<3);
  return data[j] & (1 << k);
}


bool BitVec::any() {
  for (int i = 0; i < ndata; i++) {
    if(data[i] > 0) {
      return true;
    }
  }
  return false;
}


bool BitVec::none() {
  return !any();
}




