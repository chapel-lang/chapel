/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _fun_H_
#define _fun_H_

class ESet;
class AVar;

// Functions

class Fun : public gc {
 public:
  PDB *pdb;
  Sym *sym;
  AST *ast;

  PNode *entry;
  PNode *exit;
  Region *region;

  uint init_function : 1; // everything is global

  // fa.cpp
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
  
  // clone.cpp
  Vec<EntrySet *> called_ess;
  Vec<CreationSet *> called_css;
  Vec<Vec<EntrySet *> *> equiv_sets;

  // clone.cpp typings
  Vec<Var *> args;
  Vec<Var *> rets;

  // cg.cpp
  char *cg_string;
  char *cg_structural_string;
  
  void collect_PNodes(Vec<PNode *> &v);
  void collect_Vars(Vec<Var *> &v, Vec<PNode *> *vv = 0);

  void build_cfg();
  void build_ssu();

  void setup_ast();

  Fun(PDB *apdb, Sym *afn, int aninit_function = 0);
  Fun() {}
  Fun *copy();
};
#define forv_Fun(_f, _v) forv_Vec(Fun, _f, _v)

#endif
