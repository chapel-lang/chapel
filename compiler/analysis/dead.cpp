/* -*-Mode: c++;-*-
 Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "pattern.h"
#include "prim.h"
#include "if1.h"
#include "builtin.h"
#include "pdb.h"
#include "fun.h"
#include "pnode.h"
#include "fa.h"
#include "ast.h"
#include "var.h"
#include "callbacks.h"

static int mark_live_avars_again = 0;

static void
mark_all_dead(FA *fa) {
  forv_Fun(f, fa->funs) {
    forv_PNode(p, f->fa_all_PNodes)
      p->is_dead = 1;
    forv_Var(v, f->fa_all_Vars)
      for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key)
	v->avars.v[i].value->is_dead = 1;
    forv_Sym(s, fa->pdb->if1->allsyms)
      if (s->var)
	for (int i = 0; i < s->var->avars.n; i++) if (s->var->avars.v[i].key)
	  s->var->avars.v[i].value->is_dead = 1;
  }
}

static void
print_dead(FA *fa) {
  int ndead_pnodes = 0, ndead_avars = 0;
  forv_Fun(f, fa->funs) {
    forv_PNode(p, f->fa_all_PNodes)
      if (p->is_dead) {
	ndead_pnodes++;
	printf("PNode %d DEAD\n", p->id);
      }
    forv_Var(v, f->fa_all_Vars)
      for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
	if (v->avars.v[i].value->is_dead) {
	  ndead_avars++;
	  printf("AVar %d:%d:%d DEAD\n", v->sym->id, v->id, v->avars.v[i].value->id);
	}
      }
  }
  printf("%d PNodes DEAD\n", ndead_pnodes);
  printf("%d AVars DEAD\n", ndead_avars);
}

static void 
mark_initial_live_pnodes(FA *fa) {
  forv_Fun(f, fa->funs) {
    forv_PNode(p, f->fa_all_PNodes) {
      if (p->code && p->code->kind == Code_SEND && 
	  (!is_functional(if1, p->code) &&
	   p->prim != prim_reply))
	p->is_dead = 0;
    }
  }
}

static void 
mark_live_avar(AVar *av) {
  av->is_dead = 0;
  mark_live_avars_again = 1;
  if (av->var->def)
    av->var->def->is_dead = 0;
  forv_AVar(aav, av->backward) if (aav) {
    if (aav->is_dead)
      mark_live_avar(aav);
  }
}

static void 
mark_live_avars(FA *fa) {
  forv_Fun(f, fa->funs) {
    forv_PNode(p, f->fa_all_PNodes) {
      if (!p->is_dead) {
	forv_Var(v, p->rvals) {
	  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
	    AVar *av = v->avars.v[i].value;
	    if (av->is_dead) 
	      mark_live_avar(av);
	  }
	}
      }
    }
    forv_Var(v, f->fa_all_Vars) {
      Sym *s = v->sym;
      for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
	AVar *av = v->avars.v[i].value;
	if (av->is_dead) {
	  if (s->is_pattern) {
	    forv_Sym(h, s->has) {
	      AVar *hav = make_AVar(h->var, (EntrySet*)av->contour);
	      if (!hav->is_dead)
		mark_live_avar(av);
	    }
	  }
	  forv_CreationSet(cs, *av->out) {
	    forv_AVar(iv, cs->vars) {
	      if (!iv->is_dead)
		mark_live_avar(av);
	    }
	  }
	}
      }
    }
  }
}

static void
mark_live_var(Var *v) {
  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
    AVar *av = v->avars.v[i].value;
    if (av->is_dead) 
      mark_live_avar(av);
  }
}

static void 
mark_live_pnodes(FA *fa) {
  forv_Fun(f, fa->funs) {
    forv_PNode(p, f->fa_all_PNodes) {
      if (p->is_dead && p->code)
	switch (p->code->kind) {
	  default: break;
	  case Code_SEND:
	    forv_Var(v, p->lvals) {
	      for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
		AVar *av = v->avars.v[i].value;
		if (!av->is_dead) { 
		  p->is_dead = 0;
		  mark_live_avars_again = 1;
		}
	      }
	    }
	    break;
	  case Code_IF:
	    mark_live_var(p->rvals.v[0]); // conservative
	    break;
	}
    }
  }
}

int 
mark_dead_code(FA *fa, Fun *top) {
  (void) top;
  mark_all_dead(fa);
  mark_initial_live_pnodes(fa);
  do {
    mark_live_avars_again = 0;
    mark_live_avars(fa);
    mark_live_pnodes(fa);
  } while (mark_live_avars_again);
  if (verbose_level) 
    print_dead(fa);
  return 0;
}




