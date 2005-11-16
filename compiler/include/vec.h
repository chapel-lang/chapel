/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak
*/

#ifndef _vec_H_
#define _vec_H_

#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include "chplalloc.h"
#include "misc.h"

// Simple Vector class, also supports open hashed sets

#define VEC_INTEGRAL_SIZE       4               /* must be a power of 2 */
#define VEC_INITIAL_SHIFT       3
#define VEC_INITIAL_SIZE        (1 << VEC_INITIAL_SHIFT)

#define SET_MAX_SEQUENTIAL      5
#define SET_INITIAL_INDEX       2

template <class C> 
class Vec : public gc {
 public:
  int           n;
  int           i;      // size index for sets
  C             *v;
  C             e[VEC_INTEGRAL_SIZE];
  
  Vec<C>();
  Vec<C>(const Vec<C> &vv);

  inline void add(C a);  
  inline int add_exclusive(C a);
  inline C& add();
  inline C pop();
  inline void clear();
  inline void set_clear();
  inline C *set_add(C a);
  C *set_add_internal(C a);
  int set_union(Vec<C> &v);
  int set_intersection(Vec<C> &v);
  int some_intersection(Vec<C> &v);
  int some_disjunction(Vec<C> &v);
  int some_difference(Vec<C> &v);
  void set_intersection(Vec<C> &v, Vec<C> &result);
  void set_disjunction(Vec<C> &v, Vec<C> &result);
  void set_difference(Vec<C> &v, Vec<C> &result);
  int set_count();
  inline int count();
  inline C *in(C a);
  inline C *set_in(C a);
  inline C first();
  C *set_in_internal(C a);
  void set_expand();
  inline int index(C a);
  void set_to_vec();
  void vec_to_set();
  inline void move(Vec<C> &v);
  void copy_internal(const Vec<C> &v);
  inline void copy(const Vec<C> &v);
  inline void fill(int n);
  inline void append(const Vec<C> &v);
  void reverse();
  C* end() { return v + n; }
  Vec<C>& operator=(Vec<C> &v) { this->copy(v); return *this; }
  int length () { return n; }
  void quickSort(int left, int right);
  
 private:
  void add_internal(C a);
  C& add_internal();
  void addx();
};

