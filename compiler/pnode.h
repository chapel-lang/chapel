/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _pn_H_
#define _pn_H_

class Prim;

/* #define CONC_IMPLEMENTED 1 */

// Program node

class PNode : public gc {
 public:
  Code *code;

  Vec<Var *> lvals;
  Vec<Var *> rvals;

  // Control-Flow Graph (CFG): cfg.cpp, ssu.cpp 
  int mark; // ssu.c
  Vec<PNode *> cfg_succ;
  Vec<PNode *> cfg_pred;
  Map<PNode *, int> cfg_pred_index;  // temporary in ssu.cpp
#ifdef CONC_IMPLEMENTED
  Vec<PNode *> conc_succ;
  Vec<PNode *> conc_pred;
#endif

  // Single-Entry Single-Exit Region (SESE): ssu.cpp
  Region *region;

  // Single-Static Value Functions
  Vec<PNode *> phi;
  Vec<PNode *> phy;

  // fa.cpp
  Prim *prim;

  // temporary space
  union {
    UNode *unode; // ssu.cpp
    int	used; // ssu.cpp
    BlockHash<Var *, PointerHashFns> *live_vars;
  };
  Vec<Var *> lvals_set; // ssu.cpp

  PNode(Code *c);
  PNode();
};
#define forv_PNode(_p, _v) forv_Vec(PNode, _p, _v)

void collect_Vars_PNode(PNode *n, Vec<Var *> &vars);

#endif

