#include <string.h>
#include <typeinfo>
#include "expr.h"
#include "misc.h"
#include "module.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "symtab.h"


Stmt::Stmt(astType_t astType) :
  BaseAST(astType),
  parentFn(Symboltable::getCurrentFn())
{}


Stmt* Stmt::copy(void) {
  if (!this->isNull()) {
    INT_FATAL(this, "copy not implemented for Stmt subclass");
  }
  return nilStmt;
}


Stmt* Stmt::copyList(void) {
  Stmt* newStmtList = nilStmt;
  Stmt* oldStmt = this;

  while (oldStmt) {
    newStmtList = appendLink(newStmtList, oldStmt->copy());

    oldStmt = nextLink(Stmt, oldStmt);
  }

  return newStmtList;
}


bool Stmt::isNull(void) {
  return (this == nilStmt);
}


bool Stmt::canLiveAtFileScope(void) {
  return false;
}


bool Stmt::topLevelExpr(Expr* expr) {
  return false;
}


void Stmt::traverse(Traversal* traversal, bool atTop) {
  if (isNull()) {
    return;
  }

  // explore Stmt and components
  if (traversal->processTop || !atTop) {
    traversal->preProcessStmt(this);
  }
  if (atTop || traversal->exploreChildStmts) {
    this->traverseStmt(traversal);
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


void Stmt::codegenVarDef(FILE* outfile) {
}


NoOpStmt::NoOpStmt(void) :
  Stmt(STMT_NOOP)
{}


Stmt* NoOpStmt::copy(void) {
  return new NoOpStmt();
}


void NoOpStmt::print(FILE* outfile) {
  fprintf(outfile, ";");
}


void NoOpStmt::codegen(FILE* outfile) {
  fprintf(outfile, "{}\n");
}


VarDefStmt::VarDefStmt(VarSymbol* init_var, Expr* init_init) :
  Stmt(STMT_VARDEF),
  var(init_var),
  init(init_init) 
{}


Stmt* VarDefStmt::copy(void) {
  return Symboltable::defineVarDefStmt(var, var->type, init->copy(), 
				       var->varClass, var->isConst);
}


bool VarDefStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == init);
}


void VarDefStmt::traverseStmt(Traversal* traversal) {
  var->traverseList(traversal, false);
  var->type->traverse(traversal, false);
  init->traverse(traversal, false);
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
	if (typeid(*initExpr) == typeid(DomainExpr)) {
	  initExpr = ((DomainExpr*)initExpr)->domains;
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
       // TODO: hoist this into a traversal that rewrites vardefs as
       // assignments?
       AssignOp* assignment = new AssignOp(GETS_NORM, new Variable(aVar), 
					   initExpr);
       assignment->codegen(outfile);
       fprintf(outfile, ";");
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

  if (aVar->scope->level == SCOPE_FILE) { /* if in file scope, hoist to internal header */
    outfile = currentModule->intheadfile;
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


TypeDefStmt::TypeDefStmt(Type* init_type) :
  Stmt(STMT_TYPEDEF),
  type(init_type)
{}


Stmt* TypeDefStmt::copy(void) {
  Type* newType = type->copy();
  Symboltable::define(type->name);
  return new TypeDefStmt(newType);
}


bool TypeDefStmt::canLiveAtFileScope(void) {
  return true;
}


void TypeDefStmt::traverseStmt(Traversal* traversal) {
  type->traverseDef(traversal, false);
}


void TypeDefStmt::print(FILE* outfile) {
  type->printDef(outfile);
  fprintf(outfile, ";");
}


void TypeDefStmt::codegen(FILE* outfile) {
  FILE* deffile = outfile;
  /* if in file scope, hoist to internal header so that it will be
     defined before global variables at file scope. */  
  if (type->name->scope->level == SCOPE_FILE) { 
    deffile = currentModule->intheadfile;
  }
  type->codegenDef(deffile);

  type->codegenIORoutines(outfile);
  type->codegenConstructors(outfile);
}


FnDefStmt::FnDefStmt(FnSymbol* init_fn) :
  Stmt(STMT_FNDEF),
  fn(init_fn)
{}


Stmt* FnDefStmt::copy(void) {
  FnSymbol* fncopy = Symboltable::startFnDef(glomstrings(3, "__", fn->name, "_clone"));
  // do this first to make sure symbols are defined before used when body is
  // copied
  Symbol* newformals;
  if (typeid(*(fn->formals)) == typeid(ParamSymbol)) {
    ParamSymbol* oldformals = (ParamSymbol*)(fn->formals);
    newformals = Symboltable::copyParams(oldformals);
  } else {
    newformals = nilSymbol;
  }
  return Symboltable::finishFnDef(fncopy, newformals, fn->type->copy(), 
				  fn->body->copyList(), fn->exportMe);
}


Stmt* FnDefStmt::clone(void) {
  SymScope* prevScope = Symboltable::setCurrentScope(fn->scope);
  Stmt* newStmt = this->copy();
  Symboltable::setCurrentScope(prevScope);
  this->add(newStmt);

  return newStmt;
}


bool FnDefStmt::isNull(void) {
  return (this == nilFnDefStmt);
}


bool FnDefStmt::canLiveAtFileScope(void) {
  return true;
}


void FnDefStmt::traverseStmt(Traversal* traversal) {
  // BLC: could move this into a traverseDef method?
  fn->traverse(traversal, false);
  fn->formals->traverseList(traversal, false);
  fn->type->traverse(traversal, false);
  fn->body->traverse(traversal, false);
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
    headfile = currentModule->extheadfile;
  } else {
    headfile = currentModule->intheadfile;
  }
  fn->codegenDef(headfile);
  fprintf(headfile, ";\n");

  fn->codegenDef(outfile);
  fprintf(outfile, " ");
  fn->body->codegen(outfile);
  fprintf(outfile, "\n");
}


ExprStmt::ExprStmt(Expr* init_expr) :
  Stmt(STMT_EXPR),
  expr(init_expr) 
{}


Stmt* ExprStmt::copy(void) {
  return new ExprStmt(expr->copy());
}


bool ExprStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == expr);
}


