/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "expr.h"

#include "alist.h"
#include "astutil.h"
#include "AstVisitor.h"
#include "ForLoop.h"
#include "insertLineNumbers.h"
#include "iterator.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "type.h"
#include "wellknown.h"
#include "WhileStmt.h"


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include <ostream>
#include <stack>

class FnSymbol;

// some prototypes

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

Expr::Expr(AstTag astTag) :
  BaseAST(astTag),
  parentSymbol(NULL),
  parentExpr(NULL),
  list(NULL),
  prev(NULL),
  next(NULL)
{ }

Expr::~Expr() {

}

bool Expr::isStmt() const {
  return false;
}

const char* DefExpr::name() const {
  const char* retval = 0;

  if (isLcnSymbol(sym)    == true ||
      isTypeSymbol(sym)   == true ||
      isFnSymbol(sym)     == true ||
      isModuleSymbol(sym) == true) {
    retval = sym->name;
  }

  return retval;
}

//
// BHARSH TODO: Fix up PRIM_ADDR_OF's return type function to correctly
// handle this
//
// The 'returnInfoRef' function for PRIM_ADDR_OF is currently set up to
// always return with the Qualifier QUAL_REF. This function is intended
// as a workaround to handle the case where we have a wide-ref in a
// PRIM_ADDR_OF:
//
// (move "wide-ref dest" (addr-of "wide-ref src"))
//
// Otherwise, 'returnInfoRef' would hide the fact that the actual is a wide-ref
// and it would appear as though we had a local reference.
//
// Note that this case is actually just a copy of a pointer's address, and
// will not generate an addrof in the generated code.
//
static
bool isAddrOfWideRefVar(Expr* e)
{
  // ADDR_OF of a wide ref variable is a wide ref (not a ref).
  if (CallExpr* call = toCallExpr(e))
    if (call->isPrimitive(PRIM_ADDR_OF))
      if (SymExpr* se = toSymExpr(call->get(1)))
        if (se->symbol()->qualType().isWideRef())
          return true;

  return false;
}

bool Expr::isRef() {
  if (SymExpr* se = toSymExpr(this))
    return se->symbol()->isRef();

  if (isAddrOfWideRefVar(this))
    return false; // wide ref, not ref

  return this->qualType().isRef();
}

bool Expr::isWideRef() {
  if(SymExpr* se = toSymExpr(this))
    return se->symbol()->isWideRef();

  if (isAddrOfWideRefVar(this))
    return true;

  return this->qualType().isWideRef();
}

bool Expr::isRefOrWideRef() {
  return isRef() || isWideRef();
}

// Returns true if 'this' properly contains the given expr, false otherwise.
bool Expr::contains(const Expr* expr) const {
  const Expr* parent = expr->parentExpr;

  while (parent != NULL && parent != this) {
    parent = parent->parentExpr;
  }

  return (parent == this) ? true : false;
}

// Return true if this expression is a ModuleDefinition i.e. it
// is a DefExpr and the referenced symbol is a Module Symbol

bool Expr::isModuleDefinition() {
  bool retval = false;

#if 1
  //  MDN 2014/07/02
  //  Leaving the old definition here until the scope-less BlockStmt
  //  change is stable.
  if (BlockStmt* block = toBlockStmt(this))
    if (block->length() == 1)
      if (DefExpr* def = toDefExpr(block->body.only()))
        if (isModuleSymbol(def->sym))
          retval = true;
#endif

  if (DefExpr* def = toDefExpr(this))
    if (isModuleSymbol(def->sym))
      retval = true;

  return retval;
}

bool Expr::isStmtExpr() const {
  bool retval = false;

  if (isStmt() == true) {
    retval = true;

  // NOAKES 2014/11/28 A WhileStmt is currently a BlockStmt
  // but needs special handling
  } else if (WhileStmt* parent = toWhileStmt(parentExpr)) {
    retval = (parent->condExprGet() != this) ? true : false;

  // NOAKES 2014/11/30 A ForLoop is currently a BlockStmt
  // but needs special handling
  } else if (ForLoop* parent = toForLoop(parentExpr)) {
    retval = (parent->indexGet() != this && parent->iteratorGet() != this) ? true : false;

  } else {
    retval = isBlockStmt(parentExpr);
  }

  return retval;
}

Expr* Expr::getStmtExpr() {
  for (Expr* expr = this; expr; expr = expr->parentExpr) {
    if (expr->isStmt() == true) {
      return expr;

    // NOAKES 2014/11/28 A WhileStmt is currently a BlockStmt
    // but needs special handling
    } else if (WhileStmt* parent = toWhileStmt(expr->parentExpr)) {
      if (parent->condExprGet() != expr) {
        return expr;
      }

    // NOAKES 2014/11/30 A ForLoop is currently a BlockStmt
    // but needs special handling
    } else if (ForLoop* parent = toForLoop(parentExpr)) {
      if (parent->indexGet() != this && parent->iteratorGet() != this)
        return expr;

    } else if (isBlockStmt(expr->parentExpr)) {
      return expr;
    }
  }

  return NULL;
}

