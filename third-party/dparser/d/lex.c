/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/

#include "d.h"

typedef struct NFAState {
  uint			index;
  Vec(struct NFAState*)	chars[256];
  Vec(struct NFAState*)	epsilon;
  Vec(Action*)		accepts;
  Vec(Action*)		live;
} NFAState;

typedef struct DFAState {
  Vec(struct NFAState*)	states;
  struct DFAState	*chars[256];	
  ScanState		*scan;
} DFAState;

typedef Vec(DFAState *) VecDFAState;
typedef Vec(NFAState *) VecNFAState;

typedef struct LexState {
  uint nfa_index;
  VecNFAState allnfas;
  uint transitions;
  uint scanners;
  uint ignore_case;
} LexState;

static NFAState *
new_NFAState(LexState *ls) {
  NFAState *n = MALLOC(sizeof(NFAState));
  memset(n, 0, sizeof(NFAState));
  n->index = ls->nfa_index++;
  vec_add(&ls->allnfas, n);
  return n;
}

static DFAState *
new_DFAState() {
  DFAState *n = MALLOC(sizeof(DFAState));
  memset(n, 0, sizeof(DFAState));
  return n;
}

static void
free_DFAState(DFAState *y) {
  vec_free(&y->states);
  FREE(y);
}

static void
free_VecDFAState(VecDFAState *dfas) {
  int i;
  for (i = 0; i < dfas->n; i++)
    free_DFAState(dfas->v[i]);
  vec_free(dfas);
}

static void
free_NFAState(NFAState *y) {
  int i;
  for (i = 0; i < 256; i++)
    vec_free(&y->chars[i]);
  vec_free(&y->epsilon);
  vec_free(&y->accepts);
  FREE(y);
}

static void
free_VecNFAState(VecNFAState *nfas) {
  int i;
  for (i = 0; i < nfas->n; i++)
    free_NFAState(nfas->v[i]);
  vec_free(nfas);
}

static ScanState *
new_ScanState() {
  ScanState *n = MALLOC(sizeof(ScanState));
  memset(n, 0, sizeof(ScanState));
  return n;
}

