/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _var_H_
#define _var_H_

// Variables
class AVar;
class CreationSet;
class AType;
typedef MapElem<void *, AVar *> AVarMapElem;
typedef Map<void *,AVar*> AVarMap;

class Var : public gc {
 public:
  Sym			*sym;
  Sym			*type;
  int			mark;	// used by ssu.cpp
  PNode			*def;
  AVarMap		avars;	// used by fa.cpp
  CreationSet		*as_CreationSet; // used by fa.cpp
  AType			*as_AType; // used by fa.cpp
  uint			clone_for_constants : 1;
  char			*cg_string; // used by cg.cpp

  Var *copy();
  Var(Sym *s) : sym(s), mark(0), def(0), as_CreationSet(0), as_AType(0),
    clone_for_constants(0), cg_string(0) {}
};
#define forv_Var(_p, _v) forv_Vec(Var, _p, _v)
#define forv_AVarMapElem(_p, _v) forv_Vec(AVarMapElem, _p, _v)

#define Var_is_local(_v, _f) (!(_v)->sym->constant && (_v)->sym->in == (_f)->sym && \
  !(_f)->init_function && !(_v)->sym->lvalue)

typedef Map<Var *, Var *> VarMap;

#endif
