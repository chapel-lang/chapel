/* -*-Mode: c++;-*-
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
typedef Map<void *, AVar*> AVarMap;
typedef Map<Var *, Var*> VarMap;

extern int var_id;

class Var : public gc {
 public:
  Sym                   *sym;
  int                   id;
  Sym                   *type;
  int                   mark;   // used by ssu.cpp
  PNode                 *def;
  AVarMap               avars;  // used by fa.cpp
  CreationSet           *as_CreationSet; // used by fa.cpp
  unsigned int          is_internal:1;
  unsigned int          is_filtered:1;
  unsigned int          is_formal:1;
  char                  *cg_string; // used by cg.cpp

  Var *copy();
  Var(Sym *s);
};
#define forv_Var(_p, _v) forv_Vec(Var, _p, _v)
#define forv_AVarMapElem(_p, _v) forv_Vec(AVarMapElem, _p, _v)

#define Var_is_local(_v, _f)            \
  (!(_v)->sym->is_constant &&           \
   (_v)->sym->in == (_f)->sym &&        \
   !(_v)->sym->nesting_depth &&         \
   !(_v)->sym->is_lvalue &&             \
   !(_v)->sym->is_var)

void pp(Var *);

#endif
