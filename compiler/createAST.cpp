#include <stdlib.h>
#include "createAST.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "link.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"


Stmt* program;
Symbol* yypst;

char* yyfilename;
int yylineno;


static void ParseFile(char* filename, bool prelude = false) {
  yyfilename = filename;
  yylineno = 1;

  if (!prelude) {
    Symboltable::pushScope(SCOPE_FILE);
  }

  yyin = openInputFile(filename);
  
  if (yyin == NULL) {
    fail("Cannot read '%s'", filename);
  } else {
    program = new NullStmt();
    yyparse();

    closeInputFile(yyin);
  }

  if (!prelude) {
    Symboltable::popScope();
  }
}


Stmt* fileToAST(char* filename, int debug) {
  static char* preludePath = NULL;

  if (preludePath == NULL) {
    initType(); // BLC : clean these up
    initExpr();

    preludePath = glomstrings(2, system_dir, "/AST/prelude.chpl");
    ParseFile(preludePath, true);
  }

  yydebug = debug;
  ParseFile(filename);

  return program;
}
