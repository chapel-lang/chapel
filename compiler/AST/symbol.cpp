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
  defPoint(NULL),
  parentScope(NULL),
  asymbol(0)
{}


void Symbol::setParentScope(SymScope* init_parentScope) {
  parentScope = init_parentScope;
}


Symbol* Symbol::copyList(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Symbol* newSymbolList = NULL;
  Symbol* oldSymbol = this;

  while (oldSymbol) {
    newSymbolList = appendLink(newSymbolList, oldSymbol->copy(clone, map, analysis_clone));

    oldSymbol = nextLink(Symbol, oldSymbol);
  }
  
  return newSymbolList;
}


Symbol* Symbol::copy(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
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
  return NULL;
}


void Symbol::traverse(Traversal* traversal, bool atTop) {
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
  if (traversal->processTop || !atTop) {
    traversal->preProcessSymbol(this);
  }
  traverseDefSymbol(traversal);
  if (traversal->processTop || !atTop) {
    traversal->postProcessSymbol(this);
  }
}


void Symbol::traverseSymbol(Traversal* traversal) {
  
}


void Symbol::traverseDefSymbol(Traversal* traversal) {
}


bool Symbol::isConst(void) {
  return true;
}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}

void Symbol::codegen(FILE* outfile) {
  fprintf(outfile, "%s", cname);
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

void Symbol::setDefPoint(BaseAST* init_defPoint) {
  Symbol* tmp = this;
  while (tmp) {
    tmp->defPoint = init_defPoint;
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
		     bool init_isConstant) :
  Symbol(SYMBOL_VAR, init_name, init_type),
  varClass(init_varClass),
  isConstant(init_isConstant),
  init(init_expr)
{
  Symboltable::define(this);
  SET_BACK(init);
}


Symbol* VarSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new VarSymbol(copystring(name), type, init, varClass, isConstant);
}


void VarSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
  TRAVERSE(init, traversal, false);
}


void VarSymbol::printDef(FILE* outfile) {
  if (varClass == VAR_CONFIG) {
    fprintf(outfile, "config ");
  }
  if (varClass == VAR_STATE) {
    fprintf(outfile, "state ");
  }
  if (isConstant) {
    fprintf(outfile, "const ");
  }
  else {
    fprintf(outfile, "var ");
  }
  print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
  if (init) {
    fprintf(outfile, " = ");
    if (!init->next) {
      init->print(outfile);
    } else {
      fprintf(outfile, "(");
      init->printList(outfile);
      fprintf(outfile, ")");
    }
  }
}


bool VarSymbol::initializable(void) {
  switch (parentScope->type) {
  case SCOPE_FUNCTION:
  case SCOPE_LOCAL:
  case SCOPE_MODULE:
    return true;
  case SCOPE_INTRINSIC:
  case SCOPE_INTERNAL_PRELUDE:
  case SCOPE_PRELUDE:
  case SCOPE_POSTPARSE:
  case SCOPE_PARAM:
  case SCOPE_FORLOOP:
  case SCOPE_FORALLEXPR:
  case SCOPE_CLASS:
    return false;
  default:
    INT_FATAL(this, "unhandled case in needsTypeInitialization()");
  }
  return false;
}


bool VarSymbol::isConst(void) {
  return isConstant;
}


void VarSymbol::codegenDef(FILE* outfile) {
  if (parentScope->type == SCOPE_MODULE) {
    outfile = intheadfile;
    fprintf(outfile, "static ");
  }
  if (isConstant) {
    fprintf(outfile, "const ");
  }
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
  if (this->initializable()) {
    type->codegenSafeInit(outfile);
  }
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
  return new ParamSymbol(intent, copystring(name), type, init->copy(clone, map, analysis_clone));
}


void ParamSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(this, traversal, false);
  TRAVERSE(type, traversal, false);
  TRAVERSE(init, traversal, false);
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


bool ParamSymbol::isConst(void) {
  // TODO: need to also handle case of PARAM_BLANK for scalar types
  return (intent == PARAM_CONST); 
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
  Type* new_type = type->copy(clone, map, analysis_clone);
  TypeSymbol* new_type_symbol = new TypeSymbol(copystring(name), new_type);
  new_type->addSymbol(new_type_symbol);
  return new_type_symbol;
}