// c -- class, p -- pointer to elements of v, v -- vector
#define forv_Vec(_c, _p, _v) if ((_v).n) for (_c *qq__##_p = (_c*)0, *_p = (_v).v[0]; \
                    ((intptr_t)(qq__##_p) < (_v).length()) && ((_p = (_v).v[(intptr_t)qq__##_p]) || 1); qq__##_p = (_c*)(((intptr_t)qq__##_p) + 1))

template <class C> class Accum : public gc { public:
  Vec<C> asset;
  Vec<C> asvec;
  void add(C c) { if (asset.set_add(c)) asvec.add(c); }
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
// 'representitive elements'.
// Elements are numbered from 0 to N-1.
class UnionFind : public Vec<int> {
 public:
  // set number of elements, initialized to singletons, may be called repeatedly to increase size
  void size(int n); 
  // return representitive element
  int find(int n);
  // unify the sets containing the two elements
  void unify(int n, int m);
};

extern unsigned int prime2[];



/* IMPLEMENTATION */

template <class C> inline
Vec<C>::Vec() : n(0), i(0), v(0) {
}

template <class C> inline
Vec<C>::Vec(const Vec<C> &vv) {
  copy(vv);
}

template <class C> inline void 
Vec<C>::add(C a) {
  if (n & (VEC_INTEGRAL_SIZE-1))
    v[n++] = a;
  else if (!v)
    (v = e)[n++] = a;
  else
    add_internal(a);
}

template <class C> inline C&
Vec<C>::add() {
  C *ret;
  if (n & (VEC_INTEGRAL_SIZE-1))
    ret = &v[n++];
  else if (!v)
    ret = &(v = e)[n++];
  else
    ret = &add_internal();
  return *ret;
}

template <class C> inline C
Vec<C>::pop() {
  n--;
  C ret = v[n];
  if (!n)
    clear();
  return ret;
}

template <class C> inline void
Vec<C>::clear() {
  v = NULL;
  n = 0;
  i = 0;
}

template <class C> inline void
Vec<C>::set_clear() {
  memset(v, 0, n * sizeof(C));
}

template <class C> inline C *
Vec<C>::set_add(C a) {
  if (n < VEC_INTEGRAL_SIZE) {
    for (C *c = v; c < v + n; c++)
      if (*c == a)
        return 0;
    add(a);
    return &v[n-1];
  }
  if (n == VEC_INTEGRAL_SIZE) {
    Vec<C> vv(*this);
    clear();
    for (C *c = vv.v; c < vv.v + vv.n; c++)
      set_add_internal(*c);
  }
  return set_add_internal(a);
}

template <class C> inline int
Vec<C>::count() {
  int x = 0;
  for (C *c = v; c < v + n; c++)
    if (*c)
      x++;
  return x;
}

template <class C> inline C*
Vec<C>::in(C a) {
  for (C *c = v; c < v + n; c++)
    if (*c == a)
      return c;
  return NULL;
}

template <class C> inline int
Vec<C>::add_exclusive(C a) {
  if (!in(a)) {
    add(a);
    return 1;
  } else 
    return 0;
}

template <class C> inline C *
Vec<C>::set_in(C a) {
  if (n <= VEC_INTEGRAL_SIZE)
    return in(a);
  return set_in_internal(a);
}

template <class C> inline C
Vec<C>::first() {
  for (C *c = v; c < v + n; c++)
    if (*c)
      return *c;
  return 0;
}

template <class C> inline int
Vec<C>::index(C a) {
  for (C *c = v; c < v + n; c++)
    if (*c == a)
      return c - v;
  return -1;
}

template <class C> inline void 
Vec<C>::move(Vec<C> &vv)  {
  n = vv.n;
  i = vv.i;
  v = vv.v;
  if (vv.v == &vv.e[0]) { 
    memcpy(e, &vv.e[0], sizeof(e));
    v = e;
  }
  vv.clear();
}

template <class C> inline void 
Vec<C>::copy(const Vec<C> &vv)  {
  n = vv.n;
  i = vv.i;
  if (vv.v == &vv.e[0]) { 
    memcpy(e, &vv.e[0], sizeof(e));
    v = e;
  } else {
    if (vv.v) 
      copy_internal(vv);
    else
      v = 0;
  }
}

template <class C> inline void 
Vec<C>::fill(int nn)  {
  for (int i = n; i < nn; i++)
    add() = 0;
}

template <class C> inline void
Vec<C>::append(const Vec<C> &vv)  {
  for (C *c = vv.v; c < vv.v + vv.n; c++)
    if (*c)
      add(*c);
}

template <class C> inline void 
Vec<C>::addx() {
  if (v == e) {
    v = (C*)MALLOC(VEC_INITIAL_SIZE * sizeof(C));
    memcpy(v, &e[0], n * sizeof(C));
  } else {
    if ((n & (VEC_INITIAL_SIZE -1)) == 0) {
      int l = n, nl = (1 + VEC_INITIAL_SHIFT);
      l = l >> VEC_INITIAL_SHIFT;
      while (!(l&1)) { l = l >> 1; nl++; }
      l = l >> 1;
      if (!l) {
        void *vv = (void*)v;
        nl = 1 << nl;
        v = (C*)MALLOC(nl * sizeof(C));
        memcpy(v, vv, n * sizeof(C));
        memset(&v[n], 0, (nl - n) * sizeof(C));
      }
    }
  }
}

template <class C> void 
Vec<C>::add_internal(C a) {
  addx();
  v[n++] = a;
}

template <class C> C&
Vec<C>::add_internal() {
  addx();
  return v[n++];
}

template <class C> void
Vec<C>::set_expand() {
  if (!n)
    i = SET_INITIAL_INDEX;
  else
    i = i + 1;
  n = prime2[i];
  v = (C*)MALLOC(n * sizeof(C));
  memset(v, 0, n * sizeof(C));
}

template <class C> C *
Vec<C>::set_add_internal(C c) {
  int j, k;
  if (n) {
    uint h = (uint)(uintptr_t)c;
    h = h % n;
    for (k = h, j = 0;
         k < n && j < SET_MAX_SEQUENTIAL;
         k = ((k + 1) % n), j++)
    {
      if (!v[k]) {
        v[k] = c;
        return &v[k];
      } else if (v[k] == c)
        return 0;
    }
  }
  Vec<C> vv(*this);
  set_expand();
  if (vv.v)
    set_union(vv);
  return set_add(c);
}

template <class C> C *
Vec<C>::set_in_internal(C c) {
  int j, k;
  if (n) {
    uint h = (uint)(uintptr_t)c;
    h = h % n;
    for (k = h, j = 0;
         k < n && j < SET_MAX_SEQUENTIAL;
         k = ((k + 1) % n), j++)
    {
      if (!v[k])
        return 0;
      else if (v[k] == c)
        return &v[k];
    }
  }
  return 0;
}

template <class C> int
Vec<C>::set_union(Vec<C> &vv) {
  int changed = 0;
  for (int i = 0; i < vv.n; i++)
    if (vv.v[i])
      changed = set_add(vv.v[i]) || changed;
  return changed;
} 

template <class C> int
Vec<C>::set_intersection(Vec<C> &vv) {
  Vec<C> tv;
  tv.move(*this);
  int changed = 0;
  for (int i = 0; i < tv.n; i++)
    if (tv.v[i]) {
      if (vv.set_in(tv.v[i]))
        set_add(tv.v[i]);
      else
        changed = 1;
    }
  return changed;
} 

template <class C> int
Vec<C>::some_intersection(Vec<C> &vv) {
  for (int i = 0; i < n; i++)
    if (v[i])
      if (vv.set_in(v[i]))
        return 1;
  return 0;
} 

template <class C> int
Vec<C>::some_disjunction(Vec<C> &vv) {
  for (int i = 0; i < n; i++)
    if (v[i])
      if (!vv.set_in(v[i]))
        return 1;
  for (int i = 0; i < vv.n; i++)
    if (vv.v[i])
      if (!set_in(vv.v[i]))
        return 1;
  return 0;
} 

template <class C> void
Vec<C>::set_intersection(Vec<C> &vv, Vec<C> &result) {
  for (int i = 0; i < n; i++)
    if (v[i])
      if (vv.set_in(v[i]))
        result.set_add(v[i]);
} 

template <class C> void
Vec<C>::set_disjunction(Vec<C> &vv, Vec<C> &result) {
  for (int i = 0; i < n; i++)
    if (v[i])
      if (!vv.set_in(v[i]))
        result.set_add(v[i]);
  for (int i = 0; i < vv.n; i++)
    if (vv.v[i])
      if (!set_in(vv.v[i]))
        result.set_add(vv.v[i]);
} 

template <class C> void
Vec<C>::set_difference(Vec<C> &vv, Vec<C> &result) {
  for (int i = 0; i < n; i++)
    if (v[i])
      if (!vv.set_in(v[i]))
        result.set_add(v[i]);
} 

template <class C> int
Vec<C>::some_difference(Vec<C> &vv) {
  for (int i = 0; i < n; i++)
    if (v[i])
      if (!vv.set_in(v[i]))
        return 1;
  return 0;
} 

template <class C> int
Vec<C>::set_count() {
  int x = 0;
  for (int i = 0; i < n; i++)
    if (v[i])
      x++;
  return x;
} 

template <class C> void
Vec<C>::set_to_vec() {
  Vec<C> vv(*this);
  clear();
  for (C *c = vv.v; c < vv.v + vv.n; c++)
    if (*c)
      add(*c);
}

template <class C> void
Vec<C>::vec_to_set() {
  Vec<C> vv(*this);
  clear();
  for (C *c = vv.v; c < vv.v + vv.n; c++)
    set_add(*c);
}

template <class C>  void 
Vec<C>::reverse() {
  for (int i = 0; i < n/2; i++) {
    C *s = &v[i], *e = &v[n - 1 - i];
    C t;
    memcpy(&t, s, sizeof(t));
    memcpy(s, e, sizeof(t));
    memcpy(e, &t, sizeof(t));
  }
}

template <class C> void 
Vec<C>::copy_internal(const Vec<C> &vv) {
  int l = n, nl = (1 + VEC_INITIAL_SHIFT);
  l = l >> VEC_INITIAL_SHIFT;
  while (l) { l = l >> 1; nl++; }
  nl = 1 << nl;
  v = (C*)MALLOC(nl * sizeof(C));
  memcpy(v, vv.v, n * sizeof(C));
  memset(v + n, 0, (nl - n) * sizeof(C)); 
}

void test_vec();

#endif
