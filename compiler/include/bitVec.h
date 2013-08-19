#ifndef _CHPL_BIT_VEC_H_
#define _CHPL_BIT_VEC_H_

class BitVec {
 public:
  unsigned* data;
  int in_size;
  int ndata;

  BitVec(int in_size);
  ~BitVec();
  void clear();
  bool get(int i);
  void unset(int i);
  void disjunction(BitVec& other);
  void intersection(BitVec& other);
  
  
  // Added functionality to make this compatible with std::bitset and thus 
  // boosts dynamic bitset if that gets into the STL, or we start using boost
  bool equals(BitVec& other);
  void set();
  void set(int i);
  void reset();
  void reset(int i);
  void flip();
  void flip(int i);
  int count();
  int size();
  bool test(int i);
  bool any();
  bool none();
};

#endif
