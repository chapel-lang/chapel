/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/
#include "d.h"

typedef struct ScannerBlock { 
  int state_index; 
  int scanner_index; 
  int block_index; 
  ScanState **chars; 
  ScanStateTransition **transitions; 
} ScannerBlock;
typedef Vec(ScannerBlock*)	VecScannerBlock;
typedef Vec(State *)		VecState;

static int
scanner_size(State *s) {
  if (s->scanner.states.n < 255 && s->scanner.transitions.n < 255)
    return 1;
  if (s->scanner.states.n < 32384 && s->scanner.transitions.n < 32384)
    return 2;
  return 4;
}

static char *
make_type(int i) {
  switch (i) {
    case 1: return "unsigned char";
    case 2: return "unsigned short";
    case 4: return "unsigned int";
    default: d_fail("bad case"); return "";
  }
}

static char *
scanner_type(State *s) {
  return make_type(scanner_size(s));
}

static char *
make_u_type(int i) {
  switch (i) {
    case 1: return "uint8";
    case 2: return "uint16";
    case 4: return "uint32";
    default: d_fail("bad case"); return "";
  }
}

static char *
scanner_u_type(State *s) {
  return make_u_type(scanner_size(s));
}

static uint32
scanner_block_hash_fn(ScannerBlock *b, hash_fns_t *fns) {
  uint32 hash = 0;
  int i, block_size = (int)fns->data[0];
  ScanState **sb = b->chars;

  for (i = 0; i < block_size; i++) {
    hash *= 17;
    hash += sb[i] ? sb[i]->index + 2 : 1;
  }
  return hash;
}

static int
scanner_block_cmp_fn(ScannerBlock *a, ScannerBlock *b, hash_fns_t *fns) {
  int i, block_size = (int)fns->data[0];
  ScanState **sa = a->chars;
  ScanState **sb = b->chars;
    
  for (i = 0; i < block_size; i++) {
    if (sa[i] == sb[i])
      continue;
    if (!sa[i] || !sb[i])
      return 1;
    if (sa[i]->index != sb[i]->index)
      return 1;
  }
  return 0;
}

hash_fns_t 
scanner_block_fns = {
  (hash_fn_t)scanner_block_hash_fn,
  (cmp_fn_t)scanner_block_cmp_fn,
  {0, 0}
};

static uint32
trans_scanner_block_hash_fn(ScannerBlock *b, hash_fns_t *fns) {
  uint32 hash = 0;
  int i, block_size = (int)fns->data[0];
  ScanStateTransition **sb = b->transitions;

  for (i = 0; i < block_size; i++) {
    hash *= 3;
    hash += sb[i] ? sb[i]->index + 1 : 0;
  }
  return hash;
}

static int
trans_scanner_block_cmp_fn(ScannerBlock *a, ScannerBlock *b, hash_fns_t *fns) {
  int i, block_size = (int)fns->data[0];
  ScanStateTransition **sa = a->transitions;
  ScanStateTransition **sb = b->transitions;
    
  for (i = 0; i < block_size; i++) {
    if (sa[i] == sb[i])
      continue;
    if (!sa[i] || !sb[i])
      return 1;
    if (sa[i]->index != sb[i]->index)
      return 1;
  }
  return 0;
}

hash_fns_t 
trans_scanner_block_fns = {
  (hash_fn_t)trans_scanner_block_hash_fn,
  (cmp_fn_t)trans_scanner_block_cmp_fn,
  {0, 0}
};

static uint32
shift_hash_fn(Action *sa, hash_fns_t *fns) {
  return sa->term->index + (sa->kind == ACTION_SHIFT_TRAILING ? 1000000 : 0);
}

static int
shift_cmp_fn(Action *sa, Action *sb, hash_fns_t *fns) {
  return (sa->term->index != sb->term->index) || (sa->kind != sb->kind);
}

hash_fns_t 
shift_fns = {
  (hash_fn_t)shift_hash_fn,
  (cmp_fn_t)shift_cmp_fn,
  {0, 0}
};

