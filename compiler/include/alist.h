#ifndef _CHPL_LIST_H_
#define _CHPL_LIST_H_

// Lists for use with Stmts and Exprs (only)

#include <stdio.h>
#include "chpl.h"
#include "baseAST.h"

extern BaseAST* ast_wrap(BaseAST* context, BaseAST* ast);
extern void update_symbols(BaseAST* ast, ASTMap* map);
extern void sibling_insert_help(BaseAST* parent, BaseAST* ast);

template <class elemType>
class AList : public gc {
 public:
  elemType* head;
  elemType* tail;

  // constructors
  AList();
  AList(BaseAST*);
  AList(BaseAST*, BaseAST*);
  AList(BaseAST*, BaseAST*, BaseAST*);
  AList(BaseAST*, BaseAST*, BaseAST*, BaseAST*);
  void clear(void);

  // copy routines
  virtual AList<elemType>* copy(ASTMap* map = NULL, bool internal = false);

  // checks for length
  bool isEmpty(void);
  int length(void);

  // iteration
  elemType* first(void); // begin iteration over a list
  elemType* last(void);  // begin backwards iteration over a list

  // other ways to get elements from the list
  elemType* only(void); // return the single element in a list
  elemType* get(int index); // get the index-th element in a list

  // add element(s) at beginning of list
  void insertAtHead(BaseAST* new_ast);
  void insertAtHead(AList<elemType>* new_ast);

  // add element(s) at end of list
  void insertAtTail(BaseAST* new_ast);
  void insertAtTail(AList<elemType>* new_ast);

  // different ways to print/codegen lists
  void print(FILE* outfile, char* separator = ", ");
  void printDef(FILE* outfile, char* separator = ", ");
  void codegen(FILE* outfile, char* separator = ", ");
  void codegenDef(FILE* outfile, char* separator = ", ");
};

#define for_alist(elemtype, node, list)                                 \
  for (elemtype *node = (list) ? ((list)->head ? dynamic_cast<elemtype*>((list)->head->next) : NULL) : NULL, \
         *_alist_next = (node) ? dynamic_cast<elemtype*>((node)->next) : NULL; \
       _alist_next;                                                     \
       node = _alist_next,                                              \
         _alist_next = (node) ? dynamic_cast<elemtype*>((node)->next) : NULL)

#define for_alist_sc(elemtype, node, list)                              \
  for (elemtype *node = (list)->head ? (elemtype*)((list)->head->next) : NULL, \
         *_alist_next = node ? (elemtype*)(node->next) : NULL;          \
       _alist_next;                                                     \
       node = _alist_next,                                              \
         _alist_next = node ? (elemtype*)(node->next) : NULL)

#define for_formals(formal, fn)                                         \
  for (ArgSymbol *formal = ((fn)->formals) ? (((fn)->formals)->head ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>(((fn)->formals)->head->next)->sym) : NULL) : NULL, \
         *_alist_next = (formal) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_next,                                            \
         _alist_next = (formal) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL)

#define for_formals_backward(formal, fn)                                \
  for (ArgSymbol *formal = ((fn)->formals) ? (((fn)->formals)->tail ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>(((fn)->formals)->tail->prev)->sym) : NULL) : NULL, \
         *_alist_prev = (formal) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->prev)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_prev,                                            \
         _alist_prev = (formal) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->prev)->sym) : NULL)

#define for_formals_actuals(formal, actual, call)                       \
  FnSymbol* _alist_fn = (call)->isResolved();                             \
  if (_alist_fn->formals->length() != (call)->argList->length())        \
    INT_FATAL(call, "number of actuals does not match number of formals"); \
  int _alist_i = 1;                                                       \
  Expr* actual = ((call)->argList->length() >= _alist_i) ? (call)->get(_alist_i) : NULL; \
  for (ArgSymbol *formal = ((_alist_fn)->formals) ? (((_alist_fn)->formals)->head ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>(((_alist_fn)->formals)->head->next)->sym) : NULL) : NULL, \
         *_alist_next = (formal) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_next,                                            \
         _alist_next = (formal) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL, \
         _alist_i++, actual = formal ? (call)->get(_alist_i) : NULL)

#define for_actuals(actual, call)               \
  for_alist(Expr, actual, (call)->argList)

#define for_actuals_backward(actual, call)               \
  for_alist_backward(Expr, actual, (call)->argList)

#define for_fields(field, ct)                                           \
  for (Symbol *field = ((ct)->fields) ? (((ct)->fields)->head ? dynamic_cast<DefExpr*>(((ct)->fields)->head->next)->sym : NULL) : NULL, \
         *_alist_next = (field) ? dynamic_cast<DefExpr*>((field)->defPoint->next)->sym : NULL; \
       (field);                                                         \
       field = _alist_next,                                             \
         _alist_next = (field) ? dynamic_cast<DefExpr*>((field)->defPoint->next)->sym : NULL)

#define for_fields_backward(field, ct)                                  \
  for (Symbol *field = ((ct)->fields) ? (((ct)->fields)->tail ? dynamic_cast<DefExpr*>(((ct)->fields)->tail->prev)->sym : NULL) : NULL, \
         *_alist_prev = (field) ? dynamic_cast<DefExpr*>((field)->defPoint->prev)->sym : NULL; \
       (field);                                                         \
       field = _alist_prev,                                             \
         _alist_prev = (field) ? dynamic_cast<DefExpr*>((field)->defPoint->prev)->sym : NULL)

