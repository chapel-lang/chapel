#include <string.h>
#include <typeinfo>
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "symtab.h"
#include "../traversals/fixup.h"
#include "../traversals/cleanup.h"


Stmt::Stmt(astType_t astType) :
  BaseAST(astType),
  ainfo(NULL),
  back(NULL)
{}


bool Stmt::isNull(void) {
  return (this == nilStmt);
}


bool Stmt::canLiveAtFileScope(void) {
  return false;
}


bool Stmt::topLevelExpr(Expr* expr) {
  return false;
}


Stmt* Stmt::copyList(CloneCallback* analysis_clone) {
  Stmt* newStmtList = copyListInternal(analysis_clone);
  call_cleanup_ls(newStmtList);
  return newStmtList;
}


Stmt* Stmt::copy(CloneCallback* analysis_clone) {
  Stmt* new_stmt = copyInternal(analysis_clone);
  call_cleanup(new_stmt);
  return new_stmt;
}


Stmt* Stmt::copyListInternal(CloneCallback* analysis_clone) {
  Stmt* newStmtList = nilStmt;
  Stmt* oldStmt = this;

  while (oldStmt) {
    newStmtList = appendLink(newStmtList, oldStmt->copyInternal(analysis_clone));

    oldStmt = nextLink(Stmt, oldStmt);
  }

  return newStmtList;
}


Stmt* Stmt::copyInternal(CloneCallback* analysis_clone) {
  Stmt* new_stmt = copyStmt(analysis_clone);

  new_stmt->lineno = lineno;
  new_stmt->filename = filename;
  if (analysis_clone) {
    analysis_clone->clone(this, new_stmt);
  }
  return new_stmt;
}


Stmt* Stmt::copyStmt(CloneCallback* analysis_clone) {
  if (!this->isNull()) {
    INT_FATAL(this, "copy not implemented for Stmt subclass");
  }
  return nilStmt;
}


void Stmt::traverse(Traversal* traversal, bool atTop) {
  if (isNull()) {
    return;
  }
  if (traversal->processTop || !atTop) {
    traversal->preProcessStmt(this);
  }
  if (atTop || traversal->exploreChildStmts) {
    traverseStmt(traversal);
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessStmt(this);
  }
}


void Stmt::traverseStmt(Traversal* traversal) {
}


void Stmt::codegenVarDefs(FILE* outfile) {
  Stmt* nextStmt = this;

  do {
    nextStmt->codegenVarDef(outfile);
    nextStmt = nextLink(Stmt, nextStmt);
  } while (nextStmt);
}


void Stmt::codegenVarDef(FILE* outfile) { }


/* printing out vardefs in a list, names modified by premod and postmod */
void Stmt::codegenVarNames(FILE* outfile, char* premod, char* postmod) {
  Stmt* nextStmt = this;

  do {
    nextStmt->codegenVarName(outfile, premod, postmod);
    nextStmt = nextLink(Stmt, nextStmt);
    if (nextStmt) {
      fprintf(outfile, ",\n");
    }
  } while (nextStmt);
  fprintf(outfile, "\n");
}


void Stmt::codegenVarName(FILE* outfile, char* premod, char* postmod) { }


static void call_fixup(Stmt* stmt) {
  Fixup* fixup = new Fixup();

  fixup->stmtParent.add(stmt->parentSymbol);
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(stmt->parentSymbol)) {
    TRAVERSE_LS(fn->body, fixup, true);
  }
  else if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(stmt->parentSymbol)) {
    TRAVERSE_LS(mod->stmts, fixup, true);
  }
  else if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(stmt->parentSymbol)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(type->type)) {
      TRAVERSE(class_type, fixup, true);
    }
  }
}


