#ifndef _fa_H_
#define _fa_H_

#if !defined(__FreeBSD__) || (__FreeBSD_version >= 500000)
#endif
#include <sys/types.h>
#include "map.h"
#include "sym.h"
#include "extern.h"

#define DEFAULT_NUM_CONSTANTS_PER_VARIABLE	0

#define GLOBAL_CONTOUR ((void*)1)

class Prim;
class RegisteredPrim;
class Fun;
class PNode;
class PDB;
class Edge;
class AVar;
class AEdge;
class AType;
class SettersClasses;
class SettersHashFns;
class Setters;
class CreationSet;
class ATypeViolation;
class CDB;
class Patterns;
class Match;
class MPosition;

typedef Map<PNode *, Map<Fun *, AEdge *> *> EdgeMap;
typedef BlockHash<AEdge *, PointerHashFns> EdgeHash;
typedef Vec<CreationSet *> VecCreationSet;
typedef Vec<Vec<CreationSet *> *> CSSS;

class AType : public Vec<CreationSet *> { 
 public:
  uint 			hash;
  AType			*top;
  Vec<CreationSet *>	sorted;
  Map<AType *, AType *> union_map;
  Map<AType *, AType *> intersection_map;

  AType(CreationSet *cs);
  AType(AType &a);
  AType() : hash(0), top(0) {}

  AType *constants();
};
#define forv_AType(_p, _v) forv_Vec(AType, _p, _v)

class EntrySet : public gc {
 public:
  Fun			*fun;
  uint			dfs_color : 2;
  Map<MPosition*,AVar*> args;
  Vec<AVar *>		rets;
  EdgeHash		edges;
  EdgeMap		out_edge_map;
  Vec<AEdge *>		out_edges;
  Vec<AEdge *>		backedges;
  Vec<AEdge *>		es_cs_backedges;
  Vec<CreationSet *>	cs_backedges;
  Vec<CreationSet *>	creates;
  EntrySet		*split;
  Vec<EntrySet *>	*equiv;		// clone.cpp

  EntrySet(Fun *af): fun(af), equiv(0) {}
};
#define forv_EntrySet(_p, _v) forv_Vec(EntrySet, _p, _v)

class CreationSet : public gc {
 public:
  Sym 			*sym;
  int			id;
  uint			clone_for_constants : 1;
  uint			added_element_var : 1;
  uint			dfs_color : 2;
  Vec<AVar *> 		defs;
  AType 		*atype; 	// the type that this creation set belongs to
  Vec<AVar *>		vars;
  Map<char *, AVar *>	var_map;
  Vec<EntrySet *>	ess;		// entry sets restricted by this creation set
  Vec<EntrySet *>	es_backedges;	// entry sets restricted by this creation set
  CreationSet		*split;		// creation set this one was split from
  Vec<CreationSet *>	*equiv;		// used by clone.cpp & fa.cpp
  Vec<CreationSet *>	not_equiv;	// used by clone.cpp
  Sym 			*type;		// used by clone.cpp & fa.capp

  CreationSet(Sym *s);
  CreationSet(CreationSet *cs);
};
#define forv_CreationSet(_p, _v) forv_Vec(CreationSet, _p, _v)

class SettersClasses : public Vec<Setters *> {
 public:
  uint					hash;
  Vec<Setters *>			sorted;
  BlockHash<Setters *, SettersHashFns>	used_by;
};
#define forv_SettersClasses(_p, _v) forv_Vec(SettersClasses, _p, _v)

class Setters : public Vec<AVar *> {
 public:
  uint			hash;
  Vec<AVar *>		sorted;
  SettersClasses	*eq_classes;
  Map<AVar *, Setters*>	add_map;

  Setters() : hash(0), eq_classes(0) { }
};
#define forv_Setters(_p, _v) forv_Vec(Setters, _p, _v)

class AVar : public gc {
 public:
  Var				*var;
  int				id;
  void				*contour;
  Vec<AVar *>			forward;
  Vec<AVar *>			backward;
  AVar				*lvalue;
  AVar				*copy_of;
  AType 			*in;
  AType 			*out;
  AType				*restrict;
  AVar				*container;
  Setters			*setters;
  Setters			*setter_class;
  CreationSet 			*creation_set;
  uint				in_send_worklist:1;
  uint				contour_is_entry_set:1;
  uint				is_lvalue:1;
  uint				is_dead:1;
  Vec<AVar *>			arg_of_send;
  Link<AVar>			send_worklist_link;

  AVar(Var *v, void *acontour);
};
#define forv_AVar(_p, _v) forv_Vec(AVar, _p, _v)

class AEdge : public gc {
 public:
  EntrySet		*from, *to;
  PNode			*pnode;
  Map<MPosition*,AVar*> args;
  Map<MPosition*,AVar*> filtered_args;
  Vec<AVar *>		rets;
  Match			*match;
  uint			in_edge_worklist : 1;
  uint			es_backedge : 1;
  uint			es_cs_backedge : 1;
  Link<AEdge>		edge_worklist_link;

  AEdge() : from(0), to(0), pnode(0), match(0), in_edge_worklist(0) {}
};
#define forv_AEdge(_p, _v) forv_Vec(AEdge, _p, _v)
typedef MapElem<MPosition *, AVar *> MapMPositionAVar;
#define form_MPositionAVar(_p, _v) form_Map(MapMPositionAVar, _p, _v)

class ATypeChainHashFns {
 public:
  static uint hash(AType *a) { return a->hash; }
  static int equal(AType *a, AType *b) {
    if (a->sorted.n != b->sorted.n)
      return 0;
    for (int i = 0; i < a->sorted.n; i++)
      if (a->sorted.v[i] != b->sorted.v[i])
	return 0;
    return 1;
  }
};

