#include <stdlib.h>
#include "expr.h"
#include "link.h"
#include "stmt.h"
#include "yy.h"


Stmt* program;
Symbol* yypst;


/*
int main() {
  Expr* ten = new Literal(10);
  Expr* one = new Literal(1);
  Expr* root = new BinOp(ten, one);

  Stmt* stmt = new ExprStmt(root);

  Stmt* loopstmt = new WhileLoopStmt(true, root, stmt);

  loopstmt->print(stdout);
}
*/


int main(int argc, char* argv[]) {

  setupTypes();

  if (argc > 1) {
    if ((yyin = fopen(argv[1], "r")) == NULL) {
      printf("Can't open file\n");
      exit(1);
    }
  } else {
    yyin = stdin;
  }

  //  yydebug = 1;

  while (!feof(yyin)) {
    yyparse();
    program->printList(stdout, "");
  }

  fclose (yyin);
}
