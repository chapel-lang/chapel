#ifndef _codegen_H_
#define _codegen_H_

#include <stdio.h>

class Sym;
class FA;
class Stmt;
class ParseAST;

extern FILE* extheadfile;
extern FILE* intheadfile;

void genDT(FILE* outfile, Sym* pdt);
void genASTDecls(FILE* outfile, ParseAST* ast);
void genAST(FILE* outfile, ParseAST* ast);

void codegen(FA* fa, char* filename, char* compilerDir);
void codegen(char* filename, char* compilerDir, Stmt* program);

#endif
