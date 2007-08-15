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
  void codegen(FILE* outfile, const char* separator = ", ");
};

#define for_alist(node, list)                                           \
  for (Expr *node = (list)->head,                                       \
         *_alist_next = (node) ? (node)->next : NULL;                   \
       node;                                                            \
       node = _alist_next,                                              \
         _alist_next = (node) ? (node)->next : NULL)

#define for_alist_backward(node, list)                                  \
  for (Expr *node = (list)->tail,                                       \
         *_alist_prev = (node) ? (node)->prev : NULL;                   \
       node;                                                            \
       node = _alist_prev,                                              \
         _alist_prev = (node) ? (node)->prev : NULL)

#define for_defs(node, list)                                            \
  for (DefExpr *node = toDefExpr((list)->head),                         \
         *_alist_next = (node) ? toDefExpr((node)->next) : NULL;        \
       node;                                                            \
       node = _alist_next,                                              \
         _alist_next = (node) ? toDefExpr((node)->next) : NULL)

#define for_defs_backward(node, list)                                   \
  for (DefExpr *node = toDefExpr((list)->tail),                         \
         *_alist_prev = (node) ? toDefExpr((node)->prev) : NULL;        \
       node;                                                            \
       node = _alist_prev,                                              \
         _alist_prev = (node) ? toDefExpr((node)->prev) : NULL)

#define for_formals(formal, fn)                                         \
  for (ArgSymbol *formal = ((fn)->formals->head) ? toArgSymbol(toDefExpr((fn)->formals->head)->sym) : NULL, \
         *_alist_next = (formal && formal->defPoint->next) ? toArgSymbol(toDefExpr((formal)->defPoint->next)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_next,                                            \
         _alist_next = (formal && formal->defPoint->next) ? toArgSymbol(toDefExpr((formal)->defPoint->next)->sym) : NULL)

#define for_formals_backward(formal, fn)                                \
  for (ArgSymbol *formal = ((fn)->formals->tail) ? toArgSymbol(toDefExpr((fn)->formals->tail)->sym) : NULL, \
         *_alist_prev = (formal && formal->defPoint->prev) ? toArgSymbol(toDefExpr((formal)->defPoint->prev)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_prev,                                            \
         _alist_prev = (formal && formal->defPoint->prev) ? toArgSymbol(toDefExpr((formal)->defPoint->prev)->sym) : NULL)

#define for_actuals(actual, call)               \
  for_alist(actual, (call)->argList)

#define for_actuals_backward(actual, call)      \
  for_alist_backward(actual, (call)->argList)

#define for_formals_actuals(formal, actual, call)                       \
  FnSymbol* _alist_fn = (call)->isResolved();                           \
  if (_alist_fn->formals->length() != (call)->argList->length())        \
    INT_FATAL(call, "number of actuals does not match number of formals"); \
  Expr* actual = ((call)->argList->head);                               \
  Expr* _alist_actual_next = (actual) ? actual->next : NULL;            \
  for (ArgSymbol *formal = (_alist_fn->formals->head) ? toArgSymbol(toDefExpr((_alist_fn->formals)->head)->sym) : NULL, \
         *_alist_formal_next = (formal && formal->defPoint->next) ? toArgSymbol(toDefExpr((formal)->defPoint->next)->sym) : NULL; \
       (formal);                                                        \
       formal = _alist_formal_next,                                     \
         _alist_formal_next = (formal && formal->defPoint->next) ? toArgSymbol(toDefExpr((formal)->defPoint->next)->sym) : NULL, \
         actual = _alist_actual_next,                                   \
         _alist_actual_next = (actual) ? actual->next : NULL)

#define for_fields(field, ct)                                           \
  for (Symbol *field = ((ct)->fields->head) ? toDefExpr(((ct)->fields)->head)->sym : NULL, \
         *_alist_next = (field && field->defPoint->next) ? toDefExpr(field->defPoint->next)->sym : NULL; \
       (field);                                                         \
       field = _alist_next,                                             \
         _alist_next = (field && field->defPoint->next) ? toDefExpr(field->defPoint->next)->sym : NULL)

#define for_fields_backward(field, ct)                                  \
  for (Symbol *field = ((ct)->fields->tail) ? toDefExpr(((ct)->fields)->tail)->sym : NULL, \
         *_alist_prev = (field && field->defPoint->prev) ? toDefExpr(field->defPoint->prev)->sym : NULL; \
       (field);                                                         \
       field = _alist_prev,                                             \
         _alist_prev = (field && field->defPoint->prev) ? toDefExpr(field->defPoint->prev)->sym : NULL)

#endif
