#ifndef _fun_H_
#define _fun_H_

class PDB;
class ESet;
class AVar;
class LoopGraph;
class LoopNode;
class Dom;
class CDB_EntrySet;
class MPosition;
class Region;
class EntrySet;
class Fun;
class Sym;
class PNode;
class CreationSet;

class CallPoint : public gc {
 public:
  Fun 	*fun;
  PNode *pnode;
  CallPoint(Fun *afun, PNode *apnode) : fun(afun), pnode(apnode) {}
};
#define forv_CallPoint(_c, _v) forv_Vec(CallPoint, _c, _v)

// Functions

class Fun : public gc {
 public:
  int id;
  Sym *sym;
  AST *ast;

  PNode *entry;
  PNode *exit;
  Region *region;

  uint init_function : 1; // everything is global
  uint has_return : 1;

  // cdb
  char *cdb_id;
  int prof_id;
  Vec<int> prof_ess;
  Vec<CDB_EntrySet *> es_info;

  // fa
  uint fa_collected : 1;
  uint clone_for_constants : 1;
  Vec<EntrySet *> ess;
  Vec<Var *> fa_Vars;
  Vec<Var *> fa_all_Vars;
  Vec<PNode *> fa_all_PNodes;
  Vec<PNode *> fa_move_PNodes;
  Vec<PNode *> fa_phi_PNodes;
  Vec<PNode *> fa_phy_PNodes;
  Vec<PNode *> fa_send_PNodes;

  // pattern
  Vec<MPosition *> arg_positions;
  Vec<MPosition *> numeric_arg_positions;
  Map<MPosition*, Sym*> arg_syms;
  Map<MPosition *, Var*> filtered_args;
  Map<MPosition *, Var*> args; // underlying argument
  Vec<Var *> rets;
  Vec<MPosition *> out_positions;
  Map<MPosition *, AST *> default_args;
  
  // generic/default/pointwise wrappers
  Fun *wraps;
  Vec<Fun *> wrappers;
  Vec<MPosition *> defaulted_set;
  Map<Sym *, Sym *> instantiated;
  Map<MPosition *, Sym *> point_wised;

  // clone
  Vec<EntrySet *> called_ess;
  Vec<CreationSet *> called_css;
  Vec<Vec<EntrySet *> *> equiv_sets;
  Map<PNode *, PNode*> *nmap;
  Map<Var *, Var*> *vmap;

  // clone typings and call graph
  Map<PNode *, Vec<Fun *> *> calls;
  void calls_funs(Vec<Fun *> &funs);
  Vec<CallPoint *> called;
  void called_by_funs(Vec<Fun *> &funs);

  // loop
  LoopGraph *loops;
  LoopNode *loop_node;
  Dom *dom;
  
  // inline
  float execution_frequency;
  int size;
  
  // cg
  char *cg_string;
  char *cg_structural_string;
  
  char *pathname();
  char *filename();
  int line();
  
  void collect_PNodes(Vec<PNode *> &v);
  void collect_Vars(Vec<Var *> &v, Vec<PNode *> *vv = 0);

  void build_cfg();
  void build_ssu();

  void setup_ast();

  Fun(Sym *afn, int aninit_function = 0);
  Fun() {}
  Fun *copy();
  Fun *default_wrapper(Vec<MPosition *> &defaults) { return 0; }
};
#define forv_Fun(_f, _v) forv_Vec(Fun, _f, _v)

int compar_funs(const void *ai, const void *aj);

#endif
