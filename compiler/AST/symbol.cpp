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
  defPoint(nilStmt),
  parentScope(NULL),
  asymbol(0)
{}


void Symbol::setParentScope(SymScope* init_parentScope) {
  parentScope = init_parentScope;
}


bool Symbol::isNull(void) {
  return (this == nilSymbol);
}


Symbol* Symbol::copyList(bool clone, CloneCallback* analysis_clone) {
  Symbol* newSymbolList = nilSymbol;
  Symbol* oldSymbol = this;

  while (oldSymbol && !oldSymbol->isNull()) {
    newSymbolList = appendLink(newSymbolList, oldSymbol->copy(clone, analysis_clone));

    oldSymbol = nextLink(Symbol, oldSymbol);
  }
  
  return newSymbolList;
}


Symbol* Symbol::copy(bool clone, CloneCallback* analysis_clone) {
  if (isNull()) {
    return this;
  }

  Symbol* new_symbol = copySymbol(clone, analysis_clone);

  new_symbol->lineno = lineno;
  new_symbol->filename = filename;

  if (analysis_clone) {
    analysis_clone->clone(this, new_symbol);
  }
  return new_symbol;
}


Symbol* Symbol::copySymbol(bool clone, CloneCallback* analysis_clone) {
  INT_FATAL(this, "Symbol::copySymbol() not anticipated to be needed");
  return nilSymbol;
}


void Symbol::traverse(Traversal* traversal, bool atTop) {
  if (isNull()) {
    return;
  }
  if (traversal->processTop || !atTop) {
    traversal->preProcessSymbol(this);
  }
  if (atTop || traversal->exploreChildSymbols) {
    traverseSymbol(traversal);
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessSymbol(this);
  }
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

void Symbol::setDefPoint(Stmt* stmt) {
  Symbol* tmp = this;
  while (tmp) {
    tmp->defPoint = stmt;
    tmp = nextLink(Symbol, tmp);
  }
}


UnresolvedSymbol::UnresolvedSymbol(char* init_name, char* init_cname) :
  Symbol(SYMBOL_UNRESOLVED, init_name)
{
  if (init_cname) {
    cname = init_cname;
  }
}


void UnresolvedSymbol::codegen(FILE* outfile) {
  INT_FATAL(this, "ERROR:  Cannot codegen an unresolved symbol.");
}


Symbol* UnresolvedSymbol::copySymbol(bool clone, CloneCallback* analysis_clone) {
  return new UnresolvedSymbol(copystring(name));
}


VarSymbol::VarSymbol(char* init_name,
		     Type* init_type,
		     Expr* init_expr,
		     varType init_varClass, 
		     bool init_isConst) :
  Symbol(SYMBOL_VAR, init_name, init_type),
  varClass(init_varClass),
  isConst(init_isConst),
  init(init_expr)
{
  Symboltable::define(this);
  SET_BACK(init);
}


Symbol* VarSymbol::copySymbol(bool clone, CloneCallback* analysis_clone) {
  return new VarSymbol(copystring(name), type, init, varClass, isConst);
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
  "const",
  "ref"
};


ParamSymbol::ParamSymbol(paramType init_intent, char* init_name, 
			 Type* init_type) :
  Symbol(SYMBOL_PARAM, init_name, init_type),
  intent(init_intent)
{
  Symboltable::define(this);
}


Symbol* ParamSymbol::copySymbol(bool clone, CloneCallback* analysis_clone) {
  return new ParamSymbol(intent, copystring(name), type);
}


void ParamSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", paramTypeNames[intent]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


bool ParamSymbol::requiresCPtr(void) {
  return ((intent == PARAM_OUT || intent == PARAM_INOUT || 
	   intent == PARAM_REF) && 
	  type->outParamNeedsPtr());
}


bool ParamSymbol::requiresCopyBack(void) {
  return ((intent == PARAM_OUT || intent == PARAM_INOUT) &&
	  type->outParamNeedsPtr());
}


bool ParamSymbol::requiresCTmp(void) {
  return type->requiresCParamTmp(intent);
}


void ParamSymbol::codegen(FILE* outfile) {
  bool requiresDeref = requiresCPtr();
 
  if (requiresDeref) {
    fprintf(outfile, "(*");
  }
  Symbol::codegen(outfile);
  if (requiresDeref) {
    fprintf(outfile, ")");
  }
}


void ParamSymbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  if (requiresCPtr()) {
    fprintf(outfile, "*");
  }
  fprintf(outfile, " ");
  Symbol::codegen(outfile);
}


