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

This code is based on map.h developed by John Plevyak and released as
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


#ifndef _map_H_
#define _map_H_

#include <cstdlib>
#include <cstring>
#include <cassert>
#include "vec.h"
#include "list.h"

static inline char *
_dupstr(char *s, char *e = 0) {
  int l = e ? e-s : strlen(s);
  char *ss = (char*)malloc(l+1);
  memcpy(ss, s, l);
  ss[l] = 0;
  return ss;
}

// Simple direct mapped Map (pointer hash table) and Environment
// The key ((K)0) has a special meaning and so should not be used.
// The removal operation is not provided.

template <class K, class C> class MapElem {
 public:
  K     key;
  C     value;
  bool operator==(MapElem &e) { return e.key == key; }

  //
  // A Map needs some way of indicating whether a slot is full or empty.
  // We use the pointer value of a key to indicate that status. Note that
  // a key can be a BaseAST, which can be deleted over the course of
  // compilation. Therefore, we cannot access the memory pointed to by
  // this key in order to test full/empty status.
  //
  operator uintptr_t()     { return (uintptr_t)key; }

  MapElem()                 : key(0)                     { }
  MapElem(K akey, C avalue) : key(akey),  value(avalue)  { }
  MapElem(MapElem &e)       : key(e.key), value(e.value) { }
  MapElem(unsigned long x)                               { assert(!x); key = 0; }
};

template<class K, class C>
uintptr_t _vec_hasher(MapElem<K,C> obj) {
  return _vec_hasher(obj.key);
}

template <class K, class C> class Map : public Vec<MapElem<K,C> > {
 public:
  using Vec<MapElem<K, C> >::n;
  using Vec<MapElem<K, C> >::i;
  using Vec<MapElem<K, C> >::v;
  MapElem<K,C> *put(K akey, C avalue);
  C get(K akey);
  void get_keys(Vec<K> &keys);
  void get_keys_set(Vec<K> &keys);
  void get_values(Vec<C> &values);
  MapElem<K,C> *get_record(K akey);
  void map_union(Map<K,C> &m);
};

template <class C> class HashFns {
 public:
  static unsigned int hash(C a);
  static int equal(C a, C b);
};

template <class K, class AHashFns, class C> class HashMap : public Map<K,C> {
 public:
  using Map<K, C>::n;
  using Map<K, C>::i;
  using Map<K, C>::v;
  using Map<K, C>::e;
  MapElem<K,C> *get_internal(K akey);
  C get(K akey);
  MapElem<K,C> *put(K akey, C avalue);
  void get_keys(Vec<K> &keys);
  void get_values(Vec<C> &values);
};