void Stmt::replace(Stmt* new_stmt) {
  Stmt* first = dynamic_cast<Stmt*>(new_stmt->head());
  Stmt* last = dynamic_cast<Stmt*>(new_stmt->tail());

  if (!first || !last) {
    INT_FATAL(this, "Illegal call to replace, new_stmt list invalid");
  }

  if (first != new_stmt) {
    INT_FATAL(this, "Illegal replace, new_stmt is not head of list");
  }

  last->next = next;
  if (next && !next->isNull()) {
    next->prev = last;
    Stmt* tmp = dynamic_cast<Stmt*>(next);
    tmp->back = &(Stmt*&)last->next; // UGH --SJD
  }
  first->prev = prev;
  first->back = back;
  *back = first;
  /* NOT NECESSARY BECAUSE OF PRECEDING LINE
    if (prev && !prev->isNull()) {
      prev->next = first;
    }
  */
  prev = nilStmt;
  next = nilStmt;
  call_fixup(this);
}


void Stmt::insertBefore(Stmt* new_stmt) {
  Stmt* first = dynamic_cast<Stmt*>(new_stmt->head());
  Stmt* last = dynamic_cast<Stmt*>(new_stmt->tail());

  if (!first || !last) {
    INT_FATAL(this, "Illegal call to insertBefore, new_stmt list invalid");
  }

  if (first != new_stmt) {
    INT_FATAL(this, "Illegal insertBefore, new_stmt is not head of list");
  }

  first->prev = prev;
  first->back = back;
  *back = first;
  /* NOT NECESSARY BECAUSE OF PRECEDING LINE
    if (prev && !prev->isNull()) {
      prev->next = first;
    }
  */
  back = &(Stmt*&)last->next; // UGH --SJD

  prev = last;
  last->next = this;

  call_fixup(this);
}


void Stmt::insertAfter(Stmt* new_stmt) {
  Stmt* first = dynamic_cast<Stmt*>(new_stmt->head());
  Stmt* last = dynamic_cast<Stmt*>(new_stmt->tail());

  if (!first || !last) {
    INT_FATAL(this, "Illegal call to insertAfter, new_stmt list invalid");
  }

  if (first != new_stmt) {
    INT_FATAL(this, "Illegal insertAfter, new_stmt is not head of list");
  }

  last->next = next;
  if (next && !next->isNull()) {
    next->prev = last;
    Stmt* tmp = dynamic_cast<Stmt*>(next);
    tmp->back = &(Stmt*&)last->next; // UGH --SJD
  }
  next = first;
  first->prev = this;
  first->back = &(Stmt*&)next; // UGH --SJD

  call_fixup(this);
}


void Stmt::append(ILink* new_stmt) {
  if (new_stmt->isNull()) {
    return;
  }

  Stmt* first = dynamic_cast<Stmt*>(new_stmt->head());
  Stmt* last = dynamic_cast<Stmt*>(new_stmt->tail());

  if (!first || !last) {
    INT_FATAL(this, "Illegal call to append, new_stmt list invalid");
  }

  if (first != new_stmt) {
    INT_FATAL(this, "Illegal append, new_stmt is not head of list");
  }

  Stmt* append_point = dynamic_cast<Stmt*>(this->tail());

  append_point->next = first;
  first->prev = append_point;
  // UGH!! --SJD
  first->back = &(Stmt*&)append_point->next;
  if ((*first->back)->isNull()) {
    INT_FATAL(this, "major error in back");
  }
}


Stmt* Stmt::extract(void) {
  if (next && !next->isNull()) {
    if (Stmt* next_stmt = dynamic_cast<Stmt*>(next)) { 
      next_stmt->prev = prev;
      next_stmt->back = back;
      *back = next_stmt;
      /* NOT NECESSARY BECAUSE OF PRECEDING LINE
	 if (prev && !prev->isNull()) {
	 prev->next = next;
	 }
      */
    }
    else {
      INT_FATAL(this, "Illegal call to extract, stmt is invalid");
    }
  }
  else {
    *back = nilStmt;
  }
  next = nilStmt;
  prev = nilStmt;
  back = NULL;

  return this;
}


NoOpStmt::NoOpStmt(void) :
  Stmt(STMT_NOOP)
{}


Stmt* NoOpStmt::copyStmt(CloneCallback* analysis_clone) {
  return new NoOpStmt();
}