static void
write_scanner_data_as_C(FILE *fp, Grammar *g, char *tag) {
  State *s;
  ScannerBlock *vsblock, *xv, *yv;
  VecScannerBlock scanner_block_hash[4], *pscanner_block_hash;
  VecScannerBlock trans_scanner_block_hash[4], *ptrans_scanner_block_hash;
  VecAction shift_hash;
  int nvsblocks, ivsblock, i, j, k, x, xx;
  VecScanState *ss;
  char speculative_code[256];
  Term *t;

  /* shift_actions */
  for (i = 0; i < g->terminals.n; i++) {
    t = g->terminals.v[i];
    if (t->regex_production && t->regex_production->rules.v[0]->speculative_code.code)
      sprintf(speculative_code, "d_speculative_reduction_code_%d_%d_%s", 
	      t->regex_production->index, t->regex_production->rules.v[0]->index, tag);
    else
      strcpy(speculative_code, "NULL");
    fprintf(fp, 
	    "D_Shift d_shift_%d_%s = { %d, %d, %d, %d, %d, %s };\n",
	    i, tag,
	    g->terminals.v[i]->index + g->productions.n,
	    g->terminals.v[i]->scan_kind,
	    g->terminals.v[i]->op_assoc,
	    g->terminals.v[i]->op_priority,
	    g->terminals.v[i]->term_priority,
	    speculative_code);
    g->write_line++;
    if (g->terminals.v[i]->trailing_context) {
      fprintf(fp, 
	      "D_Shift d_tshift_%d_%s = { %d, %d, %d, %d, %d, %s };\n",
	      i, tag,
	      g->terminals.v[i]->index + g->productions.n,
	      D_SCAN_TRAILING,
	      g->terminals.v[i]->op_assoc,
	      g->terminals.v[i]->op_priority,
	      g->terminals.v[i]->term_priority,
	      speculative_code);
      g->write_line++;
    }
  }
  fprintf(fp,"\n"); g->write_line++;
  /* scanners */
  nvsblocks = 0;
  for (i = 0; i < g->states.n; i++)
    nvsblocks += g->states.v[i]->scanner.states.n * g->scanner_blocks;
  vsblock = MALLOC((nvsblocks ? nvsblocks : 1) * sizeof(ScannerBlock));
  for (i = 0; i < 4; i++) {
    vec_clear(&scanner_block_hash[i]);
    vec_clear(&trans_scanner_block_hash[i]);
  }
  scanner_block_fns.data[0] = (void*)g->scanner_block_size;
  scanner_block_fns.data[1] = (void*)g;
  trans_scanner_block_fns.data[0] = (void*)g->scanner_block_size;
  trans_scanner_block_fns.data[1] = (void*)g;
  /* shift */
  vec_clear(&shift_hash);
  ivsblock = 0;
  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    if (s->same_shifts)
      continue;
    ss = &s->scanner.states;
    /* build accepts differences */
    for (j = 0; j < s->scanner.transitions.n; j++) {
      VecAction *va = &s->scanner.transitions.v[j]->accepts_diff;
      fprintf(fp, "D_Shift *d_accepts_diff_%d_%d_%s[] = {", 
	      i, j, tag);
      for (k = 0; k < va->n; k++) {
	if (va->v[k]->kind != ACTION_SHIFT_TRAILING)
	  fprintf(fp, "&d_shift_%d_%s,", va->v[k]->term->index, tag); 
	else
	  fprintf(fp, "&d_tshift_%d_%s,", va->v[k]->term->index, tag); 
      }
      fprintf(fp, "0};\n");
      g->write_line += 2;
    }
    if (s->scanner.transitions.n) {
      fprintf(fp, "D_Shift **d_accepts_diff_%d_%s[] = {\n", i, tag);
      for (j = 0; j < s->scanner.transitions.n; j++) {
	fprintf(fp, "d_accepts_diff_%d_%d_%s%s", 
	      i, j, tag, j == s->scanner.transitions.n - 1 ? "\n" : ",\n");
	if (j != s->scanner.transitions.n - 1)
	  g->write_line++;
      }
      fprintf(fp, "};\n\n");
      g->write_line += 3;
    }
    /* build scanner_block_hash */
    pscanner_block_hash = &scanner_block_hash[scanner_size(s)-1]; 
    ptrans_scanner_block_hash = &trans_scanner_block_hash[scanner_size(s)-1]; 
    for (j = 0; j < ss->n; j++) {
      if (!s->same_shifts) {
	for (k = 0; k < g->scanner_blocks; k++) {
	  vsblock[ivsblock].state_index = s->index;
	  vsblock[ivsblock].scanner_index = j;
	  vsblock[ivsblock].block_index = k;
	  vsblock[ivsblock].chars = 
	    (void*)&ss->v[j]->chars[k * g->scanner_block_size];
	  vsblock[ivsblock].transitions = 
	    (void*)&ss->v[j]->transition[k * g->scanner_block_size];
	  xv = &vsblock[ivsblock];
	  ivsblock++;
	  assert(ivsblock <= nvsblocks);
	  /* output state scanner blocks */
	  yv = set_add_fn(pscanner_block_hash, xv, &scanner_block_fns);
	  if (xv == yv) {
	    fprintf(fp, 
		    "%s d_scanner_%d_%d_%d_%s[SCANNER_BLOCK_SIZE] = {\n",
		    scanner_type(s), i, j, k, tag);
	    for (x = 0; x < g->scanner_block_size; x++) {
	      xx = x + k * g->scanner_block_size;
	      fprintf(fp, "%d", ss->v[j]->chars[xx] ?
		      ss->v[j]->chars[xx]->index + 1 : 0);
	      if (x != g->scanner_block_size) fprintf(fp, ",");
	      if (x % 16 == 15) { fprintf(fp, "\n"); g->write_line++; }
	    }
	    fprintf(fp, "};\n\n");
	    g->write_line += 3;
	  }
	  if (s->scan_kind != D_SCAN_LONGEST || s->trailing_context) {
	    /* output accept_diff scanner blocks */
	    yv = set_add_fn(ptrans_scanner_block_hash, xv, 
			    &trans_scanner_block_fns);
	    if (xv == yv) {
	      fprintf(fp, 
		"%s d_accepts_diff_%d_%d_%d_%s[SCANNER_BLOCK_SIZE] = {\n",
                scanner_type(s), i, j, k, tag);
	      for (x = 0; x < g->scanner_block_size; x++) {
		xx = x + k * g->scanner_block_size;
		fprintf(fp, "%d", ss->v[j]->transition[xx]->index);
		if (x != g->scanner_block_size) fprintf(fp, ",");
		if (x % 16 == 15) { fprintf(fp, "\n"); g->write_line++; }
	      }
	      fprintf(fp, "};\n\n");
	      g->write_line += 3;
	    }
	  }
	}
	/* output shifts */
	if (ss->v[j]->accepts.n) {
	  char tmp[256];
	  sprintf(tmp, "d_shift_%d_%d_%s", i, j, tag);
	  for (k = 0; k < ss->v[j]->accepts.n; k++) {
	    Action *a = ss->v[j]->accepts.v[k], *aa;
	    if (ss->v[j]->accepts.n == 1) {
	      a->temp_string = strdup(tmp);
	      aa = set_add_fn(&shift_hash, a, &shift_fns);
	      if (aa != a)
		continue;
	    }
	    /* output shifts */
	    if (!k) 
	      fprintf(fp, "D_Shift *%s[] = { ", tmp);
	    if (a->kind != ACTION_SHIFT_TRAILING)
	      fprintf(fp, "&d_shift_%d_%s%s",
		      a->term->index, tag, k == ss->v[j]->accepts.n - 1 ? ", NULL};\n\n" : ", ");
	    else
	      fprintf(fp, "&d_tshift_%d_%s%s",
		      a->term->index, tag, k == ss->v[j]->accepts.n - 1 ? ", NULL};\n\n" : ", ");
	    if (k == ss->v[j]->accepts.n - 1)
	      g->write_line += 2;
	  }
	}
      }
    }
  }
  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    ss = &s->scanner.states;
    ivsblock = 0;
    if (ss->n && !s->same_shifts) {
      /* output scanner state transition tables */
      fprintf(fp, "SB_%s d_scanner_%d_%s[%d] = {\n", 
	      scanner_u_type(s), i, tag, ss->n);
      g->write_line += 1;
      pscanner_block_hash = &scanner_block_hash[scanner_size(s)-1]; 
      for (j = 0; j < ss->n; j++) {
	Action *a;
	if (ss->v[j]->accepts.n) {
	  a = ss->v[j]->accepts.v[0];
	  if (ss->v[j]->accepts.n == 1) {
	    a = set_add_fn(&shift_hash, a, &shift_fns);
	    fprintf(fp, "{ %s, {", a->temp_string);
	  } else
	    fprintf(fp, "{ d_shift_%d_%d_%s, {", i, j, tag);
	} else
	  fprintf(fp, "{ NULL, {");
	for (k = 0; k < g->scanner_blocks; k++) {
	  ScannerBlock vs;
	  vs.state_index = s->index;
	  vs.scanner_index = j;
	  vs.block_index = k;
	  vs.chars = (void*)&ss->v[j]->chars[k * g->scanner_block_size];
	  vs.transitions = 
	    (void*)&ss->v[j]->transition[k * g->scanner_block_size];
	  xv = &vs;
	  yv = set_add_fn(pscanner_block_hash, xv, &scanner_block_fns);
	  assert(yv != xv);
	  fprintf(fp, "d_scanner_%d_%d_%d_%s", 
		  yv->state_index, yv->scanner_index,
		  yv->block_index, tag);
	  if (k != g->scanner_blocks-1) {
	    fprintf(fp, ", ");
	    if ((k % 2) == 1) { fprintf(fp,"\n  "); g->write_line += 1; }

	  }
	}
	if (j != ss->n-1) 
	  { fprintf(fp, "}},\n"); g->write_line += 1; }
	else
	  { fprintf(fp, "}}\n"); g->write_line += 1; }
      }
      fprintf(fp, "};\n\n");
      g->write_line += 2;
      if (s->scan_kind != D_SCAN_LONGEST || s->trailing_context) {
	/* output scanner accepts diffs tables */
	fprintf(fp, "SB_trans_%s d_transition_%d_%s[%d] = {\n", 
		scanner_u_type(s), i, tag, ss->n);
	g->write_line += 1;
	ptrans_scanner_block_hash = 
	  &trans_scanner_block_hash[scanner_size(s)-1]; 
	for (j = 0; j < ss->n; j++) {
	  fprintf(fp, "{{ ");
	  for (k = 0; k < g->scanner_blocks; k++) {
	    ScannerBlock vs;
	    vs.state_index = s->index;
	    vs.scanner_index = j;
	    vs.block_index = k;
	    vs.chars = (void*)&ss->v[j]->chars[k * g->scanner_block_size];
	    vs.transitions = 
	      (void*)&ss->v[j]->transition[k * g->scanner_block_size];
	    xv = &vs;
	    yv = set_add_fn(ptrans_scanner_block_hash, xv, 
			    &trans_scanner_block_fns);
	    assert(yv != xv);
	    fprintf(fp, "d_accepts_diff_%d_%d_%d_%s", 
		    yv->state_index, yv->scanner_index,
		    yv->block_index, tag);
	    if (k != g->scanner_blocks-1) {
	      fprintf(fp, ", ");
	      if ((k % 2) == 1) { fprintf(fp,"\n  "); g->write_line += 1; }
	    }
	  }
	  if (j != ss->n-1) 
	    { fprintf(fp, "}},\n"); g->write_line += 1; }
	  else
	    { fprintf(fp, "}}\n"); g->write_line += 1; }
	}
	fprintf(fp, "};\n\n");
	g->write_line += 2;
      }
    }
  }
  for (i = 0; i < 4; i++)
    vec_free(&scanner_block_hash[i]);
  FREE(vsblock);
}

