/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "parse.h"

static char _wspace[256] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0, 0, 0 /* zero padded */
};

#define wspace(_x) (_wspace[(unsigned char)_x])

void
no_preprocessor_whitespace(D_Parser *p, d_loc_t *loc, void **p_user_globals) {
  int rec = 0;
  char *s = loc->s, *scol;

  if (p->loc.s == s) 
    scol = s; 
  else 
    scol = 0;
 Lmore:
  while (wspace(*s)) s++;
  if (*s == '\n') {
    loc->line++; 
    scol = s + 1;
    s++;
    goto Lmore;
  }
  if (s[0] == '/') {
    if (s[1] == '/') {
      while (*s && *s != '\n') { s++; }
      loc->line++;
      s++;
      goto Lmore;
    }
    if (s[1] == '*') {
      s += 2;
    LnestComment:
      rec++;
    LmoreComment:
      while (*s) {
	if (s[0] == '*' && s[1] == '/') {
	  s += 2;
	  rec--;
	  if (!rec)
	    goto Lmore;
	  goto LmoreComment;
	}
	if (s[0] == '/' && s[1] == '*') {
	  s += 2;
	  goto LnestComment;
	}
	if (*s == '\n') { 
	  loc->line++; 
	  scol = s + 1; 
	}
	s++;
      }
    }
  }
  if (scol)
    loc->col = s - scol;
  else
    loc->col = -1;
  loc->s = s;
  return;
}

static char *v_int_type_names[IF1_INT_TYPE_NUM][2] = {
  { "uint8",   "int8" },
  { "uint16",  "int16" },
  { "uint32",  "int32" },
  { "uint64",  "int64" }
};
static char *v_float_type_names[IF1_FLOAT_TYPE_NUM] = {
  "float32",  "float64", "float80", "float128"
};
static int v_float_type_sizes[IF1_FLOAT_TYPE_NUM] = {
  32, 64, 80, 128
};

void
set_primitive_types(IF1 *if1) {
  for (int signd = 0; signd < 2; signd++)
    for (int s = 0; s < IF1_INT_TYPE_NUM; s++) {
      char *tt = v_int_type_names[s][signd];
      Sym *ss = if1_get_builtin(if1, tt, tt+strlen(tt));
      if (!ss) fail("unable to find builtin type '%s'", tt);
      if1_set_int_type(if1, ss, signd, 8 << (s-1));
    }
  for (int s = 0; s < IF1_FLOAT_TYPE_NUM; s++) {
    char *tt = v_float_type_names[s];
    if (tt) {
      Sym *ss = if1_get_builtin(if1, tt, tt+strlen(tt));
      if (!ss) fail("unable to find builtin type '%s'", tt);
      if1_set_float_type(if1, ss, v_float_type_sizes[s]);
    }
  }
}

void 
get_ast(Vec<AST *> &asts, D_ParseNode *pn) {
  if (pn->user.ast)
    asts.add(pn->user.ast);
  else
    for (int i = 0; i < d_get_number_of_children(pn); i++)
      get_ast(asts, d_get_child(pn, i));
}

