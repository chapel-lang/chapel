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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif


#include "AstVisitor.h"
#include "AstVisitorTraverse.h"
#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "ForLoop.h"
#include "LoopExpr.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TransformLogicalShortCircuit.h"
#include "wellknown.h"

#include <vector>

// Finds all the UnresolvedSymExprs in the LoopExpr's indices expression and
// populates a flat AList of DefExprs. Used during scope resolution.
static void findLoopExprDefs(LoopExpr* fe, Expr* indices, AList& defIndices) {
  if (CallExpr* call = toCallExpr(indices)) {
    for_actuals(act, call) {
      findLoopExprDefs(fe, act, defIndices);
    }
  } else if (UnresolvedSymExpr* se = toUnresolvedSymExpr(indices)) {
    VarSymbol* idx = new VarSymbol(se->unresolved);
    idx->addFlag(FLAG_INDEX_VAR);
    idx->addFlag(FLAG_INSERT_AUTO_DESTROY);
    DefExpr* def = new DefExpr(idx);
    defIndices.insertAtTail(def);
  }
}

LoopExpr::LoopExpr(Expr* indices,
                       Expr* iteratorExpr,
                       Expr* expr,
                       Expr* cond,
                       bool maybeArrayType,
                       bool zippered,
                       bool forall) :
  Expr(E_LoopExpr),
  indices(indices),
  iteratorExpr(iteratorExpr),
  cond(cond),
  maybeArrayType(maybeArrayType),
  zippered(zippered),
  forall(forall)
{

  if (forall == false && maybeArrayType) {
    INT_FATAL("For-exprs cannot possibly result in an array type");
  }

  // 'expr' should be a BlockStmt so that any nested functions remain within
  // the LoopExpr (e.g. a reduction).
  if (BlockStmt* block = toBlockStmt(expr)) {
    this->expr = block;
  } else {
    this->expr = new BlockStmt(expr);
  }

  defIndices.parent = this;

  if (indices != NULL) {
    findLoopExprDefs(this, indices, defIndices);
  }

  gLoopExprs.add(this);
}

LoopExpr::LoopExpr(bool maybeArrayType, bool zippered, bool forall) :
  Expr(E_LoopExpr),
  maybeArrayType(maybeArrayType),
  zippered(zippered),
  forall(forall)
{
}

LoopExpr* LoopExpr::copyInner(SymbolMap* map) {
  LoopExpr* ret = new LoopExpr(maybeArrayType, zippered, forall);

  for_alist(ind, defIndices) {
    ret->defIndices.insertAtTail(COPY_INT(ind));
  }
  ret->indices        = COPY_INT(indices);
  ret->iteratorExpr   = COPY_INT(iteratorExpr);
  ret->expr           = COPY_INT(expr);
  ret->cond           = COPY_INT(cond);

  return ret;
}

void LoopExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == indices)
    indices = new_ast;
  else if (old_ast == iteratorExpr)
    iteratorExpr = new_ast;
  else if (old_ast == expr)
    expr = toBlockStmt(new_ast);
  else if (old_ast == cond)
    cond = new_ast;
  else
    INT_FATAL(this, "unexpected case in LoopExpr::replaceChild");
}

void
LoopExpr::verify() {
  Expr::verify(E_LoopExpr);

  if (indices)      verifyParent(indices);
  if (iteratorExpr) verifyParent(iteratorExpr);
  if (cond)         verifyParent(cond);
  if (expr)         verifyParent(expr);

  if (indices)      verifyNotOnList(indices);
  if (iteratorExpr) verifyNotOnList(iteratorExpr);
  if (cond)         verifyNotOnList(cond);
  if (expr)         verifyNotOnList(expr);
}

void LoopExpr::accept(AstVisitor* visitor) {
  if (visitor->enterLoopExpr(this)) {
    for_alist(ind, defIndices) {
      ind->accept(visitor);
    }
    if (indices)      indices->accept(visitor);
    if (iteratorExpr) iteratorExpr->accept(visitor);
    if (cond)         cond->accept(visitor);
    if (expr)         expr->accept(visitor);

    visitor->exitLoopExpr(this);
  }
}