#define reduction_index(_r) \
((_r)->same_reduction ? (_r)->same_reduction->index : (_r)->index)

static void
write_goto_data_as_C(FILE *fp, Grammar *g, char *tag) {
  Vec(int) vgoto;
  State *s;
  uint8 *goto_valid = NULL;
  int i, j, x, again, lowest, nvalid_bytes, sym, lowest_sym;

  nvalid_bytes = ((g->productions.n + g->terminals.n) + 7) / 8;
  goto_valid = MALLOC(nvalid_bytes);
  vec_clear(&vgoto);
  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    if (s->gotos.n) {
      /* check for goto on token */
      for (j = 0; j < s->gotos.n; j++)
	if (s->gotos.v[j]->elem->kind == ELEM_TERM &&
	    s->gotos.v[j]->elem->e.term->kind == TERM_TOKEN)
	  s->goto_on_token = 1;
      /* find lowest goto, set valid bits */
      memset(goto_valid, 0, nvalid_bytes);
      lowest = 0;
      lowest_sym = elem_symbol(g, s->gotos.v[0]->elem);
      SET_BIT(goto_valid, lowest_sym);
      for (j = 1; j < s->gotos.n; j++) {
	sym = elem_symbol(g, s->gotos.v[j]->elem);
	SET_BIT(goto_valid, sym);
	if (sym < lowest_sym) {
	  lowest = j;
	  lowest_sym = sym;
	}
      }
      /* insert into vgoto */
      again = 1;
      while (again) {
	again = 0;
	for (j = 0; j < s->gotos.n; j++) {
	  x = elem_symbol(g, s->gotos.v[j]->elem);
	  x -= lowest_sym;
	  while (vgoto.n <= x)
	    vec_add(&vgoto, 0);
	  if (vgoto.v[x]) {
	    again = 1;
	    /* undo the damage */
	    for (--j;j >= 0;j--) {
	      x = elem_symbol(g, s->gotos.v[j]->elem);
	      x -= lowest_sym;
	      vgoto.v[x] = 0;
	    }
	    lowest_sym--;
	    break;
	  } else
	    vgoto.v[x] = s->gotos.v[j]->state->index + 1;
	}
      }
      s->goto_table_offset = lowest_sym;
      /* valid bits */
      fprintf(fp, "unsigned char d_goto_valid_%d_%s[] = {\n", i, tag);
      g->write_line += 1;
      for (j = 0; j < nvalid_bytes; j++)
	fprintf(fp, "0x%x%s", goto_valid[j], 
		j == nvalid_bytes - 1 ? "" : ", ");
      fprintf(fp, "};\n");
      g->write_line += 1;
    } else
      s->goto_table_offset = -INT_MAX;
    /* reduce_actions */
    if (s->reduce_actions.n) {
      fprintf(fp, "D_Reduction *d_reductions_%d_%s[] = {", i, tag);
      for (j = 0; j < s->reduce_actions.n; j++)
	fprintf(fp, "&d_reduction_%d_%s%s", 
		reduction_index(s->reduce_actions.v[j]->rule), tag,
		j == s->reduce_actions.n - 1 ? "" : ", ");
      fprintf(fp, "};\n");
      g->write_line += 1;
    }
    /* modified_reduce_actions */
    if (s->right_epsilon_hints.n) {
      fprintf(fp, "D_RightEpsilonHint d_right_epsilon_hints_%d_%s[] = {",
	      i, tag);
      for (j = 0; j < s->right_epsilon_hints.n; j++) {
	fprintf(fp, "{ %d, %d, &d_reduction_%d_%s}%s",
		s->right_epsilon_hints.v[j]->depth,
		s->right_epsilon_hints.v[j]->state->index,
		reduction_index(s->right_epsilon_hints.v[j]->rule), tag,
		j == s->right_epsilon_hints.n - 1 ? "" : ", ");
      }
      fprintf(fp, "};\n");
      g->write_line += 1;
    }
  }
  /* gotos */
  if (vgoto.n) {
    fprintf(fp, "unsigned short d_gotos_%s[%d] = {\n", tag, vgoto.n);
      g->write_line += 1;
    for (j = 0; j < vgoto.n; j++) {
      fprintf(fp, "%d", vgoto.v[j]);
      if (j != vgoto.n - 1) fprintf(fp, ",");
      if (j % 16 == 15) { fprintf(fp, "\n"); g->write_line += 1; }

    }
    fprintf(fp, "};\n\n");
    g->write_line += 2;
  } else {
    fprintf(fp, "unsigned short d_gotos_%s[1] = {0};\n", tag);
    g->write_line += 1;
  }
}

