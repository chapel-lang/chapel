#include <stdlib.h>
#include "createAST.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "link.h"
#include "misc.h"
#include "nils.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"


Stmt* yystmtlist = NULL;
Stmt* internalPreludeStmts = NULL;
Stmt* preludeStmts = NULL;
Stmt* programStmts = NULL;
Stmt* entryPoint = NULL;

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
  return stmt->canLiveAtFileScope();
}


static Stmt* createInitFn(Stmt* program, char* fnNameArg = 0) {
  ILink* globstmts;
  ILink* initstmts;
  char *fnName = fnNameArg ? fnNameArg : (char*)"__entryPoint";
  

  program->filter(stmtIsGlob, &globstmts, &initstmts);

  Stmt* initFunStmts = dynamic_cast<Stmt*>(initstmts);
  program = dynamic_cast<Stmt*>(globstmts);
  Stmt* initFunBody = new BlockStmt(initFunStmts ? initFunStmts 
                                                 : nilStmt);
  FnDefStmt* initFunDef = Symboltable::defineFunction(fnName, nilSymbol, 
						      dtVoid, initFunBody, 
						      true);
  {
    FnSymbol* initFunSym = initFunDef->fn;
    Stmt* initstmt = initFunStmts;
    while (initstmt) {
      initstmt->parentFn = initFunSym;
      initstmt = nextLink(Stmt, initstmt);
    }
    initFunBody->parentFn = initFunSym;
  }


  program = appendLink(program, initFunDef);

  if (fnNameArg) {
    FnSymbol* initFunSym = initFunDef->fn;
    FnCall* initFunCall = new FnCall(new Variable(initFunSym));
    ExprStmt* initFunCallStmt = new ExprStmt(initFunCall);
    entryPoint = appendLink(entryPoint, initFunCallStmt);
  }

  return program;
}


Stmt* fileToAST(char* filename, int debug) {
  static char* preludePath = NULL;

  if (preludePath == NULL) {
    initNils();
    Symboltable::init();
    initTypes(); // BLC : clean these up
    initExpr();

    Symboltable::parseInternalPrelude();
    preludePath = glomstrings(2, system_dir, "/parser/internal_prelude.chpl");
    internalPreludeStmts = ParseFile(preludePath, true);
    findInternalTypes();

    Symboltable::parsePrelude();
    preludePath = glomstrings(2, system_dir, "/parser/prelude.chpl");
    preludeStmts = ParseFile(preludePath, true);

    Symboltable::doneParsingPreludes();
  }

  yydebug = debug;
  programStmts = ParseFile(filename);

  internalPreludeStmts = createInitFn(internalPreludeStmts, "__initIntPrelude");
  preludeStmts = createInitFn(preludeStmts, "__initPrelude");
  programStmts = createInitFn(programStmts, "__init");
  entryPoint = createInitFn(entryPoint);

  return programStmts;
}
