#ifndef _CREATE_AST_H_
#define _CREATE_AST_H_

#include "stmt.h"

extern Stmt* internalPreludeStmts;
extern Stmt* preludeStmts;
extern Stmt* program;

Stmt* fileToAST(char* filename, int debug);

#endif