TypeSymbol* TypeSymbol::clone(CloneCallback* clone_callback, Map<BaseAST*,BaseAST*>* map) {
  static int uid = 1; // Unique ID for cloned classes

  ClassType* old_class_type = dynamic_cast<ClassType*>(type);

  if (!old_class_type) {
    INT_FATAL(this, "Attempt to clone non-class type");
  }

  TypeDefStmt* old_def_stmt = dynamic_cast<TypeDefStmt*>(defPoint);

  if (!old_def_stmt) {
    INT_FATAL(this, "Attempt to clone class not defined in TypeDefStmt");
  }

  map->clear();


  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);

  ClassType* new_class_type = dynamic_cast<ClassType*>(type->copy(true, map, clone_callback));

  if (!new_class_type) {
    INT_FATAL(this, "Major error in TypeSymbol::clone");
  }
  char* clone_name = glomstrings(3, name, "_clone_", intstring(uid++));

  TypeSymbol* new_type_sym = new TypeSymbol(clone_name, new_class_type);
  new_class_type->addSymbol(new_type_sym);
  TypeDefStmt* new_def_stmt = new TypeDefStmt(new_type_sym);
  new_type_sym->setDefPoint(new_def_stmt);
  old_def_stmt->insertBefore(new_def_stmt);

  Symboltable::setCurrentScope(save_scope);
  return new_type_sym;
}


void TypeSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE_DEF(type, traversal, false);
}


void TypeSymbol::codegenDef(FILE* outfile) {
  if (!type_is_used(this)) {
    return;
  }

  FILE* deffile = outfile;
  /* if in file scope, hoist to internal header so that it will be
     defined before global variables at file scope. */  
  if (type->symbol->parentScope->type == SCOPE_MODULE) { 
    deffile = intheadfile;
  }
  type->codegenDef(deffile);

  type->codegenStringToType(outfile);
  type->codegenIORoutines(outfile);
  type->codegenConfigVarRoutines(outfile);
  type->codegenConstructors(outfile);
}


FnSymbol::FnSymbol(char* init_name, Symbol* init_formals,
		   Type* init_retType, Stmt* init_body,
		   bool init_exportMe, Symbol* init_classBinding) :
  Symbol(SYMBOL_FN, init_name, init_retType),
  exportMe(init_exportMe),
  formals(init_formals),
  retType(init_retType),
  _this(0),
  body(init_body),
  classBinding(init_classBinding),
  overload(NULL)
{
  if (!dynamic_cast<UnresolvedSymbol*>(classBinding)) {
    Symboltable::define(this);
  } // Don't want to define this if it's bound to a class
  method_type = NON_METHOD;
}

 
FnSymbol::FnSymbol(char* init_name, Symbol* init_classBinding) :
  Symbol(SYMBOL_FN, init_name, NULL),
  formals(NULL),
  retType(NULL),
  _this(0),
  body(NULL),
  classBinding(init_classBinding)
{
  if (!dynamic_cast<UnresolvedSymbol*>(classBinding)) {
    Symboltable::define(this);
  } // Don't want to define this if it's bound to a class
  method_type = NON_METHOD;
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
      !formals) {
    if (!mainFn) {
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
  return new FnSymbol(name, classBinding);
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
  static int uid = 1; // Unique ID for cloned functions
  FnDefStmt* def_stmt = dynamic_cast<FnDefStmt*>(defPoint);

  if (!def_stmt) {
    INT_FATAL(this, "Attempt to clone function not defined in FnDefStmt");
  }

  map->clear();
  FnDefStmt* this_copy = NULL;
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  Stmt* stmt_copy = def_stmt->copy(true, map, clone_callback);
  if (this_copy = dynamic_cast<FnDefStmt*>(stmt_copy)) {
    this_copy->fn->cname =
      glomstrings(3, this_copy->fn->cname, "_clone_", intstring(uid++));
    def_stmt->insertBefore(this_copy);
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
      if (formal_change->next) {
	formal_change = nextLink(Symbol, formal_change);
      }
      if (actual_change->next) {
	actual_change = nextLink(Variable, actual_change);
      }
      j++;
    }
  }
  SymScope* block_scope = Symboltable::popScope();
  BlockStmt* wrapper_block = new BlockStmt(wrapper_body);
  wrapper_block->setBlkScope(block_scope);
  block_scope->stmtContext = wrapper_block;
  Type* wrapper_return_type = retType;
  wrapper_stmt = Symboltable::finishFnDef(wrapper_symbol, wrapper_formals,
					  wrapper_return_type, wrapper_block);
  FnDefStmt* def_stmt = dynamic_cast<FnDefStmt*>(defPoint);

  if (!def_stmt) {
    INT_FATAL(this, "Attempt to make coercion wrapper for function not defined in FnDefStmt");
  }

  def_stmt->insertBefore(wrapper_stmt);
  Symboltable::setCurrentScope(save_scope);
  return wrapper_stmt->fn;
}


