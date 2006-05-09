#include "view.h"
#include "astutil.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "chpl_log.h"

void
view_ast(BaseAST* ast, bool number = false, long mark = -1, int indent = 0) {
  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    printf("\n");
    for (int i = 0; i < indent; i++)
      printf(" ");
    printf("(");
    if (ast->id == mark)
      printf("***");
    if (number)
      printf("%ld ", ast->id);
    printf("%s", astTypeName[expr->astType]);

    if (CallExpr* call = dynamic_cast<CallExpr*>(expr))
      if (call->isPrimitive())
        printf(" %s", call->primitive->name);

    if (NamedExpr* named = dynamic_cast<NamedExpr*>(expr))
      printf(" \"%s\"", named->name);

    if (dynamic_cast<DefExpr*>(expr))
      printf(" ");

    long i;
    char *str;
    if (get_int(expr, &i)) {
      printf(" %ld", i);
    } else if (get_string(expr, &str)) {
      printf(" \"%s\"", str);
    }

    if (SymExpr* sym = dynamic_cast<SymExpr*>(expr)) {
      printf(" '");
      if (sym->var->id == mark)
        printf("***");
      if (dynamic_cast<FnSymbol*>(sym->var)) {
        printf("fn ");
      } else if (dynamic_cast<ArgSymbol*>(sym->var)) {
        printf("arg ");
      } else if (dynamic_cast<TypeSymbol*>(sym->var)) {
        printf("type ");
      }
      printf("%s", sym->var->name);
      if (number)
        printf("[%ld]", sym->var->id);
      if (sym->var->type && sym->var->type->symbol) {
        printf(":%s", sym->var->type->symbol->name);
        if (number)
          printf("[%ld]", sym->var->type->symbol->id);
      }

      printf("'");
    }
  }

  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    printf("\n");
    for (int i = 0; i < indent; i++)
      printf(" ");
    printf("(");
    if (ast->id == mark)
      printf("***");
    if (number)
      printf("%ld ", ast->id);
    printf("%s", astTypeName[stmt->astType]);
  }

  if (Symbol* sym = dynamic_cast<Symbol*>(ast)) {
    printf("'");
    if (ast->id == mark)
      printf("***");
    if (dynamic_cast<FnSymbol*>(sym)) {
      printf("fn ");
    } else if (dynamic_cast<ArgSymbol*>(sym)) {
      printf("arg ");
    } else if (dynamic_cast<TypeSymbol*>(sym)) {
      printf("type ");
    }
    printf("%s", sym->name);
    if (number)
      printf("[%ld]", sym->id);
    if (sym->type && sym->type->symbol) {
      printf(":%s", sym->type->symbol->name);
      if (number)
        printf("[%ld]", sym->type->symbol->id);
    }
    printf("'");
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    view_ast(ast, number, mark, indent + 2);

  if (dynamic_cast<Expr*>(ast) || dynamic_cast<Stmt*>(ast)) {
    printf(")");
  }
}

void print_view(BaseAST* ast) {
  view_ast(ast);
  printf("\n\n");
  fflush(stdout);
}

void print_view_noline(BaseAST* ast) {
  view_ast(ast);
  fflush(stdout);
}

void nprint_view(BaseAST* ast) {
  view_ast(ast, true);
  printf("\n\n");
  fflush(stdout);
}

void nprint_view_noline(BaseAST* ast) {
  view_ast(ast, true);
  fflush(stdout);
}

void mark_view(BaseAST* ast, long id) {
  view_ast(ast, false, id);
  printf("\n\n");
  fflush(stdout);
}
