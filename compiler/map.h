/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#ifndef _map_H_
#define _map_H_

#if !defined(__FreeBSD__) || (__FreeBSD_version >= 500000)
#include <stdint.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "chpltypes.h"
#include "list.h"
#include "vec.h"

char *dupstr(char *s, char *e = 0); // from misc.h

// Simple direct mapped Map (pointer hash table) and Environment

template <class K, class C>
class MapElem {
 public:
  K	key;
  C	value;
  bool operator==(MapElem &e) { return e.key == key; }
  operator unsigned int(void) { return (unsigned int)(uintptr_t)key; }
  MapElem(unsigned int x) { assert(!x); key = 0; value = 0; }
  MapElem(K akey, C avalue) : key(akey), value(avalue) {}
  MapElem(MapElem &e) : key(e.key), value(e.value) {}
  MapElem() : key(0), value(0) {}
};

template <class K, class C> class Map : public Vec<MapElem<K,C> > {
 public:
  using Vec<MapElem<K, C> >::n;
  using Vec<MapElem<K, C> >::v;
  inline MapElem<K,C> *put(K akey, C avalue);
  inline C get(K akey);
  inline void get_keys(Vec<K> &keys);
  inline void get_values(Vec<C> &values);
};

template <class C> class HashFns {
 public:
  static unsigned int hash(C a);
  static int equal(C a, C b);
};

template <class K, class AHashFns, class C> class HashMap : public Map<K,C> {
 public:
  using Map<K, C>::n;
  using Map<K, C>::v;
  using Map<K, C>::e;
  inline MapElem<K,C> *get_internal(K akey);
  inline C get(K akey);
  inline MapElem<K,C> *put(K akey, C avalue);
  inline void get_keys(Vec<K> &keys);
  inline void get_values(Vec<C> &values);
};

class StringHashFns {
 public:
  static unsigned int hash(char *s) { 
    unsigned int h = 0;	
    // 31 changed to 27, to avoid prime2 in vec.cpp
    while (*s) h = h * 27 + (unsigned char)*s++;  
    return h;
  }
  static int equal(char *a, char *b) { return !strcmp(a, b); }
};

class PointerHashFns {
 public:
  static unsigned int hash(void *s) { return (unsigned int)(uintptr_t)s; }
  static int equal(void *a, void *b) { return a == b; }
};

template <class C, class AHashFns> class OpenHash : public Map<unsigned int, List<C> > {
 public:
  inline C put(C c);
  inline C get(C c);
};

class StringOpenHash : public OpenHash<char *, StringHashFns> {
 public:
  inline char *cannonicalize(char *s, char *e);
};

template <class C, class AHashFns, int N> class NBlockHash {
 public:
  int n;
  int i;
  C *v;
  C e[N];

  C* end() { return last(); }
  inline C *first();
  inline C *last();
  inline C put(C c);
  inline C get(C c);
  inline void del(C c);
  inline void clear();
  inline int count();
  inline NBlockHash();
};

#define DEFAULT_BLOCK_HASH_SIZE	4
template <class C, class ABlockHashFns> class BlockHash : 
  public NBlockHash<C, ABlockHashFns, DEFAULT_BLOCK_HASH_SIZE> {};
typedef BlockHash<char *, StringHashFns> StringBlockHash;

template <class K, class C> 
class Env : public gc {
 public:
  inline void put(K akey, C avalue);
  inline C get(K akey);
  inline void push();
  inline void pop();

  inline Env() {}
 private:
  Map<K,List<C> *> store;
  List<List<C> > scope;
  inline List<C> *get_bucket(K akey);
};

extern unsigned int open_hash_multipliers[256];

/* IMPLEMENTATION */

template <class K, class C> inline C 
Map<K,C>::get(K akey) {
  MapElem<K,C> e(akey, (C)0);
  MapElem<K,C> *x = set_in(e);
  if (x)
    return x->value;
  return (C)0;
}

template <class K, class C> inline MapElem<K,C> *
Map<K,C>::put(K akey, C avalue) {
  MapElem<K,C> e(akey, avalue);
  MapElem<K,C> *x = set_in(e);
  if (x) {
    x->value = avalue;
    return x;
  } else
    return set_add(e);
}

