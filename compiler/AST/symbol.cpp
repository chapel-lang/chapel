#include <typeinfo>
#include "analysis.h"
#include "files.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"


Symbol::Symbol(astType_t astType, char* init_name, Type* init_type) :
  BaseAST(astType),
  name(init_name),
  cname(name),
  type(init_type),
  scope(Symboltable::getCurrentScope()),
  asymbol(0)
{}


Symbol* Symbol::copy(void) {
  INT_FATAL(this, "Symbol::copy() not anticipated to be needed");
  return nilSymbol;
}


Symbol* Symbol::copyList(void) {
  Symbol* newSymbolList = nilSymbol;
  Symbol* oldSymbol = this;

  while (oldSymbol) {
    newSymbolList = appendLink(newSymbolList, oldSymbol->copy());

    oldSymbol = nextLink(Symbol, oldSymbol);
  }
  
  return newSymbolList;
}


bool Symbol::isNull(void) {
  return (this == nilSymbol);
}


void Symbol::traverseSymbol(Traversal* traversal) {
}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}

void Symbol::codegen(FILE* outfile) {
  if (!isNull()) {
    fprintf(outfile, "%s", cname);
  }
}


void Symbol::printDef(FILE* outfile) {
  print(outfile);
}


void Symbol::printDefList(FILE* outfile, char* separator) {
  Symbol* ptr;

  printDef(outfile);
  ptr = nextLink(Symbol, this);
  while (ptr) {
    fprintf(outfile, "%s", separator);
    ptr->printDef(outfile);
    ptr = nextLink(Symbol, ptr);
  }
}


void Symbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
  type->codegenSafeInit(outfile);
}  


void Symbol::codegenDefList(FILE* outfile, char* separator) {
  Symbol* ptr;

  codegenDef(outfile);
  ptr = nextLink(Symbol, this);
  while (ptr) {
    fprintf(outfile, "%s", separator);
    ptr->codegenDef(outfile);
    ptr = nextLink(Symbol, ptr);
  }
}


UnresolvedSymbol::UnresolvedSymbol(char* init_name) :
  Symbol(SYMBOL_UNRESOLVED, init_name)
{}


void UnresolvedSymbol::codegen(FILE* outfile) {
  //  INT_FATAL(this, "ERROR:  Cannot codegen an unresolved symbol.");
  Symbol::codegen(outfile);
}


Symbol* UnresolvedSymbol::copy(void) {
  return new UnresolvedSymbol(copystring(name));
}


VarSymbol::VarSymbol(char* init_name, Type* init_type, varType init_varClass, 
		     bool init_isConst) :
  Symbol(SYMBOL_VAR, init_name, init_type),
  varClass(init_varClass),
  isConst(init_isConst)
{}


Symbol* VarSymbol::copy(void) {
  INT_FATAL(this, "VarSymbol::copy() not implemented yet");
  return nilSymbol;
}


bool VarSymbol::isNull(void) {
  return (this == nilVarSymbol);
}


void VarSymbol::printDef(FILE* outfile) {
  print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


static char* paramTypeNames[NUM_PARAM_TYPES] = {
  "",
  "in",
  "inout",
  "out",
  "const"
};


ParamSymbol::ParamSymbol(paramType init_intent, char* init_name, 
			 Type* init_type) :
  Symbol(SYMBOL_PARAM, init_name, init_type),
  intent(init_intent)
{}


Symbol* ParamSymbol::copy(void) {
  return new ParamSymbol(intent, copystring(name), type->copy());
}


void ParamSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", paramTypeNames[intent]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


void ParamSymbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
}


TypeSymbol::TypeSymbol(char* init_name, Type* init_definition) :
  Symbol(SYMBOL_TYPE, init_name, init_definition)
{}


Symbol* TypeSymbol::copy(void) {
  INT_FATAL(this, "TypeSymbol::copy() not implemented yet");
  return nilSymbol;
}


FnSymbol::FnSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
		   Stmt* init_body, bool init_exportMe, 
		   FnSymbol* init_parentFn) :
  Symbol(SYMBOL_FN, init_name, init_retType),
  exportMe(init_exportMe),
  formals(init_formals),
  _this(0),
  body(init_body),
  parentFn(init_parentFn)
{}

 
FnSymbol::FnSymbol(char* init_name, FnSymbol* init_parentFn) :
  Symbol(SYMBOL_FN, init_name, nilType),
  formals(nilSymbol),
  _this(0),
  body(nilStmt),
  parentFn(init_parentFn)
{}


