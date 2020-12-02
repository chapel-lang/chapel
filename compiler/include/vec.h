/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

/*****************************************************************************

This code is based on vec.h developed by John Plevyak and released as
part of his iterative flow analysis package available at SourceForge
(http://sourceforge.net/projects/ifa/).  The original code is:

Copyright (c) 1992-2006 John Plevyak.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*****************************************************************************/


#ifndef _vec_H_
#define _vec_H_

#include <cstring>
#include <stdint.h>
#include <cstdlib>
#include <vector>

// Simple Vector class, also supports open hashed sets

#define VEC_INTEGRAL_SIZE       4               /* must be a power of 2 */
#define VEC_INITIAL_SHIFT       3
#define VEC_INITIAL_SIZE        (1 << VEC_INITIAL_SHIFT)

#define SET_LINEAR_SIZE         4               /* must be <= than VEC_INTEGRAL_SIZE */
#define SET_MAX_PROBE           5
#define SET_INITIAL_INDEX       2

// Do not define the generic variation of _vec_hasher, requiring us to write
// a _vec_hasher for potential types.
//
// This will help prevent us from hashing on pointer values as we once did.
template<typename T>
uintptr_t _vec_hasher(T obj);

template<>
uintptr_t _vec_hasher(const char* obj);
template<>
uintptr_t _vec_hasher(unsigned int obj);
template<>
uintptr_t _vec_hasher(int obj);

template <class C, int S = VEC_INTEGRAL_SIZE>  // S must be a power of 2
class Vec {
 public:
  int           n;
  int           i;      // size index for sets
  C             *v;
  C             e[S];
  
  Vec<C,S>();
  Vec<C,S>(const Vec<C,S> &vv);
  ~Vec() { if (v && v != e) free(v); }

  void add(C a);
  void push_back(C a) { add(a);}
  int add_exclusive(C a);
  C& add();
  C head() { return n>0 ? *v : 0; }
  C only() { return n == 1 ? *v : 0; }
  C tail() { return n>0 ? v[n-1] : 0; }
  C pop();
  void clear();
  void set_clear();
  C *set_add(C a);
  C *set_add_internal(C a);
  int set_union(Vec<C,S> &v);
  int count();
  C *in(C a);
  C *set_in(C a);
  C first();
  C *set_in_internal(C a);
  void set_expand();
  int index(C a);
  void set_to_vec();
  void vec_to_set();
  void move(Vec<C,S> &v);
  void copy(const Vec<C,S> &v);
  void fill(int n);
  void append(const Vec<C,S> &v);
  void append(const std::vector<C> &v);
  void remove(int index);
  void insert(int index, C a);
  void reverse();
  C* begin() { return v; }
  C* end() { return v + n; }
  Vec<C,S>& operator=(Vec<C,S> &v) { this->copy(v); return *this; }
  int length () { return n; }
  int size() { return n; }
  
 private:
  void move_internal(Vec<C,S> &v);
  void copy_internal(const Vec<C,S> &v);
  void add_internal(C a);
  C& add_internal();
  void addx();
};