static void
write_scanner_code_as_C(FILE *fp, Grammar *g, char *tag) {
  int i, j, l;
  Action *a;
  State *s;

  for (i = 0; i < g->states.n; i++) {
    s = g->states.v[i];
    for (j = 0; j < s->shift_actions.n; j++) {
      a = s->shift_actions.v[j];
      if (a->kind == ACTION_SHIFT && a->term->kind == TERM_CODE) {
	if (!s->scanner_code) {
	  s->scanner_code = 1;
	  fprintf(fp, "int d_scan_code_%d_%s(char **as, int *col, int *line,"
		  "unsigned short *symbol, int *term_priority,"
		  "unsigned char *op_assoc, int *op_priority) {\n"
		  "  int res;\n",
		  i, tag);
	  g->write_line += 1;
	}
	fprintf(fp, "  if ((res = ");
	l = strlen(a->term->string);
	if (a->term->string[l - 1] == ')') {
	  fwrite(a->term->string, l - 1, 1, fp);
	  fprintf(fp, ", ");
	} else
	  fprintf(fp, "%s(", a->term->string);
	fprintf(fp, "as, col, line, op_assoc, op_priority))) {\n"
		"    *symbol = %d;\n"
		"    *term_priority = %d;\n"
		"    return res;\n"
		"  }\n",
		a->term->index + g->productions.n, 
		a->term->term_priority);
	g->write_line += 1;
      }
    }
    if (s->scanner_code) {
      fprintf(fp, "  return 0;\n}\n\n");
      g->write_line += 3;
    }
  }
}

static int
find_symbol(Grammar *g, char *s, char *e, int kind) {
  while (*s && isspace(*s)) s++;
  if (e > s) {
    if (kind == D_SYMBOL_NTERM) {
      Production *p;
      if ((p = lookup_production(g, s, e-s)))
	return p->index;
    } else if (kind == D_SYMBOL_STRING) {
      int i;
      int found = -1;
      for (i = 0; i < g->terminals.n;i++)
	if (g->terminals.v[i]->kind == TERM_STRING &&
	    ((g->terminals.v[i]->term_name &&
	      strlen(g->terminals.v[i]->term_name) == e-s &&
	      !strncmp(s, g->terminals.v[i]->term_name, e-s)) ||
	     (!g->terminals.v[i]->term_name &&
	      g->terminals.v[i]->string_len == (e-s) &&
	      !strncmp(s, g->terminals.v[i]->string, e-s)))) {
	  if (found > 0) {
	    d_fail("attempt to find symbol for non-unique string '%s'\n",
		   g->terminals.v[i]->string);
	  } else
	    found = i;
	}
      if (found > 0)
	return found + g->productions.n;
    }
  }
  return -1;
}