Expr* Expr::getNextExpr(Expr* expr) {
  return this;
}

// Returns the nearest enclosing *scoped* block statement (excluding 'this')
// that contains 'this'

// It is probably an error if there is no such BlockStmt.
// Currently return NULL.  Consider throwing an internal error in the future.
BlockStmt* Expr::getScopeBlock() {
  Expr*      expr   = this->parentExpr;
  BlockStmt* retval = NULL;

  while (expr != NULL && retval == NULL) {
    BlockStmt* block = toBlockStmt(expr);

    if (block != NULL && (block->blockTag & BLOCK_SCOPELESS) == 0)
      retval = block;
    else
      expr   = expr->parentExpr;
  }

  return retval;
}

void Expr::verify() {
  if (prev || next)
    if (!list)
      INT_FATAL(this, "Expr is in list but does not point at it");

  if (prev && prev->next != this)
    INT_FATAL(this, "Bad Expr->prev->next");

  if (next && next->prev != this)
    INT_FATAL(this, "Bad Expr->next->prev");

  if (!parentSymbol)
    INT_FATAL(this, "Expr::parentSymbol is NULL");

  if (parentExpr && parentExpr->parentSymbol != parentSymbol)
    INT_FATAL(this, "Bad Expr::parentSymbol");

  if (list && parentExpr && list->parent != parentExpr)
    INT_FATAL(this, "Bad Expr::list::parent");

  if (list && !parentExpr) {
    if (Symbol* lps = toSymbol(list->parent))
      if (lps != parentSymbol)
        INT_FATAL(this, "Bad symbol Expr::list::parent");
    if (Type* lpt = toType(list->parent))
      if (lpt->symbol != parentSymbol)
        INT_FATAL(this, "Bad type Expr::list::parent");
    if (isExpr(list->parent))
      INT_FATAL(this, "Expr::list::parent is an Expr unexpectedly");
  }
}

void Expr::verify(AstTag expectedTag) {
  if (astTag != expectedTag)
    INT_FATAL(this, "bad astTag");
  Expr::verify();
}

void Expr::verifyParent(const Expr* child) {
  if (child && child->parentExpr != this)
    INT_FATAL(this, "bad parent of a child node");
}

bool Expr::inTree() {
  if (parentSymbol)
    return true;
  else
    return false;
}


QualifiedType Expr::qualType() {
  INT_FATAL(this, "Illegal call to Expr::qualType()");
  return QualifiedType(NULL);
}

bool Expr::isNoInitExpr() const {
  return false;
}

static void
callReplaceChild(Expr* expr, Expr* newAst) {

  if (expr->parentExpr) {
    expr->parentExpr->replaceChild(expr, newAst);


  } else if (expr->parentSymbol) {
    expr->parentSymbol->replaceChild(expr, newAst);


  } else {
    INT_FATAL(expr, "Expr %12d does not have a parent", expr->id);
  }
}

void Expr::prettyPrint(std::ostream *o) {
  if (BlockStmt *stmt = toBlockStmt(this))
    printf("blockstmt %s", stmt->userLabel);

  else if (CondStmt *stmt = toCondStmt(this))
    printf("condstmt %s", stmt->condExpr->parentSymbol->name);

  else if (GotoStmt *stmt = toGotoStmt(this))
    printf("gotostmt %s", stmt->label->parentSymbol->name);

  printf("Oh no! This method hasn't been defined for this class!\n");
}

Expr* Expr::remove() {
  if (list) {
    if (next)
      next->prev = prev;
    else
      list->tail = prev;

    if (prev)
      prev->next = next;
    else
      list->head = next;

    list->length--;

    next = NULL;
    prev = NULL;
    list = NULL;
  } else {
    callReplaceChild(this, NULL);
  }

  if (parentSymbol) {
    remove_help(this, 'r');
  } else {
    trace_remove(this, 'R');
  }

  return this;
}