class SettersHashFns {
 public:
  static uint hash(Setters *a) { return a->hash; }
  static int equal(Setters *a, Setters *b) {
    if (a->sorted.n != b->sorted.n)
      return 0;
    for (int i = 0; i < a->sorted.n; i++)
      if (a->sorted.v[i] != b->sorted.v[i])
	return 0;
    return 1;
  }
};

class SettersClassesHashFns {
 public:
  static uint hash(SettersClasses *a) { return a->hash; }
  static int equal(SettersClasses *a, SettersClasses *b) {
    if (a->sorted.n != b->sorted.n)
      return 0;
    for (int i = 0; i < a->sorted.n; i++)
      if (a->sorted.v[i] != b->sorted.v[i])
	return 0;
    return 1;
  }
};

enum ATypeViolation_kind {
  ATypeViolation_PRIMITIVE_ARGUMENT,
  ATypeViolation_SEND_ARGUMENT,
  ATypeViolation_DISPATCH_AMBIGUITY,
  ATypeViolation_MEMBER,
  ATypeViolation_MATCH,
  ATypeViolation_NOTYPE,
  ATypeViolation_BOXING
};
  
class ATypeViolation : public gc {
 public:
  ATypeViolation_kind kind;
  AVar *av;
  AVar *send;
  AType *type;
  Vec<Fun *> *funs;

  ATypeViolation(ATypeViolation_kind akind, AVar *aav, AVar *asend) 
    : kind(akind), av(aav), send(asend), type(0), funs(0) {}
};
#define forv_ATypeViolation(_p, _v) forv_Vec(ATypeViolation, _p, _v)

class ATypeViolationHashFuns {
 public:
  static uint hash(ATypeViolation *x) { 
    return (uint)((uint)x->kind + (13 * (uintptr_t)x->av) + (100003 * (uintptr_t)x->send));
  }
  static int equal(ATypeViolation *x, ATypeViolation *y) {
    return x->kind == y->kind && x->av == y->av && x->send == y->send;
  }
};

class ATypeFold : public gc {
 public:
  Prim *p;
  AType *a;
  AType *b;
  AType *result;

  ATypeFold(Prim *ap, AType *aa, AType *ab, AType *aresult = 0) : p(ap), a(aa), b(ab), result(aresult) {}
};
#define forv_ATypeFold(_p, _v) forv_Vec(ATypeFold, _p, _v)

class ATypeFoldChainHashFns {
 public:
  static uint hash(ATypeFold *x) { 
    return (uint)((uintptr_t)x->p + (1009 * (uintptr_t)x->a) + (100003 * (uintptr_t)x->b));
  }
  static int equal(ATypeFold *x, ATypeFold *y) {
    return x->p == y->p && x->a == y->a && x->b == y->b;
  }
};

typedef void (*PrimitiveTransferFunctionPtr)(PNode *pn, EntrySet *es);

class FA : public gc {
 public:
  PDB *pdb;
  CDB *cdb;
  Patterns *patterns;
  Vec<Fun *> funs;
  AEdge *top_edge;
  Vec<EntrySet *> ess;		// all used entry sets as array
  Vec<EntrySet *> ess_set;	// all used entry sets as set
  Vec<Sym *> basic_types;
  Vec<CreationSet *> css, css_set;
  Vec<AVar *> global_avars;
  ChainHashMap<char*, StringHashFns, RegisteredPrim*> primitive_transfer_functions;
  
  bool permit_boxing;

  FA(PDB *apdb) : pdb(apdb), cdb(0), patterns(0), top_edge(0),
    permit_boxing(0) {}

  int analyze(Fun *f);
  int concretize();
};

AVar *make_AVar(Var *, EntrySet *);
Sym *coerce_num(Sym *, Sym *);
Sym *type_info(AST *a, Sym *s = 0);
void call_info(Fun *f, AST *a, Vec<Fun *> &funs);
int constant_info(AST *a, Vec<Sym *> &constants, Sym *s);
int constant_info(Var *v, Vec<Sym *> &constants);
AType *make_AType(Vec<CreationSet *> &css);
AType *make_abstract_type(Sym *s);
void update_in(AVar *v, AType *t);
void flow_vars(AVar *v, AVar *vv);
CreationSet *creation_point(AVar *v, Sym *s);
void type_violation(ATypeViolation_kind akind, AVar *av, AType *type, AVar *send,
		    Vec<Fun *> *funs = NULL);
AType *type_cannonicalize(AType *t);
AType *type_diff(AType *, AType *);
AType *type_intersection(AType *, AType *);
AType *type_union(AType *a, AType *b);
void log_var_types(Var *, Fun *);
int compar_creation_sets(const void *, const void *);
void set_container(AVar *av, AVar *container);
AVar * unique_AVar(Var *v, void *contour);
AVar *unique_AVar(Var *v, EntrySet *es);
void qsort_pointers(void **left, void **right);

extern AType *bottom_type;
extern AType *void_type;
extern AType *unknown_type;
extern AType *top_type;
extern AType *any_type;
extern AType *bool_type;
extern AType *size_type;
extern AType *anyint_type;
extern AType *anynum_kind;
extern AType *fun_type;
extern AType *symbol_type;
extern AType *fun_symbol_type;
extern AType *anyclass_type;

EXTERN int num_constants_per_variable EXTERN_INIT(DEFAULT_NUM_CONSTANTS_PER_VARIABLE);

#endif