Expr* LoopExpr::getFirstExpr() {
  INT_FATAL(this, "LoopExpr::getFirstExpr() is not implemented");
  return NULL;
}

GenRet LoopExpr::codegen() {
  GenRet ret;
  INT_FATAL(this, "LoopExpr::codegen called");
  return ret;
}


static int loopexpr_uid = 1;

static CallExpr* buildLoopExprFunctions(LoopExpr* faExpr);

class LowerLoopExprVisitor : public AstVisitorTraverse
{
  public:
    LowerLoopExprVisitor() { }
    virtual ~LowerLoopExprVisitor() { }

    virtual bool enterLoopExpr(LoopExpr* node);
};

//
// Lower outer-most LoopExprs first, in order to simplify scope-resolution
// of newly-created functions. If we lower innermost LoopExprs first, and a
// top-level LoopExpr attempts to scope-resolve its functions later, scope
// resolution could fail due to nested functions already added to the symbol
// table.
//
bool LowerLoopExprVisitor::enterLoopExpr(LoopExpr* node) {
  // Don't touch LoopExprs in DefExprs, they should be copied later into
  // BlockStmts.
  if (isAlive(node) && node->getStmtExpr() != NULL) {
    SET_LINENO(node);

    CallExpr* replacement = buildLoopExprFunctions(node);

    node->replace(replacement);

    Expr* iterExpr = replacement->get(1);

    // The iterator expr might be a loop-expr itself, make sure it gets
    // lowered.
    iterExpr->accept(this);
  }

  return false;
}

void lowerLoopExprs(BaseAST* ast) {
  LowerLoopExprVisitor vis;
  ast->accept(&vis);
}

static Expr* removeOrNull(Expr* arg) { return arg ? arg->remove() : NULL; }

