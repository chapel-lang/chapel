#include <typeinfo>
#include "view.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "log.h"

View::View(bool initNumberSymbols) {
  indent = 0;
  numberSymbols = initNumberSymbols;
}

void View::run(Vec<ModuleSymbol*>* modules) {
  forv_Vec(ModuleSymbol, mod, *modules) {
    printf("\nMODULE: %s\n", mod->name);
    mod->startTraversal(this);
  }
  printf("\n\n");
}

void View::preProcessStmt(Stmt* stmt) {
  printf("\n");
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
  printf("(%s", astTypeName[stmt->astType]);
  indent += 2;
}

void View::postProcessStmt(Stmt* stmt) {
  printf(")");
  indent -= 2;
}

void View::preProcessExpr(Expr* expr) {
  printf("\n");
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
  printf("(");
  printf("%s", astTypeName[expr->astType]);
  if (IntLiteral* e = dynamic_cast<IntLiteral*>(expr)) {
    printf(" %ld", e->val);
  } else if (StringLiteral* e = dynamic_cast<StringLiteral*>(expr)) {
    printf(" \"%s\"", e->str);
  }
  indent += 2;
}

void View::postProcessExpr(Expr* expr) {
  printf(")");
  indent -= 2;
}

void View::preProcessSymbol(Symbol* sym) {
  printf(" '");
  if (dynamic_cast<FnSymbol*>(sym)) {
    printf("fn ");
  } else if (dynamic_cast<ParamSymbol*>(sym)) {
    printf("arg ");
  } else if (dynamic_cast<TypeSymbol*>(sym)) {
    printf("type ");
  }
  printf("%s", sym->name);
  if (numberSymbols) {
    printf("[%ld]", sym->id);
  }
  if (sym->type && sym->type->symbol) {
    printf(":%s", sym->type->symbol->name);
    if (numberSymbols) {
      printf("[%ld]", sym->type->symbol->id);
    }
  }
  printf("'");
}

void View::postProcessSymbol(Symbol* sym) {

}

void View::preProcessType(Type* type) {

}

void View::postProcessType(Type* type) {

}

void print_view(BaseAST* ast) {
  TRAVERSE(ast, new View(), true);
  printf("\n\n");
  fflush(stdout);
}

void nprint_view(BaseAST* ast) {
  TRAVERSE(ast, new View(true), true);
  printf("\n\n");
  fflush(stdout);
}
