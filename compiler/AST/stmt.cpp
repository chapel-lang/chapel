#include <string.h>
#include "expr.h"
#include "stmt.h"
#include "yy.h"


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

    aVar = (VarSymbol*)(aVar->next);
    if (aVar) {
      fprintf(outfile, "\n");
    }
  }
}

void VarDefStmt::codegen(FILE* outfile) {

  VarSymbol* aVar = var;
  
  while (aVar != NULL) {
    if (aVar->isConst) {
      fprintf(outfile, "const ");
    }
    
    if (aVar->type != NULL) {
      if (strcmp(aVar->type->name->name, "integer") == 0) {
	fprintf(outfile, "int ");
      } 
      else if (strcmp(aVar->type->name->name, "float") == 0) {
	fprintf(outfile, "float ");
      }

    }

    if (strcmp(aVar->type->name->name, "???") == 0) {
      fprintf(outfile, "int ");
    }

    aVar->codegen(outfile);

    if (init != NULL && !init->isNull()) {
      fprintf(outfile, " = ");
      init->codegen(outfile);
      fprintf(outfile, ";");
    }

    aVar = (VarSymbol*)(aVar->next);
    if (aVar) {
      fprintf(outfile, "\n");
    }
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
  fprintf(outfile, "This is TypeDefStmt's codegen method.\n");
}


FnDefStmt::FnDefStmt(FunSymbol* init_fun) :
  fun(init_fun)
{}


void FnDefStmt::print(FILE* outfile) {
  fprintf(outfile, "function ");
  fun->print(outfile);
  fprintf(outfile, "(");
  fun->formals->printDefList(outfile, ";\n");
  fprintf(outfile, ")");
  if (fun->retType->isNull()) {
    fprintf(outfile, " ");
  } else {
    fprintf(outfile, ": ");
    fun->retType->print(outfile);
  }
  fun->body->print(outfile);
  fprintf(outfile, "\n\n");
}


void FnDefStmt::codegen(FILE* outfile) {
  fprintf(outfile, "This is FnDefStmt's codegen method.\n");
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
  fprintf(outfile, "This is BlockStmt's codegen method.\n");
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