static BlockStmt*
handleArrayTypeCase(FnSymbol* fn, Expr* indices, ArgSymbol* iteratorExprArg, BlockStmt* expr) {
  BlockStmt* block         = new BlockStmt();
  bool hasSpecifiedIndices = indices != NULL;

  fn->addFlag(FLAG_MAYBE_TYPE);

  if (!hasSpecifiedIndices)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");

  checkIndices(indices);

  //
  // nested function to compute isArrayType which is set to true if
  // the inner expression is a type and false otherwise
  //
  // this nested function is called in a type block so that it is
  // never executed; placing all this code in a separate function
  // inside the type block is essential for two reasons:
  //
  // first, so that the iterators in any nested parallel loop
  // expressions are not pulled all the way out during cleanup
  //
  // second, so that types and functions declared in this nested
  // function do not get removed from the IR when the type block gets
  // removed
  //
  FnSymbol* isArrayTypeFn = new FnSymbol("_isArrayTypeFn");
  isArrayTypeFn->addFlag(FLAG_INLINE);
  fn->insertAtTail(new DefExpr(isArrayTypeFn));

  // Result of '_isArrayTypeFn'
  Symbol* isArrayType = newTemp("_isArrayType");
  isArrayType->addFlag(FLAG_MAYBE_PARAM);
  fn->insertAtTail(new DefExpr(isArrayType));

  // Build enough of an iterator invocation to allow resolution to later
  // reason about the type of the index variables.
  VarSymbol* iteratorSym = newTemp("_iterator");
  isArrayTypeFn->insertAtTail(new DefExpr(iteratorSym));
  isArrayTypeFn->insertAtTail(new CallExpr(PRIM_MOVE, iteratorSym,
                                new CallExpr("_getIterator", iteratorExprArg)));

  VarSymbol* index = newTemp("_indexOfInterest");
  index->addFlag(FLAG_INDEX_OF_INTEREST);
  isArrayTypeFn->insertAtTail(new DefExpr(index));
  isArrayTypeFn->insertAtTail(new CallExpr(PRIM_MOVE, index,
                                new CallExpr("iteratorIndex", iteratorSym)));

  BlockStmt* indicesBlock = new BlockStmt();
  destructureIndices(indicesBlock, indices->copy(), new SymExpr(index), false);
  indicesBlock->blockTag = BLOCK_SCOPELESS;
  isArrayTypeFn->insertAtTail(indicesBlock);
  indicesBlock->flattenAndRemove();

  // Will be true if the loop expression results in a type
  Symbol* isTypeResult = newTemp("isType_result");
  isTypeResult->addFlag(FLAG_PARAM);
  isArrayTypeFn->insertAtTail(new DefExpr(isTypeResult));

  BlockStmt* exprCopy = expr->copy();
  Expr* lastExpr = exprCopy->body.tail->remove();
  exprCopy->insertAtTail(new CallExpr(PRIM_MOVE, isTypeResult, new CallExpr("isType", lastExpr)));
  isArrayTypeFn->insertAtTail(exprCopy);
  isArrayTypeFn->insertAtTail(new CondStmt(
                                new SymExpr(isTypeResult),
                                new CallExpr(PRIM_MOVE, isArrayType, gTrue),
                                new CallExpr(PRIM_MOVE, isArrayType, gFalse)));

  BlockStmt* typeBlock = new BlockStmt();
  typeBlock->blockTag = BLOCK_TYPE;
  typeBlock->insertAtTail(new CallExpr(isArrayTypeFn));
  fn->insertAtTail(typeBlock);

  Symbol* arrayType = newTemp("_arrayType");
  arrayType->addFlag(FLAG_EXPR_TEMP);
  arrayType->addFlag(FLAG_MAYBE_TYPE);

  BlockStmt* thenStmt = new BlockStmt();
  thenStmt->insertAtTail(new DefExpr(arrayType));
  Symbol* domain = newTemp("_domain");
  domain->addFlag(FLAG_EXPR_TEMP);
  domain->addFlag(FLAG_NO_AUTO_DESTROY);
  thenStmt->insertAtTail(new DefExpr(domain));
  thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, domain,
                             new CallExpr("chpl__ensureDomainExpr",
                                          iteratorExprArg)));

  if (hasSpecifiedIndices) {
    VarSymbol* msg = new_StringSymbol(
                     "unimplemented feature: if you are attempting to use "
                     "skyline arrays, they are not yet supported; if not, "
                     "remove the index expression from this array type "
                     "specification");
    thenStmt->insertAtTail(new CallExpr("compilerError", msg));
    thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, arrayType, gNil));
  } else {
    BlockStmt* exprCopy = expr->copy();
    Expr* lastExpr = exprCopy->body.tail->remove();
    exprCopy->insertAtTail(new CallExpr(PRIM_MOVE, arrayType,
                             new CallExpr("chpl__buildArrayRuntimeType",
                                          domain, lastExpr)));
    thenStmt->insertAtTail(exprCopy);
  }

  thenStmt->insertAtTail(new CallExpr(PRIM_RETURN, arrayType));
  fn->insertAtTail(new CondStmt(new SymExpr(isArrayType), thenStmt, block));

  return block;
}

static FnSymbol* buildSerialIteratorFn(FnSymbol* fn,
                                       const char* iteratorName,
                                       BlockStmt* expr,
                                       Expr* cond,
                                       Expr* indices,
                                       bool zippered,
                                       Expr*& stmt)
{
  FnSymbol* sifn = new FnSymbol(iteratorName);
  sifn->addFlag(FLAG_ITERATOR_FN);

  ArgSymbol* sifnIterator = new ArgSymbol(INTENT_BLANK, "iterator", dtAny);
  sifn->insertFormalAtTail(sifnIterator);

  // Note: 'stmt' is later used to generate the follower body
  Expr* last = expr->body.tail->remove();
  expr->insertAtTail(new CallExpr(PRIM_YIELD, last));
  stmt = expr;

  if (cond)
    stmt = new CondStmt(new CallExpr("_cond_test", cond), stmt);

  sifn->insertAtTail(ForLoop::buildForLoop(indices,
                                           new SymExpr(sifnIterator),
                                           new BlockStmt(stmt),
                                           false, // is it a coforall?
                                           zippered));

  return sifn;
}

