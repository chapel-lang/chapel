#include "alist.h"
#include "astutil.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"


AList::AList(Expr* elem) :
  head(NULL),
  tail(NULL),
  parent(NULL)
{
  if (elem)
    insertAtTail(elem);
}


bool AList::isEmpty(void) {
  return head == NULL;
}


int AList::length(void) {
  int numNodes = 0;
  for_alist(node, this) {
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
  for_alist(node, this) {
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
  if (dynamic_cast<Symbol*>(new_ast))
    INT_FATAL(new_ast, "Argument is a symbol in AList::insertAtHead");
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


void AList::insertAtHead(AList* new_ast) {
  for_alist_backward(elem, new_ast) {
    elem->remove();
    insertAtHead(elem);
  }
}


void AList::insertAtTail(Expr* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtTail");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtTail");
  if (dynamic_cast<Symbol*>(new_ast))
    INT_FATAL(new_ast, "Argument is a symbol in AList::insertAtTail");
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


void AList::insertAtTail(AList* new_ast) {
  for_alist(elem, new_ast) {
    elem->remove();
    insertAtTail(elem);
  }
}


void AList::codegen(FILE* outfile, const char* separator) {
  for_alist(node, this) {
    node->codegen(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


AList* AList::copy(ASTMap* map, bool internal) {
  if (isEmpty()) {
    return new AList();
  }

  ASTMap localMap;
  if (!map)
    map = &localMap;

  AList* newList = new AList();
  for_alist(node, this) {
    Expr* newnode = COPY_INT(node);
    newnode->next = NULL;
    newnode->prev = NULL;
    newnode->list = NULL;
    newList->insertAtTail(newnode);
  }

  if (!internal) {
    for_alist(node, newList)
      update_symbols(node, map);
  }
  return newList;
}
