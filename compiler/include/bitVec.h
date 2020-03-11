/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include <cstddef>

class BitVec {
public:
  unsigned* data;
  size_t    in_size;
  size_t    ndata;


         BitVec(size_t in_size);
         BitVec(const BitVec& rhs);

        ~BitVec();

  void   clear();
  bool   get(size_t i) const;
  bool   operator[](size_t i) const { return get(i); }

  void   unset(size_t i);

  void   disjunction(const BitVec& other);
  void   intersection(const BitVec& other);

  void   operator =  (const BitVec& other) { this->copy(other);         }

  // Synonyms for disjunction (union) and (conjunction) intersection above.
  void   operator |= (const BitVec& other) { this->disjunction(other);  }
  void   operator += (const BitVec& other) { this->disjunction(other);  }
  void   operator &= (const BitVec& other) { this->intersection(other); }
  void   operator -= (const BitVec& other);

  // Added functionality to make this compatible with std::bitset and thus
  // boosts dynamic bitset if that gets into the STL, or we start using boost
  bool   equals(const BitVec& other) const;

  void   set();
  void   set(size_t i);

  void   reset();
  void   reset(size_t i);

  void   copy(const BitVec& other);
  void   copy(size_t i, bool value);

  void   flip();
  void   flip(size_t i);

  size_t count()                                                     const;
  size_t size()                                                      const;

  bool   test(size_t i)                                              const;

  bool   any()                                                       const;
  bool   none()                                                      const;
};

inline void BitVec::operator-=(const BitVec& other)
{
  BitVec not_other(other);

  not_other.flip();

  this->intersection(not_other);
}

inline bool operator==(const BitVec& a, const BitVec& b)
{
  return a.equals(b);
}

inline bool operator!=(const BitVec& a, const BitVec& b)
{
  return ! a.equals(b);
}

inline BitVec operator&(const BitVec& a, const BitVec& b)
{
  BitVec result(a);

  result.intersection(b);

  return result;
}

inline BitVec operator|(const BitVec& a, const BitVec& b)
{
  BitVec result(a);

  result.disjunction(b);

  return result;
}

// An alias for operator|.
inline BitVec operator+(const BitVec& a, const BitVec& b)
{
  return a | b;
}

inline BitVec operator-(const BitVec& a, const BitVec& b)
{
  BitVec result(b);

  result.flip();
  result.intersection(a);

  return result;
}

#endif
