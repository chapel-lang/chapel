#include "defs.h"
#include "if1.h"
#include "pnode.h"
#include "var.h"

static int pnode_id = 1;

PNode::PNode() : code(0), mark(0), region(0), prim(0), unode(NULL), creates(0), call_context(0),
                 execution_frequency(0.0), false_branch_frequency(0.0) 
{
  id = pnode_id++;
}

PNode::PNode(Code *c) : 
  code(c), mark(0), region(0), prim(0), 
  unode(0), creates(0),
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

void
pp(PNode *p) {
  printf("(PNode %d ", p->id);
  printf("%s ", code_string[p->code->kind]);
  if (p->lvals.n) {
    printf("\n  (lvals %d ", p->lvals.n);
    forv_Var(v, p->lvals) {
      pp(v);
      printf(" ");
    }
    printf(") ");
  }
  if (p->rvals.n) {
    printf("\n  (rvals %d ", p->rvals.n);
    forv_Var(v, p->rvals) {
      pp(v);
      printf(" ");
    }
    printf(") ");
  }
  if (p->tvals.n) {
    printf("\n  (tvals %d ", p->tvals.n);
    forv_Var(v, p->tvals) {
      pp(v);
      printf(" ");
    }
    printf(") ");
  }
  if (p->is_dead)
    printf("DEAD ");
  printf(")\n");
}