static void
write_code_as_C(FILE *fp, Grammar *g, Rule *r, char *code,
		char *fname, int line, char *pathname) 
{
  char *c;

  if (g->write_line_directives) {
    fprintf(fp, "#line %d \"%s\"\n", line, pathname);
    g->write_line++;
  }
  fprintf(fp, "%s{ ", fname);
  c = code;
  while (*c) {
    if (*c == '\n')
      g->write_line++;
    if (*c == '$') {
      c++;
      if (*c == '#') {
	c++;
	if (isdigit(*c)) {
	  int n = atoi(c);
	  fprintf(fp, "(d_get_number_of_children((D_PN(_children[%d], _offset))))", n);
	  if (n > r->elems.n-1)
	    d_fail("$nXXXX greater than number of children at line %d", line);
	  while (isdigit(*c)) c++;
	} else
	  fprintf(fp, "(_n_children)");
      } else if (*c == 'g') {
	fprintf(fp, "(D_PN(_ps, _offset)->globals)");
	c++;
      } else if (*c == 'n') {
	++c;
	if (isdigit(*c)) {
	  int n = atoi(c);
	  fprintf(fp, "(*(D_PN(_children[%d], _offset)))", n);
	  if (n > r->elems.n-1)
	    d_fail("$nXXXX greater than number of children at line %d", line);
	  while (isdigit(*c)) c++;
	} else 
	  fprintf(fp, "(*(D_PN(_ps, _offset)))");
      } else if (*c == '$') {
	fprintf(fp, "(D_PN(_ps, _offset)->user)");
	c++;
      } else if (isdigit(*c)) {
	int n = atoi(c);
	fprintf(fp, "(D_PN(_children[%d], _offset)->user)", n);
	while (isdigit(*c)) c++;
      } else if (*c == '{') {
	char *e = ++c, *a;
	while (*e && *e != '}' && !isspace(*e)) e++;
	a = e;
	if (isspace(*a)) a++;
	while (*a && *a != '}') a++;
	if (!*a)
	  d_fail("unterminated ${...} at line %d", line);
	if (STREQ(c, e-c, "child")) {
	  char xx[2][4096], *x, *y;
	  int i = 0;
	  *xx[0] = 0; *xx[1] = 0;
	  while (*e != '}') {
	    char *ss = e, *n;
	    x = xx[i];
	    y = xx[!i];
	    while (*e && *e != '}' && *e != ',') e++;
	    if (!*e || ss == e)
	      d_fail("bad ${...} at line %d", line);
	    n = dup_str(ss, e);
	    if (!*y)
	      sprintf(x, "(D_PN(_children[%s], _offset))", n);
	    else
	      sprintf(x, "d_get_child(%s, %s)", y, n);
	    if (*e == ',') e++;
	    if (isspace(*e)) e++;
	    i = !i;
	  }
	  if (!xx[!i])
	    d_fail("empty ${child } at line %d", line);
	  fprintf(fp, "%s", xx[!i]);
	} else if (STREQ(c, e-c, "reject")) {
	  fprintf(fp, " return -1 ");
	} else if (STREQ(c, e-c, "free_below")) {
	  fprintf(fp, " free_D_ParseTreeBelow(_parser, (D_PN(_ps, _offset)))");
	} else if (STREQ(c, e-c, "scope")) {
	  fprintf(fp, "(D_PN(_ps, _offset)->scope)");
	} else if (STREQ(c, e-c, "parser")) {
	  fprintf(fp, "_parser");
	} else if (STREQ(c, e-c, "nterm")) {
	  fprintf(fp, "%d", find_symbol(g, e, a, D_SYMBOL_NTERM));
	} else if (STREQ(c, e-c, "string")) {
	  fprintf(fp, "%d", find_symbol(g, e, a, D_SYMBOL_STRING));
	} else if (STREQ(c, e-c, "pass")) {
	  D_Pass *p = find_pass(g, e, a);
	  if (!p)
	    d_fail("unknown pass '%s' line %d", dup_str(e, a), line);
	  fprintf(fp, "%d", p->index);
	} else
	  d_fail("bad $ escape in code line %u\n", line);
	c = a + 1;
      } else
	d_fail("bad $ escape in code line %u\n", line);
    } else { 
      fputc(*c, fp); 
      c++;
    }
  }
  fprintf(fp, "  return 0;");
  fprintf(fp, "}\n\n");
  g->write_line += 2;
  if (g->write_line_directives) {
    fprintf(fp, "#line %d \"%s\"\n", g->write_line, g->write_pathname);
    g->write_line++;
  }
}

static void
write_global_code_as_C(FILE *fp, Grammar *g, char *tag) {
  int i;
  char *c;

  for (i = 0; i < g->ncode; i++) {
    if (g->write_line_directives) {
      fprintf(fp, "#line %d \"%s\"\n", g->code[i].line, g->pathname);
      g->write_line++;
    }
    c = g->code[i].code;
    while (*c) {
      if (*c == '\n')
	g->write_line++;
      if (*c == '$') {
	c++;
	if (*c == '{') {
	  char *e = ++c, *a;
	  while (*e && *e != '}' && !isspace(*e)) ++e;
	  a = e;
	  if (isspace(*a)) ++a;
	  while (*a && *a != '}') a++;
	  if (STREQ(c, e-c, "nterm")) {
	    fprintf(fp, "%d", find_symbol(g, e, a, D_SYMBOL_NTERM));
	  } else if (STREQ(c, e-c, "string")) {
	    fprintf(fp, "%d", find_symbol(g, e, a, D_SYMBOL_STRING));
	  } else if (STREQ(c, e-c, "pass")) {
	    D_Pass *p = find_pass(g, e, a);
	    if (!p)
	      d_fail("unknown pass '%s' line %d", dup_str(e, a), g->code[i].line + i);
	    fprintf(fp, "%d", p->index);
	  } else
	    d_fail("bad $ escape in code line %u\n", g->code[i].line + i);
	  c = a + 1;
	}
	else
	  d_fail("bad $ escape in code line %u\n", g->code[i].line + i);
      } else {
	fputc(*c, fp);
	c++;
      }
    }
    fprintf(fp, "\n");
    g->write_line += 1;
  }
  if (g->write_line_directives) {
    fprintf(fp, "#line %d \"%s\"\n", g->write_line, g->write_pathname);
    g->write_line++;
  }
}

static char * reduction_args = "(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)";

