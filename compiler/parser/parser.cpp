#include <stdlib.h>
#include "files.h"
#include "parser.h"
#include "symtab.h"
#include "yy.h"

Stmt* yystmtlist = NULL;
char* yyfilename;
int yylineno;


Stmt* ParseFile(char* filename, bool prelude) {
  Stmt* stmtList = nilStmt;
  yyfilename = filename;
  yylineno = 1;

  if (!prelude) {
    Symboltable::pushScope(SCOPE_FILE);
  }

  yyin = openInputFile(filename);
  
  if (yyin == NULL) {
    fail("Cannot read '%s'", filename);
  } else {
    yystmtlist = nilStmt;
    yyparse();
    stmtList = yystmtlist;

    closeInputFile(yyin);
  }

  if (!prelude) {
    Symboltable::popScope();
  }

  yyfilename = "<internal>";
  yylineno = 0;

  return stmtList;
}

