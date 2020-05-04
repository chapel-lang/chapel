/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "ImportStmt.h"
#include "misc.h"
#include "passes.h"
#include "stlUtil.h"
#include "stringutil.h"

#include "AstVisitor.h"
#include "ResolveScope.h"

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

VisibilityStmt::VisibilityStmt(AstTag astTag): Stmt(astTag) {
}

VisibilityStmt::~VisibilityStmt() {
}

// Specifically for when the module being used or imported is renamed
bool VisibilityStmt::isARename() const {
  return modRename[0] != '\0';
}

const char* VisibilityStmt::getRename() const {
  return modRename;
}

bool VisibilityStmt::isARenamedSym(const char* name) const {
  return renamed.count(name) == 1;
}

const char* VisibilityStmt::getRenamedSym(const char* name) const {
  std::map<const char*, const char*>::const_iterator it;
  const char*                                        retval = NULL;

  it = renamed.find(name);

  if (it != renamed.end()) {
    retval = it->second;
  }

  return retval;
}

//
// Returns the module symbol if the name provided matches the module imported or
// used
//
Symbol* VisibilityStmt::checkIfModuleNameMatches(const char* name) {
  if (isARename()) {
    // Statements that rename the module should only allow us to find the
    // new name, not the original one.
    if (name == getRename()) {
      SymExpr* actualSe = toSymExpr(src);
      INT_ASSERT(actualSe);
      // Could be either an enum or a module, but either way we should be able
      // to find the new name
      Symbol* actualSym = toSymbol(actualSe->symbol());
      INT_ASSERT(actualSym);
      return actualSym;
    }
  } else if (SymExpr* se = toSymExpr(src)) {
    if (ModuleSymbol* modSym = toModuleSymbol(se->symbol())) {
      if (name == se->symbol()->name) {
        return modSym;
      }
    }
  } else {
    // Things like `use M.N.O` (and though we don't support it yet, things like
    // `import M.N.O`) probably wouldn't reach here because we resolve such
    // cases element-by-element rather than wholesale.  Nothing else should fall
    // under this category
    INT_FATAL("Malformed src");
  }
  return NULL;
}


//
// Extends the scope's block statement to store this node, after replacing the
// UnresolvedSymExpr we store with the found symbol
//
void VisibilityStmt::updateEnclosingBlock(ResolveScope* scope, Symbol* sym) {
  src->replace(new SymExpr(sym));

  remove();
  scope->asBlockStmt()->useListAdd(this);

  scope->extend(this);
}

