/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "if1.h"
#include "pnode.h"
#include "var.h"

static int pnode_id = 1;

PNode::PNode() : code(0), mark(0), region(0), prim(0), callees(0), unode(0), creates(0),
  execution_frequency(0.0), false_branch_frequency(0.0) 
{
  id = pnode_id++;
}

PNode::PNode(Code *c) : 
  code(c), mark(0), region(0), prim(0), callees(0), unode(0), creates(0),
  execution_frequency(0.0), false_branch_frequency(0.0) 
{
  forv_Sym(s, c->rvals)
    rvals.add(s->var ? s->var : (s->var = new Var(s)));
  forv_Sym(s, c->lvals)
    lvals.add(s->var ? s->var : (s->var = new Var(s)));
  id = pnode_id++;
}

int
compar_pnodes(const void *ai, const void *aj) {
  int i = (*(PNode**)ai)->id;
  int j = (*(PNode**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

