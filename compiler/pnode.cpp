/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

PNode::PNode() : code(0), mark(0), region(0), prim(0), unode(0) {
}

PNode::PNode(Code *c) : 
  code(c), mark(0), region(0), prim(0), unode(0) 
{
  forv_Sym(s, c->rvals)
    rvals.add(s->var ? s->var : (s->var = new Var(s)));
  forv_Sym(s, c->lvals)
    lvals.add(s->var ? s->var : (s->var = new Var(s)));
}