void NoOpStmt::print(FILE* outfile) {
  fprintf(outfile, ";");
}


void NoOpStmt::codegen(FILE* outfile) {
  fprintf(outfile, "{}\n");
}


WithStmt::WithStmt(Expr* init_withExpr) :
  Stmt(STMT_WITH),
  withExpr(init_withExpr)
{
  SET_BACK(withExpr);
}


ClassType* WithStmt::getClass(void) {
  if (Variable* var = dynamic_cast<Variable*>(withExpr)) {
    if (ClassType* result = 
	dynamic_cast<ClassType*>(var->var->type)) {
      return result;
    }
    else if (UnresolvedSymbol* unresolved = 
	     dynamic_cast<UnresolvedSymbol*>(var->var)) {
      if (ClassType* result = 
	  dynamic_cast<ClassType*>(Symboltable::lookup(unresolved->name)->type)) {
	return result;
      }
      else {
	INT_FATAL(this, "Bad with statement");
      }
    }
    else {
      INT_FATAL(this, "Bad with statement");
    }
  }
  else {
    INT_FATAL(this, "Bad with statement");
  }
  return NULL;
}


Stmt* WithStmt::copyStmt(CloneCallback* analysis_clone) {
  return new WithStmt(withExpr->copyInternal(analysis_clone));
}


void WithStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(withExpr, traversal, false);
}


void WithStmt::print(FILE* outfile) {
  fprintf(outfile, "with ");
  withExpr->print(outfile);
  fprintf(outfile, "\n");
}


void WithStmt::codegen(FILE* outfile) {
  INT_FATAL(this, "With statement encountered in codegen()");
}


VarDefStmt::VarDefStmt(VarSymbol* init_var, Expr* init_init) :
  Stmt(STMT_VARDEF),
  var(init_var),
  init(init_init) 
{
  SET_BACK(init);
}


Stmt* VarDefStmt::copyStmt(CloneCallback* analysis_clone) {
  return Symboltable::defineVarDefStmt(var, var->type, init->copyInternal(analysis_clone), 
				       var->varClass, var->isConst);
}


bool VarDefStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == init);
}


void VarDefStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE_LS(var, traversal, false);
  TRAVERSE(var->type, traversal, false);
  TRAVERSE(init, traversal, false);
}


void VarDefStmt::print(FILE* outfile) {
  VarSymbol* aVar = var;

  while (aVar) {
    switch (aVar->varClass) {
    case VAR_NORMAL:
      break;
    case VAR_CONFIG:
      fprintf(outfile, "config ");
      break;
    case VAR_STATE:
      fprintf(outfile, "state ");
      break;
    }
    if (aVar->isConst) {
      fprintf(outfile, "const ");
    } else {
      fprintf(outfile, "var ");
    }
    aVar->printDef(outfile);
    if (!init->isNull()) {
      fprintf(outfile, " = ");
      if (init->next->isNull()) {
	init->print(outfile);
      } else {
	fprintf(outfile, "(");
	init->printList(outfile);
	fprintf(outfile, ")");
      }
    }
    fprintf(outfile, ";");

    aVar = nextLink(VarSymbol, aVar);
    if (aVar) {
      fprintf(outfile, "\n");
    }
  }
}

