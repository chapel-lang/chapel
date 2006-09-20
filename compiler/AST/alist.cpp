#include "alist.h"
#include "astutil.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"


AList::AList() :
  head(new BaseAST()),
  tail(new BaseAST()),
  parent(NULL)
{
  clear();
}


AList::AList(BaseAST* elem) :
  head(new BaseAST()),
  tail(new BaseAST()),
  parent(NULL)
{
  clear();
  if (elem) insertAtTail(elem);
}


void AList::clear(void) {
  head->next = tail;
  tail->prev = head;
  head->parentSymbol = (Symbol*)0x1;
  tail->parentSymbol = (Symbol*)0x1;
}


bool AList::isEmpty(void) {
  if (this == NULL) {
    return true;
  } else {
    return (head->next == tail);
  }
}


int AList::length(void) {
  int numNodes = 0;
  for_alist_sc(BaseAST, node, this) {
    numNodes++;
  }
  return numNodes;
}


BaseAST* AList::first(void) {
  if (this == NULL || isEmpty())
    return NULL;
  return dynamic_cast<BaseAST*>(head->next);
}


BaseAST* AList::last(void) {
  if (this == NULL || isEmpty())
    return NULL;
  return dynamic_cast<BaseAST*>(tail->prev);
}


BaseAST* AList::only(void) {
  if (isEmpty()) {
    INT_FATAL("only() called on empty list");
  }
  if (head->next->next == tail) {
    return first();
  } else {
    INT_FATAL("only() called on list with more than one element");
    return NULL;
  }
}


BaseAST* AList::get(int index) {
  if (index <=0) {
    INT_FATAL("Indexing list must use positive integer");
  }
  int i = 0;
  for_alist_sc(BaseAST, node, this) {
    i++;
    if (i == index) {
      return node;
    }
  }
  INT_FATAL("Indexing list out of bounds");
  return NULL;
}


void AList::insertAtHead(BaseAST* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtHead");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtHead");
  if (dynamic_cast<Symbol*>(new_ast))
    INT_FATAL(new_ast, "Argument is a symbol in AList::insertAtHead");
  new_ast->prev = head;
  new_ast->next = head->next;
  head->next = new_ast;
  new_ast->next->prev = new_ast;
  parent_insert_help(parent, new_ast);
}


void AList::insertAtHead(AList* new_ast) {
  for_alist_backward(BaseAST, elem, new_ast) {
    elem->remove();
    insertAtHead(elem);
  }
}


void AList::insertAtTail(BaseAST* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtTail");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtTail");
  if (dynamic_cast<Symbol*>(new_ast))
    INT_FATAL(new_ast, "Argument is a symbol in AList::insertAtTail");
  new_ast->next = tail;
  new_ast->prev = tail->prev;
  tail->prev = new_ast;
  new_ast->prev->next = new_ast;
  parent_insert_help(parent, new_ast);
}


void AList::insertAtTail(AList* new_ast) {
  for_alist_sc(BaseAST, elem, new_ast) {
    elem->remove();
    insertAtTail(elem);
  }
}


void AList::print(FILE* outfile, char* separator) {
  for_alist_sc(BaseAST, node, this) {
    node->print(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


void AList::printDef(FILE* outfile, char* separator) {
  for_alist_sc(BaseAST, node, this) {
    node->printDef(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


void AList::codegen(FILE* outfile, char* separator) {
  for_alist_sc(BaseAST, node, this) {
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
  for_alist_sc(BaseAST, node, this) {
    BaseAST* newnode = COPY_INT(node);
    newnode->next = NULL;
    newnode->prev = NULL;
    newList->insertAtTail(newnode);
  }

  if (!internal) {
    for_alist_sc(BaseAST, node, newList)
      update_symbols(node, map);
  }
  return newList;
}
