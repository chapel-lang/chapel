#include <stdlib.h>
#include "createAST.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
// #include "getstuff.h"
// #include "findUnknownTypes.h"
#include "link.h"
#include "misc.h"
#include "nils.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
//#include "verifyASTType.h"
#include "yy.h"


Stmt* yystmtlist = NULL;
Stmt* internalPreludeStmts = NULL;
Stmt* preludeStmts = NULL;
Stmt* programStmts = NULL;

char* yyfilename;
int yylineno;


static Stmt* ParseFile(char* filename, bool prelude = false) {
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

  return stmtList;
}


static bool stmtIsGlob(ILink* link) {
  Stmt* stmt = dynamic_cast<Stmt*>(link);

  if (stmt == NULL) {
    INT_FATAL("Non-Stmt found in StmtIsGlob");
  }
  if (stmt->isNull() ||
      (dynamic_cast<ExprStmt*>(stmt) != NULL) ||
      (dynamic_cast<BlockStmt*>(stmt) != NULL) ||
      (dynamic_cast<CondStmt*>(stmt) != NULL) ||
      (dynamic_cast<VarDefStmt*>(stmt) != NULL) ||
      (dynamic_cast<NoOpStmt*>(stmt) != NULL)) {
    return false;
  } else {
    return true;
  }
}


static Stmt* createInitFn(Stmt* program, char* fnName = "__init") {
  ILink* globstmts;
  ILink* initstmts;

  program->filter(stmtIsGlob, &globstmts, &initstmts);

  Stmt* initFunStmts = dynamic_cast<Stmt*>(initstmts);
  program = dynamic_cast<Stmt*>(globstmts);
  Stmt* initFunBody = new BlockStmt(initFunStmts ? initFunStmts 
                                                 : nilStmt);
  FnDefStmt* initFunDef = Symboltable::defineFunction(fnName, nilSymbol, 
						      dtVoid, initFunBody, 
						      true);


  program = appendLink(program, initFunDef);

  //  program->traverseList(new FindUnknownTypes());
  //  program->traverse(new PrintStmts());
  //  program->traverse(new VerifyASTType());
  /*
  Vec<Stmt*> stmts;
  getLinkElements(stmts, program);
  for (int i=0; i<stmts.n; i++) {
    stmts.e[i]->print(stdout);
  }
  */

  return program;
}


Stmt* fileToAST(char* filename, int debug) {
  static char* preludePath = NULL;

  if (preludePath == NULL) {
    initNils();
    Symboltable::init();
    initType(); // BLC : clean these up
    initExpr();

    Symboltable::parseInternalPrelude();
    preludePath = glomstrings(2, system_dir, "/AST/internal_prelude.chpl");
    internalPreludeStmts = ParseFile(preludePath, true);

    Symboltable::parsePrelude();
    preludePath = glomstrings(2, system_dir, "/AST/prelude.chpl");
    preludeStmts = ParseFile(preludePath, true);

    Symboltable::doneParsingPreludes();
  }

  yydebug = debug;
  programStmts = ParseFile(filename);

  //  extern void testGetStuff(Stmt*);
  //  testGetStuff(program);

  internalPreludeStmts = createInitFn(internalPreludeStmts, "__initIntPrelude");
  preludeStmts = createInitFn(preludeStmts, "__initPrelude");
  programStmts = createInitFn(programStmts);

  /*
  fprintf(stderr, "-------------------------------------\n");
  internalPreludeStmts->printList(stderr, "\n");
  fprintf(stderr, "-------------------------------------\n");
  preludeStmts->printList(stderr, "\n");
  fprintf(stderr, "-------------------------------------\n");
  */
  

  //  Symboltable::dump(stdout);

  return programStmts;
}
