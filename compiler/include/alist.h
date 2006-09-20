#ifndef _CHPL_LIST_H_
#define _CHPL_LIST_H_

// Lists for use with Stmts and Exprs (only)

#include <stdio.h>
#include "chpl.h"
#include "baseAST.h"

class AList {
 public:
  BaseAST* head;
  BaseAST* tail;
  BaseAST* parent;

  // constructors
  AList();
  AList(BaseAST*);
  void clear(void);

  // copy routines
  virtual AList* copy(ASTMap* map = NULL, bool internal = false);

  // checks for length
  bool isEmpty(void);
  int length(void);

  // iteration
  BaseAST* first(void); // begin iteration over a list
  BaseAST* last(void);  // begin backwards iteration over a list

  // other ways to get elements from the list
  BaseAST* only(void); // return the single element in a list
  BaseAST* get(int index); // get the index-th element in a list

  // add element(s) at beginning of list
  void insertAtHead(BaseAST* new_ast);
  void insertAtHead(AList* new_ast);

  // add element(s) at end of list
  void insertAtTail(BaseAST* new_ast);
  void insertAtTail(AList* new_ast);

  // different ways to print/codegen lists
  void print(FILE* outfile, char* separator = ", ");
  void printDef(FILE* outfile, char* separator = ", ");
  void codegen(FILE* outfile, char* separator = ", ");
};

#define for_alist(elemtype, node, list)                                 \
  for (elemtype *node = (list) ? ((list)->head ? dynamic_cast<elemtype*>((list)->head->next) : NULL) : NULL, \
         *_alist_next = (node) ? dynamic_cast<elemtype*>((node)->next) : NULL; \
       node;                                                            \
       node = _alist_next,                                              \
         _alist_next = (node) ? dynamic_cast<elemtype*>((node)->next) : NULL)

#define for_alist_sc(elemtype, node, list)                              \
  for (elemtype *node = (list) ? ((list)->head ? (elemtype*)((list)->head->next) : NULL) : NULL, \
         *_alist_next = node ? (elemtype*)(node->next) : NULL;          \
       _alist_next;                                                     \
       node = _alist_next,                                              \
         _alist_next = node ? (elemtype*)(node->next) : NULL)

#define for_formals(formal, fn)                                         \
  for (ArgSymbol *formal = (fn->formals->head->next->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>(((fn)->formals)->head->next)->sym) : NULL, \
         *_alist_next = (formal && formal->defPoint->next->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_next,                                            \
         _alist_next = (formal && formal->defPoint->next->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL)

#define for_formals_backward(formal, fn)                                \
  for (ArgSymbol *formal = (fn->formals->tail->prev->prev) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>(((fn)->formals)->tail->prev)->sym) : NULL, \
         *_alist_prev = (formal && formal->defPoint->prev->prev) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->prev)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_prev,                                            \
         _alist_prev = (formal && formal->defPoint->prev->prev) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->prev)->sym) : NULL)

#define for_formals_actuals(formal, actual, call)                       \
  FnSymbol* _alist_fn = (call)->isResolved();                           \
  if (_alist_fn->formals->length() != (call)->argList->length())        \
    INT_FATAL(call, "number of actuals does not match number of formals"); \
  int _alist_i = 1;                                                     \
  Expr* actual = ((call)->argList->length() >= _alist_i) ? (call)->get(_alist_i) : NULL; \
  for (ArgSymbol *formal = (_alist_fn->formals->head->next->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((_alist_fn->formals)->head->next)->sym) : NULL, \
         *_alist_next = (formal && formal->defPoint->next->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_next,                                            \
         _alist_next = (formal && formal->defPoint->next->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL, \
         _alist_i++, actual = formal ? (call)->get(_alist_i) : NULL)

#define for_actuals(actual, call)               \
  for_alist(Expr, actual, (call)->argList)

#define for_actuals_backward(actual, call)               \
  for_alist_backward(Expr, actual, (call)->argList)

#define for_fields(field, ct)                                           \
  for (Symbol *field = (ct->fields->head->next->next) ? dynamic_cast<DefExpr*>(((ct)->fields)->head->next)->sym : NULL, \
         *_alist_next = (field && field->defPoint->next->next) ? dynamic_cast<DefExpr*>(field->defPoint->next)->sym : NULL; \
       (field);                                                         \
       field = _alist_next,                                             \
         _alist_next = (field && field->defPoint->next->next) ? dynamic_cast<DefExpr*>(field->defPoint->next)->sym : NULL)

#define for_fields_backward(field, ct)                                  \
  for (Symbol *field = (ct->fields->tail->prev->prev) ? dynamic_cast<DefExpr*>(((ct)->fields)->tail->prev)->sym : NULL, \
         *_alist_prev = (field && field->defPoint->prev->prev) ? dynamic_cast<DefExpr*>(field->defPoint->prev)->sym : NULL; \
       (field);                                                         \
       field = _alist_prev,                                             \
         _alist_prev = (field && field->defPoint->prev->prev) ? dynamic_cast<DefExpr*>(field->defPoint->prev)->sym : NULL)

#define for_alist_backward(elemtype, node, list)                        \
  for (elemtype *node = (list) ? ((list)->tail ? dynamic_cast<elemtype*>((list)->tail->prev) : NULL) : NULL, \
         *_alist_prev = (node) ? dynamic_cast<elemtype*>((node)->prev) : NULL; \
       node;                                                            \
       node = _alist_prev,                                              \
         _alist_prev = (node) ? dynamic_cast<elemtype*>((node)->prev) : NULL)

#endif
