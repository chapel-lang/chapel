/*
  Copyright 2003-2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "ast.h"
#include "parse_ast.h"

ParseAST *
new_AST(AST_kind k, D_ParseNode *pn) {
  return new ParseAST(k, pn);
}
