#ifndef _CHPL_LIST_H_
#define _CHPL_LIST_H_

// Lists for use with Stmts and Exprs (only)

#include <stdio.h>
#include "chpl.h"
#include "baseAST.h"

class AList {
 public:
  Expr* head;
  Expr* tail;
  BaseAST* parent;

  // constructors
  AList(Expr* elem = NULL);

  // copy routines
  AList* copy(ASTMap* map = NULL, bool internal = false);

  // checks for length
  bool isEmpty(void);
  int length(void);

  // iteration
  Expr* first(void); // begin iteration over a list
  Expr* last(void);  // begin backwards iteration over a list

  // other ways to get elements from the list
  Expr* only(void); // return the single element in a list
  Expr* get(int index); // get the index-th element in a list

  // add element(s) at beginning of list
  void insertAtHead(Expr* new_ast);
  void insertAtHead(AList* new_ast);

  // add element(s) at end of list
  void insertAtTail(Expr* new_ast);
  void insertAtTail(AList* new_ast);

  // codegen list
  void codegen(FILE* outfile, char* separator = ", ");
};

#define for_alist(elt_type, node, list)                                 \
  for (elt_type *node = dynamic_cast<elt_type*>((list)->head),          \
         *_alist_next = (node) ? dynamic_cast<elt_type*>((node)->next) : NULL; \
       node;                                                            \
       node = _alist_next,                                              \
         _alist_next = (node) ? dynamic_cast<elt_type*>((node)->next) : NULL)

#define for_asts(node, list)                        \
  for (Expr *node = (list)->head,                   \
         *_alist_next = (node) ? node->next : NULL; \
       node;                                        \
       node = _alist_next,                          \
         _alist_next = (node) ? node->next : NULL)

#define for_formals(formal, fn)                                         \
  for (ArgSymbol *formal = ((fn)->formals->head) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((fn)->formals->head)->sym) : NULL, \
         *_alist_next = (formal && formal->defPoint->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_next,                                            \
         _alist_next = (formal && formal->defPoint->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL)

#define for_formals_backward(formal, fn)                                \
  for (ArgSymbol *formal = ((fn)->formals->tail) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((fn)->formals->tail)->sym) : NULL, \
         *_alist_prev = (formal && formal->defPoint->prev) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->prev)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_prev,                                            \
         _alist_prev = (formal && formal->defPoint->prev) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->prev)->sym) : NULL)

#define for_formals_actuals(formal, actual, call)                       \
  FnSymbol* _alist_fn = (call)->isResolved();                           \
  if (_alist_fn->formals->length() != (call)->argList->length())        \
    INT_FATAL(call, "number of actuals does not match number of formals"); \
  Expr* actual = ((call)->argList->head);                               \
  Expr* _alist_actual_next = (actual) ? actual->next : NULL;            \
  for (ArgSymbol *formal = (_alist_fn->formals->head) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((_alist_fn->formals)->head)->sym) : NULL, \
         *_alist_formal_next = (formal && formal->defPoint->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_formal_next,                                     \
         _alist_formal_next = (formal && formal->defPoint->next) ? dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>((formal)->defPoint->next)->sym) : NULL, \
         actual = _alist_actual_next,                                   \
         _alist_actual_next = (actual) ? actual->next : NULL)

#define for_actuals(actual, call)               \
  for_alist(Expr, actual, (call)->argList)

#define for_actuals_backward(actual, call)              \
  for_alist_backward(Expr, actual, (call)->argList)

#define for_fields(field, ct)                                           \
  for (Symbol *field = ((ct)->fields->head) ? dynamic_cast<DefExpr*>(((ct)->fields)->head)->sym : NULL, \
         *_alist_next = (field && field->defPoint->next) ? dynamic_cast<DefExpr*>(field->defPoint->next)->sym : NULL; \
       (field);                                                         \
       field = _alist_next,                                             \
         _alist_next = (field && field->defPoint->next) ? dynamic_cast<DefExpr*>(field->defPoint->next)->sym : NULL)

#define for_fields_backward(field, ct)                                  \
  for (Symbol *field = ((ct)->fields->tail) ? dynamic_cast<DefExpr*>(((ct)->fields)->tail)->sym : NULL, \
         *_alist_prev = (field && field->defPoint->prev) ? dynamic_cast<DefExpr*>(field->defPoint->prev)->sym : NULL; \
       (field);                                                         \
       field = _alist_prev,                                             \
         _alist_prev = (field && field->defPoint->prev) ? dynamic_cast<DefExpr*>(field->defPoint->prev)->sym : NULL)

#define for_alist_backward(elt_type, node, list)                        \
  for (elt_type *node = dynamic_cast<elt_type*>((list)->tail),          \
         *_alist_prev = (node) ? dynamic_cast<elt_type*>((node)->prev) : NULL; \
       node;                                                            \
       node = _alist_prev,                                              \
         _alist_prev = (node) ? dynamic_cast<elt_type*>((node)->prev) : NULL)

#endif
