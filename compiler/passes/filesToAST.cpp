#include "driver.h"
#include "filesToAST.h"
#include "module.h"
#include "parser.h"
#include "nils.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"


Stmt* internalPreludeStmts = NULL;
Stmt* preludeStmts = NULL;
Stmt* entryPoint = NULL;


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


void FilesToAST::run(Module* moduleList) {
  Symboltable::parseInternalPrelude();
  char* preludePath = glomstrings(2, system_dir, 
				  "/parser/internal_prelude.chpl");
  internalPreludeStmts = ParseFile(preludePath, true);
  internalPreludeStmts = createInitFn(internalPreludeStmts, "__initIntPrelude");
  findInternalTypes();

  Symboltable::parsePrelude();
  preludePath = glomstrings(2, system_dir, "/parser/prelude.chpl");
  preludeStmts = ParseFile(preludePath, true);
  preludeStmts = createInitFn(preludeStmts, "__initPrelude");
  
  Symboltable::doneParsingPreludes();


  yydebug = debugParserLevel;
  Module* mod = moduleList;
  while (mod) {
    mod->stmts = ParseFile(mod->filename);
    mod->stmts = createInitFn(mod->stmts, "__init");

    mod = nextLink(Module, mod);
  }

  Symboltable::doneParsingUserFiles();

  entryPoint = createInitFn(entryPoint);
}