#define for_alist_backward(elemtype, node, list)  \
  for (elemtype *node = (list) ? ((list)->tail ? dynamic_cast<elemtype*>((list)->tail->prev) : NULL) : NULL,      \
         *_alist_prev = (node) ? dynamic_cast<elemtype*>((node)->prev) : NULL;                  \
       _alist_prev;                                                                             \
       node = _alist_prev,                                                                      \
         _alist_prev = (node) ? dynamic_cast<elemtype*>((node)->prev) : NULL)


template <class elemType>
AList<elemType>::AList() :
  head(new elemType()),
  tail(new elemType())
{
  clear();
}


template <class elemType>
AList<elemType>::AList(BaseAST* elem1) :
  head(new elemType()),
  tail(new elemType())
{
  clear();
  if (elem1) insertAtTail(elem1);
}


template <class elemType>
AList<elemType>::AList(BaseAST* elem1, BaseAST* elem2) :
  head(new elemType()),
  tail(new elemType())
{
  clear();
  if (elem1) insertAtTail(elem1);
  if (elem2) insertAtTail(elem2);
}


template <class elemType>
AList<elemType>::AList(BaseAST* elem1, BaseAST* elem2, BaseAST* elem3) :
  head(new elemType()),
  tail(new elemType())
{
  clear();
  if (elem1) insertAtTail(elem1);
  if (elem2) insertAtTail(elem2);
  if (elem3) insertAtTail(elem3);
}


template <class elemType>
AList<elemType>::AList(BaseAST* elem1, BaseAST* elem2,
                       BaseAST* elem3, BaseAST* elem4) :
  head(new elemType()),
  tail(new elemType())
{
  clear();
  if (elem1) insertAtTail(elem1);
  if (elem2) insertAtTail(elem2);
  if (elem3) insertAtTail(elem3);
  if (elem4) insertAtTail(elem4);
}


template <class elemType>
void AList<elemType>::clear(void) {
  head->next = tail;
  tail->prev = head;
}


template <class elemType>
bool AList<elemType>::isEmpty(void) {
  if (this == NULL) {
    return true;
  } else {
    return (head->next == tail);
  }
}


template <class elemType>
int AList<elemType>::length(void) {
  int numNodes = 0;
  for_alist(elemType, node, this) {
    numNodes++;
  }
  return numNodes;
}


template <class elemType>
elemType* AList<elemType>::first(void) {
  if (this == NULL || isEmpty())
    return NULL;
  return dynamic_cast<elemType*>(head->next);
}


template <class elemType>
elemType* AList<elemType>::last(void) {
  if (this == NULL || isEmpty())
    return NULL;
  return dynamic_cast<elemType*>(tail->prev);
}


template <class elemType>
elemType* AList<elemType>::only(void) {
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


template <class elemType>
elemType* AList<elemType>::get(int index) {
  if (index <=0) {
    INT_FATAL("Indexing list must use positive integer");
  }
  int i = 0;
  for_alist(elemType, node, this) {
    i++;
    if (i == index) {
      return node;
    }
  }
  INT_FATAL("Indexing list out of bounds");
  return NULL;
}


template <class elemType>
void AList<elemType>::insertAtHead(BaseAST* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtHead");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtHead");
  new_ast = ast_wrap(head, new_ast);
  new_ast->prev = head;
  new_ast->next = head->next;
  head->next = new_ast;
  new_ast->next->prev = new_ast;
  sibling_insert_help(head, new_ast);
}


template <class elemType>
void AList<elemType>::insertAtHead(AList<elemType>* new_ast) {
  for_alist_backward(elemType, elem, new_ast) {
    elem->remove();
    insertAtHead(elem);
  }
}


template <class elemType>
void AList<elemType>::insertAtTail(BaseAST* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtTail");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtTail");
  new_ast = ast_wrap(head, new_ast);
  new_ast->next = tail;
  new_ast->prev = tail->prev;
  tail->prev = new_ast;
  new_ast->prev->next = new_ast;
  sibling_insert_help(tail, new_ast);
}


template <class elemType>
void AList<elemType>::insertAtTail(AList<elemType>* new_ast) {
  for_alist(elemType, elem, new_ast) {
    elem->remove();
    insertAtTail(elem);
  }
}


template <class elemType>
void AList<elemType>::print(FILE* outfile, char* separator) {
  for_alist(elemType, node, this) {
    node->print(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


template <class elemType>
void AList<elemType>::printDef(FILE* outfile, char* separator) {
  for_alist(elemType, node, this) {
    node->printDef(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


template <class elemType>
void AList<elemType>::codegen(FILE* outfile, char* separator) {
  for_alist(elemType, node, this) {
    node->codegen(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}



template <class elemType>
void AList<elemType>::codegenDef(FILE* outfile, char* separator) {
  for_alist(elemType, node, this) {
    node->codegenDef(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


template <class elemType>
AList<elemType>* AList<elemType>::copy(ASTMap* map, bool internal) {
  if (isEmpty()) {
    return new AList<elemType>();
  }

  ASTMap localMap;
  if (!map)
    map = &localMap;

  AList<elemType>* newList = new AList<elemType>();
  for_alist(elemType, node, this) {
    elemType* newnode = COPY_INT(node);
    newnode->next = NULL;
    newnode->prev = NULL;
    newList->insertAtTail(newnode);
  }

  if (!internal) {
    for_alist(elemType, node, newList)
      update_symbols(node, map);
  }
  return newList;
}

#endif
