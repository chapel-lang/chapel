#ifndef _CHPL_BIT_VEC_H_
#define _CHPL_BIT_VEC_H_

class BitVec {
 public:
  unsigned* data;
  size_t in_size;
  size_t ndata;

  BitVec(size_t in_size);
  BitVec(const BitVec& rhs);
  ~BitVec();
  void clear();
  bool get(size_t i) const;
  bool operator[](size_t i) const { return get(i); }
  void unset(size_t i);
  void disjunction(const BitVec& other);
  void intersection(const BitVec& other);
  
  
  // Added functionality to make this compatible with std::bitset and thus 
  // boosts dynamic bitset if that gets into the STL, or we start using boost
  bool equals(const BitVec& other) const;
  void set();
  void set(size_t i);
  void reset();
  void reset(size_t i);
  void copy(const BitVec& other);
  void copy(size_t i, bool value);
  void flip();
  void flip(size_t i);
  size_t count() const;
  size_t size() const;
  bool test(size_t i) const;
  bool any() const;
  bool none() const;
};

inline BitVec operator+(const BitVec& a, const BitVec& b)
{
  BitVec result(a);
  result.disjunction(b);
  return result;
}

inline BitVec operator-(const BitVec& a, const BitVec& b)
{
  BitVec result(b);
  result.flip();
  result.intersection(a);
  return result;
}

#endif