TypeSymbol::TypeSymbol(char* init_name, Type* init_definition) :
  Symbol(SYMBOL_TYPE, init_name, init_definition)
{
  Symboltable::define(this);
}


Symbol* TypeSymbol::copySymbol(bool clone, CloneCallback* analysis_clone) {
  return new TypeSymbol(copystring(name), type);
}


FnSymbol::FnSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
		   Stmt* init_body, bool init_exportMe) :
  Symbol(SYMBOL_FN, init_name, init_retType),
  exportMe(init_exportMe),
  formals(init_formals),
  retType(init_retType),
  _this(0),
  body(init_body),
  overload(nilFnSymbol)
{
  Symboltable::define(this);
}

 
FnSymbol::FnSymbol(char* init_name) :
  Symbol(SYMBOL_FN, init_name, nilType),
  formals(nilSymbol),
  retType(nilType),
  _this(0),
  body(nilStmt)
{
  Symboltable::define(this);
}


void FnSymbol::finishDef(Symbol* init_formals, Type* init_retType,
			 Stmt* init_body, SymScope* init_paramScope,
			 bool init_exportMe) {
  formals = init_formals;
  type = init_retType;
  retType = init_retType;
  body = init_body;
  SET_BACK(body);
  exportMe = init_exportMe;
  paramScope = init_paramScope;

  if (strcmp(name, "main") == 0 && 
      (parentScope->type == SCOPE_MODULE || 
       parentScope->type == SCOPE_POSTPARSE) && 
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


Symbol* FnSymbol::copySymbol(bool clone, CloneCallback* analysis_clone) {
  return new FnSymbol(name);
}


bool FnSymbol::isNull(void) {
  return (this == nilFnSymbol);
}


void FnSymbol::codegenDef(FILE* outfile) {
  if (!exportMe) {
    fprintf(outfile, "static ");
  }
  retType->codegen(outfile);
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
{
  SET_BACK(init);
  Symboltable::define(this);
}


Symbol* EnumSymbol::copySymbol(bool clone, CloneCallback* analysis_clone) {
  return new EnumSymbol(copystring(name), init->copy(clone, analysis_clone), val);
}


void EnumSymbol::set_values(void) {
  EnumSymbol* tmp = this;
  int tally = 0;

  while (tmp) {
    if (!tmp->init->isNull()) {
      if (tmp->init->isComputable() == false) {
	USR_FATAL(tmp->init, "Enumerator value for %s must be integer parameter", tmp->name);
      }
      tally = tmp->init->intVal();
    }
    tmp->val = tally++;
    tmp = nextLink(EnumSymbol, tmp);
  }
}


ModuleSymbol::ModuleSymbol(char* init_name, bool init_internal) :
  Symbol(SYMBOL_MODULE, init_name),
  internal(init_internal),
  stmts(nilStmt),
  initFn(nilFnSymbol),
  modScope(NULL)
{}


void ModuleSymbol::setModScope(SymScope* init_modScope) {
  modScope = init_modScope;
}


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


void ModuleSymbol::startTraversal(Traversal* traversal) {
  SymScope* prevScope;

  if (modScope) {
    prevScope = Symboltable::setCurrentScope(modScope);
  }
  TRAVERSE_LS(stmts, traversal, true);
  if (modScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


static bool stmtIsGlob(ILink* link) {
  Stmt* stmt = dynamic_cast<Stmt*>(link);

  if (stmt == NULL) {
    INT_FATAL("Non-Stmt found in StmtIsGlob");
  }
  return dynamic_cast<TypeDefStmt*>(stmt) || dynamic_cast<FnDefStmt*>(stmt);
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

  if (definition) {
    definition->append(initFunDef);
  }
  else {
    definition = initFunDef;
  }

  stmts = definition;
  SET_BACK(stmts); // SJD: Eliminate please.
}


bool ModuleSymbol::isFileModule(void) {
  return (lineno == 0);
}
