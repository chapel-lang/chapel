#include <stdlib.h>
#include "files.h"
#include "parser.h"
#include "stringutil.h"
#include "symbol.h"
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
  yylineno = 1;

  char* modulename = filenameToModulename(filename);
  ModuleSymbol* newModule = Symboltable::defineModule(modulename, prelude);

  if (!prelude) {
    Symboltable::pushScope(SCOPE_MODULE);
  }

  yyin = openInputFile(filename);
  
  yystmtlist = nilStmt;
  yyparse();
  newModule->stmts = yystmtlist;

  closeInputFile(yyin);

  if (!prelude) {
    SymScope* modScope = Symboltable::popScope();
    modScope->setContext(nilStmt, newModule);
  }

  yyfilename = "<internal>";
  yylineno = 0;

  return newModule;
}

