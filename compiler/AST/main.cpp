#include <stdlib.h>
#include "expr.h"
#include "link.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"


Stmt* program;
Symbol* yypst;

char* yyfilename;
int yylineno;


static char* preludeName = "prelude.chpl";


static void ParseFile(char* filename) {
  yyfilename = filename;
  yylineno = 1;

  if (filename != preludeName) {
    Symboltable::pushScope(SCOPE_FILE);
  }
    

  yyin = fopen(yyfilename, "r");
  
  if (yyin == NULL) {
    fprintf(stdout, "ERROR: Can't open file %s\n", yyfilename);
    exit(1);
  }

  yyparse();
  fclose(yyin);

  if (filename != preludeName) {
    Symboltable::popScope();
  }
}


int main(int argc, char* argv[]) {
  int i;

  initType();
  initExpr();

  //  yydebug = 1;

  ParseFile(preludeName);
  
  if (argc > 1) {
    for (i=1; i<argc; i++) {
      ParseFile(argv[i]);

      program->printList(stdout, "\n");
    }
  } else {
    fprintf(stdout, "Usage: %s <filenames>\n", argv[0]);
    exit(1);
  }

  return 0;
}