void Expr::replace(Expr* new_ast) {
  if (new_ast->parentSymbol || new_ast->parentExpr)
    INT_FATAL(new_ast, "Argument is already in AST in Expr::replace");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in Expr::replace");
  if (list) {
    new_ast->next = next;
    new_ast->prev = prev;
    new_ast->list = list;
    if (next)
      next->prev = new_ast;
    else
      list->tail = new_ast;
    if (prev)
      prev->next = new_ast;
    else
      list->head = new_ast;
    next = NULL;
    prev = NULL;
    list = NULL;
  } else {
    callReplaceChild(this, new_ast);
  }

  Symbol* myParentSymbol = parentSymbol;
  Expr* myParentExpr = parentExpr;
  remove_help(this, 'p');
  insert_help(new_ast, myParentExpr, myParentSymbol);

  // Update the _this field in a FnSymbol if necessary.
  if (DefExpr* def = toDefExpr(this))
    if (ArgSymbol* arg = toArgSymbol(def->sym))
      if (FnSymbol* fn = toFnSymbol(myParentSymbol))
        if (fn->_this == arg)
          fn->_this = toDefExpr(new_ast)->sym;
}


void Expr::insertBefore(Expr* new_ast) {
  if (new_ast->parentSymbol || new_ast->parentExpr)
    INT_FATAL(new_ast, "Argument is already in AST in Expr::insertBefore");
  if (!list)
    INT_FATAL(this, "Cannot call insertBefore on Expr not in a list");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in Expr::insertBefore");
  new_ast->prev = prev;
  new_ast->next = this;
  new_ast->list = list;
  if (prev)
    prev->next = new_ast;
  else
    list->head = new_ast;
  prev = new_ast;
  if (parentSymbol)
    sibling_insert_help(this, new_ast);
  list->length++;
}

void Expr::insertAfter(Expr* new_ast) {
  if (new_ast->parentSymbol || new_ast->parentExpr)
    INT_FATAL(new_ast, "Argument is already in AST in Expr::insertAfter");
  if (!list)
    INT_FATAL(this, "Cannot call insertAfter on Expr not in a list");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in Expr::insertAfter");
  new_ast->prev = this;
  new_ast->next = next;
  new_ast->list = list;
  if (next)
    next->prev = new_ast;
  else
    list->tail = new_ast;
  next = new_ast;
  if (parentSymbol)
    sibling_insert_help(this, new_ast);
  list->length++;
}

void Expr::insertAfter(Expr* e1, Expr* e2) {
  insertAfter(e2);
  insertAfter(e1);
}
void Expr::insertAfter(Expr* e1, Expr* e2, Expr* e3) {
  insertAfter(e3);
  insertAfter(e2);
  insertAfter(e1);
}
void Expr::insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4) {
  insertAfter(e4);
  insertAfter(e3);
  insertAfter(e2);
  insertAfter(e1);
}
void Expr::insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4, Expr* e5) {
  insertAfter(e5);
  insertAfter(e4);
  insertAfter(e3);
  insertAfter(e2);
  insertAfter(e1);
}
void Expr::insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4,
                       Expr* e5, Expr* e6) {
  insertAfter(e6);
  insertAfter(e5);
  insertAfter(e4);
  insertAfter(e3);
  insertAfter(e2);
  insertAfter(e1);
}


void Expr::insertBefore(AList exprs) {
  Expr* curr = this;
  for_alist_backward(prev, exprs) {
    prev->remove();
    curr->insertBefore(prev);
    curr = prev;
  }
}


void Expr::insertAfter(AList exprs) {
  Expr* prev = this;
  for_alist(curr, exprs) {
    curr->remove();
    prev->insertAfter(curr);
    prev = curr;
  }
}


void
Expr::replace(const char* format, ...) {
  va_list args;

  va_start(args, format);
  replace(new_Expr(format, args));
  va_end(args);
}


void
Expr::insertBefore(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertBefore(new_Expr(format, args));
  va_end(args);
}


void
Expr::insertAfter(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAfter(new_Expr(format, args));
  va_end(args);
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

SymExpr::SymExpr(Symbol* init_var) :
  Expr(E_SymExpr),
  var(init_var),
  symbolSymExprsPrev(NULL),
  symbolSymExprsNext(NULL)
{
  if (!init_var)
    INT_FATAL(this, "Bad call to SymExpr");
  gSymExprs.add(this);

  // No need to call var->addSymExpr here since it will be called
  // when the SymExpr is added to the tree.
}

bool SymExpr::isNoInitExpr() const {
  return var == gNoInit;
}

void SymExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  INT_FATAL(this, "Unexpected case in SymExpr::replaceChild");
}

Expr* SymExpr::getFirstExpr() {
  return this;
}

