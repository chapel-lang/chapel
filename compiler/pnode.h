/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _pn_H_
#define _pn_H_

class Prim;
class Dom;
class LoopNode;

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
#ifdef CONC_IMPLEMENTED
  Vec<PNode *> conc_succ;
  Vec<PNode *> conc_pred;
#endif

  // Single-Entry Single-Exit Region (SESE): ssu.cpp
  Region *region;

  // Single-Static Value Functions
  Vec<PNode *> phi;
  Vec<PNode *> phy;

  // Primitive
  Prim *prim;

  // Temporary Space
  union {
    UNode *unode; // ssu.cpp
    int	used; // ssu.cpp
    BlockHash<Var *, PointerHashFns> *live_vars; // ssu.cpp
    LoopNode *loop_node; // loop.cpp
  };
  Vec<Var *> lvals_set; // ssu.cpp
  Map<PNode *, int> cfg_pred_index;  // ssu.cpp

  // Dominators
  Dom *dom; // dom.cpp

  // inline.cpp
  float execution_frequency;
  float false_branch_frequency;

  // Constructors
  PNode(Code *c);
  PNode();
};
#define forv_PNode(_p, _v) forv_Vec(PNode, _p, _v)

void collect_Vars_PNode(PNode *n, Vec<Var *> &vars);

typedef Vec<PNode *> VecPNode;
typedef Map<PNode *, VecPNode> MapPNVecPN;

#endif

