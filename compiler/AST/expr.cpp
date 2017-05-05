/*
 * Copyright 2004-2017 Cray Inc.
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

// IPE: Provide the name of the symbol/variable being defined
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
    retval = isDirectlyUnderBlockStmt(this);
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

    } else if (isDirectlyUnderBlockStmt(expr)) {
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


bool Expr::inTree() {
  if (parentSymbol)
    return parentSymbol->inTree();
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

Expr* SymExpr::getFirstChild() {
  return NULL;
}

Expr* SymExpr::getFirstExpr() {
  return this;
}

void SymExpr::verify() {
  Expr::verify();

  if (astTag != E_SymExpr)
    INT_FATAL(this, "SymExpr::verify %12d: Bad astTag", id);

  if (var == NULL)
    INT_FATAL(this, "SymExpr::verify %12d: var is NULL", id);

  if (var->defPoint) {
    if (var->defPoint->parentSymbol == NULL)
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


Expr* UnresolvedSymExpr::getFirstChild() {
  return NULL;
}

Expr* UnresolvedSymExpr::getFirstExpr() {
  return this;
}

void
UnresolvedSymExpr::verify() {
  Expr::verify();
  if (astTag != E_UnresolvedSymExpr)
    INT_FATAL(this, "bad UnresolvedSymExpr::astTag");
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

Expr* DefExpr::getFirstChild() {
  return NULL;
}

Expr* DefExpr::getFirstExpr() {
  return this;
}

void DefExpr::verify() {
  Expr::verify();
  if (astTag != E_DefExpr) {
    INT_FATAL(this, "Bad DefExpr::astTag");
  }
  if (!sym) {
    INT_FATAL(this, "DefExpr has no sym");
  }
  if (toFnSymbol(sym) && (exprType || init))
    INT_FATAL(this, "Bad FnSymbol::defPoint");
  if (toArgSymbol(sym) && (exprType || init))
    INT_FATAL(this, "Bad ArgSymbol::defPoint");
  if (init && init->parentExpr != this)
    INT_FATAL(this, "Bad DefExpr::init::parentExpr");
  if (exprType && exprType->parentExpr != this)
    INT_FATAL(this, "Bad DefExpr::exprType::parentExpr");
  if (sym->defPoint != this)
    INT_FATAL(this, "Bad DefExpr::sym->defPoint");
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

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/


void DefExpr::prettyPrint(std::ostream *o) {
  *o << "<DefExprType>";
}

static void callExprHelper(CallExpr* call, BaseAST* arg) {
  if (!arg)
    return;
  if (toSymbol(arg) || toExpr(arg))
    call->insertAtTail(arg);
  else
    INT_FATAL(call, "Bad argList in CallExpr constructor");
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

CallExpr::CallExpr(BaseAST* base,
                   BaseAST* arg1,
                   BaseAST* arg2,
                   BaseAST* arg3,
                   BaseAST* arg4,
                   BaseAST* arg5) :
  Expr(E_CallExpr),
  primitive(NULL),
  baseExpr(NULL),
  argList(),
  partialTag(false),
  methodTag(false),
  square(false)
{
  if (Symbol* b = toSymbol(base)) {
    baseExpr = new SymExpr(b);
  } else if (Expr* b = toExpr(base)) {
    baseExpr = b;
  } else {
    INT_FATAL(this, "Bad baseExpr in CallExpr constructor");
  }

  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  callExprHelper(this, arg5);

  argList.parent = this;

  gCallExprs.add(this);
}


CallExpr::CallExpr(PrimitiveOp* prim,
                   BaseAST*     arg1,
                   BaseAST*     arg2,
                   BaseAST*     arg3,
                   BaseAST*     arg4,
                   BaseAST*     arg5) :
  Expr(E_CallExpr),
  primitive(prim),
  baseExpr(NULL),
  argList(),
  partialTag(false),
  methodTag(false),
  square(false) {
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  callExprHelper(this, arg5);

  argList.parent = this;

  gCallExprs.add(this);
}

CallExpr::CallExpr(PrimitiveTag prim,
                   BaseAST*     arg1,
                   BaseAST*     arg2,
                   BaseAST*     arg3,
                   BaseAST*     arg4,
                   BaseAST*     arg5) :
  Expr(E_CallExpr),
  primitive(primitives[prim]),
  baseExpr(NULL),
  argList(),
  partialTag(false),
  methodTag(false),
  square(false) {
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  callExprHelper(this, arg5);

  argList.parent = this;

  gCallExprs.add(this);
}


CallExpr::CallExpr(const char* name,
                   BaseAST*    arg1,
                   BaseAST*    arg2,
                   BaseAST*    arg3,
                   BaseAST*    arg4,
                   BaseAST*    arg5) :
  Expr(E_CallExpr),
  primitive(NULL),
  baseExpr(new UnresolvedSymExpr(name)),
  argList(),
  partialTag(false),
  methodTag(false),
  square(false) {
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  callExprHelper(this, arg5);

  argList.parent = this;

  gCallExprs.add(this);
}


CallExpr::~CallExpr() {
}


bool CallExpr::isEmpty() const {
  return primitive == NULL && baseExpr == NULL;
}

bool CallExpr::isPrimitive() const {
  return primitive != NULL;
}

bool CallExpr::isPrimitive(PrimitiveTag primitiveTag) const {
  return primitive && primitive->tag == primitiveTag;
}

bool CallExpr::isPrimitive(const char* primitiveName) const {
  return primitive && !strcmp(primitive->name, primitiveName);
}

Expr* CallExpr::getFirstChild() {
  Expr* retval = NULL;

  if (baseExpr)
    retval = baseExpr;

  else if (argList.head)
    retval = argList.head;

  return retval;
}

Expr* CallExpr::getFirstExpr() {
  Expr* retval = NULL;

  if (baseExpr != NULL)
    retval = baseExpr->getFirstExpr();

  else if (argList.head != NULL)
    retval = argList.head->getFirstExpr();

  else
    retval = this;

  return retval;
}

Expr* CallExpr::getNextExpr(Expr* expr) {
  Expr* retval = this;

  if (expr == baseExpr && argList.head != NULL)
    retval = argList.head->getFirstExpr();

  return retval;
}

void CallExpr::verify() {
  Expr::verify();

  if (astTag != E_CallExpr) {
    INT_FATAL(this, "Bad CallExpr::astTag");
  }

  if (! parentExpr)
    INT_FATAL(this, "Every CallExpr is expected to have a parentExpr");

  if (argList.parent != this)
    INT_FATAL(this, "Bad AList::parent in CallExpr");

  if (baseExpr && baseExpr->parentExpr != this)
    INT_FATAL(this, "Bad baseExpr::parent in CallExpr");

  if (normalized && isPrimitive(PRIM_RETURN)) {
    FnSymbol* fn  = toFnSymbol(parentSymbol);
    SymExpr*  sym = toSymExpr(get(1));

    if (!fn)
      INT_FATAL(this, "Return is not in a function.");

    if (fn->body->body.last() != this)
      INT_FATAL(this, "Return is in middle of function.");

    if (!sym)
      INT_FATAL(this, "Return does not return a symbol.");
  }

  for_actuals(actual, this) {
    if (actual->parentExpr != this)
      INT_FATAL(this, "Bad CallExpr::argList::parentExpr");

    if (isSymExpr(actual)                           &&
        toSymExpr(actual)->symbol() == gMethodToken &&
        actual != this->get(1))
      INT_FATAL(this,
                "If present, the method token must be the first argument.");
  }

  if (primitive) {
    if (!(PRIM_UNKNOWN <= primitive->tag && primitive->tag < NUM_KNOWN_PRIMS))
      INT_FATAL(this, "invalid primitive->tag");

    switch (primitive->tag) {
    case PRIM_BLOCK_PARAM_LOOP:
    case PRIM_BLOCK_WHILEDO_LOOP:
    case PRIM_BLOCK_DOWHILE_LOOP:
    case PRIM_BLOCK_FOR_LOOP:
    case PRIM_BLOCK_BEGIN:
    case PRIM_BLOCK_COBEGIN:
    case PRIM_BLOCK_COFORALL:
    case PRIM_BLOCK_ON:
    case PRIM_BLOCK_BEGIN_ON:
    case PRIM_BLOCK_COBEGIN_ON:
    case PRIM_BLOCK_COFORALL_ON:
    case PRIM_BLOCK_LOCAL:
      if (toBlockStmt(parentExpr)) {

      } else {
        INT_FATAL(this, "blockInfo-type CallExpr not in a BlockStmt");
      }
      break;
    case PRIM_BLOCK_UNLOCAL:
      INT_FATAL("PRIM_BLOCK_UNLOCAL between passes");
      break;
    case PRIM_TYPE_INIT:
      // A "type init" call is always expected to have a parent.
      INT_ASSERT(toCallExpr(this->parentExpr));
      break;
    default:
      break; // do nothing
    }
  }

  verifyNotOnList(baseExpr);
}

CallExpr* CallExpr::copyInner(SymbolMap* map) {
  CallExpr* _this = 0;

  if (primitive)
    _this = new CallExpr(primitive);
  else
    _this = new CallExpr(COPY_INT(baseExpr));

  for_actuals(expr, this)
    _this->insertAtTail(COPY_INT(expr));

  _this->primitive  = primitive;
  _this->partialTag = partialTag;
  _this->methodTag  = methodTag;
  _this->square     = square;

  return _this;
}


void CallExpr::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == baseExpr) {
    baseExpr = newAst;
  } else {
    INT_FATAL(this, "Unexpected case in CallExpr::replaceChild");
  }
}


void CallExpr::insertAtHead(BaseAST* ast) {
  Expr* toInsert = NULL;

  if (Symbol* a = toSymbol(ast))
    toInsert = new SymExpr(a);
  else
    toInsert = toExpr(ast);

  argList.insertAtHead(toInsert);

  parent_insert_help(this, toInsert);
}


void CallExpr::insertAtTail(BaseAST* ast) {
  Expr* toInsert = NULL;

  if (Symbol* a = toSymbol(ast))
    toInsert = new SymExpr(a);
  else
    toInsert = toExpr(ast);

  argList.insertAtTail(toInsert);

  parent_insert_help(this, toInsert);
}

void CallExpr::setUnresolvedFunction(const char* name) {
  // Currently a PRIM_OP
  if (primitive != NULL) {
    primitive = NULL;
    baseExpr  = new UnresolvedSymExpr(astr(name));

    parent_insert_help(this, baseExpr);

  } else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(baseExpr)) {
    use->unresolved = astr(name);

  } else if (SymExpr*           se  = toSymExpr(baseExpr)) {
    se->replace(new UnresolvedSymExpr(name));

  } else {
    INT_ASSERT(false);
  }
}

bool CallExpr::isResolved() const {
  return (resolvedFunction() != NULL) ? true : false;
}

FnSymbol* CallExpr::resolvedFunction() const {
  FnSymbol* retval = NULL;

  // A PRIM-OP
  if (primitive != NULL) {
    INT_ASSERT(baseExpr  == NULL);

  // A Chapel call
  } else if (baseExpr != NULL) {
    if (isUnresolvedSymExpr(baseExpr) == true) {

    } else if (SymExpr* base = toSymExpr(baseExpr)) {
      if (FnSymbol* fn = toFnSymbol(base->symbol())) {
        retval = fn;

      // Probably an array index
      } else if (isArgSymbol(base->symbol())  == true ||
                 isVarSymbol(base->symbol())  == true) {

      // A type specifier
      } else if (isTypeSymbol(base->symbol()) == true) {

      } else {
        INT_ASSERT(false);
      }

    } else if (CallExpr* subCall = toCallExpr(baseExpr)) {
      // Confirm that this is a partial call
      INT_ASSERT(subCall->partialTag == true);

    } else {
      INT_ASSERT(false);
    }

  // The CallExpr has been purged during resolve
  } else {
    INT_ASSERT(false);
  }

  return retval;
}

void CallExpr::setResolvedFunction(FnSymbol* fn) {
  // Currently a PRIM_OP
  if (primitive != NULL) {
    primitive = NULL;
    baseExpr  = new SymExpr(fn);

    parent_insert_help(this, baseExpr);

  } else if (isUnresolvedSymExpr(baseExpr) == true) {
    baseExpr->replace(new SymExpr(fn));

  } else if (SymExpr* se = toSymExpr(baseExpr)) {
    se->setSymbol(fn);

  } else {
    INT_ASSERT(false);
  }
}

FnSymbol* CallExpr::theFnSymbol() const {
  FnSymbol* retval = NULL;

  if (SymExpr* base = toSymExpr(baseExpr))
    retval = toFnSymbol(base->symbol());

  return retval;
}


bool CallExpr::isNamed(const char* name) {
  if (SymExpr* base = toSymExpr(baseExpr))
    if (strcmp(base->symbol()->name, name) == 0)
      return true;

  if (UnresolvedSymExpr* base = toUnresolvedSymExpr(baseExpr))
    if (strcmp(base->unresolved, name) == 0)
      return true;

  return false;
}


int CallExpr::numActuals() const {
  return argList.length;
}


Expr* CallExpr::get(int index) const {
  return argList.get(index);
}


FnSymbol* CallExpr::findFnSymbol(void) {
  FnSymbol* fn = NULL;

  if (SymExpr* variable = toSymExpr(baseExpr))
    fn = toFnSymbol(variable->symbol());

  if (!fn)
    INT_FATAL(this, "Cannot find FnSymbol in CallExpr");

  return fn;
}

bool CallExpr::isCast(void) {
  return isNamed("_cast");
}

Expr* CallExpr::castFrom(void) {
  INT_ASSERT(isCast());

  return get(2);
}

Expr* CallExpr::castTo(void) {
  INT_ASSERT(isCast());

  return get(1);
}

CallExpr* createCast(BaseAST* src, BaseAST* toType)
{
  CallExpr* expr = new CallExpr("_cast", toType, src);
  return expr;
}


QualifiedType CallExpr::qualType(void) {
  QualifiedType retval(NULL);

  if (primitive) {
    retval = primitive->returnInfo(this);

  } else if (isResolved()) {
    retval = QualifiedType(resolvedFunction()->retType);

  } else {
    retval = QualifiedType(dtUnknown);
  }

  return retval;
}

void CallExpr::prettyPrint(std::ostream *o) {
  if (FnSymbol* fn = theFnSymbol()) {
    if      (fn->hasFlag(FLAG_BEGIN_BLOCK))
      *o << "begin";
    else if (fn->hasFlag(FLAG_ON_BLOCK))
      *o << "on";
  }

  bool array   = false;
  bool unusual = false;

  if (baseExpr != NULL) {
    if (UnresolvedSymExpr *expr = toUnresolvedSymExpr(baseExpr)) {
      if (strcmp(expr->unresolved, "*") == 0){
        unusual = true;
        argList.first()->prettyPrint(o);
        *o << "*(";
        argList.last()->prettyPrint(o);
        *o << ")";
      } else if (strcmp(expr->unresolved, "chpl_build_bounded_range") == 0) {
        // Note that this code path is only used by chpldoc to create function
        // return signatures and the only place a range will show up is in a
        // fully specified array, in which case the range must be fully bounded
        argList.first()->prettyPrint(o);
        *o << "..";
        argList.last()->prettyPrint(o);
        unusual = true;
      } else if (strcmp(expr->unresolved,
                        "chpl__ensureDomainExpr") == 0) {
        unusual = true;
        for_alist(expr, argList) {
          if (expr != argList.first()) {
            *o << ", ";
          }
          expr->prettyPrint(o);
        }
      } else if (strcmp(expr->unresolved,
                        "chpl__buildArrayRuntimeType") == 0) {
        *o << "[";
        array = true;
      } else if (strcmp(expr->unresolved,
                        "chpl__buildDomainRuntimeType") == 0) {
        *o << "domain(";
        argList.last()->prettyPrint(o);
        *o << ")";
        unusual = true;
      } else if (strcmp(expr->unresolved,
                        "_build_tuple") != 0) {
        baseExpr->prettyPrint(o);
      }
    } else {
      baseExpr->prettyPrint(o);
    }
  } else if (primitive != NULL) {
    if (primitive->tag == PRIM_INIT ||
      primitive->tag == PRIM_TYPE_INIT) {
      unusual = true;
      argList.head->prettyPrint(o);
    }
  }

  if (!array && !unusual)
    *o << "(";

  if (!unusual) {
    for_alist(expr, argList) {
      if (expr != argList.first()) {
        if (array && expr == argList.last()) {
          *o << "] ";
        } else {
          *o << ", ";
        }
      }
      expr->prettyPrint(o);
    }

    if (array && argList.first() == argList.last())
      *o << "]";
  }

  if (!array && !unusual) {
    *o << ")";
  }
}

void CallExpr::accept(AstVisitor* visitor) {
  if (visitor->enterCallExpr(this) == true) {

    if (baseExpr)
      baseExpr->accept(visitor);

    for_alist(next_ast, argList)
      next_ast->accept(visitor);

    visitor->exitCallExpr(this);
  }
}

bool CallExpr::isRefExternStarTuple(Symbol* formal, Expr* actual) const {
  Symbol* formalSym  = formal->type->symbol;
  Symbol* formalVal  = formalSym->getValType()->symbol;

  bool  retval       = false;

  if (formal->isRef()                     == true &&
      formalVal->hasFlag(FLAG_STAR_TUPLE) == true &&
      actual->isRef()                     == true)
    retval = true;

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

ContextCallExpr::ContextCallExpr() :
  Expr(E_ContextCallExpr),
  options()
{
  options.parent = this;
  gContextCallExprs.add(this);
}

ContextCallExpr*
ContextCallExpr::copyInner(SymbolMap* map) {
  ContextCallExpr* _this = 0;
  _this = new ContextCallExpr();
  for_alist(expr, options)
    _this->options.insertAtTail(COPY_INT(expr));
  return _this;
}

CallExpr* getDesignatedCall(const ContextCallExpr* a) {
  return toCallExpr(a->options.tail);
}

void
ContextCallExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  INT_FATAL(this, "unexpected case in ContextCallExpr::replaceChild");
}

void
ContextCallExpr::verify() {
  Expr::verify();
  if (astTag != E_ContextCallExpr)
    INT_FATAL(this, "bad ContextCallExpr::astTag");
  for_alist(expr, options) {
    if (expr->parentExpr != this)
      INT_FATAL(this, "Bad ContextCallExpr::options::parentExpr");
    if (isContextCallExpr(expr))
      INT_FATAL(this, "ContextCallExpr cannot contain a ContextCallExpr");
    if (!isCallExpr(expr))
      INT_FATAL(this, "ContextCallExpr must contain only CallExpr");
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
  CallExpr* mainCall = getDesignatedCall(this);
  if (mainCall)
    return mainCall->qualType();
  return QualifiedType(dtUnknown);
}

void ContextCallExpr::prettyPrint(std::ostream *o) {
  *o << "(options";
  for_alist(expr, options) {
    *o << " ";
    expr->prettyPrint(o);
  }
  *o << " )";
}

Expr* ContextCallExpr::getFirstChild() {
  return options.head;
}

Expr* ContextCallExpr::getFirstExpr() {
  return options.head->getFirstExpr();
}


void ContextCallExpr::setRefRValueOptions(CallExpr* refCall,
                                          CallExpr* rvalueCall) {
  // Storing the ref call after the value call allows a
  // postorder traversal to skip the value call.
  // The order is important also - the first is always the value.

  options.insertAtTail(rvalueCall);
  parent_insert_help(this, rvalueCall);
  options.insertAtTail(refCall);
  parent_insert_help(this, refCall);
}

void ContextCallExpr::setRefValueConstRefOptions(CallExpr* refCall,
                                                 CallExpr* valueCall,
                                                 CallExpr* constRefCall) {

  // ContextCallExpr::getCalls depends on this order
  options.insertAtTail(constRefCall);
  parent_insert_help(this, constRefCall);
  options.insertAtTail(valueCall);
  parent_insert_help(this, valueCall);
  options.insertAtTail(refCall);
  parent_insert_help(this, refCall);
}

CallExpr* ContextCallExpr::getRefCall() {
  // This used to check for the call with RET_REF, but
  // the return tag might change during resolution. So
  // instead we rely on them always being in order.
  return toCallExpr(options.tail);
}

CallExpr* ContextCallExpr::getRValueCall() {
  return toCallExpr(options.head);
}

void  ContextCallExpr::getCalls(CallExpr*& refCall,
                                CallExpr*& valueCall,
                                CallExpr*& constRefCall) {
  refCall      = NULL;
  valueCall    = NULL;
  constRefCall = NULL;

  if (options.length == 2) {
    refCall = getRefCall();

    CallExpr* rvalueCall = getRValueCall();
    FnSymbol* fn         = rvalueCall->resolvedFunction();

    INT_ASSERT(fn);

    if (fn->retTag == RET_CONST_REF) {
      constRefCall = rvalueCall;
    } else {
      valueCall = rvalueCall;
    }

  } else if (options.length == 3) {
    // Note: it would be nicer to check retTag to decide between
    // ref / value versions. However, doing so is challenging because
    // of the way that iterator functions no longer have the original
    // retTag.
    constRefCall = toCallExpr(options.get(1));
    valueCall    = toCallExpr(options.get(2));
    refCall      = toCallExpr(options.get(3));

  } else {
    INT_FATAL("Bad ContextCallExpr options");
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

ForallExpr::ForallExpr(Expr* indices,
                       Expr* iteratorExpr,
                       Expr* expr,
                       Expr* cond,
                       bool maybeArrayType,
                       bool zippered) :
  Expr(E_ForallExpr),
  indices(indices),
  iteratorExpr(iteratorExpr),
  expr(expr),
  cond(cond),
  maybeArrayType(maybeArrayType),
  zippered(zippered)
{
  gForallExprs.add(this);
}

ForallExpr* ForallExpr::copyInner(SymbolMap* map) {
  return new ForallExpr(
    COPY_INT(indices),
    COPY_INT(iteratorExpr),
    COPY_INT(expr),
    COPY_INT(cond),
    maybeArrayType,
    zippered);
}

void ForallExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == indices)
    indices = new_ast;
  else if (old_ast == iteratorExpr)
    iteratorExpr = new_ast;
  else if (old_ast == expr)
    expr = new_ast;
  else if (old_ast == cond)
    cond = new_ast;
  else
    INT_FATAL(this, "unexpected case in ForallExpr::replaceChild");
}

void
ForallExpr::verify() {
  Expr::verify();
  if (astTag != E_ForallExpr)
    INT_FATAL(this, "bad ForallExpr::astTag");
  INT_FATAL(this, "ForallExpr::verify() is not implemented");
}

void ForallExpr::accept(AstVisitor* visitor) {
  INT_FATAL(this, "ForallExpr::accept() is not implemented");
}

Expr* ForallExpr::getFirstChild() {
  INT_FATAL(this, "ForallExpr::getFirstChild() is not implemented");
  return NULL;
}

Expr* ForallExpr::getFirstExpr() {
  INT_FATAL(this, "ForallExpr::getFirstExpr() is not implemented");
  return NULL;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

NamedExpr::NamedExpr(const char* init_name, Expr* init_actual) :
  Expr(E_NamedExpr),
  name(init_name),
  actual(init_actual)
{
  gNamedExprs.add(this);
}


Expr* NamedExpr::getFirstChild() {
  return (actual != NULL) ? actual : NULL ;
}

Expr* NamedExpr::getFirstExpr() {
  return (actual != NULL) ? actual->getFirstExpr() : this;
}

void NamedExpr::verify() {
  Expr::verify();
  if (astTag != E_NamedExpr) {
    INT_FATAL(this, "Bad NamedExpr::astTag");
  }
  if (actual && actual->parentExpr != this)
    INT_FATAL(this, "Bad NamedExpr::actual::parentExpr");
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
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      if (VarSymbol *v = toVarSymbol(l->symbol())) {
        if (v->immediate) {
          if (v->immediate->const_kind == NUM_KIND_INT) {
            *i = v->immediate->int_value();
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool
get_uint(Expr *e, uint64_t *i) {
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      if (VarSymbol *v = toVarSymbol(l->symbol())) {
        if (v->immediate) {
          if (v->immediate->const_kind == NUM_KIND_UINT) {
            *i = v->immediate->uint_value();
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool
get_string(Expr *e, const char **s) {
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      if (VarSymbol *v = toVarSymbol(l->symbol())) {
        if (v->immediate && v->immediate->const_kind == CONST_KIND_STRING) {
          *s = v->immediate->v_string;
          return true;
        }
      }
    }
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

// This builds an allocation of enough space to hold a variable of the
// given type.
//
// This function should be used *before* resolution
CallExpr* callChplHereAlloc(Type *type, VarSymbol* md) {
  INT_ASSERT(!resolved);
  // Since the type is not necessarily known, resolution will fix up
  // this sizeof() call to take the resolved type of s as an argument
  CallExpr*  sizeExpr  = new CallExpr(PRIM_SIZEOF, new SymExpr(type->symbol));
  VarSymbol* mdExpr    = (md != NULL) ? md : newMemDesc(type);
  CallExpr*  allocExpr = new CallExpr("chpl_here_alloc", sizeExpr, mdExpr);
  // Again, as we don't know the type yet, we leave it to resolution
  // to put in the cast to the proper type
  return allocExpr;
}

// This insert normalized call expressions for allocation of enough
// space to hold a variable of the given type.
//
// This function should be used *after* resolution
void insertChplHereAlloc(Expr *call, bool insertAfter, Symbol *sym,
                         Type* t, VarSymbol* md) {
  INT_ASSERT(resolved);
  AggregateType* ct = toAggregateType(toTypeSymbol(t->symbol)->type);
  Symbol* sizeTmp = newTemp("chpl_here_alloc_size", SIZE_TYPE);
  CallExpr *sizeExpr = new CallExpr(PRIM_MOVE, sizeTmp,
                                    new CallExpr(PRIM_SIZEOF,
                                                 (ct != NULL) ?
                                                 ct->symbol : t->symbol));
  VarSymbol* mdExpr = (md != NULL) ? md : newMemDesc(t);
  Symbol *allocTmp = newTemp("chpl_here_alloc_tmp", dtCVoidPtr);
  CallExpr* allocExpr = new CallExpr(PRIM_MOVE, allocTmp,
                                     new CallExpr(gChplHereAlloc,
                                                  sizeTmp, mdExpr));
  CallExpr* castExpr = new CallExpr(PRIM_MOVE, sym,
                                    new CallExpr(PRIM_CAST,
                                                 t->symbol, allocTmp));
  if (insertAfter) {
    call->insertAfter(castExpr);
    call->insertAfter(allocExpr);
    call->insertAfter(sizeExpr);
    call->insertAfter(new DefExpr(allocTmp));
    call->insertAfter(new DefExpr(sizeTmp));
  } else {
    call->insertBefore(new DefExpr(sizeTmp));
    call->insertBefore(new DefExpr(allocTmp));
    call->insertBefore(sizeExpr);
    call->insertBefore(allocExpr);
    call->insertBefore(castExpr);
  }
}


// Similar to callChplHereAlloc(), above but this can be called any time
CallExpr* callChplHereFree(BaseAST* p) {
  // Don't have a good way to do the following?
  //if (fNoMemoryFrees)
  //  return;

  if( p->typeInfo()->symbol->hasFlag(FLAG_DATA_CLASS))
    INT_FATAL(p->typeInfo()->symbol, "cannot delete data class");

  // Note: Prior to resolution, we do not have complete type info, and
  // so resolution will fix up this cast operation if a dereference is
  // needed
  CallExpr* castExpr = new CallExpr(PRIM_CAST_TO_VOID_STAR, p);
  if (!resolved)
    return new CallExpr("chpl_here_free", castExpr);
  else {
    return new CallExpr(gChplHereFree, castExpr);
  }
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
