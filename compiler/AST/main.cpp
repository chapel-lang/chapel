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


int main(int argc, char* argv[]) {

  initType();
  initExpr();

  yyfilename = "prelude.chpl";
  yylineno = 1;

  if ((yyin = fopen(yyfilename, "r")) == NULL) {
    printf("Can't open file %s\n", yyfilename);
  }
  yyparse();
  fclose(yyin);

  Symboltable::pushScope(SCOPE_FILE);

  yyfilename = copystring(argv[1]);
  yylineno = 1;
  if (argc > 1) {
    if ((yyin = fopen(yyfilename, "r")) == NULL) {
      printf("Can't open file\n");
      exit(1);
    }
  } else {
    yyin = stdin;
  }

  //  yydebug = 1;

  yyparse();

  program->printList(stdout, "\n");

  fclose (yyin);

  Symboltable::popScope();

  return 0;
}
