/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _parse_H_
#define _parse_H_

#include "parse_structs.h"
#include <dparse.h>
#include "if1.h"
#include "ast.h"

void no_preprocessor_whitespace(D_Parser *p, d_loc_t *loc, void **p_user_globals);
void set_primitive_types(IF1 *if1);
void get_ast(Vec<AST *> &asts, D_ParseNode *pn);

#endif