void VarDefStmt::codegen(FILE* outfile) {
  VarSymbol* aVar = var;
  Expr* initExpr;
  
  while (aVar) {
    VarSymbol* nextVar = nextLink(VarSymbol, aVar);
    if (init->isNull()) {
      initExpr = aVar->type->defaultVal;
    } else {
      initExpr = init;
    }
    if (aVar->type->needsInit()) {
      aVar->type->generateInit(outfile, aVar);
      if (!initExpr->isNull()) {
	fprintf(outfile, "/* init is: ");
	initExpr->codegen(outfile);
	fprintf(outfile, "*/\n");
      }
    } else if (!initExpr->isNull()) {

      if (typeid(*(aVar->type)) == typeid(DomainType)) {
	DomainType* domtype = (DomainType*)(aVar->type);
	int rank = domtype->numdims ? domtype->numdims : 1; // BLC: hack!
	fprintf(outfile, "_init_domain_%dD(&(", rank);
	aVar->codegen(outfile);
	fprintf(outfile, ")");
	if (typeid(*initExpr) == typeid(ForallExpr)) {
	  initExpr = ((ForallExpr*)initExpr)->domains;
	}
	if (typeid(*initExpr) == typeid(SimpleSeqExpr)) {
	  SimpleSeqExpr* initseq = (SimpleSeqExpr*)initExpr;

	  for (int i=0; i<rank; i++) {
	    fprintf(outfile, ", ");
	    initseq->lo->codegen(outfile);
	    fprintf(outfile, ", ");
	    initseq->hi->codegen(outfile);
	    fprintf(outfile, ", ");
	    initseq->str->codegen(outfile);
	    initseq = nextLink(SimpleSeqExpr, initseq);
	  }
	}
	fprintf(outfile, ");");
      } else {
	if (aVar->varClass == VAR_CONFIG) {
	  fprintf(outfile, "if (!setInCommandLine");
	  aVar->type->codegen(outfile);
	  fprintf(outfile, "(\"%s\", &%s)) {\n", aVar->name, aVar->name);
	}
	// TODO: hoist this into a traversal that rewrites vardefs as
	// assignments?
	AssignOp* assignment = new AssignOp(GETS_NORM, new Variable(aVar), 
					    initExpr);
	assignment->codegen(outfile);
	fprintf(outfile, ";");
	if (aVar->varClass == VAR_CONFIG) {
	  fprintf(outfile, "\n}");
	}
      }
      if (nextVar) {
	fprintf(outfile, "\n");
      }
    }
    aVar = nextVar;
  }
}


void VarDefStmt::codegenVarDef(FILE* outfile) {
  VarSymbol* aVar = var;

/* if in module scope, hoist to internal header */  
  if (aVar->parentScope->type == SCOPE_MODULE) { 
    outfile = intheadfile;
  }

  // TODO: for certain cases, we could use a C initializer inline rather
  // than separating the variable declaration from its initialization.

  while (aVar) {
    if (aVar->isConst) {
      fprintf(outfile, "const ");
    }

    aVar->codegenDef(outfile);

    fprintf(outfile, ";\n");
    aVar = nextLink(VarSymbol, aVar);
  }
}


void VarDefStmt::codegenVarName(FILE* outfile, char* premod, char* postmod) {
  VarSymbol* vsym = var;

  while (vsym) {
    fprintf(outfile, premod);
    vsym->codegen(outfile);
    fprintf(outfile, postmod);
    vsym = nextLink(VarSymbol, vsym);
    if (vsym) {
      fprintf(outfile, ",\n");
    }
  }
}


TypeDefStmt::TypeDefStmt(Type* init_type) :
  Stmt(STMT_TYPEDEF),
  type(init_type)
{}


Stmt* TypeDefStmt::copyStmt(CloneCallback* analysis_clone) {
  Type* newType = type->copy(analysis_clone);
  Symboltable::define(type->name);
  return new TypeDefStmt(newType);
}


bool TypeDefStmt::canLiveAtFileScope(void) {
  return true;
}


void TypeDefStmt::traverseStmt(Traversal* traversal) {
  type->traverseDef(type, traversal, false);
}


void TypeDefStmt::print(FILE* outfile) {
  type->printDef(outfile);
  fprintf(outfile, ";");
}