void SymExpr::verify() {
  Expr::verify(E_SymExpr);

  if (var == NULL)
    INT_FATAL(this, "SymExpr::verify %12d: var is NULL", id);

  if (var->defPoint) {
    bool isEndOfStatement = false;
    if (CallExpr* call = toCallExpr(parentExpr))
      if (call->isPrimitive(PRIM_END_OF_STATEMENT))
        isEndOfStatement = true;

    if (!var->defPoint->inTree() && !isEndOfStatement)
      INT_FATAL(this, "SymExpr::verify %12d:  var->defPoint is not in AST", id);
  } else {
    if (var != rootModule)
      INT_FATAL(this, "SymExpr::var is a symbol without a defPoint");
  }

  /* Check that:
      - every live SymExpr is in a Symbol's list
      - every live SymExpr is in it's Symbol's list

     using local operations on the lists.
   */

  if (this->symbolSymExprsPrev == NULL) {
    if (var->firstSymExpr() != this)
      INT_FATAL(this, "SymExpr::verify %12d: no prev but not first", id);
  } else {
    if (this->symbolSymExprsPrev->var != var)
      INT_FATAL(this, "SymExpr::verify %12d: does not match prev SymExpr", id);
    if (!this->symbolSymExprsPrev->inTree())
      INT_FATAL(this, "SymExpr::verify %12d: prev SymExpr not in tree", id);
  }

  if (this->symbolSymExprsNext == NULL) {
    if (var->lastSymExpr() != this)
      INT_FATAL(this, "SymExpr::verify %12d: no next but not last", id);
  } else {
    if (this->symbolSymExprsNext->symbol() != this->symbol())
      INT_FATAL(this, "SymExpr::verify %12d: does not match next SymExpr", id);
    if (!this->symbolSymExprsNext->inTree())
      INT_FATAL(this, "SymExpr::verify %12d: next SymExpr not in tree", id);
  }
}

SymExpr* SymExpr::copyInner(SymbolMap* map) {
  return new SymExpr(var);
}

QualifiedType SymExpr::qualType(void) {
  if (toFnSymbol(var)) {
    return QualifiedType(dtCFnPtr);
  } else {
    return var->qualType();
  }
}

void SymExpr::prettyPrint(std::ostream *o) {
  if (strcmp(var->name, "nil") != 0) {
    if (var->isImmediate()) {
      if (VarSymbol *sym = toVarSymbol(var)) {
        if (sym->immediate->const_kind == CONST_KIND_STRING)
          *o << "\"" << sym->immediate->v_string << "\"";
        else if (sym->immediate->const_kind == NUM_KIND_BOOL)
          *o << sym->immediate->bool_value();
        else if (sym->immediate->const_kind == NUM_KIND_INT)
          *o << sym->immediate->int_value();
        else if (sym->immediate->const_kind == NUM_KIND_UINT)
          *o << sym->immediate->uint_value();
      }
    } else {
      *o << var->name;
    }
  }
}

void SymExpr::accept(AstVisitor* visitor) {
  visitor->visitSymExpr(this);
}

void SymExpr::setSymbol(Symbol* s)
{
  // If the old symbol is not NULL and the SymExpr
  // is in the tree, remove the SymExpr from the old Symbol's list.
  if (var != NULL && parentSymbol != NULL) {
    var->removeSymExpr(this);
  }
  // Update the symbol
  var = s;
  // If the symbol is not NULL and the SymExpr is in the tree,
  // add the SymExpr to the new Symbol's list.
  if (s != NULL && parentSymbol != NULL) {
    s->addSymExpr(this);
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

UnresolvedSymExpr::UnresolvedSymExpr(const char* i_unresolved) :
  Expr(E_UnresolvedSymExpr),
  unresolved(astr(i_unresolved))
{
  if (!i_unresolved)
    INT_FATAL(this, "bad call to UnresolvedSymExpr");
  gUnresolvedSymExprs.add(this);
}

void
UnresolvedSymExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  INT_FATAL(this, "unexpected case in UnresolvedSymExpr::replaceChild");
}


Expr* UnresolvedSymExpr::getFirstExpr() {
  return this;
}

void
UnresolvedSymExpr::verify() {
  Expr::verify(E_UnresolvedSymExpr);
  if (!unresolved)
    INT_FATAL(this, "UnresolvedSymExpr::unresolved is NULL");
}


UnresolvedSymExpr*
UnresolvedSymExpr::copyInner(SymbolMap* map) {
  return new UnresolvedSymExpr(unresolved);
}


QualifiedType UnresolvedSymExpr::qualType(void) {
  return QualifiedType(dtUnknown);
}


void UnresolvedSymExpr::prettyPrint(std::ostream *o) {
  *o << unresolved;
}

void UnresolvedSymExpr::accept(AstVisitor* visitor) {
  visitor->visitUsymExpr(this);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

DefExpr::DefExpr(Symbol* initSym, BaseAST* initInit, BaseAST* initExprType) :
  Expr(E_DefExpr),
  sym(initSym),
  init(NULL),
  exprType(NULL)
{
  if (sym)
    sym->defPoint = this;

  if (Expr* a = toExpr(initInit)) {
    init = a;
  } else if (Symbol* a = toSymbol(initInit))
    init = new SymExpr(a);
  else if (initInit)
    INT_FATAL(this, "DefExpr initialized with bad init ast");

  if (Expr* a = toExpr(initExprType))
    exprType = a;
  else if (Symbol* a = toSymbol(initExprType))
    exprType = new SymExpr(a);
  else if (initExprType)
    INT_FATAL(this, "DefExpr initialized with bad exprType ast");

  if (init && init->parentSymbol)
    INT_FATAL(this, "DefExpr initialized with init already in tree");

  if (exprType && exprType->parentSymbol)
    INT_FATAL(this, "DefExpr initialized with exprType already in tree");

  if (isArgSymbol(sym) && (exprType || init))
    INT_FATAL(this, "DefExpr of ArgSymbol cannot have either exprType or init");

  gDefExprs.add(this);
}

Expr* DefExpr::getFirstExpr() {
  return this;
}

void DefExpr::verify() {
  Expr::verify(E_DefExpr);
  if (!sym) {
    INT_FATAL(this, "DefExpr has no sym");
  }
  if (toFnSymbol(sym) && (exprType || init))
    INT_FATAL(this, "Bad FnSymbol::defPoint");
  if (toArgSymbol(sym) && (exprType || init))
    INT_FATAL(this, "Bad ArgSymbol::defPoint");
  if (sym->defPoint != this)
    INT_FATAL(this, "Bad DefExpr::sym->defPoint");
  verifyParent(init);
  verifyParent(exprType);
  verifyNotOnList(init);
  verifyNotOnList(exprType);
}


DefExpr*
DefExpr::copyInner(SymbolMap* map) {
  return new DefExpr(COPY_INT(sym), COPY_INT(init), COPY_INT(exprType));
}


void DefExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == init) {
    init = new_ast;
  } else if (old_ast == exprType) {
    exprType = new_ast;
  } else {
    INT_FATAL(this, "Unexpected case in DefExpr::replaceChild");
  }
}


