/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

/* UnionFind by Tarjan (adapted) */

#include "geysa.h"
#include "pattern.h"

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

// binary search over intervals
static int
i_find(Intervals *i, int x) {
  assert(i->n);
  int l = 0, h = i->n;
 Lrecurse:
  if (h <= l + 2) {
    if (h <= l)
      return -(l + 1);
    if (x < i->v[l] || x > i->v[l + 1])
      return -(l + 1);
    return h;
  }
  int m = (((h - l) / 4) * 2) + l;
  if (x > i->v[m + 1]) {
    l = m;
    goto Lrecurse;
  }
  if (x < i->v[m]) {	
    h = m;
    goto Lrecurse;
  }
  return (l + 1);
}

int 
Intervals::in(int x) {
  if (!n)
    return 0;
  if (i_find(this, x) > 0)
    return 1;
  return 0;
}

// insert into interval with merge
void 
Intervals::insert(int x) {
  if (!n) {
    add(x);
    add(x);
    return;
  }
  int l = i_find(this, x);
  if (l > 0)
    return;
  l = -l - 1;

  if (x > v[l + 1]) {
    if (x == v[l + 1] + 1) {
      v[l + 1]++;
      goto Lmerge;
    }
    l += 2;
    if (l < n) {
      if (x == v[l] - 1) {
	v[l]--;
	goto Lmerge;
      }
    }
    goto Lmore;
  } else {
    assert(x < v[l]);
    if (x == v[l] - 1) {
      v[l]--;
      goto Lmerge;
    }
    if (!l)
      goto Lmore;
    l -= 2;
    if (x == v[l + 1] + 1) {
      v[l + 1]++;
      goto Lmerge;
    }
  }
 Lmore:
  fill(n + 2);
  if (n - 2 - l > 0)
    memmove(v + l + 2, v + l, sizeof(int) * (n - 2 - l));
  v[l] = x;
  v[l+1] = x;
  return;
 Lmerge:
  if (l) {
    if (v[l] - v[l-1] < 2) {
      l -= 2;
      goto Ldomerge;
    }
  }
  if (l < n - 2) {
    if (v[l + 2] - v[l + 1] < 2)
      goto Ldomerge;
  }
  return;
 Ldomerge:
  memmove(v + l + 1, v + l + 3, sizeof(int) * (n - 3 - l));
  n -= 2;
  goto Lmerge;
}

void
UnionFind::size(int s) {
  int nn = n;
  fill(s);
  for (int i = nn; i < n; i++)
    v[i] = -1;
}

int
UnionFind::find(int n) {
  int i, t;
  for (i = n; v[i] >= 0; i = v[i]);
  while (v[n] >= 0) {
    t = n;
    n = v[n];
    v[t] = i;
  }
  return i;
}

void 
UnionFind::unify(int n, int m) {
  n = find(n);
  m = find(m);
  if (n != m) {
    if (v[m] < v[n]) {
      v[m] += (v[n] - 1);
      v[n] = m;
    } else {
      v[n] += (v[m] - 1);
      v[m] = n;
    }
  }
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
  
  Intervals in;
  in.insert(1);
  assert(in.n == 2);
  in.insert(2);
  assert(in.n == 2);
  in.insert(6);
  assert(in.n == 4);
  in.insert(7);
  assert(in.n == 4);
  in.insert(9);
  assert(in.n == 6);
  in.insert(4);
  assert(in.n == 8);
  in.insert(5);
  assert(in.n == 6);
  in.insert(3);
  assert(in.n == 4);
  in.insert(8);
  assert(in.n == 2);

  UnionFind uf;
  uf.size(4);
  uf.unify(0,1);
  uf.unify(2,3);
  assert(uf.find(2) == uf.find(3));
  assert(uf.find(0) == uf.find(1));
  assert(uf.find(0) != uf.find(3));
  assert(uf.find(1) != uf.find(3));
  assert(uf.find(1) != uf.find(2));
  assert(uf.find(0) != uf.find(2));
  uf.unify(1,2);
  assert(uf.find(0) == uf.find(3));
  assert(uf.find(1) == uf.find(3));
}
#endif

class CDB_CreationSet;
class CDB_EntrySet;

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
template class Vec<Prim *>;
template class Vec<ATypeViolation *>;
template class Vec<MapElem<PNode *, Vec<Fun *> *> >;
template class Vec<CallPoint *>;
template class Vec<Vec<Fun *> *>;
template class Vec<Dom *>;
template class Vec<int>;
template class Vec<LoopNode *>;
template class Vec<Vec<LoopNode *> *>;
template class Vec<MapElem<unsigned int, List<ATypeFold *> > >;
template class Vec<CDB_CreationSet *>;
template class Vec<CDB_EntrySet *>;
template class Vec<MapElem<Sym *, Vec<CDB_CreationSet *> *> >;
template class Vec<MapElem<char *, Fun *> >;
template class Vec<SettersClasses *>;
template class Vec<MapElem<unsigned int, List<SettersClasses *> > >;
template class Vec<Setters *>;
template class Vec<MapElem<unsigned int, List<Setters *> > >;
template class Vec<MapElem<char *, AVar *> >;
template class Vec<MapElem<AVar *, AVar *> >;
template class Vec<MapElem<AVar *, Setters *> >;
template class Vec<MType *>;
template class Vec<MapElem<unsigned int, List<MPosition *> > >;
template class Vec<MapElem<MPosition *, Vec<Fun *> *> >;
template class Vec<Match *>;
template class Vec<MapElem<Fun *, Match *> >;
template class Vec<MapElem<MPosition *, AType *> >;
template class Vec<MapElem<MPosition *, AVar *> >;
template class Vec<MPosition *>;
template class Vec<MapElem<MPosition *, Sym *> >;
template class Vec<MapElem<unsigned int, List<ATypeViolation *> > >;


