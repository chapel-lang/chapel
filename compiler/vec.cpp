/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

uint prime2[] = {
  1, 3, 7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
  16381, 32749, 65521, 131071, 262139, 524287, 1048573, 2097143,
  4194301, 8388593, 16777213, 33554393, 67108859, 134217689,
  268435399, 536870909
};
  
template <class C> inline void 
Vec<C>::addx() {
  if (v == e) {
    v = (C*)MALLOC(VEC_INITIAL_SIZE * sizeof(C));
    memcpy(v, e, n * sizeof(C));
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
	memcpy(v, vv, nl * sizeof(C));
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
    uint h = ((uint)c);
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
    uint h = ((uint)c);
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
Vec<C>::set_count() {
  int n = 0;
  for (int i = 0; i < n; i++)
    if (v[i])
      n++;
  return n;
} 

template <class C> void
Vec<C>::set_to_vec() {
  Vec<C> vv(*this);
  clear();
  for (C *c = vv.v; c < vv.v + vv.n; c++)
    if (*c)
      add(*c);
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

#ifdef TEST_LIB
void
test_vec() {
  Vec<void *> v, vv, vvv;
  int tt = 99 * 50, t = 0;
  
  for (int i = 0; i < 100; i++)
    v.add((void*)i);
  for (int i = 0; i < 100; i++)
    t += (int)v.v[i];
  assert(t == tt);

  t = 0;
  for (int i = 1; i < 100; i++)
    vv.set_add((void*)i);
  for (int i = 1; i < 100; i++)
    vvv.set_add((void*)i);
  for (int i = 1; i < 100; i++)
    vvv.set_add((void*)(i * 1000));
  vv.set_union(vvv);
  for (int i = 0; i < vv.n; i++)
    if (vv.v[i])
      t += (int)vv.v[i];
  assert(t == tt + 1000 * tt);
}
#endif

template class Vec<Sym *>;
template class Vec<Code *>;
template class Vec<Label *>;
template class Vec<Fun*>;
template class Vec<PNode*>;
template class Vec<CQClass*>;
template class Vec<Region*>;
template class Vec<UNode*>;
template class Vec<UEdge*>;
template class Vec<Var*>;
template class Vec<PNode **>;
template class Vec<EntrySet *>;
template class Vec<AEdge *>;
template class Vec<AVar *>;
template class Vec<MapElem<void *, AVar*> >;
template class Vec<MapElem<Var*, List<Var *> *> >;
template class Vec<MapElem<uint, List<char*> > >;
template class Vec<MapElem<char *, Prim*> >;
template class Vec<MapElem<char *, Sym *> >;
template class Vec<CreationSet *>;
template class Vec<MapElem<Sym *, AType*> >;
template class Vec<MapElem<CreationSet*, AType*> >;
template class Vec<MapElem<Var*, AType*> >;
template class Vec<MapElem<AVar*, CreationSet*> >;
template class Vec<MapElem<AType*, AType*> >;
template class Vec<MapElem<char *, Vec<Fun *> *> >;
template class Vec<Map<char *, Vec<Fun *>*>*>;
template class Vec<MapElem<unsigned int, List<AType *> > >;
template class Vec<MapElem<char *, Var *> >;
template class Vec<MapElem<Fun *, AEdge *> >;
template class Vec<MapElem<PNode *, Map <Fun *, AEdge *> *> >;
template class Vec<MapElem<PNode *, int> >;
template class Vec<char *>;
template class Vec<Vec<EntrySet *> *>;
template class Vec<AType *>;
template class Vec<Vec<AEdge *> *>;
template class Vec<Vec<CreationSet *> *>;
template class Vec<MapElem<char *, Vec<Map<char *, Vec<Fun *> *> *> *> >;
template class Vec<AST *>;
template class Vec<Scope *>;
template class Vec<MapElem<char *, AST *> >;
template class Vec<MapElem<Var *, Var *> >;
template class Vec<MapElem<PNode *, PNode *> >;
template class Vec<MapElem<PNode *, ATypeViolation *> >;
template class Vec<Prim *>;
template class Vec<ATypeViolation *>;
template class Vec<MapElem<PNode *, Vec<Fun *> *> >;
template class Vec<CallPoint *>;