static void
write_reductions_as_C(FILE *fp, Grammar *g, char *tag) {
  int i, j, k, l, pmax;
  Production *p, *pdefault;
  Rule *r, *rdefault = NULL;
  char final_code[256], speculative_code[256], pass_code[256];
  
  pdefault = lookup_production(g, "_", 1);
  if (pdefault) {
    rdefault = pdefault->rules.v[0];
    fprintf(fp, "int d_speculative_reduction_code_%d_%d_%s%s;\n",
	    rdefault->prod->index, rdefault->index, tag, reduction_args);
    g->write_line += 1;
    fprintf(fp, "int d_final_reduction_code_%d_%d_%s%s;\n",
	    rdefault->prod->index, rdefault->index, tag, reduction_args);
    g->write_line += 1;
    fprintf(fp, "extern D_ReductionCode d_pass_code_%d_%d_%s[];\n",
	    rdefault->prod->index, rdefault->index, tag);
    g->write_line += 1;
    for (i = 0; i < rdefault->pass_code.n; i++) {
      fprintf(fp, "int d_pass_code_%d_%d_%d_%s%s;\n",
	      i, rdefault->prod->index, rdefault->index, tag, reduction_args);
      g->write_line += 1;
    }
  }
  for (i = 0; i < g->productions.n; i++) {
    p = g->productions.v[i];
    for (j = p->rules.n - 1; j >= 0; j--) {
      r = p->rules.v[j];
      for (k = 0; k < j; k++)
	if (r->elems.n == p->rules.v[k]->elems.n &&
	    r->speculative_code.code == p->rules.v[k]->speculative_code.code &&
	    r->final_code.code == p->rules.v[k]->final_code.code &&
	    r->op_priority == p->rules.v[k]->op_priority &&
	    r->op_assoc == p->rules.v[k]->op_assoc &&
	    r->rule_priority == p->rules.v[k]->rule_priority &&
	    r->rule_assoc == p->rules.v[k]->rule_assoc &&
	    r->action_index == p->rules.v[k]->action_index) 
	{
	  if (r->pass_code.n != p->rules.v[k]->pass_code.n)
	    continue;
	  for (l = 0; l < r->pass_code.n; l++) {
	    if (!r->pass_code.v[l] && !p->rules.v[k]->pass_code.v[l])
	      continue;
	    if (!r->pass_code.v[l] || !p->rules.v[k]->pass_code.v[l])
	      goto Lcontinue;
	    if (r->pass_code.v[l]->code != p->rules.v[k]->pass_code.v[l]->code)
	      goto Lcontinue;
	  }
	  r->same_reduction = p->rules.v[k];
	  break;
	Lcontinue:;
	}
    }
    for (j = 0; j < p->rules.n; j++) {
      r = p->rules.v[j];
      if (r->same_reduction)
	continue;
      if (r->speculative_code.code) {
	char fname[256];
	sprintf(fname, "int d_speculative_reduction_code_%d_%d_%s%s ",
		r->prod->index, r->index, tag, reduction_args);
	write_code_as_C(fp, g, r, r->speculative_code.code, fname, r->speculative_code.line, g->pathname);
      }
      if (r->final_code.code) {
	char fname[256];
	sprintf(fname, "int d_final_reduction_code_%d_%d_%s%s ",
		r->prod->index, r->index, tag, reduction_args);
	write_code_as_C(fp, g, r, r->final_code.code, fname, r->final_code.line, g->pathname);
      }
      for (k = 0; k < r->pass_code.n; k++) {
	if (r->pass_code.v[k]) {
	  char fname[256];
	  sprintf(fname, "int d_pass_code_%d_%d_%d_%s%s ",
		  k, r->prod->index, r->index, tag, reduction_args);
	  write_code_as_C(fp, g, r, r->pass_code.v[k]->code, fname, r->pass_code.v[k]->line, g->pathname);
	}
      }
      if (r->speculative_code.code)
	  sprintf(speculative_code, "d_speculative_reduction_code_%d_%d_%s", 
		  r->prod->index, r->index, tag);
      else if (rdefault && rdefault->speculative_code.code)
	sprintf(speculative_code, "d_speculative_reduction_code_%d_%d_%s", 
		rdefault->prod->index, rdefault->index, tag);
      else 
	strcpy(speculative_code, "NULL");
      if (r->final_code.code)
	sprintf(final_code, "d_final_reduction_code_%d_%d_%s", r->prod->index, r->index, tag);
      else if (rdefault && rdefault->final_code.code)
	sprintf(final_code, "d_final_reduction_code_%d_%d_%s", 
		rdefault->prod->index, rdefault->index, tag);
      else
	strcpy(final_code, "NULL");
      pmax = r->pass_code.n;
      if (r->pass_code.n || (rdefault && rdefault->pass_code.n)) {
	if (rdefault && rdefault->pass_code.n > pmax)
	  pmax = rdefault->pass_code.n;
	if (!r->pass_code.n)
	  sprintf(pass_code, "d_pass_code_%d_%d_%s", 
		  rdefault->prod->index, rdefault->index, tag);
	else {
	  sprintf(pass_code, "d_pass_code_%d_%d_%s", 
		  r->prod->index, r->index, tag);
	  fprintf(fp, "D_ReductionCode %s[] = {", pass_code);
	  for (k = 0; k < pmax; k++) {
	    if (r->pass_code.n > k && r->pass_code.v[k])
	      fprintf(fp, "d_pass_code_%d_%d_%d_%s%s", k, r->prod->index, r->index, tag,
		      k < pmax-1 ? ", " : "");
	    else
	      if (rdefault && rdefault->pass_code.n > k && rdefault->pass_code.v[k])
		fprintf(fp, "d_pass_code_%d_%d_%d_%s%s", k, rdefault->prod->index, 
			rdefault->index, tag, k < pmax-1 ? ", " : "");
	      else
		fprintf(fp, "NULL%s", k < pmax-1 ? ", " : "");
	  } 
	  fprintf(fp, "};\n\n");
	  g->write_line += 2;
	}
      } else
	strcpy(pass_code, "NULL");
      fprintf(fp, "D_Reduction d_reduction_%d_%s = ", r->index, tag);
      fprintf(fp, "{%d, %d, %s, %s, %d, %d, %d, %d, %d, %d, %s};\n",
	      r->elems.n, r->prod->index, 
	      speculative_code, final_code, 
	      r->op_assoc, r->rule_assoc,
	      r->op_priority, r->rule_priority,
	      r->prod->internal ? -1 : r->action_index,
	      pmax, pass_code);
      g->write_line += 1;
    }
  }
}

static uint32
er_hint_hash_fn(State *a, hash_fns_t *fns) {
  VecHint *sa = &a->error_recovery_hints;
  uint32 hash = 0, i;
  Term *ta;

  for (i = 0; i < sa->n; i++) {
    ta = sa->v[i]->rule->elems.v[sa->v[i]->rule->elems.n - 1]->e.term;
    hash += (sa->v[i]->depth + 1) * 13;
    hash += strhashl(ta->string, ta->string_len);
    if (sa->v[i]->rule)
      hash += sa->v[i]->rule->prod->index * 10007;
  }
  return hash;
}

static int
er_hint_cmp_fn(State *a, State *b, hash_fns_t *fns) {
  int i;
  VecHint *sa = &a->error_recovery_hints, *sb = &b->error_recovery_hints;
  Term *ta, *tb;
  if (sa->n != sb->n)
    return 1;
  for (i = 0; i < sa->n; i++) {
    ta = sa->v[i]->rule->elems.v[sa->v[i]->rule->elems.n - 1]->e.term;
    tb = sb->v[i]->rule->elems.v[sb->v[i]->rule->elems.n - 1]->e.term;
    if (sa->v[i]->depth != sb->v[i]->depth ||
	strcmp(ta->string, tb->string) ||
	sa->v[i]->rule->prod->index != sb->v[i]->rule->prod->index)
      return 1;
  }
  return 0;
}

hash_fns_t 
er_hint_hash_fns = {
  (hash_fn_t)er_hint_hash_fn,
  (cmp_fn_t)er_hint_cmp_fn,
  {0, 0}
};

