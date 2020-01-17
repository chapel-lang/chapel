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

#include "bitVec.h"

#include <cstdlib>

#define TYPE unsigned

BitVec::BitVec(size_t in_size) {
  if (in_size == 0) {
    ndata         = 0;
    this->in_size = 0;
    data          = NULL;
  } else {
    ndata         = 1 + (in_size - 1) / (sizeof(TYPE) << 3);
    this->in_size = in_size;
    data          = (TYPE*) calloc(ndata, sizeof(TYPE));
  }
}


BitVec::BitVec(const BitVec& rhs)
: data(NULL),
  in_size(rhs.in_size),
  ndata(rhs.ndata)
{
  if (ndata > 0)
  {
    data = (TYPE*) calloc(ndata, sizeof(TYPE));

    copy(rhs);
  }
}


BitVec::~BitVec() {
  free(data);
}


void BitVec::clear() {
  for (size_t i = 0; i < ndata; i++)
    data[i] = 0;
}


bool BitVec::get(size_t i) const {
#if DEBUG
  if (i >= in_size)
    INT_FATAL("BitVec::get -- operand out of range.");
#endif

  size_t j = i / (sizeof(TYPE) << 3);
  size_t k = i - j * (sizeof(TYPE) << 3);

  return data[j] & (1 << k);
}


void BitVec::unset(size_t i) {
  size_t j = i / (sizeof(TYPE) << 3);
  size_t k = i - j * (sizeof(TYPE) << 3);

  data[j] &= ((TYPE)-1) - (1 << k);
}


void BitVec::disjunction(const BitVec& other) {
#if DEBUG
  if (other.in_size != in_size)
    INT_FATAL("BitVec::disjunction -- operand lengths must be equal.");
#endif

  for (size_t i = 0; i < ndata; i++)
    data[i] |= other.data[i];
}


void BitVec::intersection(const BitVec& other) {
#if DEBUG
  if (other.in_size != in_size)
    INT_FATAL("BitVec::intersection -- operand lengths must be equal.");
#endif

  for (size_t i = 0; i < ndata; i++)
    data[i] &= other.data[i];
}


/*
 * Added functionality to make this compatible with std::bitset
 * and thus boosts dynamic bitset if that gets into the STL
 *
 * I would also like to implement operator overloading and a copy
 * constructor if nobody is against that
 */

bool BitVec::equals(const BitVec& other) const {
#if DEBUG
  if (other.in_size != in_size)
    INT_FATAL("BitVec::disjunction -- operand lengths must be equal.");
#endif

  bool retval = true;

  for (size_t i = 0; i < ndata && retval == true; i++) {
    if (data[i] != other.data[i]) {
      retval = false;
    }
  }

  return retval;
}


void BitVec::set() {
  for (size_t i = 0; i < ndata; i++)
    data[i] = ~0;
}


void BitVec::set(size_t i) {
  size_t j = i / (sizeof(TYPE) << 3);
  size_t k = i - j * (sizeof(TYPE) << 3);

  data[j] |= 1 << k;
}


void BitVec::reset() {
  for (size_t i = 0; i < ndata; i++)
    data[i] = 0;
}


void BitVec::reset(size_t i) {
  size_t j = i / (sizeof(TYPE) << 3);
  size_t k = i - j * (sizeof(TYPE) << 3);

  data[j] &= ((size_t)-1) - (1 << k);
}


void BitVec::copy(const BitVec& other) {
  for (size_t i = 0; i < ndata; ++i)
    data[i] = other.data[i];
}


void BitVec::copy(size_t i, bool value) {
  size_t j = i / (sizeof(TYPE) << 3);
  size_t k = i - j * (sizeof(TYPE) << 3);

  data[j] &= ~(1 << k);

  if (value)
    data[j] |= (1 << k);
}


void BitVec::flip() {
  for (size_t i = 0; i < ndata; i++)
    data[i] = ~data[i];
}


void BitVec::flip(size_t i) {
  size_t j = i / (sizeof(TYPE)<<3);
  size_t k = i - j*(sizeof(TYPE)<<3);

  data[j] ^= 1 << k;
}


/*
 * The current use of this function expects there to be almost no 1's
 * in which case this algorithm is the most efficient instead of an
 * implementation that uses divide and conquer shifting and counting with
 * hex manipulation (I have no idea what the name of that bit counting
 * algorithm is)
 */
size_t BitVec::count() const {
  size_t count = 0;

  for (size_t i = 0; i < ndata; i++) {
    size_t localCount = 0;
    size_t x          = data[i];

    for (localCount = 0; x; localCount++) {
      x &= x - 1;
    }

    count += localCount;
  }

  return count;
}


size_t BitVec::size() const {
  return in_size;
}


bool BitVec::test(size_t i) const {
  size_t j = i / (sizeof(TYPE) << 3);
  size_t k = i - j * (sizeof(TYPE) << 3);

  return data[j] & (1 << k);
}


bool BitVec::any() const {
  bool retval = false;

  for (size_t i = 0; i < ndata && retval == false; i++) {
    if (data[i] > 0) {
      retval = true;
    }
  }

  return retval;
}


bool BitVec::none() const {
  return !any();
}
