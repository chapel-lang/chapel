/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _var_H_
#define _var_H_

class AVar;
class CreationSet;
class AType;
class Sym;
class PNode;
class CreationSet;
typedef MapElem<void *, AVar *> AVarMapElem;
typedef Map<void *,AVar*> AVarMap;

extern int var_id;

class Var : public gc {
 public:
  Sym			*sym;
  int			id;
  Sym			*type;
  int			mark;	// used by ssu.cpp
  PNode			*def;
  AVarMap		avars;	// used by fa.cpp
  CreationSet		*as_CreationSet; // used by fa.cpp
  unsigned int		clone_for_constants : 1;
  char			*cg_string; // used by cg.cpp

  Var *copy();
  Var(Sym *s) : sym(s), mark(0), def(0), as_CreationSet(0),
    clone_for_constants(0), cg_string(0) { id = var_id++; }
};
#define forv_Var(_p, _v) forv_Vec(Var, _p, _v)
#define forv_AVarMapElem(_p, _v) forv_Vec(AVarMapElem, _p, _v)

#define Var_is_local(_v, _f) (!(_v)->sym->is_constant && (_v)->sym->in == (_f)->sym && \
  !(_f)->init_function && !(_v)->sym->is_lvalue && !(_v)->sym->is_var)

typedef Map<Var *, Var *> VarMap;

int compar_vars(const void *ai, const void *aj);


#endif