/************************************* | **************************************
*                                                                             *
* Verifies that all the symbols to be renamed in an import or use statement   *
* refer to symbols that are visible from that module.                         *
*                                                                             *
************************************** | *************************************/
void VisibilityStmt::validateRenamed() {
  std::map<const char*, const char*>::iterator it;

  BaseAST*            scopeToUse = getSearchScope();
  const ResolveScope* scope      = ResolveScope::getScopeFor(scopeToUse);

  for (it = renamed.begin(); it != renamed.end(); ++it) {
    std::vector<Symbol*> symbols;

    scope->getFields(it->second, symbols);

    if (symbols.size() == 0) {
      SymExpr* se = toSymExpr(src);

      USR_FATAL_CONT(this,
                     "Bad identifier in rename, no known '%s' in '%s'",
                     it->second,
                     se->symbol()->name);

    } else if (symbols.size() == 1) {
      Symbol* sym = symbols[0];

      if (sym->hasFlag(FLAG_PRIVATE) && !sym->isVisible(this)) {
        USR_FATAL_CONT(this,
                       "Bad identifier in rename, '%s' is private",
                       it->second);
      }

    } else {
      INT_ASSERT(false);
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Verifies that all the symbols to be renamed in an import or use statement   *
* are not repeats                                                             *
*                                                                             *
************************************** | *************************************/
void VisibilityStmt::noRepeatsInRenamed() const {
  std::map<const char*, const char*>::const_iterator it2;

  for (it2 = renamed.begin(); it2 != renamed.end(); ++it2) {
    std::map<const char*, const char*>::const_iterator next = it2;

    for (++next; next != renamed.end(); ++next) {
      if (strcmp(it2->second, next->second) == 0) {
        // Renamed this variable twice.  Probably a mistake on the user's part,
        // but not a catastrophic one
        USR_WARN(this, "identifier '%s' is repeated", it2->second);
      }

      if (strcmp(it2->second, next->first) == 0) {
        // This name is the old_name in one rename and the new_name in another
        // Did the user actually want to cut out the middle man?
        USR_WARN(this, "identifier '%s' is repeated", it2->second);
        USR_PRINT("Did you mean to rename '%s' to '%s'?",
                  next->second,
                  it2->first);
      }

      if (strcmp(it2->first, next->second) == 0) {
        // This name is the old_name in one rename and the new_name in another
        // Did the user actually want to cut out the middle man?
        USR_WARN(this, "identifier '%s' is repeated", it2->first);
        USR_PRINT("Did you mean to rename '%s' to '%s'?",
                  it2->second,
                  next->first);
      }
    }
  }
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
  blockInfo     = NULL;

  body.parent   = this;

  if (initBody)
    body.insertAtTail(initBody);

  gBlockStmts.add(this);
}

BlockStmt::BlockStmt(BlockTag initBlockTag) :
  Stmt(E_BlockStmt) {


  blockTag      = initBlockTag;
  useList       = NULL;
  userLabel     = NULL;
  byrefVars     = NULL;
  blockInfo     = NULL;

  body.parent   = this;

  gBlockStmts.add(this);
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

          buildEnumFunctions(et);
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

  if (!handled)
    INT_FATAL(this, "BlockStmt::replaceChild. Failed to match the oldAst ");

  // TODO: Handle the above special cases uniformly by specializing the
  // traversal of the children by block statement type.  I think blockInfo is
  // being deprecated anyway....
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
BlockStmt::useListAdd(ModuleSymbol* mod, bool privateUse) {
  useListAdd(new UseStmt(mod, "", privateUse));
}

void
BlockStmt::useListAdd(VisibilityStmt* stmt) {
  if (useList == NULL) {
    useList = new CallExpr(PRIM_USED_MODULES_LIST);

    if (parentSymbol)
      insert_help(useList, this, parentSymbol);
  }

  useList->insertAtTail(stmt);
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

    visitor->exitBlockStmt(this);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

CondStmt::CondStmt(Expr* iCondExpr,
                   BaseAST* iThenStmt, BaseAST* iElseStmt,
                   bool isIfExpr) : Stmt(E_CondStmt) {

  condExpr = iCondExpr;
  thenStmt = NULL;
  elseStmt = NULL;
  fIsIfExpr = isIfExpr;

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

static void fixIfExprFoldedBlock(Expr* stmt) {
  if (BlockStmt* block = toBlockStmt(stmt)) {
    // This addresses lifetime issues with variables created within if-exprs.
    block->flattenAndRemove();
  }
}

static void addCondMentionsToEndOfStatement(CondStmt* cond, bool isIfExpr) {
  CallExpr* end = NULL;

  if (isIfExpr) {
    // Find the next PRIM_END_OF_STATEMENT
    for (Expr* cur = cond; cur != NULL; cur = cur->next) {
      if (CallExpr* call = toCallExpr(cur)) {
        if (call->isPrimitive(PRIM_END_OF_STATEMENT)) {
          end = call;
          break;
        }
      }
    }
  }

  addMentionToEndOfStatement(cond, end);
}

CallExpr* CondStmt::foldConstantCondition(bool addEndOfStatement) {
  CallExpr* result = NULL;

  if (SymExpr* cond = toSymExpr(condExpr)) {
    if (VarSymbol* var = toVarSymbol(cond->symbol())) {
      if (var->immediate && var->immediate->const_kind == NUM_KIND_BOOL) {

        SET_LINENO(this);

        result = new CallExpr(PRIM_NOOP);

        insertBefore(result);

        bool ifExpr = isIfExpr();
        // A squashed IfExpr's result does not need FLAG_IF_EXPR_RESULT, which
        // is only used when there are multiple paths that could return a
        // different type.
        if (CallExpr* call = toCallExpr(thenStmt->body.tail)) {
          if (call->isPrimitive(PRIM_MOVE)) {
            Symbol* LHS = toSymExpr(call->get(1))->symbol();
            if (LHS->hasFlag(FLAG_IF_EXPR_RESULT)) {
              LHS->removeFlag(FLAG_IF_EXPR_RESULT);
              ifExpr = true;
            }
          }
        }

        if (addEndOfStatement)
          addCondMentionsToEndOfStatement(this, ifExpr);

        if (var->immediate->bool_value() == gTrue->immediate->bool_value()) {
          Expr* then_stmt = thenStmt;

          then_stmt->remove();
          replace(then_stmt);
          if (ifExpr) fixIfExprFoldedBlock(then_stmt);

        } else {
          Expr* else_stmt = elseStmt;

          if (else_stmt != NULL) {
            else_stmt->remove();
            replace(else_stmt);
            if (ifExpr) fixIfExprFoldedBlock(else_stmt);
          } else {
            remove();
          }
        }
      }
    }
  }

  return result;
}

bool CondStmt::isIfExpr() const {
  return fIsIfExpr;
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
                      COPY_INT(elseStmt),
                      fIsIfExpr);
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

// If 'expr' is the condExpr in a CondStmt, return that CondStmt.
// Otherwise, return NULL.
CondStmt* isConditionalInCondStmt(Expr* expr) {
  if (CondStmt* parent = toCondStmt(expr->parentExpr))
    if (expr == parent->condExpr)
      return parent;
  return NULL;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

const char* gotoTagToString(GotoTag gotoTag) {
  switch (gotoTag) {
    case GOTO_NORMAL:         return "normal";
    case GOTO_BREAK:          return "break";
    case GOTO_CONTINUE:       return "continue";
    case GOTO_RETURN:         return "return";
    case GOTO_GETITER_END:    return "getiter-end";
    case GOTO_ITER_RESUME:    return "iter-resume";
    case GOTO_ITER_END:       return "iter-end";
    case GOTO_ERROR_HANDLING: return "error-handling";
    case GOTO_BREAK_ERROR_HANDLING: return "break-error-handling";
    case GOTO_ERROR_HANDLING_RETURN: return "error-handling-return";
  }
  INT_FATAL("invalid gotoTag %d", (int)gotoTag);
  return NULL;
}

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
      {
       if (isShadowVarSymbol(this->parentSymbol)        &&
           (se->symbol()->defPoint->parentSymbol ==
            this->parentSymbol->defPoint->parentSymbol) )
        ; // this goto is in a ShadowVarSymbol::initBlock() - ok
       else
        INT_FATAL(this, "goto label is in a different function than the goto");
      }

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
  return gotoTag == GOTO_RETURN || gotoTag == GOTO_ERROR_HANDLING_RETURN;
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
      renamed[it->first] = astr(it->second);
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
