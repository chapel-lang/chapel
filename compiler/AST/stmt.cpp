/*
 * Copyright 2004-2018 Cray Inc.
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

#include "stmt.h"

#include "astutil.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stlUtil.h"
#include "stringutil.h"

#include "AstVisitor.h"

#include <cstring>
#include <algorithm>
#include <vector>

// remember these so we can update their labels' iterResumeGoto
Map<GotoStmt*,GotoStmt*> copiedIterResumeGotos;

// remember these so we can remove their iterResumeGoto
Vec<LabelSymbol*> removedIterResumeLabels;

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Stmt::Stmt(AstTag astTag) : Expr(astTag) {

}
Stmt::~Stmt() {

}

bool Stmt::isStmt() const {
  return true;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

BlockStmt::BlockStmt(Expr* initBody, BlockTag initBlockTag) :
  Stmt(E_BlockStmt) {


  blockTag      = initBlockTag;
  useList       = NULL;
  userLabel     = NULL;
  byrefVars     = NULL;
  forallIntents = NULL;
  blockInfo     = NULL;

  body.parent   = this;

  if (initBody)
    body.insertAtTail(initBody);

  gBlockStmts.add(this);
}


BlockStmt::~BlockStmt() {
  if (forallIntents)
    delete forallIntents;
}

void BlockStmt::verify() {
  Expr::verify();

  if (astTag != E_BlockStmt) {
    INT_FATAL(this, "BlockStmt::verify. Bad astTag");
  }

  if (body.parent != this)
    INT_FATAL(this, "BlockStmt::verify. Bad body.parent");

  for_alist(expr, body) {
    if (expr->parentExpr != this)
      INT_FATAL(this, "BlockStmt::verify. Bad body.expr->parentExpr");
  }

  if (blockInfo != NULL && blockInfo->parentExpr != this) {
    INT_FATAL(this, "BlockStmt::verify. Bad blockInfo->parentExpr");
  }

  if (useList   != NULL && useList->parentExpr   != this) {
    INT_FATAL(this, "BlockStmt::verify. Bad useList->parentExpr");
  }

  if (byrefVars) {
    if (byrefVars->parentExpr != this) {
      INT_FATAL(this, "BlockStmt::verify. Bad byrefVars->parentExpr");
    }

    if (!byrefVars->isPrimitive(PRIM_ACTUALS_LIST)) {
      INT_FATAL(this, "BlockStmt::byrefVars not PRIM_ACTUALS_LIST");
    }

    for_actuals(varExp, byrefVars) {
      if (!isSymExpr(varExp) && !isUnresolvedSymExpr(varExp)) {
        INT_FATAL(this, "BlockStmt::verify. Bad expression kind in byrefVars");
      }
    }
  }

  if (forallIntents) {
    forallIntents->verifyFI(this);
  }

  if (byrefVars && forallIntents) {
    INT_FATAL(this,"BlockStmt: byrefVars and forallIntents are both non-NULL");
  }

  verifyNotOnList(useList);
  verifyNotOnList(byrefVars);
  verifyNotOnList(blockInfo);
}


BlockStmt*
BlockStmt::copyInner(SymbolMap* map) {
  BlockStmt* _this = new BlockStmt();

  _this->blockTag  = blockTag;
  _this->blockInfo = COPY_INT(blockInfo);
  _this->useList   = COPY_INT(useList);
  _this->byrefVars = COPY_INT(byrefVars);
  _this->forallIntents = COPY_INT(forallIntents);

  for_alist(expr, body) {
    Expr* copy = COPY_INT(expr);
    _this->insertAtTail(copy);

    if (DefExpr* def = toDefExpr(copy)) {
      if (TypeSymbol* ts = toTypeSymbol(def->sym)) {
        if (EnumType* et = toEnumType(ts->type)) {
          // ensure we have the size, cast functions, etc.
          // Lydia NOTE: This relies on making no copies of enum types prior to
          // buildDefaultFunctions().  The creation must happen here because
          // otherwise the EnumType will not have the correct symbol, and the
          // symbol will not be in the tree.

          // Also, NOTE: This does not generate new assignment and enumerate
          // functions for the enum, as those are already local to the function
          // being instantiated and so will get copied independently and
          // updated when we replace the old type reference with the new one.
          buildFarScopeEnumFunctions(et);
        }
      }
    }
  }

  return _this;
}


// Note that newAst can be NULL to reflect deletion
void BlockStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  CallExpr* oldExpr = toCallExpr(oldAst);
  CallExpr* newExpr = (newAst != NULL) ? toCallExpr(newAst) : NULL;
  bool handled = true;

  if (oldExpr == NULL)
    handled = false;

  else if (oldExpr == blockInfo)
    blockInfo = newExpr;

  else if (oldExpr == useList)
    useList   = newExpr;

  else if (oldExpr == byrefVars)
    byrefVars = newExpr;

  else
    handled = false;

  if (!handled &&
      forallIntents && forallIntents->replaceChildFI(oldAst, newAst))
    handled = true; // OK

  if (!handled)
    INT_FATAL(this, "BlockStmt::replaceChild. Failed to match the oldAst ");

  // TODO: Handle the above special cases uniformly by specializing the
  // traversal of the children by block statement type.  I think blockInfo is
  // being deprecated anyway....
}

void BlockStmt::removeForallIntents() {
  forallIntents->removeFI(this);
  forallIntents = NULL;
}

//
// Return true when parentExpr is a BlockStmt, except for exprs
// under BlockStmt::forallIntents.
//
bool isDirectlyUnderBlockStmt(const Expr* expr) {
  if (BlockStmt* parent = toBlockStmt(expr->parentExpr))
    return !astUnderFI(expr, parent->forallIntents);

  return false;
}

CallExpr* BlockStmt::blockInfoGet() const {
  return blockInfo;
}

CallExpr* BlockStmt::blockInfoSet(CallExpr* expr) {
  blockInfo = expr;

  return blockInfo;
}

// The BISON productions generate a large number of scope-less BlockStmt
// as an artifact of the processing.  This function is intended to be
// called from well-defined points in the parser to collapse these during
// the construction of the parse tree.
void
BlockStmt::appendChapelStmt(BlockStmt* stmt) {
  if (canFlattenChapelStmt(stmt) == true) {
    for_alist(expr, stmt->body) {
      this->insertAtTail(expr->remove());
    }
  } else {
    insertAtTail(stmt);
  }
}

//
// The Parser routinely generates scope-less BlockStmts to
// represent Chapel "statements".  In some cases it is possible
// to flatten the BlockStmt when appending a Chapel statement to
// a statement list.  This function returns TRUE if this is safe
//
//
// The BlockStmt must in fact be scope-less.  Then one or more of
// the following must be true
//
// 2014/07/03:  Pass 1
//   a) The BlockStmt contains a single item
//

bool
BlockStmt::canFlattenChapelStmt(const BlockStmt* stmt) const {
  bool retval = false;

  if (stmt->isScopeless() == true) {
    if (stmt->length() == 1)
      retval =  true;

    else
      retval = false;
  }

  return retval;
}

//
// "Remove the curly braces":
// move all contents out of this BlockStmt and remove it.
//
void BlockStmt::flattenAndRemove() {
  for_alist(stmt, this->body)
    this->insertBefore(stmt->remove());
  INT_ASSERT(this->body.length == 0);
  this->remove();
}

Expr*
BlockStmt::getFirstExpr() {
  Expr* retval = 0;

  if (blockInfo != 0)
    retval = blockInfo->getFirstExpr();

  else if (body.head      != 0)
    retval = body.head->getFirstExpr();

  else
    retval = this;

  return retval;
}

Expr*
BlockStmt::getNextExpr(Expr* expr) {
  Expr* retval = this;

  if (expr == blockInfo && body.head != 0)
    retval = body.head->getFirstExpr();

  return retval;
}

void
BlockStmt::insertAtHead(Expr* ast) {
  body.insertAtHead(ast);
}


void
BlockStmt::insertAtTail(Expr* ast) {
  body.insertAtTail(ast);
}

void
BlockStmt::insertAtHead(AList exprs) {
  for_alist_backward(expr, exprs) {
    expr->remove();
    insertAtHead(expr);
  }
}


void
BlockStmt::insertAtTail(AList exprs) {
  for_alist(expr, exprs) {
    expr->remove();
    insertAtTail(expr);
  }
}

void
BlockStmt::insertAtHead(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtHead(new_Expr(format, args));
  va_end(args);
}


void
BlockStmt::insertAtTail(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtTail(new_Expr(format, args));
  va_end(args);
}


// Returns true if this statement (expression) causes a change in flow.
// When inserting cleanup code, it must be placed ahead of such flow
// statements, or it will be skipped (which means it's in the wrong place).
static bool isFlowStmt(Expr* stmt) {
  bool retval = false;

  // A goto is definitely a jump.
  if (isGotoStmt(stmt)) {
    retval = true;

  // A return primitive works like a jump. (Nothing should appear after it.)
  } else if (CallExpr* call = toCallExpr(stmt)) {
    if (call->isPrimitive(PRIM_RETURN))
      retval = true;

    // _downEndCount is treated like a flow statement because we do not want to
    // insert autoDestroys after the task says "I'm done."  This can result in
    // false-positive memory allocation errors because the waiting (parent
    // task) can then proceed to test that the subtask has not leaked before
    // the subtask release locally-(dynamically-)allocated memory.
    else if (FnSymbol* fn = call->resolvedFunction()) {
      retval = (strcmp(fn->name, "_downEndCount") == 0) ? true : false;
    }
  }

  return retval;
}

// Insert an expression at the end of a block, but before a flow statement at
// the end of the block.  The two cases we are concerned with are a goto or a
// return appearing at the end of a block
void
BlockStmt::insertAtTailBeforeFlow(Expr* ast) {
  if (isFlowStmt(body.tail))
    body.tail->insertBefore(ast);
  else
    body.insertAtTail(ast);
}

bool
BlockStmt::isRealBlockStmt() const {
  return blockInfo == 0;
}

bool
BlockStmt::isScopeless() const {
  return blockTag == BLOCK_SCOPELESS;
}

bool
BlockStmt::isBlockType(PrimitiveTag tag) const {
  return blockInfo != 0 && blockInfo->isPrimitive(tag) == true;
}

bool
BlockStmt::isLoopStmt() const {
  return false;
}

bool
BlockStmt::isWhileStmt() const {
  return false;
}

bool
BlockStmt::isWhileDoStmt() const {
  return false;
}

bool
BlockStmt::isDoWhileStmt() const {
  return false;
}

bool
BlockStmt::isParamForLoop() const {
  return false;
}

bool
BlockStmt::isForLoop() const {
  return false;
}

bool
BlockStmt::isCoforallLoop() const {
  return false;
}

bool
BlockStmt::isCForLoop() const {
  return false;
}

void
BlockStmt::checkConstLoops() {

}

bool
BlockStmt::deadBlockCleanup() {
  return false;
}

int
BlockStmt::length() const {
  return body.length;
}


void
BlockStmt::useListAdd(ModuleSymbol* mod) {
  useListAdd(new UseStmt(mod));
}

void
BlockStmt::useListAdd(UseStmt* use) {
  if (useList == NULL) {
    useList = new CallExpr(PRIM_USED_MODULES_LIST);

    if (parentSymbol)
      insert_help(useList, this, parentSymbol);
  }

  useList->insertAtTail(use);
}


// Remove a module from the list of modules used by the module this block
// statement belongs to. The list of used modules is stored in useList
bool
BlockStmt::useListRemove(ModuleSymbol* mod) {
  bool retval = false;

  if (useList != NULL) {
    for_alist(expr, useList->argList) {
      if (SymExpr* symExpr = toSymExpr(expr)) {
        if (ModuleSymbol* curMod = toModuleSymbol(symExpr->symbol())) {
          if (curMod == mod) {
            symExpr->remove();

            retval = true;
            break;
          }
        }
      }
    }
  }

  return retval;
}

void
BlockStmt::useListClear() {
  if (useList != NULL) {

    for_alist(expr, useList->argList) {
      expr->remove();
    }

    // It's possible that this use definition is not alive
    if (isAlive(useList)) {
      useList->remove();
    }

    useList = NULL;
  }
}

void
BlockStmt::accept(AstVisitor* visitor) {
  if (visitor->enterBlockStmt(this) == true) {
    for_alist(next_ast, body) {
      next_ast->accept(visitor);
    }

    if (blockInfo) {
      blockInfo->accept(visitor);
    }

    if (useList) {
      useList->accept(visitor);
    }

    if (byrefVars) {
      byrefVars->accept(visitor);
    }

    if (forallIntents) {
      forallIntents->acceptFI(visitor);
    }

    visitor->exitBlockStmt(this);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

CondStmt::CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt) :
  Stmt(E_CondStmt) {

  condExpr = iCondExpr;
  thenStmt = NULL;
  elseStmt = NULL;

  if (Expr* s = toExpr(iThenStmt)) {
    BlockStmt* bs = toBlockStmt(s);

    if (bs && bs->blockTag == BLOCK_NORMAL && bs->isRealBlockStmt()) {
      thenStmt = bs;
    } else {
      thenStmt = new BlockStmt(s);
    }

  } else {
    INT_FATAL(iThenStmt, "Bad then-stmt passed to CondStmt constructor");
  }

  if (iElseStmt != NULL) {
    if (Expr* s = toExpr(iElseStmt)) {
      BlockStmt* bs = toBlockStmt(s);

      if (bs && bs->blockTag == BLOCK_NORMAL && bs->isRealBlockStmt()) {
        elseStmt = bs;
      } else {
        elseStmt = new BlockStmt(s);
      }

    } else {
      INT_FATAL(iElseStmt, "Bad else-stmt passed to CondStmt constructor");
    }
  }

  gCondStmts.add(this);
}

CallExpr* CondStmt::foldConstantCondition() {
  CallExpr* result = NULL;

  if (SymExpr* cond = toSymExpr(condExpr)) {
    if (VarSymbol* var = toVarSymbol(cond->symbol())) {
      if (var->immediate && var->immediate->const_kind == NUM_KIND_BOOL) {

        SET_LINENO(this);

        result = new CallExpr(PRIM_NOOP);

        insertBefore(result);

        if (var->immediate->bool_value() == gTrue->immediate->bool_value()) {
          Expr* then_stmt = thenStmt;

          then_stmt->remove();

          replace(then_stmt);

        } else {
          Expr* else_stmt = elseStmt;

          if (else_stmt != NULL) {
            else_stmt->remove();

            replace(else_stmt);
          } else {
            remove();
          }
        }
      }
    }
  }

  return result;
}

void CondStmt::verify() {
  Expr::verify();

  if (astTag != E_CondStmt) {
    INT_FATAL(this, "Bad CondStmt::astTag");
  }

  if (!condExpr) {
    INT_FATAL(this, "CondStmt has no condExpr");
  }

  if (!thenStmt) {
    INT_FATAL(this, "CondStmt has no thenStmt");
  }

  if (condExpr->list) {
    INT_FATAL(this, "CondStmt::condExpr is a list");
  }

  if (thenStmt->list) {
    INT_FATAL(this, "CondStmt::thenStmt is a list");
  }

  if (elseStmt && elseStmt->list) {
    INT_FATAL(this, "CondStmt::elseStmt is a list");
  }

  if (condExpr->parentExpr != this) {
    INT_FATAL(this, "Bad CondStmt::condExpr::parentExpr");
  }

  if (thenStmt->parentExpr != this) {
    INT_FATAL(this, "Bad CondStmt::thenStmt::parentExpr");
  }

  if (elseStmt && elseStmt->parentExpr != this) {
    INT_FATAL(this, "Bad CondStmt::elseStmt::parentExpr");
  }

  verifyNotOnList(condExpr);

  verifyNotOnList(thenStmt);

  verifyNotOnList(elseStmt);
}

CondStmt* CondStmt::copyInner(SymbolMap* map) {
  return new CondStmt(COPY_INT(condExpr),
                      COPY_INT(thenStmt),
                      COPY_INT(elseStmt));
}


void CondStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == condExpr) {
    condExpr = newAst;

  } else if (oldAst == thenStmt) {
    thenStmt = toBlockStmt(newAst);

  } else if (oldAst == elseStmt) {
    elseStmt = toBlockStmt(newAst);

  } else {
    INT_FATAL(this, "Unexpected case in CondStmt::replaceChild");
  }
}

void CondStmt::accept(AstVisitor* visitor) {
  if (visitor->enterCondStmt(this) == true) {

    if (condExpr != NULL) {
      condExpr->accept(visitor);
    }

    if (thenStmt != NULL) {
      thenStmt->accept(visitor);
    }

    if (elseStmt != NULL) {
      elseStmt->accept(visitor);
    }

    visitor->exitCondStmt(this);
  }
}

Expr* CondStmt::getFirstExpr() {
  return (condExpr != 0) ? condExpr->getFirstExpr() : this;
}

Expr* CondStmt::getNextExpr(Expr* expr) {
  Expr* retval = NULL;

  if        (expr == condExpr && thenStmt != NULL) {
    retval = thenStmt->getFirstExpr();

  } else if (expr == thenStmt && elseStmt != NULL) {
    retval = elseStmt->getFirstExpr();

  } else {
    retval = this;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

GotoStmt::GotoStmt(GotoTag init_gotoTag, const char* init_label) :
  Stmt(E_GotoStmt),
  gotoTag(init_gotoTag),
  label(init_label ? (Expr*)new UnresolvedSymExpr(init_label)
                   : (Expr*)new SymExpr(gNil))
{
  gGotoStmts.add(this);
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, Symbol* init_label) :
  Stmt(E_GotoStmt),
  gotoTag(init_gotoTag),
  label(new SymExpr(init_label))
{
  gGotoStmts.add(this);
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, Expr* init_label) :
  Stmt(E_GotoStmt),
  gotoTag(init_gotoTag),
  label(init_label)
{
  if (!init_label)
    INT_FATAL(this, "GotoStmt initialized with null label");

  if (init_label->parentSymbol)
    INT_FATAL(this, "GotoStmt initialized with label already in tree");

  gGotoStmts.add(this);
}


LabelSymbol* getGotoLabelSymbol(GotoStmt* gs) {
  if (gs->label)
    if (SymExpr* labse = toSymExpr(gs->label))
      if (labse->symbol())
        return toLabelSymbol(labse->symbol());
  return NULL;
}

GotoStmt* getGotoLabelsIterResumeGoto(GotoStmt* gs) {
  LabelSymbol* labsym = getGotoLabelSymbol(gs);
  return labsym ? labsym->iterResumeGoto : NULL;
}

void GotoStmt::verify() {
  Expr::verify();

  if (astTag != E_GotoStmt) {
    INT_FATAL(this, "Bad GotoStmt::astTag");
  }

  if (!label)
    INT_FATAL(this, "GotoStmt has no label");

  if (label->list)
    INT_FATAL(this, "GotoStmt::label is a list");

  if (label && label->parentExpr != this)
    INT_FATAL(this, "Bad GotoStmt::label::parentExpr");

  // If the label has been resolved to a label
  if (SymExpr* se = toSymExpr(label)) {
    if (isLabelSymbol(se->symbol())) {
      Symbol* parent = se->symbol()->defPoint->parentSymbol;

      // The parent should either be a function or a
      // module that does not yet have the initFn installed
      if (isFnSymbol(parent) == false) {
        ModuleSymbol* module = toModuleSymbol(parent);

        if (module == 0 || module->initFn != 0) {
          INT_FATAL(this, "goto label is not in a function");
        }
      }

      if (se->symbol()->defPoint->parentSymbol != this->parentSymbol)
        INT_FATAL(this, "goto label is in a different function than the goto");

      GotoStmt* igs = getGotoLabelsIterResumeGoto(this);

      if ((gotoTag == GOTO_ITER_RESUME) == (igs == NULL))
        INT_FATAL(this,
                  "goto must be GOTO_ITER_RESUME iff its label has iterResumeGoto");

      if (gotoTag == GOTO_ITER_RESUME && igs != this)
        INT_FATAL(this,
                  "GOTO_ITER_RESUME goto's label's iterResumeGoto does not match the goto");
    }
  }

  verifyNotOnList(label);
}


GotoStmt*
GotoStmt::copyInner(SymbolMap* map) {
  GotoStmt* copy = new GotoStmt(gotoTag, COPY_INT(label));

  // For a GOTO_ITER_RESUME: has the label symbol already been copied? ...
  LabelSymbol* labsym;
  if (gotoTag == GOTO_ITER_RESUME && (labsym = getGotoLabelSymbol(this))) {
    LabelSymbol* lcopy = NULL;
    if (Symbol* mcopy = map->get(labsym)) {
      lcopy = toLabelSymbol(mcopy);
      INT_ASSERT(lcopy); // a LabelSymbol gotta map to a LabelSymbol
    }
    if (lcopy) {
      // ... yes => update the copy's field,
      lcopy->iterResumeGoto = copy;
      // printf("GotoStmt-copy %d > %d  labsym %d > %d  updating the copy\n",
      //        this->id, copy->id, labsym->id, lcopy->id);
    } else {
      // ... no => remember this goto copy for later.
      copiedIterResumeGotos.put(this, copy);
      // printf("GotoStmt-copy %d > %d  labsym %d  remembering\n",
      //        this->id, copy->id, labsym->id);
    }
  }
  return copy;
}

//
// Ensure all remembered Gotos have been taken care of.
// Reset copiedIterResumeGotos.
//
void verifyCopiedIterResumeGotos() {
  for (int i = 0; i < copiedIterResumeGotos.n; i++)
    if (GotoStmt* copy = copiedIterResumeGotos.v[i].value)
      INT_FATAL(copy, "unhandled goto in copiedIterResumeGotos");
}


void GotoStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == label) {
    label = toSymExpr(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in GotoStmt::replaceChild");
  }
}

const char* GotoStmt::getName() {
  if (SymExpr* se = toSymExpr(label))
    return se->symbol()->name;
  else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(label))
    return use->unresolved;
  else
    return NULL;
}

void GotoStmt::accept(AstVisitor* visitor) {
  if (visitor->enterGotoStmt(this) == true) {

    if (label)
      label->accept(visitor);

    visitor->exitGotoStmt(this);
  }
}

Expr* GotoStmt::getFirstExpr() {
  return (label != 0) ? label->getFirstExpr() : this;
}

bool GotoStmt::isGotoReturn() const {
  return gotoTag == GOTO_RETURN;
}

LabelSymbol* GotoStmt::gotoTarget() const {
  LabelSymbol* retval = NULL;

  if (SymExpr* labelExpr = toSymExpr(label)) {
    if (LabelSymbol* label = toLabelSymbol(labelExpr->symbol())) {
      retval = label;
    } else {
      INT_ASSERT(false);
    }

  } else {
    INT_ASSERT(false);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

ExternBlockStmt::ExternBlockStmt(const char* init_c_code) :
  Stmt(E_ExternBlockStmt),
  c_code(init_c_code)
{
  gExternBlockStmts.add(this);
}

void ExternBlockStmt::verify() {
  Expr::verify();
  if (astTag != E_ExternBlockStmt) {
    INT_FATAL(this, "Bad ExternBlockStmt::astTag");
  }
  if (!c_code) {
    INT_FATAL(this, "ExternBlockStmt has no c_code");
  }
}

void ExternBlockStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  INT_FATAL(this, "ExternBlockStmt replaceChild called");
}

ExternBlockStmt* ExternBlockStmt::copyInner(SymbolMap* map) {
  ExternBlockStmt* copy = new ExternBlockStmt(c_code);

  return copy;
}

void ExternBlockStmt::accept(AstVisitor* visitor) {
  visitor->visitEblockStmt(this);
}

Expr* ExternBlockStmt::getFirstExpr() {
  INT_FATAL(this, "unexpected ExternBlockStmt in getFirstExpr");
  return NULL;
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

ForwardingStmt::ForwardingStmt(DefExpr* toFnDef) :
  Stmt(E_ForwardingStmt),
  toFnDef(toFnDef),
  fnReturningForwarding(NULL),
  type(NULL),
  scratchFn(NULL),
  named(),
  renamed(),
  except(false)
{
  gForwardingStmts.add(this);

  if (toFnDef)
    if (FnSymbol* fn = toFnSymbol(toFnDef->sym))
      fnReturningForwarding = fn->name;
}

ForwardingStmt::ForwardingStmt(DefExpr* toFnDef, std::set<const char*>* args, bool exclude, std::map<const char*, const char*>* renames) :
  Stmt(E_ForwardingStmt),
  toFnDef(toFnDef),
  fnReturningForwarding(NULL),
  type(NULL),
  scratchFn(NULL),
  named(),
  renamed(),
  except(exclude)
{
  gForwardingStmts.add(this);

  if (toFnDef)
    if (FnSymbol* fn = toFnSymbol(toFnDef->sym))
      fnReturningForwarding = fn->name;

  if (args->size() > 0) {
    // Symbols to search when going through this module's scope from an outside
    // scope
    for_set(const char, str, *args) {
      named.insert(str);
    }
  }

  if (renames->size() > 0) {
    // The new names of symbols in the module being used, to avoid conflicts
    // for instance.
    for (std::map<const char*, const char*>::iterator it = renames->begin();
         it != renames->end(); ++it) {
      renamed[it->first] = it->second;
    }
  }
}


void ForwardingStmt::verify() {
  Expr::verify();
  if (astTag != E_ForwardingStmt) {
    INT_FATAL(this, "Bad ForwardingStmt::astTag");
  }
  if (!toFnDef && !fnReturningForwarding && !type) {
    INT_FATAL(this, "ForwardingStmt is empty");
  }
}

void ForwardingStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == toFnDef) {
    toFnDef = toDefExpr(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ForwardingStmt::replaceChild");
  }
}

ForwardingStmt* ForwardingStmt::copyInner(SymbolMap* map) {
  ForwardingStmt* ret = NULL;

  if (named.size() > 0 || renamed.size() > 0) {
    ret = new ForwardingStmt(COPY_INT(toFnDef), &named, except, &renamed);
  } else {
    ret = new ForwardingStmt(COPY_INT(toFnDef));
  }
  ret->fnReturningForwarding = fnReturningForwarding;

  return ret;
}

void ForwardingStmt::accept(AstVisitor* visitor) {
  if (visitor->enterForwardingStmt(this) == true) {

    if (toFnDef)
      toFnDef->accept(visitor);

    visitor->exitForwardingStmt(this);
  }
}

Expr* ForwardingStmt::getFirstExpr() {
  return (toFnDef != NULL) ? toFnDef->getFirstExpr() : this;
}
