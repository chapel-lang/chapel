/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _loop_H_
#define _loop_H_

class FA;

struct LoopNode : public gc {
  int 			index;
  void			*node;
  LoopNode		*parent;
  Vec<LoopNode *>	children;
  Vec<LoopNode *>	loops;
  Vec<LoopNode *>	pred;
  Vec<LoopNode *>	succ;
  Vec<LoopNode *>	dom_children;
  int			pre_dfs, post_dfs;
  int			pre_dom, post_dom;
  uint			processed : 1;
  uint			in_worklist : 1;
  int			dfs_ancestor(LoopNode *);
  int			dom_ancestor(LoopNode *);
  LoopNode(int i, void *n = 0);
};
#define forv_LoopNode(_p, _v) forv_Vec(LoopNode, _p, _v)

struct LoopGraph : public gc {	
  LoopNode *loops;
  LoopNode *entry;
  Vec<LoopNode *> nodes;
  Vec<Vec<LoopNode *> *> levels;
  UnionFind uf;

  void unify(LoopNode *n, LoopNode *m);
  LoopNode *find(LoopNode *n);

  LoopGraph();
};

void find_loops(LoopGraph *g);
void find_local_loops(FA *f);
void find_recursive_loops(FA *f);
void find_all_loops(FA *fa);

#endif
