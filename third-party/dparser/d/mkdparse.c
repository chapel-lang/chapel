/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/

#include "gramgram.h"
#include "d.h"
#include "mkdparse.h"

static void 
mkdparse_internal(Grammar *g, char *grammar_pathname, char *str) {
  if (!grammar_pathname)
    grammar_pathname = "-";
  if (parse_grammar(g, grammar_pathname, str) < 0)
    d_fail("unable to parse grammar '%s'", grammar_pathname);
  if (g->productions.n < 2)
    d_fail("no productions in grammar '%s'", grammar_pathname);
  if (build_grammar(g) < 0)
    d_fail("unable to load grammar '%s'", grammar_pathname); 
}

void 
mkdparse(Grammar *g, char *grammar_pathname) {
  mkdparse_internal(g, grammar_pathname, 0);
}

void 
mkdparse_from_string(Grammar *g, char *str) {
  mkdparse_internal(g, 0, str);
}