static FnSymbol* buildLeaderIteratorFn(FnSymbol* fn,
                                       const char* iteratorName,
                                       bool zippered)
{
  FnSymbol* lifn = new FnSymbol(iteratorName);
  lifn->addFlag(FLAG_FN_RETURNS_ITERATOR);

  Expr* tag = new SymExpr(gLeaderTag);
  ArgSymbol* lifnTag = new ArgSymbol(INTENT_PARAM, "tag", dtUnknown,
                                     new CallExpr(PRIM_TYPEOF, tag));
  lifn->insertFormalAtTail(lifnTag);

  ArgSymbol* lifnIterator = new ArgSymbol(INTENT_BLANK, "iterator", dtAny);
  lifn->insertFormalAtTail(lifnIterator);

  lifn->where = new BlockStmt(new CallExpr("==", lifnTag, tag->copy()));

  VarSymbol* leaderIterator = newTempConst("_leaderIterator");
  leaderIterator->addFlag(FLAG_EXPR_TEMP);
  lifn->insertAtTail(new DefExpr(leaderIterator));

  if( !zippered ) {
    lifn->insertAtTail(new CallExpr(PRIM_MOVE, leaderIterator, new CallExpr("_toLeader", lifnIterator)));
  } else {
    lifn->insertAtTail(new CallExpr(PRIM_MOVE, leaderIterator, new CallExpr("_toLeaderZip", lifnIterator)));
  }

  lifn->insertAtTail(new CallExpr(PRIM_RETURN, leaderIterator));

  return lifn;
}

