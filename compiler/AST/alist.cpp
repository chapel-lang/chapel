#include "alist.h"
#include "astutil.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"


AList::AList() :
  head(NULL),
  tail(NULL),
  parent(NULL)
{ }


bool AList::isEmpty(void) {
  return head == NULL;
}


int AList::length(void) {
  int numNodes = 0;
  for_alist(node, *this) {
    numNodes++;
  }
  return numNodes;
}


Expr* AList::first(void) {
  return head;
}


Expr* AList::last(void) {
  return tail;
}


Expr* AList::only(void) {
  if (!head)
    INT_FATAL("only() called on empty list");
  if (head == tail)
    return first();
  else
    INT_FATAL("only() called on list with more than one element");
  return NULL;
}


Expr* AList::get(int index) {
  if (index <=0) {
    INT_FATAL("Indexing list must use positive integer");
  }
  int i = 0;
  for_alist(node, *this) {
    i++;
    if (i == index) {
      return node;
    }
  }
  INT_FATAL("Indexing list out of bounds");
  return NULL;
}


void AList::insertAtHead(Expr* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtHead");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtHead");
  if (CallExpr* call = toCallExpr(new_ast)) {
    if (call && call->isPrimitive(PRIMITIVE_ACTUALS_LIST)) {
      for_actuals(expr, call) {
        expr->remove();
        insertAtHead(expr);
      }
      return;
    }
  }
  if (!head) {
    head = new_ast;
    tail = new_ast;
    new_ast->list = this;
  } else {
    new_ast->next = head;
    head = new_ast;
    new_ast->next->prev = new_ast;
    new_ast->list = this;
  }
  parent_insert_help(parent, new_ast);
}


void AList::insertAtTail(Expr* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtTail");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtTail");
  if (CallExpr* call = toCallExpr(new_ast)) {
    if (call && call->isPrimitive(PRIMITIVE_ACTUALS_LIST)) {
      for_actuals(expr, call) {
        expr->remove();
        insertAtTail(expr);
      }
      return;
    }
  }
  if (!tail) {
    head = new_ast;
    tail = new_ast;
    new_ast->list = this;
  } else {
    new_ast->prev = tail;
    tail = new_ast;
    new_ast->prev->next = new_ast;
    new_ast->list = this;
  }
  parent_insert_help(parent, new_ast);
}


void AList::insertAtTailBeforeGoto(Expr* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtTail");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtTail");
  if (CallExpr* call = toCallExpr(new_ast)) {
    if (call && call->isPrimitive(PRIMITIVE_ACTUALS_LIST)) {
      for_actuals(expr, call) {
        expr->remove();
        insertAtTail(expr);
      }
      return;
    }
  }
  if (!tail) {
    head = new_ast;
    tail = new_ast;
  } else if (tail->astTag == STMT_GOTO) {
    new_ast->prev = tail->prev;
    new_ast->next = tail;
    tail->prev = new_ast;
    if (head == tail)
      head = new_ast;
    else
      new_ast->prev->next = new_ast;
  } else {
    new_ast->prev = tail;
    tail = new_ast;
    new_ast->prev->next = new_ast;
  }
  new_ast->list = this;
  parent_insert_help(parent, new_ast);
}


void AList::codegen(FILE* outfile, const char* separator) {
  for_alist(node, *this) {
    node->codegen(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}
