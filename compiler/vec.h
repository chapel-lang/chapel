/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#ifndef _vec_H_
#define _vec_H_

// Simple Vector class, also supports open hashed sets

#define VEC_INTEGRAL_SIZE	4		/* must be a power of 2 */
#define VEC_INITIAL_SHIFT	3
#define VEC_INITIAL_SIZE	(1 << VEC_INITIAL_SHIFT)

#define SET_MAX_SEQUENTIAL	5
#define SET_INITIAL_INDEX	2

template <class C> class Vec : public gc {
 public:
  int		n;
  int		i;	// size index for sets
  C  		*v;
  C  		e[VEC_INTEGRAL_SIZE];
  
  Vec<C>();
  Vec<C>(const Vec<C> &vv);

  inline void add(C a);
  inline int add_exclusive(C a);
  inline C& add();
  inline void clear();
  inline void set_clear();
  inline C *set_add(C a);
  C *set_add_internal(C a);
  int set_union(Vec<C> &v);
  int set_intersection(Vec<C> &v);
  int some_intersection(Vec<C> &v);
  int some_disjunction(Vec<C> &v);
  void set_difference(Vec<C> &v, Vec<C> &result);
  int set_count();
  inline C *in(C a);
  inline C *set_in(C a);
  C *set_in_internal(C a);
  void set_expand();
  inline int index(C a);
  void set_to_vec();
  inline void move(Vec<C> &v);
  void copy_internal(const Vec<C> &v);
  inline void copy(const Vec<C> &v);
  inline void fill(int n);
  inline void append(const Vec<C> &v);
  void reverse();
  C* end() { return v + n; }

 private:
  void add_internal(C a);
  C& add_internal();
  void addx();
};

#define forv_Vec(_c, _p, _v) if ((_v).n) for (_c *qq__##_p = (_c*)0, *_p = (_v).v[0]; \
                    ((int)(qq__##_p) < (_v).n) && ((_p = (_v).v[(int)qq__##_p]) || 1); qq__##_p = (_c*)(((int)qq__##_p) + 1))


extern uint prime2[];

/* IMPLEMENTATION */

template <class C> inline
Vec<C>::Vec<C>() : n(0), i(0), v(0) {
}

template <class C> inline
Vec<C>::Vec<C>(const Vec<C> &vv) {
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
  if (vv.v == vv.e) { 
    memcpy(e, vv.e, sizeof(e));
    v = e;
  }
  vv.clear();
}

template <class C> inline void 
Vec<C>::copy(const Vec<C> &vv)  {
  n = vv.n;
  i = vv.i;
  if (vv.v == vv.e) { 
    memcpy(e, vv.e, sizeof(e));
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
    add(*c);
}


void test_vec();

#endif