static FnSymbol* buildFollowerIteratorFn(FnSymbol* fn,
                                         const char* iteratorName,
                                         bool zippered,
                                         VarSymbol*& followerIterator)
{
  FnSymbol* fifn = new FnSymbol(iteratorName);
  fifn->addFlag(FLAG_ITERATOR_FN);

  Expr* tag = new SymExpr(gFollowerTag);
  ArgSymbol* fifnTag = new ArgSymbol(INTENT_PARAM, "tag", dtUnknown,
                                     new CallExpr(PRIM_TYPEOF, tag));
  fifn->insertFormalAtTail(fifnTag);

  ArgSymbol* fifnFollower = new ArgSymbol(INTENT_BLANK, iterFollowthisArgname, dtAny);
  fifn->insertFormalAtTail(fifnFollower);

  ArgSymbol* fifnIterator = new ArgSymbol(INTENT_BLANK, "iterator", dtAny);
  fifn->insertFormalAtTail(fifnIterator);

  fifn->where = new BlockStmt(new CallExpr("==", fifnTag, tag->copy()));
  followerIterator = newTempConst("_followerIterator");
  followerIterator->addFlag(FLAG_EXPR_TEMP);
  fifn->insertAtTail(new DefExpr(followerIterator));

  if( !zippered ) {
    fifn->insertAtTail(new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFollower", fifnIterator, fifnFollower)));
  } else {
    fifn->insertAtTail(new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFollowerZip", fifnIterator, fifnFollower)));
  }

  return fifn;
}

//
// Lifted from 'createTaskFunctions'
//
// TODO: build a more general flattening or outer-finding set of functions
//
static bool isOuterVar(Symbol* sym, FnSymbol* faParent) {
  Symbol* symParent = sym->defPoint->parentSymbol;
  Symbol* parent = faParent;

  while (true) {
    if (!isFnSymbol(parent) && !isModuleSymbol(parent))
      return false;
    if (symParent == parent)
      return true;
    if (!parent->defPoint)
      return false;
    INT_ASSERT(parent->defPoint->parentSymbol &&
        parent->defPoint->parentSymbol != parent);
    parent = parent->defPoint->parentSymbol;
  }

  return false;
}

// TODO: There's some logic in flattenFuntions that creates/threads formals for
// outer variables for iterator-functions, can we leverage that?
static void findOuterVars(LoopExpr* loopExpr, std::set<Symbol*>& outerVars) {
  std::vector<SymExpr*> uses;

  collectSymExprs(loopExpr->expr, uses);
  if (loopExpr->cond) collectSymExprs(loopExpr->cond, uses);

  FnSymbol* faParent = loopExpr->getFunction();
  for_vector(SymExpr, se, uses) {
    Symbol* sym = se->symbol();
    if (VarSymbol* var = toVarSymbol(sym)) {
      if (isOuterVar(var, faParent) == true && isGlobal(var) == false) {
        outerVars.insert(var);
      }
    } else if (ArgSymbol* arg = toArgSymbol(sym)) {
      // There might be nested functions within the loopExpr, so we need to
      // check for those functions' defPoints instead of the arg's defPoitn.
      FnSymbol* parentFn = toFnSymbol(arg->defPoint->parentSymbol);
      if (loopExpr->contains(parentFn->defPoint) == false) {
        outerVars.insert(arg);
      }
    }
  }
}

//
// Builds the call that will eventually replace the LoopExpr. ArgSymbols will
// be added as needed. There will always be at least one formal, for the
// iterator expr.
//
// This function populates 'outerMap' if outerVars is non-empty.
// This function sets 'iteratorExprArg' to the function's first formal
//
static CallExpr* buildCallAndArgs(FnSymbol* fn,
                                  Expr* iteratorExpr,
                                  std::set<Symbol*>& outerVars,
                                  SymbolMap* outerMap,
                                  ArgSymbol** iteratorExprArg) {

  // MPF: We'll add the iteratorExpr to the call, so we need an
  // argument to accept it in the new function. This way,
  // the responsibility for managing the memory of whatever
  // is being iterated over (e.g. a domain literal) is in the
  // caller, where the iteration most likely occurs. That way,
  // the iterator can capture such a domain by reference.
  ArgSymbol* iterArg = new ArgSymbol(INTENT_BLANK, "iterExpr", dtAny);
  fn->insertFormalAtTail(iterArg);
  *iteratorExprArg = iterArg;

  CallExpr* ret = new CallExpr(fn->name, iteratorExpr);

  for_set(Symbol, sym, outerVars) {
    Type* argType = sym->type;
    if (argType == dtUnknown) {
      argType = dtAny;
    }
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, sym->name, argType);

    outerMap->put(sym, arg);
    fn->insertFormalAtTail(arg);

    ret->insertAtTail(new SymExpr(sym));
  }

  return ret;
}

static void scopeResolveAndNormalize(FnSymbol* fn) {
  TransformLogicalShortCircuit vis;
  addToSymbolTable(fn);
  fn->accept(&vis);
  resolveUnresolvedSymExprs(fn);
  normalize(fn);
}

// Returns a call to the top-level function wrapper for this loop-expr
static CallExpr* buildLoopExprFunctions(LoopExpr* loopExpr) {
  SET_LINENO(loopExpr);
  INT_ASSERT(loopExpr->inTree()); //otherwise no need to remove() loopExpr's pieces

  // BHARSH June 2018: Currently there are many problems with using functions
  // nested within an ArgSymbol's type/default expr. Instead of building the
  // loop-expr functions in the ArgSymbol's scope, we will insert the functions
  // at module scope and pass outer variables to a top-level wrapper (the
  // chpl__loopexpr function).
  bool insideArgSymbol = isArgSymbol(loopExpr->parentSymbol);

  // The indices will have been correctly scope-resolved at this point, but
  // the loop-creation routines we rely upon (e.g. destructureIndices) are
  // easier to use if the indices are UnresolvedSymExprs.
  Expr* ind = loopExpr->defIndices.head;
  while (ind != NULL) {
    DefExpr* def = toDefExpr(ind);
    Symbol* sym = def->sym;
    for_SymbolSymExprs(se, sym) {
      se->replace(new UnresolvedSymExpr(sym->name));
    }
    ind = def->next;
    def->remove();
  }

  std::set<Symbol*> outerVars;
  if (insideArgSymbol) findOuterVars(loopExpr, outerVars);

  // We need the individual pieces of loopExpr. We want to keep loopExpr itself
  // in the tree - that way we know where to put the replacement.
  Expr* indices        = removeOrNull(loopExpr->indices);
  Expr* iteratorExpr   = removeOrNull(loopExpr->iteratorExpr);
  BlockStmt* expr      = toBlockStmt(removeOrNull(loopExpr->expr));
  Expr* cond           = removeOrNull(loopExpr->cond);
  bool  maybeArrayType = loopExpr->maybeArrayType;
  bool  zippered       = loopExpr->zippered;
  bool  forall         = loopExpr->forall;

  const char* wrapperName = forall ? astr_forallexpr : astr_forexpr;
  FnSymbol* fn = new FnSymbol(astr(wrapperName, istr(loopexpr_uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  fn->addFlag(FLAG_FN_RETURNS_ITERATOR);
  fn->addFlag(FLAG_COMPILER_GENERATED);
  if (forall) fn->addFlag(FLAG_MAYBE_ARRAY_TYPE);

  if (insideArgSymbol) {
    loopExpr->getModule()->block->insertAtHead(new DefExpr(fn));
  } else {
    loopExpr->getStmtExpr()->insertBefore(new DefExpr(fn));
  }

  SymbolMap outerMap;
  ArgSymbol* iteratorExprArg = NULL;
  CallExpr* ret = buildCallAndArgs(fn, iteratorExpr, outerVars, &outerMap,
                                   &iteratorExprArg);

  BlockStmt* block = fn->body;

  // Only possibly true for forall-exprs
  if (maybeArrayType) {
    // handle e.g. type t = [1..3] int;
    // as in test/arrays/deitz/part4/test_array_type_alias.chpl
    // where "[1..3] int" is syntactically a "forall loop expression"
    INT_ASSERT(!cond);
    block = handleArrayTypeCase(fn, indices, iteratorExprArg, expr);
  }

  VarSymbol* iterator = newTemp("_iterator");
  iterator->addFlag(FLAG_EXPR_TEMP);
  iterator->addFlag(FLAG_MAYBE_REF);
  block->insertAtTail(new DefExpr(iterator));
  block->insertAtTail(new CallExpr(PRIM_MOVE, iterator, iteratorExprArg));
  const char* iteratorName = astr(astr_loopexpr_iter, istr(loopexpr_uid-1));
  block->insertAtTail(new CallExpr(PRIM_RETURN, new CallExpr(iteratorName, iterator)));

  FnSymbol* sifn = NULL;
  FnSymbol* lifn = NULL;
  FnSymbol* fifn = NULL;

  Expr* stmt = NULL; // Initialized by buildSerialIteratorFn.
  sifn = buildSerialIteratorFn(fn, iteratorName, expr, cond, indices, zippered, stmt);

  if (forall) {
    lifn = buildLeaderIteratorFn(fn, iteratorName, zippered);

    VarSymbol* followerIterator; // Initialized by buildFollowerIteratorFn.
    fifn = buildFollowerIteratorFn(fn, iteratorName, zippered, followerIterator);

    // do we need to use this map since symbols have not been resolved?
    SymbolMap map;
    Expr* indicesCopy = (indices) ? indices->copy(&map) : NULL;
    Expr* bodyCopy = stmt->copy(&map);
    fifn->insertAtTail(ForLoop::buildLoweredForallLoop(indicesCopy, new SymExpr(followerIterator), new BlockStmt(bodyCopy), false, zippered));
  }

  if (insideArgSymbol) {
    fn->insertAtHead(new DefExpr(sifn));

    if (forall) {
      fn->insertAtHead(new DefExpr(lifn));
      fn->insertAtHead(new DefExpr(fifn));
    }

    scopeResolveAndNormalize(fn);
    update_symbols(fn, &outerMap);
  } else {
    fn->defPoint->insertBefore(new DefExpr(sifn));
    scopeResolveAndNormalize(sifn);

    if (forall) {
      fn->defPoint->insertBefore(new DefExpr(lifn));
      scopeResolveAndNormalize(lifn);

      fn->defPoint->insertBefore(new DefExpr(fifn));
      scopeResolveAndNormalize(fifn);
    }

    scopeResolveAndNormalize(fn);
  }


  return ret;
}