static void
write_error_data_as_C(FILE *fp, Grammar *g, VecState *er_hash, char *tag) {
  int i, j;
  State *s;
  Term *t;
  State *h;
  char *ss;

  if (g->states.n) {
    for (i = 0; i < g->states.n; i++) {
      s = g->states.v[i];
      if (s->error_recovery_hints.n) {
	h = set_add_fn(er_hash, s, &er_hint_hash_fns);
	if (h == s) {
	  fprintf(fp,
		  "D_ErrorRecoveryHint d_error_recovery_hints_%d_%s[] = {%s",
		  i, tag, s->error_recovery_hints.n > 1 ? "\n" : "");
	  for (j = 0; j < s->error_recovery_hints.n; j++) {
	    t = s->error_recovery_hints.v[j]->rule->elems.v[
	      s->error_recovery_hints.v[j]->rule->elems.n - 1]->e.term;
	    ss = escape_string(t->string);
	    fprintf(fp, "{ %d, %d, \"%s\"}%s", 
		    s->error_recovery_hints.v[j]->depth,
		    s->error_recovery_hints.v[j]->rule->prod->index,
		    ss,
		    j == s->error_recovery_hints.n - 1 ? "" : ",\n");

	    if (j != s->error_recovery_hints.n - 1)
	      g->write_line += 1;
	    FREE(ss);
	  }
	  fprintf(fp, "};\n");
	  g->write_line += 1;
	}
      }
    }
  }
}

static char *scan_kind_strings[] = {"D_SCAN_ALL", "D_SCAN_LONGEST", "D_SCAN_MIXED",  NULL};

static void
write_state_data_as_C(FILE *fp, Grammar *g, VecState *er_hash, char *tag) {
  int i;
  State *s, *h, *shifts;

  fprintf(fp, "\nD_State d_states_%s[] = ", tag);
  if (g->states.n) {
    fprintf(fp, "{\n");
    for (i = 0; i < g->states.n; i++) {
      s = g->states.v[i];
      shifts = s->same_shifts ? s->same_shifts : s;
      fprintf(fp, "{ ");
      if (s->gotos.n)
	fprintf(fp,"d_goto_valid_%d_%s, ", i, tag);
      else
	fprintf(fp,"NULL, ");
      fprintf(fp,"%d, ", s->goto_table_offset);
      if (s->reduce_actions.n)
	fprintf(fp, "{ %d, d_reductions_%d_%s}, ", s->reduce_actions.n, 
		i, tag);
      else
	fprintf(fp, "{ 0, NULL}, ");
      if (s->right_epsilon_hints.n)
	fprintf(fp, "{ %d, d_right_epsilon_hints_%d_%s}, ", 
		s->right_epsilon_hints.n, i, tag);
      else
	fprintf(fp, "{ 0, NULL}, ");
      if (s->error_recovery_hints.n) {
	h = set_add_fn(er_hash, s, &er_hint_hash_fns);
	fprintf(fp, "{ %d, d_error_recovery_hints_%d_%s}, ", 
		s->error_recovery_hints.n, h->index, tag);
      } else
	fprintf(fp, "{ 0, NULL}, ");
      if (s->shift_actions.n || s->scanner_code || (g->scanner.code && s->goto_on_token))
	fprintf(fp, "1, ");
      else
	fprintf(fp, "0, ");
      if (g->scanner.code) {
	if (s->goto_on_token) 
	  fprintf(fp, "%s, ", g->scanner.code);
	else
	  fprintf(fp, "NULL, ");
      } else if (s->scanner_code)
	fprintf(fp, "d_scan_code_%d_%s, ", i, tag);
      else
	fprintf(fp, "NULL, ");
      if (s->scanner.states.n)
	fprintf(fp, "(void*)d_scanner_%d_%s, ", shifts->index, tag);
      else
	fprintf(fp, "NULL, ");
      fprintf(fp, "sizeof(%s), ", scanner_type(s));
      fprintf(fp, s->accept ? "1, " : "0, ");
      fprintf(fp, "%s, ", scan_kind_strings[s->scan_kind]);
      if ((shifts->scan_kind != D_SCAN_LONGEST || shifts->trailing_context)
	  && shifts->scanner.states.n)
	fprintf(fp, "(void*)d_transition_%d_%s, ", shifts->index, tag);
      else
	fprintf(fp, "NULL, ");
      if ((shifts->scan_kind != D_SCAN_LONGEST || shifts->trailing_context)
	  && shifts->scanner.states.n)
	fprintf(fp, "d_accepts_diff_%d_%s, ", shifts->index, tag);
      else
	fprintf(fp, "(D_Shift***)NULL, ");
      if (s->reduces_to)
	fprintf(fp, "%d", s->reduces_to->index);
      else
	fprintf(fp, "-1");
      fprintf(fp, "}%s\n", i == g->states.n - 1 ? "" : ",");
    }
    fprintf(fp, "};\n\n");
  } else
    fprintf(fp, "{{0, {0, NULL}, 0, NULL}};\n\n");
}

static int
write_header_as_C(Grammar *g, char *base_pathname, char *tag) {
  char pathname[FILENAME_MAX];
  char ver[30];
  int i, tokens = 0, states = 0, col;
  FILE *hfp;

  for (i = 0; i < g->terminals.n; i++)
    if (g->terminals.v[i]->kind == TERM_TOKEN)
      tokens = 1;
  if (g->states_for_all_nterms)
    states = 1;
  else
    for (i = 0; i < g->productions.n; i++)
      if (state_for_declaration(g, i))
	states = 1;
  if (g->write_header > 0 || (g->write_header < 0 && (tokens || states))) {
    strcpy(pathname, base_pathname);
    strcat(pathname, ".d_parser.h");
    hfp = fopen(pathname, "w");
    if (!hfp)
      d_fail("unable to open `%s` for write\n", pathname);
    d_version(ver);
    fprintf(hfp, "/*\n  Generated by Make DParser Version %s\n", ver);  
    fprintf(hfp, "  Available at http://dparser.sf.net\n*/\n\n");  
    fprintf(hfp, "#ifndef _%s_h\n", tag);
    fprintf(hfp, "#define _%s_h\n", tag);
    if (tokens) {
      if (!g->token_type) {
	for (i = 0; i < g->terminals.n; i++)
	  if (g->terminals.v[i]->kind == TERM_TOKEN)
	    fprintf(hfp, "#define %s \t%d\n",
		    g->terminals.v[i]->string,
		    g->terminals.v[i]->index + g->productions.n);
      } else {
	fprintf(hfp, "enum D_Tokens_%s {\n", tag);
	col = 0;
	for (i = 0; i < g->terminals.n; i++) {
	  if (g->terminals.v[i]->kind == TERM_TOKEN) {
	    col += g->terminals.v[i]->string_len + 7;
	    if (col > 70) { printf("\n"); col = 0; }
	    fprintf(hfp, "%s = %d%s",
		    g->terminals.v[i]->string,
		    g->terminals.v[i]->index + g->productions.n,
		    i == g->terminals.n-1 ? "" : ", ");
	  }
	}
	fprintf(hfp, "\n};\n");
      }
    }
    if (states) {
      for (i = 0; i < g->productions.n; i++)
	if (!g->productions.v[i]->internal && g->productions.v[i]->elem)
	  fprintf(hfp, "#define D_START_STATE_%s \t%d\n",
		  g->productions.v[i]->name, g->productions.v[i]->state->index);
    }
    fprintf(hfp, "#endif\n");
    fclose(hfp);
    return 1;
  }
  return 0;
}

