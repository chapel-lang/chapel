#include <stdlib.h>
#include "countTokens.h"
#include "files.h"
#include "parser.h"
#include "stringutil.h"
#include "symtab.h"
#include "symbol.h"
#include "chapel.tab.h"
#include "yy.h"

AList<Stmt>* yystmtlist = NULL;
char* yyfilename;
int yylineno;
int yystartlineno;


static char* filenameToModulename(char* filename) {
  char* modulename = stringcpy(filename);
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


ModuleSymbol* ParseFile(char* filename, modType moduletype) {
  yyfilename = filename;
  yylloc.first_column = yylloc.last_column = yylloc.first_line = yylloc.last_line = yystartlineno = yylineno = 0;

  char* modulename = filenameToModulename(filename);
  ModuleSymbol* newModule = Symboltable::startModuleDef(modulename, 
                                                        moduletype);

  yylloc.first_column = yylloc.last_column = 0;
  yylloc.first_line = yylloc.last_line = yystartlineno = yylineno = 1;
  yyin = openInputFile(filename);
  
  yystmtlist = NULL;
  if (moduletype == MOD_USER) {
    startCountingFileTokens(filename);
  }
  yyparse();
  if (moduletype == MOD_USER) {
    stopCountingFileTokens();
  }

  closeInputFile(yyin);

  Symboltable::finishModuleDef(newModule, yystmtlist);

  if (BlockStmt* first = dynamic_cast<BlockStmt*>(newModule->stmts->first())) {
    if (first->body->isEmpty()) {
      newModule->addPragmas(&first->pragmas);
    }
  }

  yyfilename = "<internal>";

  yylloc.first_column = yylloc.last_column = 0;
  yylloc.first_line = yylloc.last_line = yystartlineno = yylineno = -1;

  return newModule;
}