// forv_Vec: iterate over all elements of a Vec vector
// _c -- type that vector elements are pointers to
// _p -- loop variable to be declared by the macro
// _v -- vector to be iterated over
//
// note: loop variable is declared as type _c* in order
// to fit into a loop declaration.
#define forv_Vec(_c, _p, _v) \
  if ((_v).size()) \
    for (_c *qq__##_p = (_c*)0, *_p = (_v).begin()[0]; \
         ((intptr_t)(qq__##_p) < (int)(_v).size()) && \
          ((_p = (_v).begin()[(intptr_t)qq__##_p]) || 1); \
         qq__##_p = (_c*)(((intptr_t)qq__##_p) + 1))

template <class C, int S = VEC_INTEGRAL_SIZE> class Accum { public:
  Vec<C,S> asset;
  Vec<C,S> asvec;
  void add(C c) { if (asset.set_add(c)) asvec.add(c); }
  void add(Vec<C,S> v) { for (int i = 0; i < v.n; i++) if (v.v[i]) add(v.v[i]); }
  void clear() { asset.clear(); asvec.clear(); }
};

// Intervals store sets in interval format (e.g. [1..10][12..12]).
// Inclusion test is by binary search on intervals.
// Deletion is not supported
class Intervals : public Vec<int> {
 public:
  void insert(int n);
  int in(int n);
};

// UnionFind supports fast unify and finding of
// 'representative elements'.
// Elements are numbered from 0 to N-1.
class UnionFind : public Vec<int> {
 public:
  // set number of elements, initialized to singletons, may be called repeatedly to increase size
  void size(int n); 
  // return representative element
  int find(int n);
  // unify the sets containing the two elements
  void unify(int n, int m);
};

extern unsigned int prime2[];



/* IMPLEMENTATION */

template <class C, int S> inline
Vec<C,S>::Vec() : n(0), i(0), v(0) {
}

template <class C, int S> inline
Vec<C,S>::Vec(const Vec<C,S> &vv) {
  copy(vv);
}

template <class C, int S> inline void 
Vec<C,S>::add(C a) {
  if (n & (S-1))
    v[n++] = a;
  else if (!v) {
    v = e;
    v[n++] = a;
  } else
    add_internal(a);
}

template <class C, int S> inline C&
Vec<C,S>::add() {
  C *ret;
  if (n & (S-1))
    ret = &v[n++];
  else if (!v) {
    v = e;
    ret = &(v)[n++];
  } else
    ret = &add_internal();
  return *ret;
}

template <class C, int S> inline C
Vec<C,S>::pop() {
  if (!n)
    return 0;
  n--;
  C ret = v[n];
  if (!n)
    clear();
  return ret;
}

template <class C, int S> inline void
Vec<C,S>::clear() {
  if (v && v != e) free(v);
  v = NULL;
  n = 0;
  i = 0;
}

template <class C, int S> inline void
Vec<C,S>::set_clear() {
  memset(v, 0, n * sizeof(C));
}

template <class C, int S> inline C *
Vec<C,S>::set_add(C a) {
  if (n < SET_LINEAR_SIZE) {
    for (C *c = v; c < v + n; c++)
      if (*c == a)
        return 0;
    add(a);
    return &v[n-1];
  }
  if (n == SET_LINEAR_SIZE) {
    Vec<C,S> vv(*this);
    clear();
    for (C *c = vv.v; c < vv.v + vv.n; c++)
      set_add_internal(*c);
  }
  return set_add_internal(a);
}

template <class C, int S> inline int
Vec<C,S>::count() {
  int x = 0;
  for (C *c = v; c < v + n; c++)
    if (*c)
      x++;
  return x;
}

template <class C, int S> inline C*
Vec<C,S>::in(C a) {
  for (C *c = v; c < v + n; c++)
    if (*c == a)
      return c;
  return NULL;
}

template <class C, int S> inline int
Vec<C,S>::add_exclusive(C a) {
  if (!in(a)) {
    add(a);
    return 1;
  } else 
    return 0;
}

template <class C, int S> inline C *
Vec<C,S>::set_in(C a) {
  if (n <= SET_LINEAR_SIZE)
    return in(a);
  return set_in_internal(a);
}

template <class C, int S> inline C
Vec<C,S>::first() {
  for (C *c = v; c < v + n; c++)
    if (*c)
      return *c;
  return 0;
}

template <class C, int S> inline int
Vec<C,S>::index(C a) {
  for (C *c = v; c < v + n; c++)
    if (*c == a)
      return c - v;
  return -1;
}

template <class C, int S> inline void 
Vec<C,S>::move_internal(Vec<C,S> &vv)  {
  n = vv.n;
  i = vv.i;
  v = vv.v;
  if (vv.v == &vv.e[0]) { 
    memcpy((void*)e, &vv.e[0], sizeof(e));
    v = e;
  } else
    vv.v = 0;
}

template <class C, int S> inline void 
Vec<C,S>::move(Vec<C,S> &vv)  {
  move_internal(vv);
  vv.clear();
}

template <class C, int S> inline void 
Vec<C,S>::copy(const Vec<C,S> &vv)  {
  n = vv.n;
  i = vv.i;
  if (vv.v == &vv.e[0]) { 
    memcpy((void*)e, &vv.e[0], sizeof(e));
    v = e;
  } else {
    if (vv.v) 
      copy_internal(vv);
    else
      v = 0;
  }
}

template <class C, int S> inline void 
Vec<C,S>::fill(int nn)  {
  for (int i = n; i < nn; i++)
    add() = 0;
}

template <class C, int S> inline void
Vec<C,S>::append(const Vec<C,S> &vv)  {
  for (C *c = vv.v; c < vv.v + vv.n; c++)
    if (*c)
      add(*c);
}

// Added to ease the transition from using Vec to std::vector
template <class C, int S> inline void
Vec<C,S>::append(const std::vector<C> &v) {
  for (uint64_t i = 0; i < v.size(); i++) {
    add(v[i]);
  }
}

template <class C, int S> inline void 
Vec<C,S>::addx() {
  if (!n) {
    v = e;
    return;
  }
  if (v == e) {
    v = (C*)malloc(VEC_INITIAL_SIZE * sizeof(C));
    memcpy((void*)v, &e[0], n * sizeof(C));
  } else {
    if ((n & (VEC_INITIAL_SIZE -1)) == 0) {
      int l = n, nl = (1 + VEC_INITIAL_SHIFT);
      l = l >> VEC_INITIAL_SHIFT;
      while (!(l&1)) { l = l >> 1; nl++; }
      l = l >> 1;
      if (!l) {
        void *vv = (void*)v;
        nl = 1 << nl;
        v = (C*)malloc(nl * sizeof(C));
        memcpy((void*)v, vv, n * sizeof(C));
        memset((void*)&v[n], 0, (nl - n) * sizeof(C));
        free(vv);
      }
    }
  }
}

template <class C, int S> void 
Vec<C,S>::add_internal(C a) {
  addx();
  v[n++] = a;
}

template <class C, int S> C&
Vec<C,S>::add_internal() {
  addx();
  return v[n++];
}

template <class C, int S> void
Vec<C,S>::set_expand() {
  if (!n)
    i = SET_INITIAL_INDEX;
  else
    i = i + 1;
  n = prime2[i];
  v = (C*)malloc(n * sizeof(C));
  memset((void*)v, 0, n * sizeof(C));
}

template <class C, int S> C *
Vec<C,S>::set_add_internal(C c) {
  int j, k;
  if (n) {
    uintptr_t h = _vec_hasher(c);
    h = h % n;
    for (k = h, j = 0;
         k < n && j < SET_MAX_PROBE;
         k = ((k + ++j) % n))
    {
      if (!v[k]) {
        v[k] = c;
        return &v[k];
      } else if (v[k] == c)
        return 0;
    }
  }
  Vec<C,S> vv;
  vv.move_internal(*this);
  set_expand();
  if (vv.v)
    set_union(vv);
  return set_add(c);
}

template <class C, int S> C *
Vec<C,S>::set_in_internal(C c) {
  int j, k;
  if (n) {
    uintptr_t h = _vec_hasher(c);
    h = h % n;
    for (k = h, j = 0;
         k < n && j < SET_MAX_PROBE;
         k = ((k + ++j) % n))
    {
      if (!v[k])
        return 0;
      else if (v[k] == c)
        return &v[k];
    }
  }
  return 0;
}

template <class C, int S> int
Vec<C,S>::set_union(Vec<C,S> &vv) {
  int changed = 0;
  for (int i = 0; i < vv.n; i++)
    if (vv.v[i])
      changed = set_add(vv.v[i]) || changed;
  return changed;
} 

template <class C, int S> void
Vec<C,S>::set_to_vec() {
  Vec<C,S> vv(*this);
  clear();
  for (C *c = vv.v; c < vv.v + vv.n; c++)
    if (*c)
      add(*c);
}

template <class C, int S> void
Vec<C,S>::vec_to_set() {
  Vec<C,S> vv(*this);
  clear();
  for (C *c = vv.v; c < vv.v + vv.n; c++)
    set_add(*c);
}

template <class C, int S>  void 
Vec<C,S>::remove(int index) {
  if (n > 1)
    memmove(&v[index], &v[index+1], (n - 1 - index) * sizeof(v[0]));
  n--;
  if (n <= 0)
    v = e;
}

template <class C, int S>  void 
Vec<C,S>::insert(int index, C a) {
  addx();
  n++;
  for (int j = n-2; j >= index; j--)
    v[j+1] = v[j];
  v[index] = a;
}

template <class C, int S>  void 
Vec<C,S>::reverse() {
  for (int i = 0; i < n/2; i++) {
    C *s = &v[i], *e = &v[n - 1 - i];
    C t;
    memcpy(&t, s, sizeof(t));
    memcpy(s, e, sizeof(t));
    memcpy(e, &t, sizeof(t));
  }
}

template <class C, int S> void
Vec<C,S>::copy_internal(const Vec<C,S> &vv) {
  int l = n, nl = (1 + VEC_INITIAL_SHIFT);
  l = l >> VEC_INITIAL_SHIFT;
  while (l) { l = l >> 1; nl++; }
  nl = 1 << nl;
  v = (C*)malloc(nl * sizeof(C));
  memcpy((void*)v, vv.v, n * sizeof(C));
  memset((void*)(v + n), 0, (nl - n) * sizeof(C));
}

void test_vec();

#endif