void TypeDefStmt::codegen(FILE* outfile) {
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


FnDefStmt::FnDefStmt(FnSymbol* init_fn) :
  Stmt(STMT_FNDEF),
  fn(init_fn)
{}


Stmt* FnDefStmt::copyStmt(CloneCallback* analysis_clone) {
  FnSymbol* fncopy = dynamic_cast<FnSymbol*>(fn->copy(analysis_clone));
  Symboltable::startFnDef(fncopy);
  // do this first to make sure symbols are defined before used when
  // body is copied
  Symbol* newformals = fn->formals->copyList(analysis_clone);
  for (Symbol* tmp = newformals; tmp != NULL; tmp = nextLink(Symbol, tmp)) {
    Symboltable::define(tmp);
  }
  return Symboltable::finishFnDef(fncopy, newformals, fn->type, 
				  fn->body->copyListInternal(analysis_clone),
				  fn->exportMe);
}


FnDefStmt* FnDefStmt::clone(CloneCallback* clone_callback) {
  FnDefStmt* this_copy = NULL;
  static int uid = 1; // Unique ID for cloned functions
  SymScope* save_scope;

  save_scope = Symboltable::setCurrentScope(this->fn->parentScope);
  Stmt* stmt_copy = copy(clone_callback);
  if (this_copy = dynamic_cast<FnDefStmt*>(stmt_copy)) {
    this_copy->fn->cname =
      glomstrings(3, this_copy->fn->cname, "_clone_", intstring(uid++));
    this->insertBefore(this_copy);
  }
  else {
    INT_FATAL(this, "Unreachable statement in FnDefStmt::clone reached");
  }
  Symboltable::setCurrentScope(save_scope);
  return this_copy;
}


bool FnDefStmt::isNull(void) {
  return (this == nilFnDefStmt);
}


bool FnDefStmt::canLiveAtFileScope(void) {
  return true;
}


void FnDefStmt::traverseStmt(Traversal* traversal) {
  SymScope* prevScope;

  // BLC: could move all this into a traverseDef method?
  TRAVERSE(fn, traversal, false);
  if (fn->paramScope) {
    prevScope = Symboltable::setCurrentScope(fn->paramScope);
  }
  TRAVERSE_LS(fn->formals, traversal, false);
  TRAVERSE(fn->type, traversal, false);
  TRAVERSE(fn->body, traversal, false);
  if (fn->paramScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


void FnDefStmt::print(FILE* outfile) {
  fprintf(outfile, "function ");
  fn->print(outfile);
  fprintf(outfile, "(");
  if (!fn->formals->isNull()) {
    fn->formals->printDefList(outfile, ";\n");
  }
  fprintf(outfile, ")");
  if (fn->type == dtVoid) {
    fprintf(outfile, " ");
  } else {
    fprintf(outfile, ": ");
    fn->type->print(outfile);
  }
  fn->body->print(outfile);
  fprintf(outfile, "\n\n");
}


void FnDefStmt::codegen(FILE* outfile) {
  FILE* headfile;

  if (fn->exportMe) {
    headfile = extheadfile;
  } else {
    headfile = intheadfile;
  }
  fn->codegenDef(headfile);
  fprintf(headfile, ";\n");

  fn->codegenDef(outfile);
  fprintf(outfile, " ");
  fn->body->codegen(outfile);
  fprintf(outfile, "\n");
}


ModuleDefStmt::ModuleDefStmt(ModuleSymbol* init_module) :
  Stmt(STMT_MODDEF),
  module(init_module)
{}


void ModuleDefStmt::codegen(FILE* outfile) {
  fprintf(outfile, "/* module ");
  module->codegen(outfile);
  fprintf(outfile, " was declared here */");
}


ExprStmt::ExprStmt(Expr* init_expr) :
  Stmt(STMT_EXPR),
  expr(init_expr) 
{
  SET_BACK(expr);
}


Stmt* ExprStmt::copyStmt(CloneCallback* analysis_clone) {
  return new ExprStmt(expr->copyInternal(analysis_clone));
}


bool ExprStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == expr);
}


void ExprStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(expr, traversal, false);
}


void ExprStmt::print(FILE* outfile) {
  expr->print(outfile);
  fprintf(outfile, ";");
}


void ExprStmt::codegen(FILE* outfile) {
  expr->codegen(outfile);
  fprintf(outfile, ";");
}


ExprStmt* ExprStmt::createFnCallStmt(FnSymbol* fnSym, Expr* argList) {
  return new ExprStmt(new FnCall(new Variable(fnSym), argList));
}


