/* -*-Mode: c++;-*-
 Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

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

static void
splice_in_clone(Fun *f, AST **aast, Fun *target) {
  // splice AST
  *aast = target->ast->last();
}

int 
inline_call(FA *fa, Fun *f, AST **aa, Fun *target) {
  AST *a = *aa;
  if (!target) {
    Vec<Fun *> funs;
    call_info(f, a, funs);
    if (funs.n != 1)
      return -1;
    target = funs.v[0];
  }
  Vec<EntrySet *> ess(f->ess), newess;
  Vec<PNode *> pnodes;
  pnodes.set_union(a->pnodes);
  target = target->copy();
  forv_EntrySet(es, ess) {
    for (int i = 0; i < es->edges.n; i++) {
      if (es->edges.v[i] && pnodes.set_in(es->edges.v[i]->pnode))
	newess.set_add(es);
    }
  }
  newess.set_to_vec();
  fixup_clone(f, &newess);
  splice_in_clone(f, aa, target);
  return 0;
}

static void
inline_calls(FA *fa, Fun *f, AST **aast, int simple = 0) {
  AST *ast = *aast;
  // ignore these subtrees
  switch (ast->kind) {
    case AST_def_fun:
    case AST_const:
    case AST_def_type:
      return;
    default:
      break;
  }
  // recurse
  for (int i = 0; i < ast->n; i++)
    inline_calls(fa, f, &ast->v[i], simple);
  // get target function to inline
  Vec<Fun *> funs;
  call_info(f, ast, funs);
  if (funs.n != 1)
    return;
  Fun *ff = funs.v[0];
  // get target size
  if (ff->size < 0) {
    Vec<PNode *> nodes;
    f->collect_PNodes(nodes);
    ff->size = nodes.n;
  }
  // check criteria
  if (simple) {
    if (ff->size >= SIMPLE_INLINE_PNODES_BELOW) 
      return;
    if (ff->calls.n) 
      return;
    if (ff->has_return)
      return;
  } else {
    return;
  }
  inline_call(fa, f, aast, ff);
  f->size = -1;
}

static void
init_inline(FA *fa) {
  forv_Fun(f, fa->funs)
    f->size = -1;
}

int
inline_calls(FA *fa) {
  init_inline(fa);
  forv_Fun(f, fa->funs)
    inline_calls(fa, f, &f->ast);
  return 0;
}

int
simple_inline_calls(FA *fa) {
  init_inline(fa);
  forv_Fun(f, fa->funs)
    inline_calls(fa, f, &f->ast, 1);
  return 0;
}