void ExprStmt::traverseStmt(Traversal* traversal) {
  expr->traverse(traversal, false);
}


void ExprStmt::print(FILE* outfile) {
  expr->print(outfile);
  fprintf(outfile, ";");
}


void ExprStmt::codegen(FILE* outfile) {
  expr->codegen(outfile);
  fprintf(outfile, ";");
}


ReturnStmt::ReturnStmt(Expr* retExpr) :
  ExprStmt(retExpr)
{
  astType = STMT_RETURN;
}


Stmt* ReturnStmt::copy(void) {
  return new ReturnStmt(expr->copy());
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
  body(init_body)
{}


Stmt* BlockStmt::copy(void) {
  return new BlockStmt(body->copyList());
}


void BlockStmt::traverseStmt(Traversal* traversal) {
  body->traverseList(traversal, false);
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
}


Stmt* WhileLoopStmt::copy(void) {
  return new WhileLoopStmt(isWhileDo, condition->copy(), body->copy());
}


bool WhileLoopStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == condition);
}


void WhileLoopStmt::traverseStmt(Traversal* traversal) {
  if (isWhileDo) {
    condition->traverse(traversal, false);
    body->traverse(traversal, false);
  } else {
    body->traverse(traversal, false);
    condition->traverse(traversal, false);
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
    domain(init_domain) 
{
  astType = STMT_FORLOOP;
}


Stmt* ForLoopStmt::copy(void) {
  return new ForLoopStmt(forall, dynamic_cast<VarSymbol*>(index->copy()), 
			 domain->copy(), body->copy());
}


bool ForLoopStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == domain);
}


void ForLoopStmt::traverseStmt(Traversal* traversal) {
  index->traverse(traversal, false);
  domain->traverse(traversal, false);
  body->traverse(traversal, false);
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
{}


Stmt* CondStmt::copy(void) {
  return new CondStmt(condExpr->copy(), thenStmt->copy(), elseStmt->copy());
}


bool CondStmt::topLevelExpr(Expr* testExpr) {
  return (testExpr == condExpr);
}


void CondStmt::traverseStmt(Traversal* traversal) {
  condExpr->traverse(traversal, false);
  thenStmt->traverse(traversal, false);
  elseStmt->traverse(traversal, false);
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