ReturnStmt::ReturnStmt(Expr* retExpr) :
  ExprStmt(retExpr)
{
  astType = STMT_RETURN;
}


Stmt* ReturnStmt::copyStmt(CloneCallback* analysis_clone) {
  return new ReturnStmt(expr->copyInternal(analysis_clone));
}


void ReturnStmt::print(FILE* outfile) {
  fprintf(outfile, "\n");
  fprintf(outfile, "return");
  if (!expr->isNull()) {
    fprintf(outfile, " ");
    expr->print(outfile);
  }
  fprintf(outfile, ";");
}


void ReturnStmt::codegen(FILE* outfile) {
  fprintf(outfile, "return");
  if (!expr->isNull()) {
    fprintf(outfile, " ");
    expr->codegen(outfile);
  }
  fprintf(outfile, ";");
}


BlockStmt::BlockStmt(Stmt* init_body) :
  Stmt(STMT_BLOCK),
  body(init_body),
  blkScope(NULL)
{
  SET_BACK(body);
}


void BlockStmt::addBody(Stmt* init_body) {
  if (body->isNull()) {
    body = init_body;
    SET_BACK(body); // SJD: Eliminate please.
  } else {
    INT_FATAL(this, "Adding a body to a for loop that already has one");
  }
}


void BlockStmt::setBlkScope(SymScope* init_blkScope) {
  blkScope = init_blkScope;
}


Stmt* BlockStmt::copyStmt(CloneCallback* analysis_clone) {
  return new BlockStmt(body->copyListInternal(analysis_clone));
}


void BlockStmt::traverseStmt(Traversal* traversal) {
  SymScope* prevScope;
  if (blkScope) {
    prevScope = Symboltable::setCurrentScope(blkScope);
  }
  TRAVERSE_LS(body, traversal, false);
  if (blkScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


void BlockStmt::print(FILE* outfile) {
  fprintf(outfile, "{\n");
  body->printList(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "}");
}

void BlockStmt::codegen(FILE* outfile) {
  fprintf(outfile, "{\n");
  body->codegenVarDefs(outfile);
  body->codegenList(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "}");
}


WhileLoopStmt::WhileLoopStmt(bool init_whileDo, 
			     Expr* init_cond, 
			     Stmt* init_body) 
  : BlockStmt(init_body), 
    isWhileDo(init_whileDo), 
    condition(init_cond) 
{
  astType = STMT_WHILELOOP;
  SET_BACK(condition);
}


Stmt* WhileLoopStmt::copyStmt(CloneCallback* analysis_clone) {
  return new WhileLoopStmt(isWhileDo, condition->copyInternal(analysis_clone), body->copyInternal(analysis_clone));
}


bool WhileLoopStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == condition);
}


void WhileLoopStmt::traverseStmt(Traversal* traversal) {
  if (isWhileDo) {
    TRAVERSE(condition, traversal, false);
    TRAVERSE(body, traversal, false);
  } else {
    TRAVERSE(body, traversal, false);
    TRAVERSE(condition, traversal, false);
  }
}


void WhileLoopStmt::print(FILE* outfile) {

  if (isWhileDo) {
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ") ");
    body->print(outfile);
  } else {
    fprintf(outfile, "do ");
    body->print(outfile);
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ")");
  }
}


void WhileLoopStmt::codegen(FILE* outfile) {

  if (isWhileDo) {
    fprintf(outfile, "while (");
    condition->codegen(outfile);
    fprintf(outfile, ") {\n");
    body->codegen(outfile);
    fprintf(outfile, "\n}");
  } else { 
    fprintf(outfile, "do {\n");
    body->codegen(outfile);
    fprintf(outfile, "\n} while (");
    condition->codegen(outfile);
    fprintf(outfile, ");\n");
  }
}


ForLoopStmt::ForLoopStmt(bool init_forall,
			 VarSymbol* init_index,
			 Expr* init_domain,
			 Stmt* body)
  : BlockStmt(body),
    forall(init_forall),
    index(init_index),
    domain(init_domain),
    indexScope(NULL)
{
  astType = STMT_FORLOOP;
  SET_BACK(domain);
}


