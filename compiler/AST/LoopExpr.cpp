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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif


#include "AstVisitor.h"
#include "AstVisitorTraverse.h"
#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "ForLoop.h"
#include "IfExpr.h"
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
                   Expr* cond,
                   Expr* loopBody,
                   bool forall,
                   bool zippered,
                   bool maybeArrayType) :
  Expr(E_LoopExpr),
  indices(indices),
  iteratorExpr(iteratorExpr),
  cond(cond),
  loopBody(NULL),
  forall(forall),
  zippered(zippered),
  maybeArrayType(maybeArrayType)
{

  if (forall == false && maybeArrayType) {
    INT_FATAL("For-exprs cannot possibly result in an array type");
  }

  // 'expr' should be a BlockStmt so that any nested functions remain within
  // the LoopExpr (e.g. a reduction).
  if (BlockStmt* block = toBlockStmt(loopBody)) {
    this->loopBody = block;
  } else {
    this->loopBody = new BlockStmt(loopBody);
  }

  if (indices != NULL) {
    findLoopExprDefs(this, indices, defIndices);
  }

  defIndices.parent = this;
  gLoopExprs.add(this);
}

LoopExpr::LoopExpr(bool forall, bool zippered, bool maybeArrayType) :
  Expr(E_LoopExpr),
  indices(NULL),
  iteratorExpr(NULL),
  cond(NULL),
  loopBody(NULL),
  forall(forall),
  zippered(zippered),
  maybeArrayType(maybeArrayType)
{
  defIndices.parent = this;
  gLoopExprs.add(this);
}

LoopExpr* LoopExpr::copyInner(SymbolMap* map) {
  LoopExpr* ret = new LoopExpr(forall, zippered, maybeArrayType);

  for_alist(ind, defIndices) {
    ret->defIndices.insertAtTail(COPY_INT(ind));
  }
  ret->indices        = COPY_INT(indices);
  ret->iteratorExpr   = COPY_INT(iteratorExpr);
  ret->cond           = COPY_INT(cond);
  ret->loopBody       = COPY_INT(loopBody);

  return ret;
}

void LoopExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == indices)
    indices = new_ast;
  else if (old_ast == iteratorExpr)
    iteratorExpr = new_ast;
  else if (old_ast == cond)
    cond = new_ast;
  else if (old_ast == loopBody)
    loopBody = toBlockStmt(new_ast);
  else
    INT_FATAL(this, "unexpected case in LoopExpr::replaceChild");
}

void
LoopExpr::verify() {
  Expr::verify(E_LoopExpr);

  verifyParent(indices);       verifyNotOnList(indices);
  verifyParent(iteratorExpr);  verifyNotOnList(iteratorExpr);
  verifyParent(cond);          verifyNotOnList(cond);
  verifyParent(loopBody);      verifyNotOnList(loopBody);
}