FnSymbol* FnSymbol::default_wrapper(Vec<MPosition *> *defaults) {
  static int uid = 1; // Unique ID for wrapped functions
  FnDefStmt* wrapper_stmt = NULL;
  FnSymbol* wrapper_symbol;

  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  wrapper_symbol = new FnSymbol(name);
  wrapper_symbol->cname =
    glomstrings(3, cname, "_default_params_wrapper_", intstring(uid++));
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
  for (int i = 0; i < defaults->n; i++) {
    int j = 1;
    MPosition p;
    Symbol* formal_change = wrapper_formals;
    Variable* actual_change = argList;
    forv_MPosition(p, asymbol->sym->fun->numeric_arg_positions) {
      if (defaults->v[i] ==
	  asymbol->sym->fun->numeric_arg_positions.e[j]) {
	char* temp_name =
	  glomstrings(2, "_default_param_temp_", formal_change->name);
	VarSymbol* temp_symbol = new VarSymbol(temp_name, formal_change->type,
					       ((ParamSymbol*)formal_change)->init->copy());
	Stmt* temp_def_stmt = new VarDefStmt(temp_symbol);
	temp_symbol->setDefPoint(temp_def_stmt);
	temp_def_stmt->append(wrapper_body);
	wrapper_body = temp_def_stmt;
	actual_change->var = temp_symbol;
	if (formal_change == wrapper_formals) {
	  wrapper_formals = nextLink(Symbol, formal_change);
	  if (!wrapper_formals) {
	    wrapper_formals = NULL;
	  }
	}
	if (formal_change->prev) {
	  formal_change->prev->next = formal_change->next;
	}
	if (formal_change->next) {
	  formal_change->next->prev = formal_change->prev;
	}
      }
      if (formal_change->next) {
	formal_change = nextLink(Symbol, formal_change);
      }
      if (actual_change->next) {
	actual_change = nextLink(Variable, actual_change);
      }
      j++;
    }
  }
  SymScope* block_scope = Symboltable::popScope();
  BlockStmt* wrapper_block = new BlockStmt(wrapper_body);
  wrapper_block->setBlkScope(block_scope);
  block_scope->stmtContext = wrapper_block;
  Type* wrapper_return_type = retType;
  wrapper_stmt = Symboltable::finishFnDef(wrapper_symbol, wrapper_formals,
					  wrapper_return_type, wrapper_block);
  FnDefStmt* def_stmt = dynamic_cast<FnDefStmt*>(defPoint);

  if (!def_stmt) {
    INT_FATAL(this, "error in FnSymbol::default_wrapper");
  }

  def_stmt->insertBefore(wrapper_stmt);
  Symboltable::setCurrentScope(save_scope);
  return wrapper_stmt->fn;
}


FnSymbol* FnSymbol::order_wrapper(Map<MPosition *, MPosition *> *formals_to_actuals) {
  INT_FATAL(this, "FnSymbol::order_wrapper not implemented");
  return NULL;
}


