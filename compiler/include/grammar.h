/* -*-Mode: c++;-*-
*/

#ifndef _grammar_H_
#define _grammar_H_

#include "extern.h"

#define internal_D_Sym(_x) \
  EXTERN char *_x##_start EXTERN_INIT(#_x); \
  EXTERN char *_x##_end EXTERN_INIT(_x##_start + sizeof(#_x) - 1)

internal_D_Sym(__module);

#define find_internal(_s, _x) find_D_Sym(_s, _x##_start, _x##_end)
#define new_internal(_s, _x) NEW_D_SYM(_s, _x##_start, _x##_end)

char *constant_type(D_ParseNode &pn, D_Symbol *symbols);
void in_module(Globals *g, char *s, char *e, D_Scope **scope);
ParseAST *loop_AST(D_ParseNode &loop, D_ParseNode &cond, D_ParseNode *before, 
		   D_ParseNode *after, D_ParseNode &body);
ParseAST *symbol_AST(IF1 *if1, D_ParseNode *pn);
ParseAST *op_AST(IF1 *if1, D_ParseNode &pn);

#endif
