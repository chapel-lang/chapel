#include "driver.h"
#include "files.h"
#include "filesToAST.h"
#include "parser.h"
#include "nils.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"


ModuleSymbol* internalPrelude = NULL;
ModuleSymbol* prelude = NULL;
ModuleSymbol* entry = NULL;
Stmt* entryPoint = NULL;


static bool stmtIsGlob(ILink* link) {
  Stmt* stmt = dynamic_cast<Stmt*>(link);

  if (stmt == NULL) {
    INT_FATAL("Non-Stmt found in StmtIsGlob");
  }
  return stmt->canLiveAtFileScope();
}


static void createInitFn(ModuleSymbol* module, char* fnName) {
  ILink* globstmts;
  ILink* initstmts;
  Stmt* stmts = module->stmts;

  stmts->filter(stmtIsGlob, &globstmts, &initstmts);

  Stmt* initFunStmts = dynamic_cast<Stmt*>(initstmts);
  stmts = dynamic_cast<Stmt*>(globstmts);
  Stmt* initFunBody = new BlockStmt(initFunStmts ? initFunStmts 
                                                 : nilStmt);
  FnDefStmt* initFunDef = Symboltable::defineFunction(fnName, nilSymbol, 
						      dtVoid, initFunBody, 
						      true);
  module->initFn = initFunDef->fn;

  {
    Stmt* initstmt = initFunStmts;
    while (initstmt) {
      initstmt->parentFn = module->initFn;
      initstmt = nextLink(Stmt, initstmt);
    }
    initFunBody->parentFn = module->initFn;
  }

  stmts = appendLink(stmts, initFunDef);

  if (strcmp(fnName, "__entryPoint") != 0) {
    FnCall* initFunCall = new FnCall(new Variable(module->initFn));
    ExprStmt* initFunCallStmt = new ExprStmt(initFunCall);
    entryPoint = appendLink(entryPoint, initFunCallStmt);
  }

  module->stmts = stmts;
}


void FilesToAST::run(ModuleSymbol* moduleList) {
  Symboltable::parseInternalPrelude();
  char* preludePath = glomstrings(2, system_dir, 
				  "/parser/internal_prelude.chpl");
  internalPrelude = ParseFile(preludePath, true);
  createInitFn(internalPrelude, "__initIntPrelude");
  findInternalTypes();

  Symboltable::parsePrelude();
  preludePath = glomstrings(2, system_dir, "/parser/prelude.chpl");
  prelude = ParseFile(preludePath, true);
  createInitFn(prelude, "__initPrelude");
				
  
  Symboltable::doneParsingPreludes();


  yydebug = debugParserLevel;

  int filenum = 0;
  char* inputFilename = NULL;
  do {
    inputFilename = nthFilename(filenum);
    if (inputFilename) {
      ModuleSymbol* mod = ParseFile(inputFilename);
      createInitFn(mod, glomstrings(2, "__init_", mod->name));
				

      FnSymbol* mainFn = FnSymbol::mainFn;
      if (!mainFn->isNull()) {
	if (mainFn->scope->symContext == mod) {
	  FnCall* initCall = new FnCall(new Variable(mod->initFn));
	  ExprStmt* initStmt = new ExprStmt(initCall);
	  initStmt->append(mainFn->body);
	  mainFn->body = new BlockStmt(initStmt);
	}
      }
    }
    filenum++;
  } while (inputFilename);

  Symboltable::doneParsingUserFiles();

  entry = new ModuleSymbol("entryPoint");
  entry->stmts = entryPoint;
  createInitFn(entry, "__entryPoint");
  entryPoint = entry->stmts;
}