void FnSymbol::finishDef(Symbol* init_formals, Type* init_retType,
			 Stmt* init_body, bool init_exportMe) {
  formals = init_formals;
  type = init_retType;
  body = init_body;
  exportMe = init_exportMe;

  if (strcmp(name, "main") == 0 && 
      (scope->type == SCOPE_MODULE || scope->type == SCOPE_POSTPARSE) && 
      formals->isNull()) {
    if (mainFn->isNull()) {
      mainFn = this;
      exportMe = true;
      cname = copystring("_chpl_main");
    } else {
      USR_FATAL(this, "main multiply defined -- first occurrence at %s",
		mainFn->stringLoc());
    }
  }
}


Symbol* FnSymbol::copy(void) {
  INT_FATAL(this, "ReduceSymbol::copy() not implemented yet");
  return nilSymbol;
}


bool FnSymbol::isNull(void) {
  return (this == nilFnSymbol);
}


void FnSymbol::codegenDef(FILE* outfile) {
  if (!exportMe) {
    fprintf(outfile, "static ");
  }
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
  fprintf(outfile, "(");
  if (formals->isNull()) {
    fprintf(outfile, "void");
  } else {
    formals->codegenDefList(outfile, ", ");
  }
  fprintf(outfile, ")");
}

FnSymbol* FnSymbol::mainFn;

void FnSymbol::init(void) {
  mainFn = nilFnSymbol;
}



EnumSymbol::EnumSymbol(char* init_name, Expr* init_init, int init_val) :
  Symbol(SYMBOL_ENUM, init_name),
  init(init_init),
  val(init_val)
{}


Symbol* EnumSymbol::copy(void) {
  return new EnumSymbol(copystring(name), init->copy(), val);
}


void EnumSymbol::set_values(void) {
  EnumSymbol* tmp = this;
  int tally = 0;

  while (tmp) {
    if (tmp->init) {
      if (tmp->init->isComputable() == false) {
	USR_FATAL(tmp->init, "Enumerator value for %s must be integer parameter", tmp->name);
      }
      tally = tmp->init->intVal();
    }
    tmp->val = tally++;
    Symboltable::define(tmp);
    tmp = nextLink(EnumSymbol, tmp);
  }
}


ModuleSymbol::ModuleSymbol(char* init_name, bool init_internal) :
  Symbol(SYMBOL_MODULE, init_name),
  internal(init_internal),
  stmts(nilStmt),
  initFn(nilFnSymbol)
{}


void ModuleSymbol::codegenDef(void) {
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  openCFiles(name, &outfileinfo, &extheadfileinfo, &intheadfileinfo);

  fprintf(codefile, "#include \"stdchpl.h\"\n");
  fprintf(codefile, "#include \"%s\"\n", extheadfileinfo.filename);
  fprintf(codefile, "#include \"%s\"\n", intheadfileinfo.filename);
  fprintf(codefile, "\n");

  stmts->codegenList(codefile, "\n");

  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
}


static bool stmtIsGlob(ILink* link) {
  Stmt* stmt = dynamic_cast<Stmt*>(link);

  if (stmt == NULL) {
    INT_FATAL("Non-Stmt found in StmtIsGlob");
  }
  return stmt->canLiveAtFileScope();
}


void ModuleSymbol::createInitFn(void) {
  char* fnName = glomstrings(2, "__init_", name);
  ILink* globstmts;
  ILink* initstmts;
  Stmt* definition = stmts;

  definition->filter(stmtIsGlob, &globstmts, &initstmts);

  Stmt* initFunStmts = dynamic_cast<Stmt*>(initstmts);
  definition = dynamic_cast<Stmt*>(globstmts);
  Stmt* initFunBody = new BlockStmt(initFunStmts ? initFunStmts 
                                                 : nilStmt);
  FnDefStmt* initFunDef = Symboltable::defineFunction(fnName, nilSymbol, 
						      dtVoid, initFunBody, 
						      true);
  initFn = initFunDef->fn;

  {
    Stmt* initstmt = initFunStmts;
    while (initstmt) {
      initstmt->parentSymbol = initFn;
      initstmt = nextLink(Stmt, initstmt);
    }
    initFunBody->parentSymbol = initFn;
  }

  definition = appendLink(definition, initFunDef);

  stmts = definition;

  // if main() is defined in this module, prefix its body with call
  // to the module's init function
  FnSymbol* mainFn = FnSymbol::mainFn;
  if (!mainFn->isNull()) {
    if (mainFn->scope->symContext == this) {
      ExprStmt* initStmt = ExprStmt::createFnCallStmt(initFn);
      initStmt->append(mainFn->body);
      mainFn->body = new BlockStmt(initStmt);
    }
  }
}


bool ModuleSymbol::isFileModule(void) {
  return (lineno == 0);
}
