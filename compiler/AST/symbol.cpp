#include <typeinfo>
#include "analysis.h"
#include "files.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "../symtab/symlink.h"
#include "sym.h"
#include "fun.h"
#include "pattern.h"


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


Symbol* Symbol::copyList(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Symbol* newSymbolList = nilSymbol;
  Symbol* oldSymbol = this;

  while (oldSymbol && !oldSymbol->isNull()) {
    newSymbolList = appendLink(newSymbolList, oldSymbol->copy(clone, map, analysis_clone));

    oldSymbol = nextLink(Symbol, oldSymbol);
  }
  
  return newSymbolList;
}


Symbol* Symbol::copy(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  if (isNull()) {
    return this;
  }

  Symbol* new_symbol = copySymbol(clone, map, analysis_clone);

  new_symbol->lineno = lineno;
  new_symbol->filename = filename;

  if (analysis_clone) {
    analysis_clone->clone(this, new_symbol);
  }
  if (map) {
    map->put(this, new_symbol);
  }
  return new_symbol;
}


Symbol* Symbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
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
    if (atTop) {
      traverseDefSymbol(traversal);
    }
    else {
      traverseSymbol(traversal);
    }
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessSymbol(this);
  }
}


void Symbol::traverseDef(Traversal* traversal, bool atTop) {
  if (isNull()) {
    return;
  }
  if (traversal->processTop || !atTop) {
    traversal->preProcessSymbol(this);
  }
  if (atTop || traversal->exploreChildTypes) {
    traverseDefSymbol(traversal);
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessSymbol(this);
  }
}


void Symbol::traverseSymbol(Traversal* traversal) {
}


void Symbol::traverseDefSymbol(Traversal* traversal) {
}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}

void Symbol::codegen(FILE* outfile) {
  if (!isNull()) {
    fprintf(outfile, "%s", cname);
  }
}


void Symbol::codegenDef(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to Symbol::codegenDef");
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


Symbol* UnresolvedSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new UnresolvedSymbol(copystring(name));
}


void UnresolvedSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(this, traversal, false);
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


Symbol* VarSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new VarSymbol(copystring(name), type, init, varClass, isConst);
}


void VarSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(this, traversal, false);
  TRAVERSE(type, traversal, false);
  TRAVERSE(init, traversal, false);
}


bool VarSymbol::isNull(void) {
  return (this == nilVarSymbol);
}


void VarSymbol::printDef(FILE* outfile) {
  print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


void VarSymbol::codegenDef(FILE* outfile) {
  if (parentScope->type == SCOPE_MODULE) {
    outfile = intheadfile;
    fprintf(outfile, "static ");
  }
  if (isConst) {
    fprintf(outfile, "const ");
  }
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
  type->codegenSafeInit(outfile);
  fprintf(outfile, ";\n");
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
			 Type* init_type, Expr* init_init) :
  Symbol(SYMBOL_PARAM, init_name, init_type),
  intent(init_intent),
  init(init_init)
{
  Symboltable::define(this);
}


Symbol* ParamSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new ParamSymbol(intent, copystring(name), type);
}


void ParamSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(this, traversal, false);
  TRAVERSE(type, traversal, false);
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


Symbol* TypeSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new TypeSymbol(copystring(name), type);
}


void TypeSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE_DEF(type, traversal, false);
}


void TypeSymbol::codegenDef(FILE* outfile) {
  FILE* deffile = outfile;
  /* if in file scope, hoist to internal header so that it will be
     defined before global variables at file scope. */  
  if (type->name->parentScope->type == SCOPE_MODULE) { 
    deffile = intheadfile;
  }
  type->codegenDef(deffile);

  type->codegenStringToType(outfile);
  type->codegenIORoutines(outfile);
  type->codegenConfigVarRoutines(outfile);
  type->codegenConstructors(outfile);
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


Symbol* FnSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
//   if (clone) {
//     FnSymbol* fn_copy = new FnSymbol(name);
//     Symboltable::pushScope(SCOPE_PARAM);
//     Symbol* newformals = formals->copyList(clone, map, analysis_clone);
//     SymScope* paramScope = Symboltable::popScope();
//     fn_copy->finishDef(newformals, type, body, paramScope, exportMe);
//     return fn_copy;
//   }
//   else {
//     return new FnSymbol(name);
//   }
  return new FnSymbol(name);
}


void FnSymbol::traverseDefSymbol(Traversal* traversal) {
  SymScope* saveScope;

  TRAVERSE(this, traversal, false);
  if (paramScope) {
    saveScope = Symboltable::setCurrentScope(paramScope);
  }
  TRAVERSE_LS(formals, traversal, false);
  TRAVERSE(type, traversal, false);
  TRAVERSE(body, traversal, false);
  TRAVERSE(retType, traversal, false);
  if (paramScope) {
    Symboltable::setCurrentScope(saveScope);
  }
}


// FnSymbol* FnSymbol::clone(CloneCallback* clone_callback, Map<BaseAST*,BaseAST*>* map) {
//   static int uid = 1; // Unique ID for cloned functions
//   FnSymbol* this_copy;
//   SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
//   if (!(this_copy = dynamic_cast<FnSymbol*>(copy(true, map, clone_callback)))) {
//     INT_FATAL(this, "Major error in FnSymbol::clone");
//   }
//   this_copy->cname = 
//     glomstrings(3, this_copy->cname, "_clone_", intstring(uid++));
//   Symboltable::setCurrentScope(save_scope);
//   return this_copy;
// }