template <class K, class C> inline void
Map<K,C>::get_keys(Vec<K> &keys) {
  for (int i = 0; i < n; i++)
    if (v[i].key)
      keys.add(v[i].key);
}

template <class K, class C> inline void
Map<K,C>::get_values(Vec<C> &values) {
  for (int i = 0; i < n; i++)
    if (v[i].key)
      values.set_add(v[i].value);
  values.set_to_vec();
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
       k < n && j < SET_MAX_SEQUENTIAL;
       k = ((k + 1) % n), j++)
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
	   k < n && j < SET_MAX_SEQUENTIAL;
	   k = ((k + 1) % n), j++)
      {
	if (!v[k].key) {
	  v[k].key = akey;
	  v[k].value = avalue;
	  return &v[k];
	}
      }
    }
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

template <class C, class AnOpenHashFns> C
OpenHash<C, AnOpenHashFns>::put(C c) {
  unsigned int h = AnOpenHashFns::hash(c);
  List<C> *l;
  MapElem<unsigned int,List<C> > e(h, (C)0);
  MapElem<unsigned int,List<C> > *x = set_in(e);
  if (x)
    l = &x->value;
  else
    l = &Map<unsigned int, List<C> >::put(h, c)->value;
  forc_List(C, x, *l)
    if (AnOpenHashFns::equal(c, x->car))
      return x->car;
  l->push(c);
  return (C)0;
}

template <class C, class AnOpenHashFns> C
OpenHash<C, AnOpenHashFns>::get(C c) {
  unsigned int h = AnOpenHashFns::hash(c);
  MapElem<unsigned int,List<C> > e(h, (C)0);
  MapElem<unsigned int,List<C> > *x = set_in(e);
  if (!x)
    return 0;
  List<C> *l = &x->value;
  forc_List(C, x, *l)
    if (AnOpenHashFns::equal(c, x->car))
      return x->car;
  l->push(c);
  return c;
}

inline char *
StringOpenHash::cannonicalize(char *s, char *e) {
  unsigned int h = 0;
  char *a = s;
  // 31 changed to 27, to avoid prime2 in vec.cpp
  if (e)
    while (a != e) h = h * 27 + (unsigned char)*a++;  
  else
    while (*a) h = h * 27 + (unsigned char)*a++;  
  List<char*> *l;
  MapElem<unsigned int,List<char*> > me(h, (char*)0);
  MapElem<unsigned int,List<char*> > *x = set_in(me);
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
  s = dupstr(s, e);
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
  forc_List(C, e, scope.first())
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
  i = i + 1;
  n = prime2[i];
  v = (C*)MALLOC(n * sizeof(C) * N);
  memset(v, 0, n * sizeof(C) * N);
  for (;vv < ve; vv++)
    if (*vv)
      put(*vv);
  return put(c);
}

template <class C, class AHashFns, int N> inline C
NBlockHash<C, AHashFns, N>::get(C c) {
  int a;
  if (!n)
    return (C)0;
  unsigned int h = AHashFns::hash(c);
  C *x = &v[(h % n) * N];
  for (a = 0; a < N; a++) {
    if (!x[a])
      return (C)0;
    if (AHashFns::equal(c, x[a]))
      return x[a];
  }
  return (C)0;
}

template <class C, class AHashFns, int N> inline void
NBlockHash<C, AHashFns, N>::del(C c) {
  int a, b;
  if (!n)
    return;
  unsigned int h = AHashFns::hash(c);
  C *x = &v[(h % n) * N];
  for (a = 0; a < N; a++) {
    if (!x[a])
      return;
    if (AHashFns::equal(c, x[a])) {
      if (a < N - 1) {
	for (b = a + 1; b < N; b++) {
	  if (!x[b])
	    break;
	}
	if (b != a + 1)
	  x[a] = x[b - 1];
	x[b - 1] = (C)0;
      } else
	x[N - 1] = (C)0;
    }
  }
}

template <class C, class AHashFns, int N> inline void
NBlockHash<C, AHashFns, N>::clear() {
  if (v)
    memset(v, 0, n * N *sizeof(C));
}

template <class C, class AHashFns, int N> inline int
NBlockHash<C, AHashFns, N>::count() {
  int count = 0;
  C *l = last();
  for (C *xx = first(); xx < l; xx++) 
    if (*xx)
      count++;
  return count;
}

void test_map();

#endif