QualifiedType DefExpr::qualType(void) {
  INT_FATAL(this, "Illegal call to DefExpr::qualType()");
  return QualifiedType(NULL);
}


void DefExpr::accept(AstVisitor* visitor) {
  if (visitor->enterDefExpr(this) == true) {
    if (init)
      init->accept(visitor);

    if (exprType)
      exprType->accept(visitor);

    if (sym)
      sym->accept(visitor);

    visitor->exitDefExpr(this);
  }
}

void DefExpr::prettyPrint(std::ostream *o) {
  *o << "<DefExprType>";
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

ContextCallExpr::ContextCallExpr() : Expr(E_ContextCallExpr) {
  options.parent = this;

  hasValue       = false;
  hasConstRef    = false;
  hasRef         = false;

  gContextCallExprs.add(this);
}

ContextCallExpr* ContextCallExpr::copyInner(SymbolMap* map) {
  ContextCallExpr* _this = 0;

  _this              = new ContextCallExpr();

  _this->hasValue    = hasValue;
  _this->hasConstRef = hasConstRef;
  _this->hasRef      = hasRef;

  for_alist(expr, options) {
    _this->options.insertAtTail(COPY_INT(expr));
  }

  return _this;
}

CallExpr* getDesignatedCall(const ContextCallExpr* a) {
  return toCallExpr(a->options.tail);
}

void ContextCallExpr::replaceChild(Expr* oldAst, Expr* newAst) {
  INT_FATAL(this, "unexpected case in ContextCallExpr::replaceChild");
}

void ContextCallExpr::verify() {
  Expr::verify(E_ContextCallExpr);

  for_alist(expr, options) {
    verifyParent(expr);

    if (isContextCallExpr(expr) == true) {
      INT_FATAL(this, "ContextCallExpr cannot contain a ContextCallExpr");
    }

    if (isCallExpr(expr) == false) {
      INT_FATAL(this, "ContextCallExpr must contain only CallExpr");
    }
  }

  // ContextCallExpr handles ref/value/const ref, so there should
  // be 2 or 3 options.
  if (options.length < 2 || options.length > 3)
    INT_FATAL(this, "ContextCallExpr with < 2 or > 3 options");
}

void ContextCallExpr::accept(AstVisitor* visitor) {
  if (visitor->enterContextCallExpr(this) == true) {

    for_alist(expr, options) {
      expr->accept(visitor);
    }

    visitor->exitContextCallExpr(this);
  }
}

QualifiedType ContextCallExpr::qualType() {
  CallExpr*     mainCall = getDesignatedCall(this);
  QualifiedType retval   = QualifiedType(dtUnknown);

  if (mainCall) {
    retval = mainCall->qualType();
  }

  return retval;
}

void ContextCallExpr::prettyPrint(std::ostream* o) {
  *o << "(options";

  for_alist(expr, options) {
    *o << " ";
    expr->prettyPrint(o);
  }

  *o << " )";
}

Expr* ContextCallExpr::getFirstExpr() {
  return options.head->getFirstExpr();
}


void ContextCallExpr::setRefValueConstRefOptions(CallExpr* refCall,
                                                 CallExpr* valueCall,
                                                 CallExpr* constRefCall) {

  // always use order of value, const ref, ref
  // ContextCallExpr::getCalls depends on this order
  int n = 0;

  if (valueCall != NULL) {
    options.insertAtTail(valueCall);
    parent_insert_help(this, valueCall);
    hasValue = true;
    n++;
  }

  if (constRefCall != NULL) {
    options.insertAtTail(constRefCall);
    parent_insert_help(this, constRefCall);
    hasConstRef = true;
    n++;
  }

  if (refCall != NULL) {
    options.insertAtTail(refCall);
    parent_insert_help(this, refCall);
    hasRef = true;
    n++;
  }

  // We shouldn't be making a ContextCallExpr with < 2 options
  INT_ASSERT(n >= 2);
}

void ContextCallExpr::getCalls(CallExpr*& refCall,
                               CallExpr*& valueCall,
                               CallExpr*& constRefCall) const {

  // always use order of value, const ref, ref
  int n = 1;

  if (hasValue) {
    valueCall = toCallExpr(options.get(n));
    n++;
  } else {
    valueCall = NULL;
  }

  if (hasConstRef) {
    constRefCall = toCallExpr(options.get(n));
    n++;
  } else {
    constRefCall = NULL;
  }

  if (hasRef) {
    refCall = toCallExpr(options.get(n));
    n++;
  } else {
    refCall = NULL;
  }
}

CallExpr* ContextCallExpr::getValueCall() const {
  CallExpr* retval = NULL;

  if (hasValue == true) {
    retval = toCallExpr(options.get(1));
  }

  return retval;
}

CallExpr* ContextCallExpr::getConstRefCall() const {
  CallExpr* retval = NULL;

  if (hasConstRef == true) {
    int n = (hasValue == true) ? 2 : 1;

    retval = toCallExpr(options.get(n));
  }

  return retval;
}

CallExpr* ContextCallExpr::getRefCall() const {
  CallExpr* retval = NULL;

  if (hasRef == true) {
    int n = (hasValue == true && hasConstRef == true) ? 3 : 2;

    retval = toCallExpr(options.get(n));
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

NamedExpr::NamedExpr(const char* init_name, Expr* init_actual) :
  Expr(E_NamedExpr),
  name(astr(init_name)),
  actual(init_actual)
{
  gNamedExprs.add(this);
}


Expr* NamedExpr::getFirstExpr() {
  return (actual != NULL) ? actual->getFirstExpr() : this;
}

void NamedExpr::verify() {
  Expr::verify(E_NamedExpr);
  verifyParent(actual);
  verifyNotOnList(actual);
}


NamedExpr*
NamedExpr::copyInner(SymbolMap* map) {
  return new NamedExpr(astr(name), COPY_INT(actual));
}


void NamedExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == actual) {
    actual = new_ast;
  } else {
    INT_FATAL(this, "Unexpected case in NamedExpr::replaceChild");
  }
}


QualifiedType NamedExpr::qualType(void) {
  return actual->qualType();
}


void NamedExpr::prettyPrint(std::ostream *o) {
  *o << "<NamedExprType>";
}


void NamedExpr::accept(AstVisitor* visitor) {
  if (visitor->enterNamedExpr(this) == true) {

    if (actual)
      actual->accept(visitor);

    visitor->exitNamedExpr(this);
  }
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

bool
get_int(Expr *e, int64_t *i) {
  Immediate* imm = NULL;
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      imm = getSymbolImmediate(l->symbol());
    }
  }

  if (imm && imm->const_kind == NUM_KIND_INT) {
    *i = imm->int_value();
    return true;
  }

  return false;
}

