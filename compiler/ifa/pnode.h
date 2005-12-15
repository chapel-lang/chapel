/* -*-Mode: c++;-*-
*/
#ifndef _pn_H_
#define _pn_H_

class Code;
class Prim;
class Dom;
class LoopNode;
class Fun;
class MPosition;
class Region;
class UNode;
class Sym;

/* #define CONC_IMPLEMENTED 1 */

// Program node

class Callees : public gc {
 public:
  Vec<Fun *> funs; 
  Vec<MPosition *> arg_positions; 
};

class PNode : public gc {
 public:
  Code *code;
  int id;
  uint is_dead : 1;
  Vec<Var *> lvals; // variables this node assigns
  Vec<Var *> rvals; // variables this node reads
  Vec<Var *> tvals;

  // Control-Flow Graph (CFG): cfg.cpp, ssu.cpp 
  int mark; // ssu.c
  Vec<PNode *> cfg_succ;
  Vec<PNode *> cfg_pred;
#ifdef CONC_IMPLEMENTED
  Vec<PNode *> conc_succ;
  Vec<PNode *> conc_pred;
#endif

  Region *region; // Single-Entry Single-Exit Region (SESE): ssu.cpp

  // Single-Static Value Functions
  Vec<PNode *> phi; // MOVE nodes that logically follow this node
  Vec<PNode *> phy; // MOVE nodes that logically precede this node

  Prim *prim; // Primitive

  Callees *callees; // Callees
  Callees *next_callees;

  // Temporary Space
  union {
    UNode *unode; // ssu.cpp
    int used; // ssu.cpp
    BlockHash<Var *, PointerHashFns> *live_vars; // ssu.cpp
    LoopNode *loop_node; // loop.cpp
  };
  Vec<Var *> lvals_set; // ssu.cpp
  Map<PNode *, int> cfg_pred_index;  // ssu.cpp, cg.cpp

  Dom *dom; // Dominators dom.cpp 
  Vec<Sym *> *creates; // Cloning

  float execution_frequency;
  float false_branch_frequency; // inline.cpp

  PNode(Code *c);
  PNode();
};
#define forv_PNode(_p, _v) forv_Vec(PNode, _p, _v)

int compar_pnodes(const void *ai, const void *aj);

typedef Vec<PNode *> VecPNode;
typedef Map<PNode *, VecPNode> MapPNVecPN;

#endif

