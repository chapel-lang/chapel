#include <stdlib.h>
#include "files.h"
#include "parser.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"

Stmt* yystmtlist = NULL;
char* yyfilename;
int yylineno;


static char* filenameToModulename(char* filename) {
  char* modulename = copystring(filename);
  char* lastslash = strrchr(modulename, '/');
  if (lastslash) {
    modulename = lastslash+1;
  }
  char* dot = strchr(modulename, '.');
  if (dot) {
    *dot = '\0';
  }

  return modulename;
}


ModuleSymbol* ParseFile(char* filename, bool prelude) {
  yyfilename = filename;
  yylineno = 0;

  char* modulename = filenameToModulename(filename);
  ModuleSymbol* newModule = Symboltable::startModuleDef(modulename, prelude);

  yylineno = 1;
  yyin = openInputFile(filename);
  
  yystmtlist = NULL;
  yyparse();

  closeInputFile(yyin);

  Symboltable::finishModuleDef(newModule, yystmtlist);

  yyfilename = "<internal>";
  yylineno = 0;

  return newModule;
}

