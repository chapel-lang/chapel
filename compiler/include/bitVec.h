/*
 * Copyright 2004-2014 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
  bool get(size_t i);
  bool operator[](size_t i) { return get(i); }
  void unset(size_t i);
  void disjunction(BitVec& other);
  void intersection(BitVec& other);
  
  // Synonyms for disjunction (union) and (conjunction) intersection above.
  void operator|=(BitVec& other) { this->disjunction(other); }
  void operator&=(BitVec& other) { this->intersection(other); }

  // Added functionality to make this compatible with std::bitset and thus 
  // boosts dynamic bitset if that gets into the STL, or we start using boost
  bool equals(BitVec& other);
  void set();
  void set(size_t i);
  void reset();
  void reset(size_t i);
  void copy(const BitVec& other);
  void copy(size_t i, bool value);
  void flip();
  void flip(size_t i);
  size_t count();
  size_t size();
  bool test(size_t i);
  bool any();
  bool none();
};

inline BitVec* operator+(BitVec& a, BitVec& b)
{
  BitVec* result = new BitVec(a);
  result->disjunction(b);
  return result;
}

// Usurps its left argument according to the optional adoption pattern.
inline BitVec* operator+(BitVec* a, BitVec& b)
{
  BitVec* result = a;
  result->disjunction(b);
  return result;
}

inline BitVec* operator-(BitVec& a, BitVec& b)
{
  BitVec* result = new BitVec(b);
  result->flip();
  result->intersection(a);
  return result;
}

inline BitVec* operator-(BitVec* a, BitVec& b)
{
  BitVec* result = a;
  BitVec* temp = new BitVec(b);
  temp->flip();
  result->intersection(*temp);
  delete temp; temp = 0;
  return result;
}

#endif
