#include "chpl.h"
#include "bitVec.h"


#define TYPE unsigned


BitVec::BitVec(int size) {
  if (size == 0) {
    ndata = 0;
    this->size = 0;
    data = NULL;
  } else {
    ndata = 1 + (size-1) / (sizeof(TYPE)<<3);
    this->size = size;
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
  int k = i - j;
  return data[j] & (1 << k);
}


void BitVec::set(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j;
  data[j] |= 1 << k;
}


void BitVec::unset(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j;
  data[j] &= ((TYPE)-1) - (1 << k);
}


void BitVec::flip(int i) {
  int j = i / (sizeof(TYPE)<<3);
  int k = i - j;
  data[j] ^= 1 << k;
}


void BitVec::disjunction(BitVec& other) {
  for (int i = 0; i < ndata; i++)
    data[i] |= other.data[i];
}


void BitVec::intersection(BitVec& other) {
  for (int i = 0; i < ndata; i++)
    data[i] &= other.data[i];
}
