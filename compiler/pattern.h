/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _pattern_H_
#define _pattern_H_

#include <sys/types.h>
#include "sym.h"
#include "fa.h"

#define int2Position(_i) ((void*)((intptr_t)-(_i)))
#define Position2int(_p) (-((intptr_t)_p))

class MPosition : public gc {
 public:
  Vec<void*> pos;
  MPosition *parent;
  void set_top(void *p) { pos.v[pos.n-1] = p; }
  void push(int i) { pos.add(int2Position(i)); }
  void push(void *p) { pos.add(p); }
  void pop() { pos.n--; }
  void inc() { pos.v[pos.n-1] = int2Position(Position2int(pos.v[pos.n-1]) + 1); }
  void dec() { pos.v[pos.n-1] = int2Position(Position2int(pos.v[pos.n-1]) - 1); }
  MPosition() : parent(0) {}
  MPosition(MPosition &p);
};
#define forv_MPosition(_p, _v) forv_Vec(MPosition, _p, _v)

class MPositionHashFuns : public gc {
 public:
  static uint hash(MPosition *x) {
    uint h = 1;
    for (int i = 0; i < x->pos.n; i++)
      h += ((uintptr_t)x->pos.v[i]) * open_hash_multipliers[i];
    if (!h)
      h = 1;
    return h;
  }
  static int equal(MPosition *x, MPosition *y) {
    if (x->pos.n != y->pos.n)
      return 0;
    for (int i = 0; i < x->pos.n; i++)
      if (x->pos.v[i] != y->pos.v[i])
	return 0;
    return 1;
  }
};

class MType : public gc {
 public:
  Map<MPosition *, Vec<Fun *> *> funs;
};

class Patterns : public gc {
 public:
  Vec<Sym *>		types;
  Vec<Sym *>		types_set;
  Vec<MType *>		mtypes; 
};

class Match : public gc {
 public:
  Fun *fun;
  Map<MPosition *, AType *> filters;
  Match(Fun *afun) : fun(afun) {}
};
#define forv_Match(_p, _v) forv_Vec(Match, _p, _v)


void build_patterns(FA *fa);
void build_positions(FA *fa);
int pattern_match(FA *fa, Vec<AVar *> &args, Vec<Match *> &matches, AVar *send);
MPosition *cannonicalize_mposition(MPosition &p);

#endif
