#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>
#include "loc.h"
#include "misc.h"
#include "vec.h"
#include "../traversals/traversal.h"


class ILink : public Loc {
  TRAVERSABLE_ILINK(ILink);
 public:
  ILink* prev;
  ILink* next;

  ILink** back;

  ILink(void);

  virtual bool isNull(void);

  virtual void print(FILE* outfile);
  void println(FILE* outfile);
  virtual void printList(FILE* outfile, char* separator = ", ");
  virtual void codegen(FILE* outfile);
  virtual void codegenList(FILE* outfile, char* separator = ", ");

  void add(ILink* newlink);
  void append(ILink* newlink);

  void filter(bool filter(ILink*), ILink** truelinks, ILink** falselinks);

  /***  Certifiable AST Management
   ***
   ***  For use within traversals or not
   ***
   ***  The insertBefore method inserts a list of statements,
   ***  expressions, symbols, or types _before_ a single statement,
   ***  expression, symbol, or type (respectively) in the AST.  The
   ***  inserted part of the AST will _not_ be traversed.
   ***
   ***  The insertAfter method inserts a list of statements,
   ***  expressions, symbols, or types _after_ a single statement,
   ***  expression, symbol, or type (respectively) in the AST.  The
   ***  inserted part of the AST _will_ be traversed.
   ***
   ***  The extract method removes a statement from the AST.
   ***
   ***  The static replace method (called via Stmt::replace,
   ***  Expr::replace, Symbol::replace, or Type::replace) replaces a
   ***  statement, expression, symbol, or type with a list of
   ***  statements, expressions, symbols, or types (respectively) in
   ***  the AST.  The traversal will continue after this replacement.
   ***  This method cannot be called on a piece of the AST containing
   ***  the part of the AST being traversed.  That is, if traversing
   ***  an expression inside a statement, the statement cannot be
   ***  replaced (e.g. Stmt::replace(expr->stmt, ...) where expr is
   ***  the formal parameter in preProcessExpr.
   ***
   ***  >>>>>>>>>>>>>>>>>>>>>>>>>>CAUTION<<<<<<<<<<<<<<<<<<<<<<<<<<
   ***  > Do not call ILink::replace directly; use Stmt::replace, <
   ***  > Expr::replace, Symbol::replace, or Type::replace        <
   ***  >>>>>>>>>>>>>>>>>>>>>>>>END CAUTION<<<<<<<<<<<<<<<<<<<<<<<<
   ***/
  void insertBefore(ILink* new_link);
  void insertAfter(ILink* new_link);
  ILink* extract(void);
  static void replace(ILink* old_link, ILink* new_link);
};

extern ILink* nilILink;

#define prevLink(type, node) (dynamic_cast<type*>((node)->prev))
#define nextLink(type, node) (dynamic_cast<type*>((node)->next))

#define appendLink(list, node) ((((list) == NULL) || (list)->isNull()) ? \
                                (node) : \
                                ((list)->append(node), (list)))

template <class astType>
void getLinkElements(Vec<astType*>& elements, ILink* link) {
  astType* element;

  elements.clear();
  while (!link->isNull()) {
    element = dynamic_cast<astType*>(link);
    if (element) {
      elements.add(element);
    } else {
      INT_FATAL(link, "Unexpected type in getElements");
    }

    link = link->next;
  }
}

#endif