#define is_EBNF(_x) (_x == INTERNAL_CONDITIONAL || _x == INTERNAL_STAR || _x == INTERNAL_PLUS)
static char *d_internal[] = {"D_SYMBOL_NTERM", "D_SYMBOL_EBNF", "D_SYMBOL_INTERNAL"};
static char *d_symbol[] = { 
  "D_SYMBOL_STRING", "D_SYMBOL_REGEX", "D_SYMBOL_CODE", "D_SYMBOL_TOKEN" };
static void
write_symbol_data_as_C(FILE *fp, Grammar *g, char *tag) {
  int i;
  fprintf(fp, "D_Symbol d_symbols_%s[] = {\n", tag);
  g->write_line += 1;
  for (i = 0; i < g->productions.n; i++) {
    int state = -1;
    if (!g->productions.v[i]->internal && g->productions.v[i]->elem)
      state = g->productions.v[i]->state->index;
    fprintf(fp, "{%s, \"%s\", %d, %d},\n", 
	    d_internal[g->productions.v[i]->internal ? 
		      (is_EBNF(g->productions.v[i]->internal) ? 2 : 1) : 0],
	    g->productions.v[i]->name,
	    g->productions.v[i]->name_len, 
	    state);
    g->write_line += 1;
  }
  for (i = 0; i < g->terminals.n; i++) {
    char *s = escape_string(g->terminals.v[i]->string); /* so it is a string */
    char *ss = escape_string(s);   /* so it is printable */
    char *name = g->terminals.v[i]->term_name ? g->terminals.v[i]->term_name : ss;
    fprintf(fp, "{%s, \"%s\", %d},\n", d_symbol[g->terminals.v[i]->kind], 
        	name, (int) strlen(name));
    g->write_line += 1;
    FREE(s);
    FREE(ss);
  }
  fprintf(fp, "};\n\n");
  g->write_line += 2;
}

static void
write_passes_as_C(FILE *fp, Grammar *g, char *tag) {
  int i;
  if (g->passes.n) {
    fprintf(fp, "D_Pass d_passes_%s[] = {\n", tag);
    g->write_line += 1;
    for (i = 0; i < g->passes.n; i++) {
      D_Pass *p = g->passes.v[i];
      fprintf(fp, "{\"%s\", %d, 0x%X, %d}%s\n", 
	      p->name, p->name_len, p->kind, p->index,
	      i < g->passes.n-1 ? ", " : "");
    }
    fprintf(fp, "};\n\n");
    g->write_line += 2;
  }
}

void
write_parser_tables_as_C(Grammar *g, char *base_pathname, char *tag) {
  char pathname[FILENAME_MAX];
  char ver[30];
  FILE *fp;
  int whitespace_production = 0, header;
  VecState er_hash;
  Production *p;

  vec_clear(&er_hash);

  strcpy(pathname, base_pathname);
  strcat(pathname, ".d_parser.");
  strcat(pathname, g->write_extension);
  g->write_pathname = pathname;
  fp = fopen(pathname, "w");
  if (!fp)
    d_fail("unable to open `%s` for write\n", pathname);

  header = write_header_as_C(g, base_pathname, tag);

  d_version(ver);
  fprintf(fp, "/*\n  Generated by Make DParser Version %s\n", ver);  
  fprintf(fp, "  Available at http://dparser.sf.net\n*/\n\n");  
  g->write_line = 7;
  write_global_code_as_C(fp, g, tag);
  fprintf(fp, "#include \"dparse.h\"\n");
  g->write_line++;
  if (header) {
    fprintf(fp, "#include \"%s.d_parser.h\"\n", base_pathname);
    g->write_line++;
  }
  fprintf(fp, "\n");
  g->write_line++;

  write_reductions_as_C(fp, g, tag);
  write_scanner_data_as_C(fp, g, tag);
  write_scanner_code_as_C(fp, g, tag);
  write_goto_data_as_C(fp, g, tag);
  write_error_data_as_C(fp, g, &er_hash, tag);
  write_state_data_as_C(fp, g, &er_hash, tag);
  write_symbol_data_as_C(fp, g, tag);
  write_passes_as_C(fp, g, tag);

  if ((p = lookup_production(g, "whitespace", sizeof("whitespace")-1)))
    whitespace_production = p->state->index;

  fprintf(fp, "D_ParserTables parser_tables_%s = {\n", tag);
  fprintf(fp, "%d, ", g->states.n);
  fprintf(fp, "d_states_%s, ", tag);
  fprintf(fp, "d_gotos_%s, ", tag);
  fprintf(fp, "%d, ", whitespace_production);
  fprintf(fp, "%d, ", g->productions.n + g->terminals.n);
  fprintf(fp, "d_symbols_%s, ", tag);
  if (g->default_white_space)
    fprintf(fp, "%s, ", g->default_white_space);
  else
    fprintf(fp, "NULL, ");
  fprintf(fp, "%d, ", g->passes.n);
  if (g->passes.n)
    fprintf(fp, "d_passes_%s, ", tag);
  else
    fprintf(fp, "NULL, ");
  if (g->save_parse_tree)
    fprintf(fp, "1");
  else
    fprintf(fp, "0");
  fprintf(fp, "};\n");
  fclose(fp);
}

int
write_ctables(Grammar *g) {
  g->scanner_block_size = 256/g->scanner_blocks;
  write_parser_tables_as_C(g, g->pathname, 
			   *g->grammar_ident ? g->grammar_ident : NULL);
  return 0;
}
