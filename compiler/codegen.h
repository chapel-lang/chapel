#ifndef _codegen_H_
#define _codegen_H_

#include "fa.h"
#include "stmt.h"

extern FILE* extheadfile;
extern FILE* intheadfile;

void genDT(FILE* outfile, Sym* pdt);
void genASTDecls(FILE* outfile, AST* ast);
void genAST(FILE* outfile, AST* ast);

void codegen(FA* fa, char* filename, char* compilerDir);
void codegen(char* filename, char* compilerDir, Stmt* program);

#endif
