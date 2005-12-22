#include "geysa.h"
#include "fa.h"
#include "inline.h"
#include "loop.h"
#include "clone.h"
#include "if1.h"
#include "pdb.h"
#include "fun.h"
#include "pnode.h"

#define LOOP_FREQUENCY 10.0

static void
dfs_order(Fun *f, Vec<Fun *> &funs, Vec<Fun *> &fset) {
  if (!fset.set_add(f))
    return;
  funs.add(f);
  Vec<Fun *> calls_funs;
  f->calls_funs(calls_funs);
  forv_Fun(ff, calls_funs)
    dfs_order(ff, funs, fset);
}

static void
local_loop_frequency_estimation(LoopNode *l, float f) {
  forv_LoopNode(n, l->children) {
    if (n->node)
      ((PNode*)n->node)->execution_frequency = f;
    else
      local_loop_frequency_estimation(n, f * LOOP_FREQUENCY);
  }
}

static void
local_frequency_estimation(Fun *f) {
  if (f->loops->loops)
    local_loop_frequency_estimation(f->loops->loops, LOOP_FREQUENCY);
  Vec<PNode *> nodes;
  f->collect_PNodes(nodes);
  forv_PNode(n, nodes)
    if (n->execution_frequency < 1.0)
      n->execution_frequency = 1.0;
}

static void
global_loop_frequency_estimation(LoopNode *l, float f) {
  forv_LoopNode(n, l->children) {
    assert(n != l);
    if (n->node)
      ((Fun*)n->node)->execution_frequency = f;
    else
      global_loop_frequency_estimation(n, f * LOOP_FREQUENCY);
  }
}

static void
global_frequency_estimation(FA *fa) {
  if (fa->pdb->loops->loops)
    global_loop_frequency_estimation(fa->pdb->loops->loops, LOOP_FREQUENCY);
  forv_Fun(f, fa->funs)
    if (f->execution_frequency < 1.0)
      f->execution_frequency = 1.0;
  Vec<Fun *> funs, fset;
  dfs_order(fa->pdb->if1->top->fun, funs, fset);
  // propagate down the call tree
  forv_Fun(f, funs) {
    float freq = f->execution_frequency;
    Vec<PNode *> nodes;
    f->collect_PNodes(nodes);
    forv_PNode(n, nodes)
      n->execution_frequency *= freq;
    f->execution_frequency = 0;
    forv_CallPoint(c, f->called) {
      if (c->fun != f && f->loop_node->dfs_ancestor(c->fun->loop_node))
        f->execution_frequency += freq * c->pnode->execution_frequency;  
    }
    if (f->execution_frequency < 1.0)
      f->execution_frequency = 1.0;
  }
}

int 
frequency_estimation(FA *fa) {
  find_all_loops(fa);
  forv_Fun(f, fa->funs)
    local_frequency_estimation(f);
  global_frequency_estimation(fa);
  return 0;
}