void LoopExpr::accept(AstVisitor* visitor) {
  if (visitor->enterLoopExpr(this)) {
    for_alist(ind, defIndices) {
      ind->accept(visitor);
    }
    if (indices)      indices->accept(visitor);
    if (iteratorExpr) iteratorExpr->accept(visitor);
    if (cond)         cond->accept(visitor);
    if (loopBody)     loopBody->accept(visitor);

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
static void addIterRecShape(CallExpr* forallExprCall,
                            bool parallel, bool zippered);

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
  if (node->getStmtExpr() == NULL) {
    // Don't touch LoopExprs in DefExprs, they should be copied later into
    // BlockStmts.
    INT_ASSERT(isDefExpr(node->parentExpr));
  } else {
    SET_LINENO(node);

    bool noFilter = node->cond == NULL;

    CallExpr* replacement = buildLoopExprFunctions(node);

    node->replace(replacement);

    Expr* iterExpr = replacement->get(1);

    // The iterator expr might be a loop-expr itself, make sure it gets
    // lowered.
    iterExpr->accept(this);

    // Do not preserve the shape if there is a filtering predicate.
    if (noFilter) {
      normalize(replacement); // for addIterRecShape()
      addIterRecShape(replacement, node->forall, node->zippered);
    }
  }

  return false;
}

void lowerLoopExprs(BaseAST* ast) {
  INT_ASSERT(ast->inTree()); // otherwise nothing to do
  LowerLoopExprVisitor vis;
  ast->accept(&vis);
}


static Expr* getShapeForZippered(Expr* tupleRef) {
  CallExpr* buildTup = toCallExpr(getDefOfTemp(toSymExpr(tupleRef)));
  INT_ASSERT(buildTup->isNamed("_build_tuple"));
  // The shape comes from the first tuple component.
  return buildTup->get(1);
}

// 'forallExprCall', during resolution, returns an iterator record
// for the forall expression. Ensure it will get a shape.
static void addIterRecShape(CallExpr* forallExprCall,
                            bool parallel, bool zippered) {
  if (CallExpr* move = toCallExpr(forallExprCall->parentExpr)) {
    if (move->isPrimitive(PRIM_MOVE)) {
      Expr* dest = move->get(1)->copy();
      Expr* shape = forallExprCall->get(1);
      if (zippered) shape = getShapeForZippered(shape);
      move->getStmtExpr()->insertAfter(
        new CallExpr(PRIM_ITERATOR_RECORD_SET_SHAPE, dest,
                     shape->copy(), parallel ? gFalse : gTrue));
    }
  }
}


static void copyIndexDefs(LoopExpr* loopExpr, BlockStmt* indicesBlock,
                          SymbolMap* indicesMap) {
  for_alist(expr, loopExpr->defIndices)
    indicesBlock->insertAtTail(expr->copy(indicesMap));
}

static Expr* removeOrNull(Expr* arg) { return arg ? arg->remove() : NULL; }

static BlockStmt*
handleArrayTypeCase(LoopExpr* loopExpr, FnSymbol* fn, Expr* indices,
                    ArgSymbol* iteratorExprArg, BlockStmt* expr)
{
  BlockStmt* block         = new BlockStmt();
  bool hasSpecifiedIndices = indices != NULL;

  fn->addFlag(FLAG_MAYBE_TYPE);

  if (!hasSpecifiedIndices) {
    INT_ASSERT(loopExpr->defIndices.length == 0); // no DefExprs to take care of
    indices = new UnresolvedSymExpr("chpl__elidedIdx");
  }

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
  isArrayTypeFn->setGeneric(false);
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
  SymbolMap indicesMap;
  copyIndexDefs(loopExpr, indicesBlock, &indicesMap);
  Expr* indicesCopy = indices->copy(&indicesMap);
  destructureIndices(indicesBlock, indicesCopy, new SymExpr(index), false);
  indicesBlock->blockTag = BLOCK_SCOPELESS;
  isArrayTypeFn->insertAtTail(indicesBlock);
  indicesBlock->flattenAndRemove();

  // Will be true if the loop expression results in a type
  Symbol* isTypeResult = newTemp("isType_result");
  isTypeResult->addFlag(FLAG_PARAM);
  isArrayTypeFn->insertAtTail(new DefExpr(isTypeResult));

  BlockStmt* exprCopy = expr->copy(&indicesMap);
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
    // Emphasize that there are no indices to substitute in 'expr'.
    INT_ASSERT(loopExpr->defIndices.length == 0);
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


static FnSymbol* buildSerialIteratorFn(const char* iteratorName,
                                       BlockStmt* loopBody,
                                       Expr* cond,
                                       Expr* indices,
                                       bool zippered,
                                       Expr*& stmt)
{
  FnSymbol* sifn = new FnSymbol(iteratorName);
  sifn->addFlag(FLAG_ITERATOR_FN);
  sifn->setGeneric(true);

  ArgSymbol* sifnIterator = new ArgSymbol(INTENT_BLANK, "iterator", dtAny);
  sifn->insertFormalAtTail(sifnIterator);

  // Note: 'stmt' is later used to generate the follower body
  Expr* last = loopBody->body.tail->remove();
  loopBody->insertAtTail(new CallExpr(PRIM_YIELD, last));
  stmt = loopBody;

  if (cond)
    stmt = new CondStmt(new CallExpr("_cond_test", cond), stmt);

  sifn->insertAtTail(ForLoop::buildForLoop(indices,
                                           new SymExpr(sifnIterator),
                                           new BlockStmt(stmt),
                                           zippered,
                                           /*isForExpr*/ true));

  return sifn;
}

static Expr* buildLeaderIteratorWhereClause(ArgSymbol* lifnTag,
                                       ArgSymbol* lifnIterator, bool zippered)
{
  Symbol* tlsym = new_StringSymbol(zippered ? "_toLeaderZip" : "_toLeader");
  Expr* checkTag = new CallExpr("==", lifnTag, gLeaderTag);
  Expr* checkToLeader = new CallExpr(PRIM_CALL_RESOLVES, tlsym, lifnIterator);
  return new CallExpr("&&", checkTag, checkToLeader);
}

static FnSymbol* buildLeaderIteratorFn(const char* iteratorName,
                                       bool zippered)
{
  FnSymbol* lifn = new FnSymbol(iteratorName);
  lifn->addFlag(FLAG_FN_RETURNS_ITERATOR);
  lifn->setGeneric(true);

  Expr* tag = new SymExpr(gLeaderTag);
  ArgSymbol* lifnTag = new ArgSymbol(INTENT_PARAM, "tag", dtUnknown,
                                     new CallExpr(PRIM_TYPEOF, tag));
  lifn->insertFormalAtTail(lifnTag);

  ArgSymbol* lifnIterator = new ArgSymbol(INTENT_BLANK, "iterator", dtAny);
  lifn->insertFormalAtTail(lifnIterator);

  lifn->where = new BlockStmt(buildLeaderIteratorWhereClause(
                                lifnTag, lifnIterator, zippered));

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

static FnSymbol* buildFollowerIteratorFn(const char* iteratorName,
                                         bool zippered,
                                         VarSymbol*& followerIterator)
{
  FnSymbol* fifn = new FnSymbol(iteratorName);
  fifn->addFlag(FLAG_ITERATOR_FN);
  fifn->setGeneric(true);

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
// This is a variation on the standard isGlobal()
// that works on either normalized or not-yet-normalized AST.
//
static bool isGlobalVar(Symbol* sym) {
  Symbol* parent = sym->defPoint->parentSymbol;

  if (sym->hasFlag(FLAG_CHAPEL_STRING_LITERAL) ||
      sym->hasFlag(FLAG_CHAPEL_BYTES_LITERAL)) {
    return true;
  }

  if (ModuleSymbol* moduleParent = toModuleSymbol(parent)) {
    // Until normalized, only system symbols are under a ModuleSymbol.
    INT_ASSERT(normalized || moduleParent == rootModule);
    return true;
  }
  if (normalized)
    return false; // see isGlobal()

  // Until normalized, globals are under module init fns.
  if (FnSymbol* funParent = toFnSymbol(parent)) {
    return funParent->hasFlag(FLAG_MODULE_INIT);
  }
  return false;
}

//
// Is this symbol defined outside 'enclosingExpr'?
//
static bool isOuterVar(Symbol* sym, Expr* enclosingExpr) {
  Symbol* enclosingSym = enclosingExpr->parentSymbol;
  Expr* curr = sym->defPoint;
  Symbol* currParentSym = curr->parentSymbol;

  // See if we are even in the same function.
  while (currParentSym != enclosingSym) {
    if (currParentSym == NULL || currParentSym == rootModule)
      return true; // 'sym' is defined outside 'enclosingSym', so it is outer

    curr = currParentSym->defPoint;
    currParentSym = curr->parentSymbol;
  }

  // 'curr' is under the same Symbol as 'enclosingExpr'.
  while (true) {
    if (curr == NULL) {
      // 'sym' better not be defined under a Symbol
      // that is adjacent to 'enclosingExpr'.
      INT_ASSERT(currParentSym == sym->defPoint->parentSymbol);
      return true;
    }
    if (curr->parentExpr == enclosingExpr)
      return false; // 'sym' is defined within 'enclosingExpr'

    curr = curr->parentExpr;
  }

  INT_ASSERT(false); // should not get here
  return false;
}

static bool considerForOuter(Symbol* sym) {
  if (isTypeSymbol(sym->defPoint->parentSymbol)) {
    // Fields are considered 'outer'
    return true;
  }

  if (sym->hasFlag(FLAG_TYPE_VARIABLE) ||
      sym->hasFlag(FLAG_PARAM))
    return false;  // these will be eliminated anyway

  if (isArgSymbol(sym))
    return true;   // a formal is never a global var

  if (isGlobalVar(sym))
    return false;  // we do not need to handle globals

  return true;
}

// TODO: There's some logic in flattenFunctions that creates/threads formals for
// outer variables for iterator-functions, can we leverage that?
static void findOuterVars(LoopExpr* loopExpr, std::set<Symbol*>& outerVars) {
  std::vector<SymExpr*> uses;

  collectSymExprs(loopExpr->loopBody, uses);
  if (loopExpr->cond) collectSymExprs(loopExpr->cond, uses);

  for_vector(SymExpr, se, uses) {
    Symbol* sym = se->symbol();
    if (considerForOuter(sym) && isOuterVar(sym, loopExpr))
        outerVars.insert(sym);
  }
}

static ArgSymbol* newOuterVarArg(Symbol* ovar) {
  Type* argType = ovar->type;
  if (argType == dtUnknown)
    argType = dtAny;

  ArgSymbol* ret = new ArgSymbol(INTENT_BLANK, ovar->name, argType);

  // An argument might need to be a type variable if the outer variable is
  // a type field.
  if (ovar->hasFlag(FLAG_TYPE_VARIABLE)) {
    ret->addFlag(FLAG_TYPE_VARIABLE);
  }

  return ret;
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
    ArgSymbol* arg = newOuterVarArg(sym);
    outerMap->put(sym, arg);

    fn->insertFormalAtTail(arg);
    ret->insertAtTail(new SymExpr(sym));
  }

  return ret;
}

//
// Within 'ifn', replace the Symbols in 'outerVars' with newly-added formals.
//
// Note that the corresponding CallExpr is modified separately - see 'retCall'
// in buildLoopExprFunctions(). That call calls the iterator by name
// so it is not bound to the particular 'ifn'. The binding happens later,
// when we choose between serial or leader/follower.
//
static void addOuterVariableFormals(FnSymbol* ifn,
                                    std::set<Symbol*>& outerVars) {
  if (outerVars.size() == 0)
    return; // nothing to do

  SymbolMap ovMap;

  for_set(Symbol, sym, outerVars) {
    ArgSymbol* arg = newOuterVarArg(sym);
    ovMap.put(sym, arg);
    ifn->insertFormalAtTail(arg);
  }

  update_symbols(ifn, &ovMap);
}

static void adjustIndexDefPoints(FnSymbol* xifn, AList* indexDefs) {
  if (indexDefs->length == 0) return; // nothing to do

  // These blocks were created way down in ForLoop::buildForLoop().
  BlockStmt* loopWrap = toBlockStmt(xifn->body->body.last());
  BlockStmt* forLoop = toBlockStmt(loopWrap->body.last()->prev);

  for_alist(expr, *indexDefs)
    forLoop->insertAtHead(expr->remove());
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
  bool insideArgSymbol = isArgSymbol(loopExpr->parentSymbol) ||
                         isTypeSymbol(loopExpr->parentSymbol);

  std::set<Symbol*> outerVars;
  findOuterVars(loopExpr, outerVars);

  // We need the individual pieces of loopExpr. We want to keep loopExpr itself
  // in the tree - that way we know where to put the replacement.
  Expr* indices        = removeOrNull(loopExpr->indices);
  Expr* iteratorExpr   = removeOrNull(loopExpr->iteratorExpr);
  BlockStmt* loopBody  = toBlockStmt(removeOrNull(loopExpr->loopBody));
  Expr* cond           = removeOrNull(loopExpr->cond);
  bool  maybeArrayType = loopExpr->maybeArrayType;
  bool  zippered       = loopExpr->zippered;
  bool  forall         = loopExpr->forall;

  const char* wrapperName = forall ? astr_forallexpr : astr_forexpr;
  FnSymbol* fn = new FnSymbol(astr(wrapperName, istr(loopexpr_uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  fn->addFlag(FLAG_FN_RETURNS_ITERATOR);
  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->setGeneric(true);
  if (forall) fn->addFlag(FLAG_MAYBE_ARRAY_TYPE);

  if (insideArgSymbol) {
    loopExpr->getModule()->block->insertAtHead(new DefExpr(fn));
  } else {
    BlockStmt* block = NULL;
    CondStmt* ifExprCond = NULL;
    for (Expr* cur = loopExpr->getStmtExpr();
         cur != NULL;
         cur = cur->parentExpr) {
      if (BlockStmt* b = toBlockStmt(cur)) {
        block = b;
        break;
      }
    }

    if (block != NULL && isLoweredIfExprBlock(block)) {
      ifExprCond = toCondStmt(block->parentExpr);
    }

    if (ifExprCond != NULL)
      // for if-exprs, insert just before the CondStmt
      ifExprCond->insertBefore(new DefExpr(fn));
    else
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
    block = handleArrayTypeCase(loopExpr, fn, indices,
                                iteratorExprArg, loopBody);
  }

  VarSymbol* iterator = newTemp("_iterator");
  iterator->addFlag(FLAG_EXPR_TEMP);
  iterator->addFlag(FLAG_MAYBE_REF);
  block->insertAtTail(new DefExpr(iterator));
  block->insertAtTail(new CallExpr(PRIM_MOVE, iterator, iteratorExprArg));
  const char* iteratorName = astr(astr_loopexpr_iter, istr(loopexpr_uid-1));
  CallExpr*   iterCall     = new CallExpr(iteratorName, iterator);
  CallExpr* retCall = new CallExpr(PRIM_RETURN, iterCall);
  for_set(Symbol, sym, outerVars) iterCall->insertAtTail(sym);
  block->insertAtTail(retCall);
  update_symbols(fn, &outerMap);

  FnSymbol* sifn = NULL;
  FnSymbol* lifn = NULL;
  FnSymbol* fifn = NULL;

  Expr* stmt = NULL; // Initialized by buildSerialIteratorFn.
  sifn = buildSerialIteratorFn(iteratorName, loopBody, cond, indices, zippered, stmt);

  if (forall) {
    lifn = buildLeaderIteratorFn(iteratorName, zippered);
    addOuterVariableFormals(lifn, outerVars);

    VarSymbol* followerIterator; // Initialized by buildFollowerIteratorFn.
    fifn = buildFollowerIteratorFn(iteratorName, zippered, followerIterator);

    // do we need to use this map since symbols have not been resolved?
    SymbolMap map;
    AList indDefCopies;
    for_alist(defI, loopExpr->defIndices)
      indDefCopies.insertAtTail(defI->copy(&map));
    Expr* indicesCopy = (indices) ? indices->copy(&map) : NULL;
    Expr* bodyCopy = stmt->copy(&map);
    fifn->insertAtTail(
        ForLoop::buildLoweredForallLoop(
          indicesCopy, new SymExpr(followerIterator), new BlockStmt(bodyCopy),
          zippered,
          /* isForExpr */ true));
    addOuterVariableFormals(fifn, outerVars);
    adjustIndexDefPoints(fifn, &indDefCopies);
  }

  // Do this after fifn is created - so bodyCopy still references outerVars.
  addOuterVariableFormals(sifn, outerVars);
  adjustIndexDefPoints(sifn, &loopExpr->defIndices);

  if (insideArgSymbol) {
    fn->insertAtHead(new DefExpr(sifn));

    if (forall) {
      fn->insertAtHead(new DefExpr(lifn));
      fn->insertAtHead(new DefExpr(fifn));
    }

    scopeResolveAndNormalize(fn);
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