static int 
nfacmp(const void *ai, const void *aj) {
  uint32 i = (*(NFAState**)ai)->index;
  uint32 j = (*(NFAState**)aj)->index;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

static void
nfa_closure(DFAState *x) {
  int i, j, k;

  for (i = 0; i < x->states.n; i++)
    for (j = 0; j < x->states.v[i]->epsilon.n; j++) {
      for (k = 0; k < x->states.n; k++)
	if (x->states.v[i]->epsilon.v[j] == x->states.v[k])
	  goto Lbreak;
      vec_add(&x->states, x->states.v[i]->epsilon.v[j]);
    Lbreak:;
    }
  qsort(x->states.v, x->states.n, sizeof(x->states.v[0]), nfacmp);
}

static int
eq_dfa_state(DFAState *x, DFAState *y) {
  int i;

  if (x->states.n != y->states.n)
    return 0;
  for (i = 0; i < x->states.n; i++)
    if (x->states.v[i] != y->states.v[i])
      return 0;
  return 1;
}

static void
dfa_to_scanner(VecDFAState *alldfas, VecScanState *scanner) {
  int i, j, k, highest, p;

  vec_clear(scanner);
  for (i = 0; i < alldfas->n; i++) {
    alldfas->v[i]->scan = new_ScanState();
    alldfas->v[i]->scan->index = i;
    vec_add(scanner, alldfas->v[i]->scan);
  }
  for (i = 0; i < alldfas->n; i++) {
    for (j = 0; j < 256; j++)
      if (alldfas->v[i]->chars[j])
	alldfas->v[i]->scan->chars[j] = alldfas->v[i]->chars[j]->scan;
    highest = INT_MIN;
    for (j = 0; j < alldfas->v[i]->states.n; j++)
      for (k = 0; k < alldfas->v[i]->states.v[j]->accepts.n; k++) {
	p = alldfas->v[i]->states.v[j]->accepts.v[k]->term->term_priority;
	if (highest < p)
	  highest = p;
      }
    for (j = 0; j < alldfas->v[i]->states.n; j++)
      for (k = 0; k < alldfas->v[i]->states.v[j]->accepts.n; k++) {
	p = alldfas->v[i]->states.v[j]->accepts.v[k]->term->term_priority;
	if (p == highest)
	  vec_add(&alldfas->v[i]->scan->accepts,
		  alldfas->v[i]->states.v[j]->accepts.v[k]);
      }
  }
}

static void
nfa_to_scanner(NFAState *n, Scanner *s) {
  DFAState *x = new_DFAState(), *y;
  VecDFAState alldfas;
  int i, i_alldfas, i_states, i_char;
  VecScanState *scanner = &s->states;
  
  memset(&alldfas, 0, sizeof(alldfas));
  vec_add(&x->states, n);
  nfa_closure(x);
  vec_add(&alldfas, x);
  for (i_alldfas = 0; i_alldfas < alldfas.n; i_alldfas++) {
    x = alldfas.v[i_alldfas];
    for (i_char = 0; i_char < 256; i_char++) {
      y = NULL;
      for (i_states = 0; i_states < x->states.n; i_states++) {
	for (i = 0; i < x->states.v[i_states]->chars[i_char].n; i++) {
	  if (!y)
	    y = new_DFAState();
	  set_add(&y->states, x->states.v[i_states]->chars[i_char].v[i]);
	}
      }
      if (y) {
	set_to_vec(&y->states);
	nfa_closure(y);
	for (i = 0; i < alldfas.n; i++)
	  if (eq_dfa_state(y, alldfas.v[i])) {
            free_DFAState(y);
	    y = alldfas.v[i];
	    goto Lnext;
	  }
	vec_add(&alldfas, y);
      Lnext:
	x->chars[i_char] = y;
      }
    }
  }
  dfa_to_scanner(&alldfas, scanner);
  free_VecDFAState(&alldfas);
}

/* build a NFA for the regular expression */
static int
build_regex_nfa(LexState *ls, uint8 **areg, NFAState *pp, NFAState *nn, Action *trailing) {
  uint8 c, pc, *reg = *areg;
  NFAState *p = pp, *s, *x, *n = nn;
  int reversed, i, has_trailing = 0;
  uint8 mark[256];

  s = p;
  while ((c = *reg++)) {
    switch(c) {
      case '(':
	has_trailing = build_regex_nfa(ls, &reg, s, (x = new_NFAState(ls)), trailing) ||
	  has_trailing;
	p = s;
	s = x;
	break;
      case ')':
	goto Lreturn;
      case '|':
	vec_add(&s->epsilon, nn);
	vec_add(&pp->epsilon, (s = new_NFAState(ls)));
	break;
      case '[':
	if (*reg == '^') {
	  reg++;
	  reversed = 1;
	} else
	  reversed = 0;
	memset(mark, 0, sizeof(mark));
	pc = UCHAR_MAX;
	while ((c = *reg++)) {
	  switch(c) {
	    case ']':
	      goto Lsetdone;
	    case '-':
	      c = *reg++;
	      if (!c)
		goto Lerror;
	      if (c == '\\')
		c = *reg++;
	      if (!c)
		goto Lerror;
	      for (;pc <= c; pc++)
		mark[pc] = 1;
	      break;
	    case '\\':
	      c = *reg++;
	      /* fall through */
	    default:
	      pc = c;
	      mark[c] = 1;
	      break;
	  }
	}
    Lsetdone:
	x = new_NFAState(ls);
	for (i = 1; i < 256; i++)
	  if ((!reversed && mark[i]) || (reversed && !mark[i]))
	    vec_add(&s->chars[i], x);
	p = s;
	s = x;
	break;
      case '?':
	vec_add(&p->epsilon, s);
	break;
      case '*':
	vec_add(&p->epsilon, s);
	vec_add(&s->epsilon, p);
	break;
      case '+':
	vec_add(&s->epsilon, p);
	break;
      case '/':
	vec_add(&s->accepts, trailing);
	has_trailing = 1;
	break;
      case '\\':
	c = *reg++;
	if (!c)	
	  goto Lerror;
	/* fall through */
      default:
	if (!ls->ignore_case || !isalpha(c))
	  vec_add(&s->chars[c], (x = new_NFAState(ls))); 
	else {
	  vec_add(&s->chars[tolower(c)], (x = new_NFAState(ls))); 
	  vec_add(&s->chars[toupper(c)], x);
	}
	p = s;
	s = x;
	break;
    }
  }
 Lreturn:
  vec_add(&s->epsilon, n);
  *areg = reg;
  return has_trailing;
 Lerror:
  d_fail("bad (part of) regex: %s\n", *areg);
  return has_trailing;
}

static void
action_diff(VecAction *a, VecAction *b, VecAction *c) {
  int bb = 0, cc = 0;
  while (1) {
    if (bb >= b->n)
      break;
  Lagainc:
    if (cc >= c->n) {
      while (bb < b->n)
	vec_add(a, b->v[bb++]);
      break;
    }
  Lagainb:
    if (b->v[bb]->index == c->v[cc]->index) {
      bb++;
      cc++;
      continue;
    }
    if (b->v[bb]->index < c->v[cc]->index) {
      vec_add(a, b->v[bb++]);
      if (bb >= b->n)
	break;
      goto Lagainb;
    }
    cc++;
    goto Lagainc;
  }
}

static void
action_intersect(VecAction *a, VecAction *b, VecAction *c) {
  int bb = 0, cc = 0;
  while (1) {
    if (bb >= b->n)
      break;
  Lagainc:
    if (cc >= c->n)
      break;
  Lagainb:
    if (b->v[bb]->index == c->v[cc]->index) {
      vec_add(a, b->v[bb++]);
      cc++;
      continue;
    }
    if (b->v[bb]->index < c->v[cc]->index) {
      bb++;
      if (bb >= b->n)
	break;
      goto Lagainb;
    }
    cc++;
    goto Lagainc;
  }
}

static void
compute_liveness(Scanner *scanner) {
  int i, j, changed = 1;
  ScanState *ss, *sss;
  VecScanState *states = &scanner->states;

  /* basis */
  for (i = 0; i < states->n; i++) {
    ss = states->v[i];
    set_union(&ss->live, &ss->accepts);
  }
  while (changed) {
    changed = 0;
    for (i = 0; i < states->n; i++) {
      ss = states->v[i];
      for (j = 0; j < 256; j++) {
	if ((sss = ss->chars[j])) {
	  if (ss != sss)
	    if (set_union(&ss->live, &sss->live))
	      changed = 1;
	}
      }
    }
  }
  for (i = 0; i < states->n; i++) {
    ss = states->v[i];
    set_to_vec(&ss->live);
    sort_VecAction(&ss->live);
  }
}

static uint32
trans_hash_fn(ScanStateTransition *a, hash_fns_t *fns) {
  uint h = 0;
  int i;

  if (!fns->data[0])
    for (i = 0; i < a->live_diff.n; i++)
      h += 3 * a->live_diff.v[i]->index;
  for (i = 0; i < a->accepts_diff.n; i++)
    h += 3 * a->accepts_diff.v[i]->index;
  return h;
}

static int
trans_cmp_fn(ScanStateTransition *a, ScanStateTransition *b, hash_fns_t *fns) {
  int i;
  
  if (!fns->data[0])
    if (a->live_diff.n != b->live_diff.n)
      return 1;
  if (a->accepts_diff.n != b->accepts_diff.n)
    return 1;
  if (!fns->data[0])
    for (i = 0; i < a->live_diff.n; i++)
      if (a->live_diff.v[i] != b->live_diff.v[i])
	return 1;
  for (i = 0; i < a->accepts_diff.n; i++)
    if (a->accepts_diff.v[i] != b->accepts_diff.v[i])
      return 1;
  return 0;
}

static hash_fns_t trans_hash_fns = {
  (hash_fn_t)trans_hash_fn,
  (cmp_fn_t)trans_cmp_fn,
  { 0, 0 }
};

static void
build_transitions(LexState *ls, Scanner *s) {
  int i, j;
  ScanState *ss;
  ScanStateTransition *trans = NULL, *x;
  VecScanState *states = &s->states;

#ifdef LIVE_DIFF_IN_TRANSITIONS 
  trans_hash_fns.data[0] = (void*)0;
#else
  trans_hash_fns.data[0] = (void*)1;
#endif
  for (i = 0; i < states->n; i++) {
    ss = states->v[i];
    for (j = 0; j < 256; j++) {
      if (!trans) {
	trans = MALLOC(sizeof(*trans));
	memset(trans, 0, sizeof(*trans));
      }
      if (ss->chars[j]) {
	action_diff(&trans->live_diff, &ss->live, &ss->chars[j]->live);
	action_intersect(&trans->accepts_diff, &ss->accepts, 
			 &trans->live_diff);
      }
      if ((x = set_add_fn(&s->transitions, trans, &trans_hash_fns)) == trans)
	trans = NULL;
      else {
	vec_free(&trans->live_diff); 
	vec_free(&trans->accepts_diff);
      }
      ss->transition[j] = x;
    }
  }
  j = 0;
  set_to_vec(&s->transitions);
  for (i = 0; i < s->transitions.n; i++)
    s->transitions.v[i]->index = i;
  ls->transitions += s->transitions.n;
}

static void
compute_transitions(LexState *ls, Scanner *s) {
  compute_liveness(s);
  build_transitions(ls, s);
}

static void
build_state_scanner(LexState *ls, State *s) {
  NFAState *n, *nn, *nnn;
  Action *a;
  uint8 *c, *reg; 
  int j, one;

  one = 0;
  n = new_NFAState(ls);
  /* first strings since they can be trivially combined as a tree */
  for (j = 0; j < s->shift_actions.n; j++) {
    a = s->shift_actions.v[j];
    if (a->kind == ACTION_ACCEPT) {
      one = 1;
      if (!n->chars[0].n) 
	vec_add(&n->chars[0], (nnn = new_NFAState(ls)));
      else
	nnn = n->chars[0].v[0];
      vec_add(&nnn->accepts, a);
    } else if (a->kind == ACTION_SHIFT && a->term->kind == TERM_STRING) {
      one = 1;
      nn = n;
      if (!a->term->ignore_case) {
	for (c = a->term->string; *c; c++) {
	  if (!nn->chars[*c].n) 
	    vec_add(&nn->chars[*c], (nnn = new_NFAState(ls)));
	  else
	    nnn = nn->chars[*c].v[0];
	  nn = nnn;
	}
      } else { /* use new states */
	for (c = a->term->string; *c; c++) {
	  if (isalpha(*c)) {
	    vec_add(&nn->chars[toupper(*c)], (nnn = new_NFAState(ls)));
	    vec_add(&nn->chars[tolower(*c)], nnn);
	  } else
	    vec_add(&nn->chars[*c], (nnn = new_NFAState(ls)));
	  nn = nnn;
	}
      }
      vec_add(&nn->accepts, a);
    }
  }
  /* now regexes */
  for (j = 0; j < s->shift_actions.n; j++) {
    a = s->shift_actions.v[j];
    if (a->kind == ACTION_SHIFT && a->term->kind == TERM_REGEX) {
      Action *trailing_context = (Action *)MALLOC(sizeof(Action));
      memcpy(trailing_context, a, sizeof(Action));
      trailing_context->kind = ACTION_SHIFT_TRAILING;
      one = 1;
      reg = a->term->string;
      vec_add(&n->epsilon, (nnn = new_NFAState(ls)));
      nn = new_NFAState(ls);
      ls->ignore_case = a->term->ignore_case;
      if (build_regex_nfa(ls, &reg, nnn, nn, trailing_context)) {
	a->term->trailing_context = 1;
	s->trailing_context = 1;
      }
      vec_add(&nn->accepts, a);
    }
  }
  if (one) {
    nfa_to_scanner(n, &s->scanner);
    compute_transitions(ls, &s->scanner);
  }
  free_VecNFAState(&ls->allnfas);
  ls->scanners++;
}

static LexState *
new_LexState() {
  LexState *ls = MALLOC(sizeof(LexState));
  memset(ls, 0, sizeof(LexState));
  vec_clear(&ls->allnfas);
  return ls;
}

void 
build_scanners(Grammar *g) {
  int i, j, k;
  State *s;
  LexState *ls = new_LexState();

  /* detect identical scanners */
  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    if (s->same_shifts)
      continue;
    for (j = 0; j < i; j++) {
      if (g->states.v[j]->same_shifts)
	continue;
      if (g->states.v[j]->shift_actions.n != s->shift_actions.n)
	continue;
      if (g->states.v[j]->scan_kind != s->scan_kind)
	continue;
      for (k = 0; k < g->states.v[j]->shift_actions.n; k++)
	if (s->shift_actions.v[k]->term != 
	    g->states.v[j]->shift_actions.v[k]->term)
	  break;
      if (k >= g->states.v[j]->shift_actions.n) {
	s->same_shifts = g->states.v[j];
	break;
      }
    }
  }
  /* build scanners */
  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    if (s->shift_actions.n) {
      if (s->same_shifts)
	s->scanner = s->same_shifts->scanner;
      else 
	build_state_scanner(ls, s);
    }
  }
  if (d_verbose_level)
    printf("%d scanners %d transitions\n", ls->scanners, ls->transitions);
  FREE(ls);
}