FnSymbol* FnSymbol::clone(CloneCallback* clone_callback, Map<BaseAST*,BaseAST*>* map) {
  map->clear();
  FnDefStmt* this_copy = NULL;
  static int uid = 1; // Unique ID for cloned functions
  SymScope* save_scope;
  save_scope = Symboltable::setCurrentScope(parentScope);
  Stmt* stmt_copy = defPoint->copy(true, map, clone_callback);
  if (this_copy = dynamic_cast<FnDefStmt*>(stmt_copy)) {
    this_copy->fn->cname =
      glomstrings(3, this_copy->fn->cname, "_clone_", intstring(uid++));
    defPoint->insertBefore(this_copy);
  }
  else {
    INT_FATAL(this, "Unreachable statement in FnDefStmt::clone reached");
  }
  Symboltable::setCurrentScope(save_scope);
  return this_copy->fn;
}


FnSymbol* FnSymbol::coercion_wrapper(Map<MPosition *, Symbol *> *coercion_substitutions) {
  FnDefStmt* wrapper_stmt = NULL;
  static int uid = 1; // Unique ID for wrapped functions
  FnSymbol* wrapper_symbol;
  SymScope* save_scope;

  save_scope = Symboltable::setCurrentScope(parentScope);
  wrapper_symbol = new FnSymbol(name);
  wrapper_symbol->cname =
    glomstrings(3, cname, "_coercion_wrapper_", intstring(uid++));
  wrapper_symbol = Symboltable::startFnDef(wrapper_symbol);
  Symbol* wrapper_formals = formals->copyList();
  Symbol* actuals = wrapper_formals;
  Variable* argList = new Variable(actuals);
  actuals = nextLink(Symbol, actuals);
  while (actuals) {
    argList->append(new Variable(actuals));
    actuals = nextLink(Symbol, actuals);
  }
  Symboltable::pushScope(SCOPE_LOCAL);
  Stmt* wrapper_body = new ExprStmt(new FnCall(new Variable(this), argList));
  for (int i = 0; i < coercion_substitutions->n; i++) {
    int j = 1;
    MPosition p;
    Symbol* formal_change = wrapper_formals;
    Variable* actual_change = argList;
    forv_MPosition(p, asymbol->sym->fun->numeric_arg_positions) {
      if (coercion_substitutions->e[i].key ==
	  asymbol->sym->fun->numeric_arg_positions.e[j]) {
	char* temp_name =
	  glomstrings(2, "_coercion_temp_", formal_change->name);
	VarSymbol* temp_symbol = new VarSymbol(temp_name, formal_change->type,
					       new Variable(formal_change));
	Stmt* temp_def_stmt = new VarDefStmt(temp_symbol);
	temp_symbol->setDefPoint(temp_def_stmt);
	temp_def_stmt->append(wrapper_body);
	wrapper_body = temp_def_stmt;
	formal_change->type = coercion_substitutions->e[i].value->type;
	actual_change->var = temp_symbol;
      }
      if (!formal_change->next->isNull()) {
	formal_change = nextLink(Symbol, formal_change);
      }
      if (!actual_change->next->isNull()) {
	actual_change = nextLink(Variable, actual_change);
      }
      j++;
    }
  }
  SymScope* block_scope = Symboltable::popScope();
  BlockStmt* wrapper_block = new BlockStmt(wrapper_body);
  wrapper_block->setBlkScope(block_scope);
  Type* wrapper_return_type = retType;
  wrapper_stmt = Symboltable::finishFnDef(wrapper_symbol, wrapper_formals,
					  wrapper_return_type, wrapper_block);
  defPoint->insertBefore(wrapper_stmt);
  Symboltable::setCurrentScope(save_scope);
  return wrapper_stmt->fn;
}


bool FnSymbol::isNull(void) {
  return (this == nilFnSymbol);
}


void FnSymbol::codegenHeader(FILE* outfile) {
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


void FnSymbol::codegenDef(FILE* outfile) {
  FILE* headfile;

  if (!function_is_used(this)) {
    return;
  }
  if (exportMe) {
    headfile = extheadfile;
  } else {
    headfile = intheadfile;
  }
  codegenHeader(headfile);
  fprintf(headfile, ";\n");

  codegenHeader(outfile);
  fprintf(outfile, " ");
  body->codegen(outfile);
  fprintf(outfile, "\n\n");
  if (overload) {
    overload->codegenDef(outfile);
  }
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


Symbol* EnumSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new EnumSymbol(copystring(name), init->copy(clone, map, analysis_clone), val);
}


void EnumSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(this, traversal, false);
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


void EnumSymbol::codegenDef(FILE* outfile) {
  /* Do nothing */
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
  fprintf(codefile, "#include \"_chpl_common.h\"\n");
  fprintf(codefile, "#include \"%s\"\n", extheadfileinfo.filename);
  fprintf(codefile, "#include \"%s\"\n", intheadfileinfo.filename);
  fprintf(codefile, "\n");

  modScope->codegen(codefile, "\n");
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


/** SJD: Makes sense for this to take place of above startTraversal **/
void ModuleSymbol::traverseDefSymbol(Traversal* traversal) {
  startTraversal(traversal);
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
  //SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  FnDefStmt* initFunDef = Symboltable::defineFunction(fnName, nilSymbol, 
						      dtVoid, initFunBody, 
						      true);
  //Symboltable::setCurrentScope(saveScope);
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