FnSymbol* FnSymbol::instantiate_generic(Map<Type *, Type *> *generic_substitutions) {
  FnDefStmt* def_stmt = dynamic_cast<FnDefStmt*>(defPoint);

  if (!def_stmt) {
    INT_FATAL(this, "Attempt to instantiate generic function not defined in FnDefStmt");
  }

  FnDefStmt* this_copy = NULL;
  static int uid = 1; // Unique ID for cloned functions
  SymScope* save_scope;
  save_scope = Symboltable::setCurrentScope(parentScope);
  Map<BaseAST*,BaseAST*> map;
  Stmt* stmt_copy = def_stmt->copy(true, &map);
  if (this_copy = dynamic_cast<FnDefStmt*>(stmt_copy)) {
    for (int i = 0; i < map.n; i++) {
      if (map.v[i].key) {
	if (Variable* var = dynamic_cast<Variable*>(map.v[i].key)) {
	  if (Type* new_type = generic_substitutions->get(var->var->type)) {
	    Variable* new_var = dynamic_cast<Variable*>(map.v[i].value);
	    new_var->var->type = new_type;
	  }
	}
	else if (Symbol* sym = dynamic_cast<Symbol*>(map.v[i].key)) {
	  if (Type* new_type = generic_substitutions->get(sym->type)) {
	    Symbol* new_sym = dynamic_cast<Symbol*>(map.v[i].value);
	    new_sym->type = new_type;
	  }
	}
      }
    }
    this_copy->fn->cname =
      glomstrings(3, this_copy->fn->cname, "_instantiate_", intstring(uid++));
    def_stmt->insertBefore(this_copy);
  }
  else {
    INT_FATAL(this, "Error in FnSymbol::instantiate_generic");
  }
  Symboltable::setCurrentScope(save_scope);
  return this_copy->fn;
}


void FnSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "function ");
  print(outfile);
  fprintf(outfile, "(");
  if (formals) {
    formals->printDefList(outfile, ";\n");
  }
  fprintf(outfile, ")");
  if (retType == dtVoid) {
    fprintf(outfile, " ");
  } else {
    fprintf(outfile, ": ");
    type->print(outfile);
    fprintf(outfile, " ");
  }
  body->print(outfile);
  fprintf(outfile, "\n\n");
}


void FnSymbol::codegenHeader(FILE* outfile) {
  if (!exportMe) {
    fprintf(outfile, "static ");
  }
  retType->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
  fprintf(outfile, "(");
  if (!formals) {
    fprintf(outfile, "void");
  } else {
    formals->codegenDefList(outfile, ", ");
  }
  fprintf(outfile, ")");
}


void FnSymbol::codegenDef(FILE* outfile) {
  FILE* headfile;

  if (function_is_used(this)) {

    if (exportMe) {
      headfile = extheadfile;
    } else {
      headfile = intheadfile;
    }
    codegenHeader(headfile);
    fprintf(headfile, ";\n");

    codegenHeader(outfile);
    // need an extra set of curly braces in case (a) body is
    // non-compound statement, or (b) formal parameters share same name
    // as local variable
    fprintf(outfile, " {\n");
    body->codegen(outfile);
    fprintf(outfile, "\n");
    fprintf(outfile, "}\n");
    fprintf(outfile, "\n\n");
  }
 if (overload) {
    overload->codegenDef(outfile);
  }
}

FnSymbol* FnSymbol::mainFn;

void FnSymbol::init(void) {
  mainFn = NULL;
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
    if (tmp->init) {
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
  stmts(NULL),
  initFn(NULL),
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

  fprintf(codefile, "#include \"_CommonModule.h\"\n");
  fprintf(codefile, "#include \"_CommonModule-internal.h\"\n");
  /** SJD: Better to export things? (above line) **/
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
                                                 : NULL);
  //SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  FnDefStmt* initFunDef = Symboltable::defineFunction(fnName, NULL, 
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


LabelSymbol::LabelSymbol(char* init_name) :
  Symbol(SYMBOL_LABEL, init_name, NULL) {
  Symboltable::define(this);
}


void LabelSymbol::codegenDef(FILE* outfile) { }
