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
