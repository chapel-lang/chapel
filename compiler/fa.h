/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _fa_H_
#define _fa_H_

#define GLOBAL_CONTOUR ((void*)1)

class Fun;
class PNode;
class PDB;
class Edge;
class AVar;
class AEdge;
class AType;
class CreationSet;

typedef Map<PNode *, Map<Fun *, AEdge *> *> EdgeMap;
typedef BlockHash<AEdge *, PointerHashFns> EdgeHash;
typedef Vec<CreationSet *> VecCreationSet;

class AType : public Vec<CreationSet *> { 
 public:
  uint 			hash;
  Vec<CreationSet *>	sorted;
  Map<AType *, AType *> union_map;
  Map<AType *, AType *> intersection_map;
  AType			*top;

  AType(CreationSet *cs);
  AType(AType &a);
  AType() : hash(0), top(0) {}
};
#define forv_AType(_p, _v) forv_Vec(AType, _p, _v)

class EntrySet : public gc {
 public:
  Fun			*fun;
  Vec<AVar *>		args;
  Vec<AVar *>		rets;
  EdgeHash		edges;
  EdgeMap		out_edge_map;
  Vec<AEdge *>		out_edges;
  Vec<EntrySet *>	reachable;
  Vec<EntrySet *>	*equiv;
  EntrySet(Fun *af): fun(af), equiv(0) {}
};
#define forv_EntrySet(_p, _v) forv_Vec(EntrySet, _p, _v)

class CreationSet : public gc {
 public:
  Sym 			*sym;
  Vec<AVar *> 		defs;
  AType 		*atype; 	// the type that this creation set belongs to
  Vec<AVar *>		vars;
  uint			clone_for_constants : 1;
  Vec<CreationSet*>	*equiv;		// used by clone.cpp & fa.cpp
  Vec<CreationSet*>	not_equiv;	// used by clone.cpp
  Sym 			*type;		// used by clone.cpp & fa.capp

  CreationSet(Sym *s, AVar *def) : sym(s), atype(0), clone_for_constants(0), 
    equiv(0) { defs.add(def); }
  CreationSet(Sym *s) : sym(s), atype(0), clone_for_constants(0), equiv(0) { }
};
#define forv_CreationSet(_p, _v) forv_Vec(CreationSet, _p, _v)

class AVar : public gc {
 public:
  Var			*var;
  void			*contour;

  Vec<AVar *>		forward;
  Vec<AVar *>		backward;

  AType 		*in;
  AType 		*out;
  AType			*restrict;

  CreationSet		*creation_set;

  uint			in_send_worklist:1;
  uint			contour_is_entry_set:1;

  Vec<AVar *>		arg_of_send;
  SLink<AVar>		send_worklist_link;

  void *constant();

  AVar(Var *v, void *acontour);
};
#define forv_AVar(_p, _v) forv_Vec(AVar, _p, _v)

class AEdge : public gc {
 public:
  EntrySet	*from, *to;
  PNode		*send;
  Fun 		*fun;	
  Vec<AVar *>	args;
  uint		in_edge_worklist:1;
  
  SLink<AEdge>	edge_worklist_link;
  AEdge() : from(0), to(0), send(0), fun(0), in_edge_worklist(0) {}
};
#define forv_AEdge(_p, _v) forv_Vec(AEdge, _p, _v)

class FA : public gc {
 public:
  PDB *pdb;

  Vec<Fun *> funs;
  Vec<EntrySet *> ess;		// all used entry sets as array
  Vec<EntrySet *> ess_set;	// all used entry sets as set
  Vec<Sym *> basic_types;

  int analyze(Fun *f);
  int concretize();

  FA(PDB *apdb) : pdb(apdb) {}
};

class ATypeOpenHashFns {
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

AVar * make_AVar(Var *v, EntrySet *es);
Sym *coerce_num(Sym *a, Sym *b);
Sym *coerce_type(IF1 *i, Sym *a, Sym *b);
Sym *type_info(AST *a, Sym *s = 0);
void call_info(Fun *f, AST *a, Vec<Fun *> &funs);

#endif
