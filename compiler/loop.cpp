/* -*-Mode: c++;-*-
 Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

// "Identifying Loops in Almost Linear Time" by G. Ramalingam
// Modified Sreedhar-Gao-Lee Algorithm
// with modifications:
//   1) build a tree of loops
//   2) handle single node loops

LoopNode::LoopNode(int i, void *n) : index(i), node(n), pre_dfs(-1), pre_dom(-1), 
  processed(0), in_worklist(0) 
{
}

void 
LoopGraph::unify(LoopNode *n, LoopNode *m) { 
  uf.unify(n->index, m->index); 
}

LoopNode *
LoopGraph::find(LoopNode *n) { 
  return nodes.v[uf.find(n->index)]; 
}

LoopGraph::LoopGraph() : loops(0) {
}

int
LoopNode::dfs_ancestor(LoopNode *x) {
  return x->pre_dfs <= pre_dfs && x->post_dfs > post_dfs;
}

int
LoopNode::dom_ancestor(LoopNode *x) {
  return x->pre_dom <= pre_dom && x->post_dom > post_dom;
}

static void
collapse(LoopGraph *g, Vec<LoopNode *> &body, LoopNode *header) {
  forv_LoopNode(z, body) if (z) {
    if (z->parent)
      header->children.add(z->parent);
    z->parent = header;
    header->children.add(z);
    g->unify(z, header);
    if (header->pre_dfs < 0) {
      header->pre_dfs = z->pre_dfs;
      header->post_dfs = z->post_dfs;
      header->pre_dom = z->pre_dom;
      header->post_dom = z->post_dom;
    } else {
      if (header->pre_dfs > z->pre_dfs) header->pre_dfs = z->pre_dfs;
      if (header->post_dfs < z->post_dfs) header->post_dfs = z->post_dfs;
      if (header->pre_dom > z->pre_dom) header->pre_dom = z->pre_dom;
      if (header->post_dom < z->post_dom) header->post_dom = z->post_dom;
    }
  }
}

static void
self_loop(LoopGraph *g, LoopNode *header) {
  LoopNode *rep = new LoopNode(g->nodes.n);
  g->nodes.add(rep);
  Vec<LoopNode *> body;
  body.add(header);
  header->processed = 1;
  collapse(g, body, rep);
}

static void 
find_loop(LoopGraph *g, LoopNode *header, Vec<LoopNode *> &worklist) {
  LoopNode *rep = new LoopNode(g->nodes.n);
  g->nodes.add(rep);
  Vec<LoopNode *> body, b;
  body.set_add(header); b.add(header);
  while (worklist.n) {
    LoopNode *y = worklist.pop();
    y->in_worklist = 0;
    body.set_add(y); b.add(y);
    y->processed = 1;
    forv_LoopNode(z, y->pred) {
      LoopNode *zr = g->find(z);
      if (!zr->dfs_ancestor(header))
	zr->loops.add(rep);
      else if (!body.set_in(zr) && !zr->in_worklist) {
	worklist.add(zr);
	zr->in_worklist = 1;
      }
    }
  }
  collapse(g, b, rep);
}

static void 
dfs_number(LoopNode *p, int &pre, int &post, int level, Vec<Vec<LoopNode *> *> &levels) {
  levels.fill(level + 1);
  if (!levels.v[level])
    levels.v[level] = new Vec<LoopNode *>;
  levels.v[level]->add(p);
  p->pre_dfs = pre++;
  forv_LoopNode(pp, p->succ) {
    if (pp->pre_dfs < 0)
      dfs_number(pp, pre, post, level + 1, levels);
  }
  p->post_dfs = post++;
}

static void 
dom_number(LoopNode *p, int &pre, int &post) {
  p->pre_dom = pre++;
  forv_LoopNode(pp, p->dom_children) {
    if (pp->pre_dom < 0)
      dom_number(pp, pre, post);
  }
  p->post_dom = post++;
}

void
find_loops(LoopGraph *g) {
  int pre_dfs = 0, post_dfs = 1, pre_dom = 0, post_dom = 1;
  dfs_number(g->entry, pre_dfs, post_dfs, 0, g->levels);
  dom_number(g->entry, pre_dom, post_dom);
  if (g->nodes.n) {
    g->uf.size(g->nodes.n * 3);
    for (int i = g->levels.n - 1; i >= 0; i--) {
      forv_LoopNode(x, *g->levels.v[i]) {
	Vec<LoopNode *> worklist;
	forv_LoopNode(y, x->pred) {
	  if (y->dfs_ancestor(x) && y->dom_ancestor(x))
	    worklist.add(y);
	  if (x == y)
	    self_loop(g, x);
	}
	if (worklist.n) find_loop(g, x, worklist);
      }
      forv_LoopNode(x, *g->levels.v[i]) {
	if (!x->processed) {
	  Vec<LoopNode *> worklist;
	  forv_LoopNode(y, x->pred) {
	    if (y->dfs_ancestor(x) && !y->dom_ancestor(x))
	      worklist.add(y);
	  }
	  if (worklist.n) find_loop(g, x, worklist);
	}	
      }
    }
    g->loops = g->nodes.v[g->nodes.n-1];
    if (g->loops->node)
      g->loops = 0;
  }
}

void
find_local_loops(FA *fa, Fun *f) {
  Vec<PNode *> nodes;
  f->collect_PNodes(nodes);
  LoopGraph *g = f->loops = new LoopGraph;
  forv_PNode(n, nodes) {
    n->loop_node = new LoopNode(g->nodes.n, n);
    g->nodes.add(n->loop_node);
  }
  forv_PNode(n, nodes) {
    forv_PNode(nn, n->cfg_succ) n->loop_node->succ.add(nn->loop_node);
    forv_PNode(nn, n->cfg_pred) n->loop_node->pred.add(nn->loop_node);
    forv_Dom(nn, n->dom->children) n->loop_node->dom_children.add(((PNode*)nn->node)->loop_node);
  }
  g->entry = f->entry->loop_node;
  find_loops(g);
}

void
find_recursive_loops(FA *fa) {
  LoopGraph *g = fa->pdb->loops = new LoopGraph;
  forv_Fun(f, fa->funs) {
    f->loop_node = new LoopNode(g->nodes.n, f);
    g->nodes.add(f->loop_node);
  }
  build_call_dominators(fa);
  forv_Fun(f, fa->funs) {
    Vec<Fun *> calls;
    f->calls_funs(calls);
    forv_Fun(ff, calls) f->loop_node->succ.add(ff->loop_node);   
    f->called_by_funs(calls);
    forv_Fun(ff, calls) f->loop_node->pred.add(ff->loop_node);
    forv_Dom(ff, f->dom->children) f->loop_node->dom_children.add(((Fun*)ff->node)->loop_node);
  }
  g->entry = fa->pdb->if1->top->fun->loop_node;
  find_loops(g);
}

void
find_all_loops(FA *fa) {
  forv_Fun(f, fa->funs)
    find_local_loops(fa, f);
  find_recursive_loops(fa);
}
