#ifndef _codegen_H_
#define _codegen_H_

#include <stdio.h>
#include "stmt.h"

extern FILE* extheadfile;
extern FILE* intheadfile;

void codegen(char* infilename, char* compilerDir, Stmt* program);

#endif
