/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _parse_H_
#define _parse_H_

#include "geysa.h"
#include "parse_structs.h"
#include "dparse.h"
#include "if1.h"
#include "grammar.h"
#include "parse_ast.h"

void no_preprocessor_whitespace(D_Parser *p, d_loc_t *loc, void **p_user_globals);
void get_ast(Vec<ParseAST *> &asts, D_ParseNode *pn);

#endif
