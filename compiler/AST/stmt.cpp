#include <string.h>
#include <typeinfo>
#include "codegen.h"
#include "expr.h"
#include "stmt.h"
#include "yy.h"


void Stmt::codegenVarDefs(FILE* outfile) {
  Stmt* nextStmt = this;

  do {
    nextStmt->codegenVarDef(outfile);
    nextStmt = nextLink(Stmt, nextStmt);
  } while (nextStmt != NULL);
}


void Stmt::codegenVarDef(FILE* outfile) {
}


bool NullStmt::isNull(void) {
  return true;
}


void NullStmt::print(FILE* outfile) {
  fprintf(outfile, "/* NullStmt!! */\n");
}

void NullStmt::codegen(FILE* outfile) {
  fprintf(outfile, "This is NullStmt's codegen method.\n");
}


VarDefStmt::VarDefStmt(VarSymbol* init_var, Expr* init_init) :
  var(init_var),
  init(init_init) 
{}


void VarDefStmt::print(FILE* outfile) {
  VarSymbol* aVar = var;

  while (aVar != NULL) {
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
    aVar->printWithType(outfile);
    if (init != NULL && !init->isNull()) {
      fprintf(outfile, " = ");
      if (init->next) {
	fprintf(outfile, "(");
	init->printList(outfile);
	fprintf(outfile, ")");
      } else {
	init->print(outfile);
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
  
  while (aVar != NULL) {
    VarSymbol* nextVar = nextLink(VarSymbol, aVar);
    if (init != NULL && !init->isNull()) {
      if (typeid(*(aVar->type)) == typeid(DomainType)) {
	DomainType* domtype = (DomainType*)aVar->type;
	int rank = domtype->numdims ? domtype->numdims : 1; // BLC: hack!
	fprintf(outfile, "_init_domain_%dD(&(", rank);
	aVar->codegen(outfile);
	fprintf(outfile, ")");
	if (typeid(*init) == typeid(DomainExpr)) {
	  init = ((DomainExpr*)init)->domains;
	}
	if (typeid(*init) == typeid(SimpleSeqExpr)) {
	  SimpleSeqExpr* initseq = (SimpleSeqExpr*)init;

	  for (int i=0; i<rank; i++) {
	    fprintf(outfile, ", ");
	    initseq->lo->codegen(outfile);
	    fprintf(outfile, ", ");
	    initseq->hi->codegen(outfile);
	    fprintf(outfile, ", ");
	    initseq->str->codegen(outfile);
	  }
	}
	fprintf(outfile, ");");
      } else {
	aVar->codegen(outfile);
	fprintf(outfile, " = ");
	init->codegen(outfile);
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

  if (aVar->level == 1) { /* if in file scope, hoist to internal header */
    outfile = intheadfile;
  }

  // TODO: for certain cases, we could use a C initializer inline rather
  // than separating the variable declaration from its initialization.

  while (aVar != NULL) {
    if (aVar->isConst) {
      fprintf(outfile, "const ");
    }

    aVar->codegenDef(outfile);

    fprintf(outfile, ";\n");
    aVar = nextLink(VarSymbol, aVar);
  }
}


TypeDefStmt::TypeDefStmt(Type* init_type) :
  type(init_type)
{}


void TypeDefStmt::print(FILE* outfile) {
  type->printDef(outfile);
  fprintf(outfile, ";");
}

void TypeDefStmt::codegen(FILE* outfile) {
  FILE* deffile = outfile;
  /* if in file scope, hoist to internal header so that it will be
     defined before global variables at file scope. */  
  if (type->name->level == 1) { 
    deffile = intheadfile;
  }
  type->codegenDef(deffile);

  type->codegenIORoutines(outfile);
}


FnDefStmt::FnDefStmt(FnSymbol* init_fn) :
  fn(init_fn)
{}


void FnDefStmt::print(FILE* outfile) {
  fprintf(outfile, "function ");
  fn->print(outfile);
  fprintf(outfile, "(");
  fn->formals->printDefList(outfile, ";\n");
  fprintf(outfile, ")");
  if (fn->type->isNull()) {
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


ExprStmt::ExprStmt(Expr* init_expr) :
  expr(init_expr) 
{}


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
{}


void ReturnStmt::print(FILE* outfile) {
  fprintf(outfile, "\n");
  fprintf(outfile, "return");
  if (!expr->isNull()) {
    fprintf(outfile, " ");
    expr->print(outfile);
  }
  fprintf(outfile, ";");
}


BlockStmt::BlockStmt(Stmt* init_body) :
  body(init_body)
{}


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


WhileLoopStmt::WhileLoopStmt(whileLoopType init_type, 
			     Expr* init_cond, 
			     Stmt* init_body) 
  : BlockStmt(init_body), 
    type(init_type), 
    condition(init_cond) 
{}


void WhileLoopStmt::print(FILE* outfile) {
  switch (type) {
  case LOOP_WHILEDO:
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ") ");
    break;
  case LOOP_DOWHILE:
    fprintf(outfile, "do ");
    break;
  case LOOP_REPEAT:
    fprintf(outfile, "repeat ");
    break;
  }
  
  body->print(outfile);

  switch (type) {
  case LOOP_WHILEDO:
    break;
  case LOOP_DOWHILE:
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ")");
    break;
  case LOOP_REPEAT:
    fprintf(outfile, "until (");
    condition->print(outfile);
    fprintf(outfile, ")");
    break;
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
{}


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
  while (aVar != NULL) {
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
  condExpr(init_condExpr),
  thenStmt(init_thenStmt),
  elseStmt(init_elseStmt)
{}


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
  fprintf(outfile, "This is CondStmt's codegen method.\n");
}