void ForLoopStmt::setIndexScope(SymScope* init_indexScope) {
  indexScope = init_indexScope;
}


Stmt* ForLoopStmt::copyStmt(CloneCallback* analysis_clone) {
  return new ForLoopStmt(forall, dynamic_cast<VarSymbol*>(index->copy(analysis_clone)), 
			 domain->copyInternal(analysis_clone), body->copyInternal(analysis_clone));
}


bool ForLoopStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == domain);
}


void ForLoopStmt::traverseStmt(Traversal* traversal) {
  SymScope* prevScope;

  TRAVERSE(domain, traversal, false);
  if (indexScope) {
    prevScope = Symboltable::setCurrentScope(indexScope);
  }
  TRAVERSE(index, traversal, false);
  TRAVERSE(body, traversal, false);
  if (indexScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


void ForLoopStmt::print(FILE* outfile) {
  fprintf(outfile, "for");
  if (forall) {
    fprintf(outfile, "all");
  }
  fprintf(outfile, " ");
  index->print(outfile);
  fprintf(outfile, " in ");
  domain->print(outfile);
  fprintf(outfile, " ");
  body->print(outfile);
}


void ForLoopStmt::codegen(FILE* outfile) {
  VarSymbol* aVar = index;
  fprintf(outfile, "{\n");
  int rank = 0;

  // TODO: Unify with VarDefStmt?  Have parser insert one here?
  // is it a challenge that we may not know the domain exprs at that point?
  while (aVar) {
    aVar->codegenDef(outfile);
    fprintf(outfile, ";\n");
    rank++;

    aVar = nextLink(VarSymbol, aVar);
  }
  fprintf(outfile, "\n");
  
  aVar = index;
  for (int i=0; i<rank; i++) {
    fprintf(outfile, "_FOR");
    if (forall) {
      fprintf(outfile, "ALL");
    }
    fprintf(outfile, "(");
    aVar->codegen(outfile);
    fprintf(outfile, ", ");
    domain->codegen(outfile);
    fprintf(outfile, ", %d) {\n", i);

    aVar = nextLink(VarSymbol, aVar);
  }

  body->codegen(outfile);
  fprintf(outfile, "\n");

  for (int i=0; i<rank; i++) {
    fprintf(outfile, "}\n");
  }
  
  fprintf(outfile, "}\n");
}


CondStmt::CondStmt(Expr*  init_condExpr, Stmt* init_thenStmt, 
		   Stmt* init_elseStmt) :
  Stmt(STMT_COND),
  condExpr(init_condExpr),
  thenStmt(init_thenStmt),
  elseStmt(init_elseStmt)
{
  SET_BACK(condExpr);
  SET_BACK(thenStmt);
  SET_BACK(elseStmt);
}


Stmt* CondStmt::copyStmt(CloneCallback* analysis_clone) {
  return new CondStmt(condExpr->copyInternal(analysis_clone), thenStmt->copyInternal(analysis_clone), elseStmt->copyInternal(analysis_clone));
}


bool CondStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == condExpr);
}


void CondStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(condExpr, traversal, false);
  TRAVERSE(thenStmt, traversal, false);
  TRAVERSE(elseStmt, traversal, false);
}


void CondStmt::print(FILE* outfile) {
  fprintf(outfile, "if (");
  condExpr->print(outfile);
  fprintf(outfile, ") ");
  thenStmt->print(outfile);
  if (!elseStmt->isNull()) {
    fprintf(outfile, " else ");
    elseStmt->print(outfile);
  }
}

void CondStmt::codegen(FILE* outfile) {
  fprintf(outfile, "if (");
  condExpr->codegen(outfile);
  fprintf(outfile, ") {\n");
  thenStmt->codegen(outfile);
  fprintf(outfile, "\n}");
  if (!elseStmt->isNull()) {
    fprintf(outfile, " else {\n");
    elseStmt->codegen(outfile);
    fprintf(outfile, "\n}");
  }
}
