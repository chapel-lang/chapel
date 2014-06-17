/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _CHPL_BIT_VEC_H_
#define _CHPL_BIT_VEC_H_

class BitVec {
 public:
  unsigned* data;
  int size;
  int ndata;

  BitVec(int size);
  ~BitVec();
  void clear();
  bool get(int i);
  void set(int i);
  void unset(int i);
  void flip(int i);
  void disjunction(BitVec& other);
  void intersection(BitVec& other);
};

#endif