#define form_Map(_c, _p, _v) if ((_v).n) for (_c *qq__##_p = (_c*)0, *_p = &(_v).v[0]; \
             ((intptr_t)(qq__##_p) < (_v).n) && ((_p = &(_v).v[(intptr_t)qq__##_p]) || 1); \
             qq__##_p = (_c*)(((intptr_t)qq__##_p) + 1)) \
          if ((_p)->key)


class StringHashFns : public HashFns<const char*> {
 public:
  static unsigned int hash(const char *s) { 
    unsigned int h = 0; 
    // 31 changed to 27, to avoid prime2 in vec.cpp
    while (*s) h = h * 27 + (unsigned char)*s++;  
    return h;
  }
  static int equal(const char *a, const char *b) { return !strcmp(a, b); }
};

template <class C, class AHashFns> class ChainHash : public Map<unsigned int, List<C> > {
 public:
  using Map<unsigned int, List<C> >::n;
  using Map<unsigned int, List<C> >::v;
  C put(C c);
  C get(C c);
  int del(C avalue);
  void get_elements(Vec<C> &elements);
};

template <class K, class AHashFns, class C> class ChainHashMap : 
  public Map<unsigned int, List<MapElem<K,C> > > {
 public:
  using Map<unsigned int, List<MapElem<K, C> > >::n;
  using Map<unsigned int, List<MapElem<K, C> > >::v;
  MapElem<K,C> *put(K akey, C avalue);
  C get(K akey);
  int del(K akey);
  void get_keys(Vec<K> &keys);
  void get_values(Vec<C> &values);
};

class StringChainHash : public ChainHash<char *, StringHashFns> {
 public:
  char *canonicalize(char *s, char *e);
};

template <class C, class AHashFns, int N> class NBlockHash {
 public:
  int n;
  int i;
  C *v;
  C e[N];

  C* end() { return last(); }
  int length() { return N * n; }
  int size() {return n;}
  C *first();
  C *last();
  C put(C c);
  C get(C c);
  int del(C c);
  void clear();
  int count();
  void copy(const NBlockHash<C,AHashFns,N> &hh);
  void move(NBlockHash<C,AHashFns,N> &hh);
  NBlockHash();
  NBlockHash(NBlockHash<C,AHashFns,N> &hh) { copy(hh); }
};

/* use forv_Vec on BlockHashs */

#define DEFAULT_BLOCK_HASH_SIZE 4
template <class C, class ABlockHashFns> class BlockHash : 
  public NBlockHash<C, ABlockHashFns, DEFAULT_BLOCK_HASH_SIZE> {};
typedef BlockHash<char *, StringHashFns> StringBlockHash;

template <class K, class C> class Env {
 public:
  void put(K akey, C avalue);
  C get(K akey);
  void push();
  void pop();
  void clear() { store.clear(); scope.clear(); }

  Env() {}
 private:
  Map<K,List<C> *> store;
  List<List<K> > scope;
  List<C> *get_bucket(K akey);
};

extern unsigned int open_hash_multipliers[256];

/* IMPLEMENTATION */

template <class K, class C> inline C 
Map<K,C>::get(K akey) {
  MapElem<K,C> e(akey, (C)0);
  MapElem<K,C> *x = this->set_in(e);
  if (x)
    return x->value;
  return (C)0;
}

template <class K, class C> inline MapElem<K,C> *
Map<K,C>::put(K akey, C avalue) {
  MapElem<K,C> e(akey, avalue);
  MapElem<K,C> *x = this->set_in(e);
  if (x) {
    x->value = avalue;
    return x;
  } else
    return this->set_add(e);
}

template <class K, class C> inline void
Map<K,C>::get_keys(Vec<K> &keys) {
  for (int i = 0; i < n; i++)
    if (v[i].key)
      keys.add(v[i].key);
}

template <class K, class C> inline void
Map<K,C>::get_keys_set(Vec<K> &keys) {
  for (int i = 0; i < n; i++)
    if (v[i].key)
      keys.set_add(v[i].key);
}

template <class K, class C> inline void
Map<K,C>::get_values(Vec<C> &values) {
  for (int i = 0; i < n; i++)
    if (v[i].key)
      values.add(v[i].value);
}

template <class K, class C> inline MapElem<K,C> *
Map<K,C>::get_record(K akey) {
  MapElem<K,C> e(akey, (C)0);
  MapElem<K,C> *x = this->set_in(e);
  return x;
}

template <class K, class C> inline void
Map<K,C>::map_union(Map<K,C> &m) {
  for (int i = 0; i < m.n; i++)
    if (m.v[i].key)
      put(m.v[i].key, m.v[i].value);
}

template <class K, class C> inline void
map_set_add(Map<K, Vec<C> *> &m, K akey, C avalue) {
  Vec<C> *v = m.get(akey);
  if (!v)
    m.put(akey, (v = new Vec<C>));
  v->set_add(avalue);
}

template <class K, class C> inline void
map_set_add(Map<K, Vec<C> *> &m, K akey, Vec<C> *madd) {
  Vec<C> *v = m.get(akey);
  if (!v)
    m.put(akey, (v = new Vec<C>));
  v->set_union(*madd);
}

template <class K, class AHashFns, class C> inline MapElem<K,C> * 
HashMap<K,AHashFns,C>::get_internal(K akey) {
  if (!n)
    return 0;
  if (n <= VEC_INTEGRAL_SIZE) {
    for (MapElem<K,C> *c = v; c < v + n; c++)
      if (c->key)
        if (AHashFns::equal(akey, c->key))
          return c;
    return 0;
  }
  unsigned int h = AHashFns::hash(akey);
  h = h % n;
  for (int k = h, j = 0;
       k < n && j < SET_MAX_PROBE;
       k = ((k + ++j) % n))
  {
    if (!v[k].key)
      return 0;
    else if (AHashFns::equal(akey, v[k].key))
      return &v[k];
  }
  return 0;
}

template <class K, class AHashFns, class C> inline C 
HashMap<K,AHashFns,C>::get(K akey) {
  MapElem<K,C> *x = get_internal(akey);
  if (!x)
    return 0;
  return x->value;
}

template <class K, class AHashFns, class C> inline MapElem<K,C> *
HashMap<K,AHashFns,C>::put(K akey, C avalue) {
  MapElem<K,C> *x = get_internal(akey);
  if (x) {
    x->value = avalue;
    return x;
  } else {
    if (n < VEC_INTEGRAL_SIZE) {
      if (!v)
        v = e;
      v[n].key = akey;
      v[n].value = avalue;
      n++;
      return &v[n-1];
    }
    if (n > VEC_INTEGRAL_SIZE) {
      unsigned int h = AHashFns::hash(akey);
      h = h % n;
      for (int k = h, j = 0;
           k < n && j < SET_MAX_PROBE;
           k = ((k + ++j) % n))
      {
        if (!v[k].key) {
          v[k].key = akey;
          v[k].value = avalue;
          return &v[k];
        }
      }
    } else
      i = SET_INITIAL_INDEX-1; // will be incremented in set_expand
  }
  HashMap<K,AHashFns,C> vv(*this);
  Map<K,C>::set_expand();
  for (int i = 0; i < vv.n; i++)
    if (vv.v[i].key)
      put(vv.v[i].key, vv.v[i].value);
  return put(akey, avalue);
}

template <class K, class AHashFns, class C> inline void
HashMap<K,AHashFns,C>::get_keys(Vec<K> &keys) { Map<K,C>::get_keys(keys); }

template <class K, class AHashFns, class C> inline void
HashMap<K,AHashFns,C>::get_values(Vec<C> &values) { Map<K,C>::get_values(values); }

template <class C, class AHashFns> C
ChainHash<C, AHashFns>::put(C c) {
  unsigned int h = AHashFns::hash(c);
  List<C> *l;
  MapElem<unsigned int,List<C> > e(h, (C)0);
  MapElem<unsigned int,List<C> > *x = this->set_in(e);
  if (x)
    l = &x->value;
  else {
    l = &Map<unsigned int, List<C> >::put(h, c)->value;
    return l->head->car;
  }
  forc_List(C, x, *l)
    if (AHashFns::equal(c, x->car))
      return x->car;
  l->push(c);
  return (C)0;
}

template <class C, class AHashFns> C
ChainHash<C, AHashFns>::get(C c) {
  unsigned int h = AHashFns::hash(c);
  List<C> empty;
  MapElem<unsigned int,List<C> > e(h, empty);
  MapElem<unsigned int,List<C> > *x = this->set_in(e);
  if (!x)
    return 0;
  List<C> *l = &x->value;
  forc_List(C, x, *l)
    if (AHashFns::equal(c, x->car))
      return x->car;
  return 0;
}

template <class C, class AHashFns> void
ChainHash<C, AHashFns>::get_elements(Vec<C> &elements) {
  for (int i = 0; i < n; i++) {
    List<C> *l = &v[i].value;
    forc_List(C, x, *l)
      elements.add(x);
  }
}

template <class C, class AHashFns> int
ChainHash<C, AHashFns>::del(C c) {
  unsigned int h = AHashFns::hash(c);
  List<C> *l;
  MapElem<unsigned int,List<C> > e(h, (C)0);
  MapElem<unsigned int,List<C> > *x = this->set_in(e);
  if (x)
    l = &x->value;
  else
    return 0;
  ConsCell<C> *last = 0;
  forc_List(C, x, *l) {
    if (AHashFns::equal(c, x->car)) {
      if (!last)
        l->head = x->cdr;
      else
        last->cdr = x->cdr;
      return 1;
    }
    last = x;
  }
  return 0;
}

template <class K, class AHashFns, class C>  MapElem<K,C> *
ChainHashMap<K, AHashFns, C>::put(K akey, C avalue) {
  unsigned int h = AHashFns::hash(akey);
  List<MapElem<K,C> > empty;
  List<MapElem<K,C> > *l;
  MapElem<K, C> c(akey, avalue);
  MapElem<unsigned int,List<MapElem<K,C> > > e(h, empty);
  MapElem<unsigned int,List<MapElem<K,C> > > *x = this->set_in(e);
  if (x)
    l = &x->value;
  else {
    l = &Map<unsigned int, List<MapElem<K,C> > >::put(h, c)->value;
    return &l->head->car;
  }
  for (ConsCell<MapElem<K,C> > *p  = l->head; p; p = p->cdr)
    if (AHashFns::equal(akey, p->car.key)) {
      p->car.value = avalue;
      return &p->car;
    }
  l->push(c);
  return 0;
}

template <class K, class AHashFns, class C> C
ChainHashMap<K, AHashFns, C>::get(K akey) {
  unsigned int h = AHashFns::hash(akey);
  List<MapElem<K,C> > empty;
  MapElem<unsigned int,List<MapElem<K,C> > > e(h, empty);
  MapElem<unsigned int,List<MapElem<K,C> > > *x = this->set_in(e);
  if (!x)
    return 0;
  List<MapElem<K,C> > *l = &x->value;
  if (l->head) 
    for (ConsCell<MapElem<K,C> > *p  = l->head; p; p = p->cdr)
      if (AHashFns::equal(akey, p->car.key))
        return p->car.value;
  return 0;
}

template <class K, class AHashFns, class C>  int
ChainHashMap<K, AHashFns, C>::del(K akey) {
  unsigned int h = AHashFns::hash(akey);
  List<MapElem<K,C> > empty;
  List<MapElem<K,C> > *l;
  MapElem<unsigned int,List<MapElem<K,C> > > e(h, empty);
  MapElem<unsigned int,List<MapElem<K,C> > > *x = this->set_in(e);
  if (x)
    l = &x->value;
  else
    return 0;
  ConsCell<MapElem<K,C> > *last = 0;
  for (ConsCell<MapElem<K,C> > *p = l->head; p; p = p->cdr) {
    if (AHashFns::equal(akey, p->car.key)) {
      if (!last)
        l->head = p->cdr;
      else
        last->cdr = p->cdr;
      return 1;
    }
    last = p;
  }
  return 0;
}

template <class K, class AHashFns, class C> void
ChainHashMap<K, AHashFns, C>::get_keys(Vec<K> &keys) {
  for (int i = 0; i < n; i++) {
    List<MapElem<K,C> > *l = &v[i].value;
    if (l->head) 
      for (ConsCell<MapElem<K,C> > *p  = l->head; p; p = p->cdr)
        keys.add(p->car.key);
  }
}

template <class K, class AHashFns, class C> void
ChainHashMap<K, AHashFns, C>::get_values(Vec<C> &values) {
  for (int i = 0; i < n; i++) {
    List<MapElem<K,C> > *l = &v[i].value;
    if (l->head) 
      for (ConsCell<MapElem<K,C> > *p  = l->head; p; p = p->cdr)
        values.add(p->car.value);
  }
}

inline char *
StringChainHash::canonicalize(char *s, char *e) {
  unsigned int h = 0;
  char *a = s;
  // 31 changed to 27, to avoid prime2 in vec.cpp
  if (e)
    while (a != e) h = h * 27 + (unsigned char)*a++;  
  else
    while (*a) h = h * 27 + (unsigned char)*a++;  
  List<char*> *l;
  MapElem<unsigned int,List<char*> > me(h, (char*)0);
  MapElem<unsigned int,List<char*> > *x = this->set_in(me);
  if (x) {
    l = &x->value;
    forc_List(char *, x, *l) {
      a = s;
      char *b = x->car;
      while (1) {
        if (!*b) {
          if (a == e)
            return x->car;
          break;
        }
        if (a >= e || *a != *b)
          break;
        a++; b++;
      }
    }
  }
  s = _dupstr(s, e);
  char *ss = put(s);
  if (ss)
    return ss;
  return s;
}

template <class K, class C> inline C 
Env<K,C>::get(K akey) {
  MapElem<K,List<C> *> e(akey, 0);
  MapElem<K,List<C> *> *x = store.set_in(e);
  if (x)
    return x->value->first();
  return (C)0;
}

template <class K, class C> inline List<C> *
Env<K,C>::get_bucket(K akey) {
  List<C> *bucket = store.get(akey);
  if (bucket)
    return bucket;
  bucket = new List<C>();
  store.put(akey, bucket);
  return bucket;
}

template <class K, class C> inline void
Env<K,C>::put(K akey, C avalue) {
  scope.head->car.push(akey);
  get_bucket(akey)->push(avalue);
}

template <class K, class C> inline void
Env<K,C>::push() {
  scope.push();
}

template <class K, class C> inline void
Env<K,C>::pop() {
  forc_List(K, e, scope.first())
    get_bucket(e->car)->pop();
}

template <class C, class AHashFns, int N> inline 
NBlockHash<C, AHashFns, N>::NBlockHash() : n(1), i(0) {
  memset(&e[0], 0, sizeof(e));
  v = e;
}

template <class C, class AHashFns, int N> inline C*
NBlockHash<C, AHashFns, N>::first() {
  return &v[0];
}

template <class C, class AHashFns, int N> inline C*
NBlockHash<C, AHashFns, N>::last() {
  return &v[n * N];
}

template <class C, class AHashFns, int N> inline C
NBlockHash<C, AHashFns, N>::put(C c) {
  int a;
  unsigned int h = AHashFns::hash(c);
  C *x = &v[(h % n) * N];
  for (a = 0; a < N; a++) {
    if (!x[a])
      break;
    if (AHashFns::equal(c, x[a]))
      return x[a];
  }
  if (a < N) {
    x[a] = c;
    return (C)0;
  }
  C *vv = first(), *ve = last();
  C *old_v = v;
  i = i + 1;
  n = prime2[i];
  v = (C*)malloc(n * sizeof(C) * N);
  memset(v, 0, n * sizeof(C) * N);
  for (;vv < ve; vv++)
    if (*vv)
      put(*vv);
  if (old_v != &e[0])
    free(old_v);
  return put(c);
}

template <class C, class AHashFns, int N> inline C
NBlockHash<C, AHashFns, N>::get(C c) {
  if (!n)
    return (C)0;
  unsigned int h = AHashFns::hash(c);
  C *x = &v[(h % n) * N];
  for (int a = 0; a < N; a++) {
    if (!x[a])
      return (C)0;
    if (AHashFns::equal(c, x[a]))
      return x[a];
  }
  return (C)0;
}

template <class C, class AHashFns, int N> inline int
NBlockHash<C, AHashFns, N>::del(C c) {
  int a, b;
  if (!n)
    return 0;
  unsigned int h = AHashFns::hash(c);
  C *x = &v[(h % n) * N];
  for (a = 0; a < N; a++) {
    if (!x[a])
      return 0;
    if (AHashFns::equal(c, x[a])) {
      if (a < N - 1) {
        for (b = a + 1; b < N; b++) {
          if (!x[b])
            break;
        }
        if (b != a + 1)
          x[a] = x[b - 1];
        x[b - 1] = (C)0;
        return 1;
      } else {
        x[N - 1] = (C)0;
        return 1;
      }
    }
  }
  return 0;
}

template <class C, class AHashFns, int N> inline void
NBlockHash<C, AHashFns, N>::clear() {
  if (v)
    memset(v, 0, n * N *sizeof(C));
}

template <class C, class AHashFns, int N> inline int
NBlockHash<C, AHashFns, N>::count() {
  int nelements = 0;
  C *l = last();
  for (C *xx = first(); xx < l; xx++) 
    if (*xx)
      nelements++;
  return nelements;
}

template <class C, class AHashFns, int N> inline void 
NBlockHash<C, AHashFns, N>::copy(const NBlockHash<C, AHashFns, N> &hh) {
  clear();
  n = hh.n;
  i = hh.i;
  if (hh.v == &hh.e[0]) { 
    memcpy(e, &hh.e[0], sizeof(e));
    v = e;
  } else {
    if (hh.v) {
      v = (C*)malloc(n * sizeof(C) * N);
      memcpy(v, hh.v, n * sizeof(C) * N);
    } else
      v = 0;
  }
}

template <class C, class AHashFns, int N> inline void 
NBlockHash<C, AHashFns, N>::move(NBlockHash<C, AHashFns, N> &hh) {
  n = hh.n;
  i = hh.i;
  v = hh.v;
  if (hh.v == &hh.e[0]) { 
    memcpy(e, &hh.e[0], sizeof(e));
    v = e;
  }
  hh.clear();
}

void test_map();

#endif