bool
get_uint(Expr *e, uint64_t *i) {
  Immediate* imm = NULL;
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      imm = getSymbolImmediate(l->symbol());
    }
  }

  if (imm && imm->const_kind == NUM_KIND_UINT) {
    *i = imm->uint_value();
    return true;
  }

  return false;
}

bool
get_string(Expr *e, const char **s) {
  Immediate* imm = NULL;
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      imm = getSymbolImmediate(l->symbol());
    }
  }

  if (imm && imm->const_kind == CONST_KIND_STRING) {
    *s = imm->v_string;
    return true;
  }

  return false;
}

const char*
get_string(Expr* e) {
  const char* s = NULL;
  if (!get_string(e, &s))
    INT_FATAL(e, "string literal expression expected");
  return s;
}

Expr* getNextExpr(Expr* expr) {
  Expr* retval = NULL;

  if (expr->next) {
    retval = expr->next->getFirstExpr();

  } else if (Expr* parent = expr->parentExpr) {
    retval = parent->getNextExpr(expr);

  }

  return retval;
}

static bool
isIdentifierChar(const char c) {
  return ((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          (c == '$') ||
          (c == '_') || (c == '.'));
}


/*********** new_Expr() ***********/
/*

new_Expr() lets you build AST more succinctly.

You can call new_Expr() directly, or implicitly by calling:

  BlockStmt::insertAtHead
  BlockStmt::insertAtTail
  FnSymbol::insertAtHead
  FnSymbol::insertAtTail
  Expr::insertBefore
  Expr::insertAfter
  Expr::replace

Synopsis:

  new_Expr(const char* format, ...)

The format string should contain a Chapel statement or expression.

SIMPLE CALLS

The code

  block->insertAtTail("foo()");

is equivalent to

  block->insertAtTail(new CallExpr("foo"));

USING SYMBOLS AND EXPRESSIONS

Symbols and expressions can be added to the newly created expressions
using %S and %E format flags.  For example, given:

  VarSymbol* tmp;
  CallExpr* call;

the code

  block->insertAtTail("foo(%S)", tmp);
  block->insertAtTail("foo(%E)", call);

is equivalent to

  block->insertAtTail(new CallExpr("foo", tmp));
  block->insertAtTail(new CallExpr("foo", call));

UNRESOLVED SYMBOLS

An unresolved symbol can be referenced via its name without quotes.
For example, the code

  block->insertAtTail("foo(bar)");

is equivalent to

  block->insertAtTail(new CallExpr("foo", new UnresolvedSymExpr("bar")));

PRIMITIVES

Primitives can be defined by enclosing the name of the primitive in
apostrophes.  So the code

  block->insertAtTail("'move'(%S, new CallExpr("foo"))");

is equivalent to

  block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr("foo")));

STRING LITERALS

String literals are also supported by enclosing a string in
apostrophes.

BLOCK STATEMENTS

Finally, block statements and type block statements
(BlockStmt::blockTag == BLOCK_TYPE) are supported
via curly brackets and semicolons. For example:

  new_Expr("{TYPE 'move'(%S, iteratorIndex(%S)) }", followIdx, followIter);

METHOD CALLS

Note that AST represents method calls differently before and after normalize.

Here are examples before normalize:

  // localOp.accumulate(followIdx)
  new_Expr(".(%S, 'accumulate')(%S)", localOp, followIdx);

  // globalOp.generate()
  new_Expr(".(%S, 'generate')()", globalOp);

  // Paren-less calls are perhaps done so: localOp.identity
  new_Expr(".(%S, 'identity')", localOp);

After normalize method calls are represented as procedure calls with
the first argument being gMethodToken:

  // rvar.identity
  new_Expr("identity(%S,%S)", gMethodToken, rvar);

  // rvar.accumulate(svar)
  new_Expr("accumulate(%S,%S,%S)", gMethodToken, rvar, svar);

FINAL EXAMPLE

The code

  leadBlock->insertAtTail(new CallExpr(PRIM_MOVE, leadIter, new CallExpr("_getIterator", new CallExpr("_toLeader", iter))));

can be written as

  leadBlock->insertAtTail("'move'(%S, _getIterator(_toLeader(%S)))", leadIter, iter);

*/

Expr*
new_Expr(const char* format, ...) {
  va_list vl;
  va_start(vl, format);
  Expr* result = new_Expr(format, vl);
  va_end(vl);
  return result;
}

Expr*
new_Expr(const char* format, va_list vl) {
  std::stack<Expr*> stack;

  for (int i = 0; format[i] != '\0'; i++) {
    if (isIdentifierChar(format[i])) {
      int n = 1;
      while (isIdentifierChar(format[i+n]))
        n++;
      const char* str = asubstr(&format[i], &format[i+n]);
      i += n-1;
      if (!strcmp(str, "TYPE")) {
        if (stack.size() == 0) {
          INT_FATAL("You neglected to provide a \"{ TYPE ...\" for a block type statement.");
        } // Accessing the stack would result in unspecified behavior
        BlockStmt* block = toBlockStmt(stack.top());
        INT_ASSERT(block);
        block->blockTag = BLOCK_TYPE;
      } else {
        stack.push(new UnresolvedSymExpr(str));
      }
    } else if (format[i] == '\'') {
      int n = 1;
      while (format[i+n] != '\'')
        n++;
      const char* str = asubstr(&format[i+1], &format[i+n]);
      i += n;
      if (format[i+1] == '(') {
        PrimitiveOp* prim = primitives_map.get(str);
        INT_ASSERT(prim);
        stack.push(new CallExpr(prim));
        i++;
      } else {
        stack.push(new SymExpr(new_StringSymbol(str)));
      }
    } else if (format[i] == '%') {
      i++;
      if (format[i] == 'S')
        stack.push(new SymExpr(va_arg(vl, Symbol*)));
      else if (format[i] == 'E')
        stack.push(va_arg(vl, Expr*));
      else
        INT_FATAL("unknown format specifier in new_Expr");
    } else if (format[i] == '(') {
      Expr* expr = stack.top();
      stack.pop();
      INT_ASSERT(expr);
      stack.push(new CallExpr(expr));
      if (format[i+1] == ')') // handle empty calls
        i++;
    } else if (format[i] == ',') {
      Expr* expr = stack.top();
      stack.pop();
      INT_ASSERT(expr);
      if (stack.size() == 0) {
        INT_FATAL("There was nothing before the \',\'");
      } // Accessing the stack would result in unspecified behavior
      CallExpr* call = toCallExpr(stack.top());
      INT_ASSERT(call);
      call->insertAtTail(expr);
    } else if (format[i] == ')') {
      Expr* expr = stack.top();
      stack.pop();
      INT_ASSERT(expr);
      if (stack.size() == 0) {
        INT_FATAL("This closing parentheses is unmatched");
      } // Accessing the stack would result in unspecified behavior
      CallExpr* call = toCallExpr(stack.top());
      INT_ASSERT(call);
      call->insertAtTail(expr);
    } else if (format[i] == '{') {
      stack.push(new BlockStmt());
    } else if (format[i] == ';') {
      Expr* expr = stack.top();
      stack.pop();
      INT_ASSERT(expr);
      if (stack.size() == 0) {
        INT_FATAL("There was nothing before the \';\'");
      } // Accessing the stack would result in unspecified behavior
      BlockStmt* block = toBlockStmt(stack.top());
      INT_ASSERT(block);
      block->insertAtTail(expr);
    } else if (format[i] == '}') {
      Expr* expr = stack.top();
      stack.pop();
      INT_ASSERT(expr);
      if (stack.size() == 0) {
        INT_FATAL("This closing curly bracket is unmatched");
      } // Accessing the stack would result in unspecified behavior
      BlockStmt* block = toBlockStmt(stack.top());
      INT_ASSERT(block);
      block->insertAtTail(expr);
    }
  }

  INT_ASSERT(stack.size() == 1);
  return stack.top();
}


static CallExpr* findOptimizationInfo(Expr* anchor) {
  if (anchor && anchor->prev)
    if (CallExpr* call = toCallExpr(anchor->prev))
      if (call->isPrimitive(PRIM_OPTIMIZATION_INFO))
        return call;
  if (anchor)
    if (CallExpr* call = toCallExpr(anchor))
      if (call->isPrimitive(PRIM_OPTIMIZATION_INFO))
        return call;
  if (anchor && anchor->next)
    if (CallExpr* call = toCallExpr(anchor->next))
      if (call->isPrimitive(PRIM_OPTIMIZATION_INFO))
        return call;
  if (anchor && anchor->next && anchor->next->next)
    if (CallExpr* call = toCallExpr(anchor->next->next))
      if (call->isPrimitive(PRIM_OPTIMIZATION_INFO))
        return call;

  return NULL;
}

void addOptimizationFlag(Expr* insertAfter, Flag flag) {
  Symbol* optInfoSym = NULL;
  CallExpr* optInfo = findOptimizationInfo(insertAfter);
  if (optInfo) {
    optInfoSym = toSymExpr(optInfo->get(1))->symbol();
  } else {
    SET_LINENO(insertAfter);
    optInfoSym = newTemp("optinfo", dtInt[INT_SIZE_DEFAULT]);
    optInfoSym->addFlag(FLAG_NO_CODEGEN);
    DefExpr* def = new DefExpr(optInfoSym);
    insertAfter->insertAfter(def);
    def->insertAfter(new CallExpr(PRIM_OPTIMIZATION_INFO, optInfoSym));
  }

  optInfoSym->addFlag(flag);
}

// Returns true if the PRIM_OPTIMIZATION_INFO includes this flag
bool hasOptimizationFlag(Expr* anchor, Flag flag) {
  Symbol* optInfoSym = NULL;
  CallExpr* optInfo = findOptimizationInfo(anchor);
  if (optInfo) {
    optInfoSym = toSymExpr(optInfo->get(1))->symbol();
    return optInfoSym->hasFlag(flag);
  }

  return false;
}
