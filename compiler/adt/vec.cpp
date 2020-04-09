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

This code is based on vec.cc developed by John Plevyak and released as
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


/* UnionFind by Tarjan (adapted) */

#include "misc.h"
#include "vec.h"

unsigned int prime2[] = {
  1, 3, 7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
  16381, 32749, 65521, 131071, 262139, 524287, 1048573, 2097143,
  4194301, 8388593, 16777213, 33554393, 67108859, 134217689,
  268435399, 536870909
};

template<>
uintptr_t _vec_hasher(const char* obj) {
  uintptr_t h = 0;
  while (obj != NULL && *obj) h = h * 27 + (unsigned char)*obj++;
  return h;
}
template<>
uintptr_t _vec_hasher(unsigned int obj) {
  return obj;
}
template<>
uintptr_t _vec_hasher(int obj) {
  return obj;
}
  
// binary search over intervals
static int
i_find(Intervals *i, int x) {
  INT_ASSERT(i->n);
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
    INT_ASSERT(x < v[l]);
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
