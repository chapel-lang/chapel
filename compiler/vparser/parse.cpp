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

void 
get_ast(Vec<ParseAST *> &asts, D_ParseNode *pn) {
  if (pn->user.ast)
    asts.add(pn->user.ast);
  else
    for (int i = 0; i < d_get_number_of_children(pn); i++)
      get_ast(asts, d_get_child(pn, i));
}

