/*
  Copyright 2003 John Plevyak
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
