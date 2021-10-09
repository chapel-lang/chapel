/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

/* This file supports conversion of uAST to the older AST used by
   the rest of the compiler. It uses recursive functions to perform
   the conversion.
 */

#include "convert-uast.h"

#include "CatchStmt.h"
#include "DoWhileStmt.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "TryStmt.h"
#include "WhileDoStmt.h"
#include "build.h"
#include "docsDriver.h"

#include "chpl/uast/all-uast.h"
#include "chpl/util/string-escapes.h"

using namespace chpl;

namespace {

struct Converter {
  chpl::Context* context = nullptr;
  bool inTupleDecl = false;

  // Cache strings for special identifiers we might compare against.
  UniqueString atomicStr;
  UniqueString dmappedStr;
  UniqueString domainStr;
  UniqueString reduceAssignStr;
  UniqueString singleStr;
  UniqueString syncStr;
  UniqueString thisStr;
  UniqueString typeStr;

  // TODO: Figure out a better way to do this sort of caching.
  Converter(chpl::Context* context) : context(context) {
    atomicStr = UniqueString::build(context, "atomic");
    dmappedStr = UniqueString::build(context, "dmapped");
    domainStr = UniqueString::build(context, "domain");
    reduceAssignStr = UniqueString::build(context, "reduce=");
    singleStr = UniqueString::build(context, "single");
    syncStr = UniqueString::build(context, "sync");
    thisStr = UniqueString::build(context, "this");
    typeStr = UniqueString::build(context, "type");
  }

  Expr* convertAST(const uast::ASTNode* node);

  Expr* convertExprOrNull(const uast::ASTNode* node) {
    if (node == nullptr)
      return nullptr;

    Expr* ret = convertAST(node);
    INT_ASSERT(ret);
    return ret;
  }

  Expr* visit(const uast::Comment* node) {
    // old ast does not represent comments
    return nullptr;
  }

  Expr* visit(const uast::ErroneousExpression* node) {
    return new CallExpr(PRIM_ERROR);
  }

  UnresolvedSymExpr* visit(const uast::Identifier* node) {
    return new UnresolvedSymExpr(node->name().c_str());
  }

  /// SimpleBlockLikes ///

  BlockStmt* createBlockWithStmts(
      uast::ASTListIteratorPair<uast::Expression> stmts) {
    BlockStmt* block = new BlockStmt();
    for (auto stmt: stmts) {
      Expr* e = convertAST(stmt);
      if (e) {
        block->insertAtTail(e);
      }
    }
    return block;
  }

  BlockStmt* visit(const uast::Begin* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  BlockStmt* visit(const uast::Block* node) {
    return createBlockWithStmts(node->stmts());
  }

  BlockStmt* visit(const uast::Defer* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  BlockStmt* visit(const uast::Local* node) {
    BlockStmt* body = createBlockWithStmts(node->stmts());
    Expr* condition = convertExprOrNull(node->condition());
    if (condition) {
      return buildLocalStmt(condition, body);
    } else {
      return buildLocalStmt(body);
    }
  }

  BlockStmt* visit(const uast::On* node) {
    Expr* expr = toExpr(convertAST(node->destination()));
    Expr* stmt = toExpr(createBlockWithStmts(node->stmts()));
    return buildOnStmt(expr, stmt);
  }

  BlockStmt* visit(const uast::Serial* node) {
    BlockStmt* body = createBlockWithStmts(node->stmts());
    Expr* condition = convertExprOrNull(node->condition());

    if (condition) {
      return buildSerialStmt(condition, body);
    } else {
      return buildSerialStmt(new SymExpr(gTrue), body);
    }
  }

  CondStmt* visit(const uast::When* node) {
    auto args = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto caseExpr : node->caseExprs()) {
      args->insertAtTail(convertAST(caseExpr));
    }

    CallExpr* when = new CallExpr(PRIM_WHEN, args);
    BlockStmt* block = createBlockWithStmts(node->stmts());

    return new CondStmt(when, block);
  }

  /// Expressions ///

  CallExpr* visit(const uast::Delete* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* visit(const uast::Dot* node) {

    // These are the arguments that 'buildDotExpr' requires.
    BaseAST* base = toExpr(convertAST(node->receiver()));
    auto member = node->field();

    if (!typeStr.compare(member)) {
      return new CallExpr(PRIM_TYPEOF, base);
    } else if (!domainStr.compare(member)) {
      return buildDotExpr(base, "_dom");
    } else {
      return buildDotExpr(base, member.c_str());
    }
  }

  Expr* visit(const uast::ExternBlock* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* visit(const uast::Require* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  BlockStmt* visit(const uast::Import* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  CallExpr* visit(const uast::New* node) {
    Expr* newedType = convertAST(node->typeExpression());
    CallExpr* typeCall = new CallExpr(newedType);

    switch (node->management()) {
      case uast::New::DEFAULT_MANAGEMENT:
        return new CallExpr(PRIM_NEW, typeCall);
      case uast::New::OWNED:
        return new CallExpr(PRIM_NEW, typeCall,
                            new NamedExpr(astr_chpl_manager,
                                          new SymExpr(dtOwned->symbol)));
      case uast::New::SHARED:
        return new CallExpr(PRIM_NEW, typeCall,
                            new NamedExpr(astr_chpl_manager,
                                          new SymExpr(dtShared->symbol)));
      case uast::New::UNMANAGED:
        return new CallExpr(PRIM_NEW, typeCall,
                            new NamedExpr(astr_chpl_manager,
                                          new SymExpr(dtUnmanaged->symbol)));
      case uast::New::BORROWED:
        return new CallExpr(PRIM_NEW, typeCall,
                            new NamedExpr(astr_chpl_manager,
                                          new SymExpr(dtBorrowed->symbol)));
    }
    INT_FATAL("case not handled");
    return nullptr;
  }

  std::pair<Expr*, Expr*> convertAs(const uast::As* node) {
    Expr* one = toExpr(convertAST(node->symbol()));
    Expr* two = toExpr(convertAST(node->rename()));
    return std::pair<Expr*, Expr*>(one, two);
  }

  Expr* visit(const uast::As* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  PotentialRename* convertRename(const uast::Expression* node) {
    PotentialRename* ret = new PotentialRename();

    if (auto as = node->toAs()) {
      ret->tag = PotentialRename::DOUBLE;
      ret->renamed = new std::pair<Expr*, Expr*>(convertAs(as));
    } else {
      ret->tag = PotentialRename::SINGLE;
      ret->elem = toExpr(convertAST(node));
    }

    return ret;
  }

  BlockStmt* convertUsePossibleLimitations(const uast::Use* node) {
    INT_ASSERT(node->numVisibilityClauses() == 1);

    auto vc = node->visibilityClause(0);

    if (vc->limitationKind() == uast::VisibilityClause::NONE) {
      INT_ASSERT(vc->numLimitations() == 0);
      return convertUseNoLimitations(node);
    }

    // These are the arguments required by 'buildUseStmt' with limitations.
    Expr* mod = nullptr;
    Expr* rename = nullptr;
    auto names = new std::vector<PotentialRename*>();
    bool except = false;
    bool privateUse = node->visibility() != uast::Decl::PUBLIC;

    if (vc->limitationKind() == uast::VisibilityClause::EXCEPT) {
      except = true;
    }

    if (auto as = vc->symbol()->toAs()) {
      auto exprs = convertAs(as);
      mod = exprs.first;
      rename = exprs.second;
    } else {
      mod = toExpr(convertAST(vc->symbol()));
      rename = new UnresolvedSymExpr("");
    }

    // Build the limitations list.
    for (auto limitation : vc->limitations()) {
      names->push_back(convertRename(limitation));
    }

    return buildUseStmt(mod, rename, names, except, privateUse);
  }

  BlockStmt* convertUseNoLimitations(const uast::Use* node) {
    auto args = new std::vector<PotentialRename*>();
    bool privateUse = node->visibility() != uast::Decl::PUBLIC;

    for (auto vc : node->visibilityClauses()) {
      INT_ASSERT(vc->limitationKind() == uast::VisibilityClause::NONE);
      INT_ASSERT(vc->numLimitations() == 0);
      PotentialRename* pr = convertRename(vc->symbol());
      args->push_back(pr);
    }

    return buildUseStmt(args, privateUse);
  }

  BlockStmt* visit(const uast::Use* node) {
    INT_ASSERT(node->numVisibilityClauses() > 0);

    if (node->numVisibilityClauses() == 1) {
      return convertUsePossibleLimitations(node);
    } else {
      return convertUseNoLimitations(node);
    }
  }

  Expr* visit(const uast::VisibilityClause* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  // TODO: Speed comparison for this vs. using cached unique strings?
  Expr* convertScanReduceOp(UniqueString op) {
    if (op == "+") return new UnresolvedSymExpr("SumReduceScanOp");
    if (op == "*") return new UnresolvedSymExpr("ProductReduceScanOp");
    if (op == "&&") return new UnresolvedSymExpr("LogicalAndReduceScanOp");
    if (op == "||") return new UnresolvedSymExpr("LogicalOrReduceScanOp");
    if (op == "&") return new UnresolvedSymExpr("BitwiseAndReduceScanOp");
    if (op == "|") return new UnresolvedSymExpr("BitwiseOrReduceScanOp");
    if (op == "^") return new UnresolvedSymExpr("BitwiseXorReduceScanOp");
    return new UnresolvedSymExpr(op.c_str());
  }

  // Note that there are two ways to translate this. In all cases the
  // contents get converted to a PRIM_ACTUALS_LIST. However, some of the
  // loop nodes (Forall/Foreach/BracketLoop) will want to call
  // 'addForallIntent' when adding converted children to the list, while
  // everything else will want to call 'addTaskIntent'.
  CallExpr* convertWithClause(const uast::WithClause* node,
                              const uast::ASTNode* parent) {
    if (node == nullptr) return nullptr;

    CallExpr* ret = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto expr : node->exprs()) {
      ShadowVarSymbol* svs = nullptr;

      // Normal conversion of TaskVar, reduce intents handled below.
      if (const uast::TaskVar* tv = expr->toTaskVar()) {
        svs = convertTaskVar(tv);
        INT_ASSERT(svs);

      // Handle reductions in with clauses explicitly here.
      } else if (const uast::Reduce* rd = expr->toReduce()) {
        Expr* ovar = toExpr(convertAST(rd->actual(0)));
        Expr* riExpr = convertScanReduceOp(rd->op());
        svs = ShadowVarSymbol::buildFromReduceIntent(ovar, riExpr);
      } else {
        INT_FATAL("Not handled!");
      }

      INT_ASSERT(svs != nullptr);

      if (parent->isBracketLoop() || parent->isForall() ||
          parent->isForeach()) {
        addForallIntent(ret, svs);
      } else {
        addTaskIntent(ret, svs);
      }
    }

    return ret;
  }

  Expr* visit(const uast::WithClause* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  GotoStmt* visit(const uast::Break* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  CatchStmt* visit(const uast::Catch* node) {
    auto errorVar = node->error();
    const char* name = errorVar ? errorVar->name().c_str() : nullptr;
    Expr* type = errorVar ? convertExprOrNull(errorVar->typeExpression())
                          : nullptr;
    BlockStmt* body = toBlockStmt(convertAST(node->body()));
    CatchStmt* ret = nullptr;

    if (name && type) {
      ret = CatchStmt::build(name, type, body);
    } else if (name) {
      ret = CatchStmt::build(name, body);
    } else {
      ret = CatchStmt::build(body);
    }

    assert(ret != nullptr);

    return ret;
  }

  BlockStmt* visit(const uast::Cobegin* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* visit(const uast::Conditional* node) {
    Expr* condExpr = toExpr(convertAST(node->condition()));
    BlockStmt* thenBlock = createBlockWithStmts(node->thenStmts());
    BlockStmt* elseBlock = nullptr;

    if (node->hasElseBlock())
      elseBlock = createBlockWithStmts(node->elseStmts());

    return buildIfStmt(condExpr, thenBlock, elseBlock);
  }

  GotoStmt* visit(const uast::Continue* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  DefExpr* visit(const uast::Label* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  CallExpr* visit(const uast::Return* node) {
    CallExpr* ret = new CallExpr(PRIM_RETURN);

    if (node->value()) {
      ret->insertAtTail(toExpr(convertAST(node->value())));
    }

    return ret;
  }

  BlockStmt* visit(const uast::Select* node) {
    Expr* selectCond = toExpr(convertAST(node->expr()));
    BlockStmt* whenStmts = new BlockStmt();

    for (auto when : node->whenStmts()) {
      whenStmts->insertAtTail(toExpr(convertAST(when)));
    }

    return buildSelectStmt(selectCond, whenStmts);
  }

  BlockStmt* visit(const uast::Sync* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  CallExpr* visit(const uast::Throw* node) {
    CallExpr* ret = new CallExpr(PRIM_THROW);
    ret->insertAtTail(toExpr(convertAST(node->errorExpression())));
    return ret;
  }

  BlockStmt* visit(const uast::Try* node) {
    if (node->isExpressionLevel()) {

      assert(node->numStmts() == 1);
      assert(node->stmt(0)->isExpression() && !node->stmt(0)->isBlock());
      bool tryBang = node->isTryBang();
      Expr* expr = convertAST(node->stmt(0));
      return TryStmt::build(tryBang, expr);

    } else {
      bool tryBang = node->isTryBang();
      BlockStmt* body = createBlockWithStmts(node->stmts());
      BlockStmt* catches = new BlockStmt();
      bool isSyncTry = false; // TODO: When can this be true?

      for (auto handler : node->handlers()) {
        assert(handler->isCatch());
        auto conv = toExpr(convertAST(handler));
        catches->insertAtTail(conv);
      }

      return TryStmt::build(tryBang, body, catches, isSyncTry);
    }
  }

  CallExpr* visit(const uast::Yield* node) {
    CallExpr* ret = new CallExpr(PRIM_YIELD);
    ret->insertAtTail(convertAST(node->value()));
    return ret;
  }

  /// Loops ///

  BlockStmt* visit(const uast::DoWhile* node) {
    Expr* condExpr = toExpr(convertAST(node->condition()));
    BlockStmt* body = createBlockWithStmts(node->stmts());
    return DoWhileStmt::build(condExpr, body);
  }

  BlockStmt* visit(const uast::While* node) {
    Expr* condExpr = toExpr(convertAST(node->condition()));
    BlockStmt* body = createBlockWithStmts(node->stmts());
    return WhileDoStmt::build(condExpr, body);
  }

  /// IndexableLoops ///

  // In the uAST, loop index variables are represented as Decl, but in the
  // old AST they are represented as expressions.
  Expr* convertLoopIndexDecl(const uast::Decl* index) {
    if (index == nullptr) return nullptr;

    // Simple variables just get reverted to UnresolvedSymExpr.
    if (const uast::Variable* var = index->toVariable()) {
      return new UnresolvedSymExpr(var->name().c_str());

    // For tuples, recursively call 'convertLoopIndexDecl' on each element.
    } else if (const uast::TupleDecl* td = index->toTupleDecl()) {
      CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);
      for (auto decl : td->decls()) {
        Expr* d2e = convertLoopIndexDecl(decl);
        actualList->insertAtTail(d2e);
      }
      return new CallExpr("_build_tuple", actualList);

    // Else it's something that we haven't seen yet.
    } else {
      INT_FATAL("Not handled yet!");
      return nullptr;
    }
  }

  // TODO: Use for BracketLoop/Forall...
  BlockStmt* convertCommonIndexableLoop(const uast::IndexableLoop* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* convertBracketLoopExpr(const uast::BracketLoop* node) {
    assert(node->isExpressionLevel());
    assert(node->numStmts() == 1);

    // The pieces that we need for 'buildForallLoopExpr'.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    Expr* expr = nullptr;
    Expr* cond = nullptr;

    // Deduced by looking at 'buildForallLoopExpr' calls in for_expr:
    bool maybeArrayType = !node->iterand()->isZip() && node->index() &&
                          !node->stmt(0)->isConditional();
    bool zippered = node->iterand()->isZip();

    // TODO: Assert conditional has no else block, pull out the cond and
    // expr pieces of the conditional.
    if (node->stmt(0)->isConditional()) {
      INT_FATAL("TODO");
      return nullptr;
    } else {
      expr = toExpr(convertAST(node->stmt(0)));
    }

    assert(expr != nullptr);

    return buildForallLoopExpr(indices, iteratorExpr, expr, cond,
                               maybeArrayType,
                               zippered);
  }

  // Note that block statements in type expressions for variables need to
  // be handled by a separate builder, as those are array types.
  Expr* visit(const uast::BracketLoop* node) {
    if (node->isExpressionLevel()) {
      return convertBracketLoopExpr(node);
    } else {
      INT_ASSERT(node->iterand());

      // These are the arguments that 'ForallStmt::build' requires.
      Expr* indices = convertLoopIndexDecl(node->index());
      Expr* iterator = toExpr(convertAST(node->iterand()));
      CallExpr* intents = nullptr;
      BlockStmt* body = createBlockWithStmts(node->stmts());
      bool zippered = node->iterand()->isZip();
      bool serialOK = true;

      if (node->withClause()) {
        intents = convertWithClause(node->withClause(), node);
        INT_ASSERT(intents);
      }

      return ForallStmt::build(indices, iterator, intents, body, zippered,
                               serialOK);
    }
  }

  // TODO: Create a common converter for all IndexableLoop if possible?
  BlockStmt* visit(const uast::Coforall* node) {
    INT_ASSERT(!node->isExpressionLevel());

    // These are the arguments that 'buildCoforallLoopStmt' requires.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iterator = toExpr(convertAST(node->iterand()));
    CallExpr* byref_vars = convertWithClause(node->withClause(), node);
    BlockStmt* body = createBlockWithStmts(node->stmts());
    bool zippered = node->iterand()->isZip();

    return buildCoforallLoopStmt(indices, iterator, byref_vars, body,
                                 zippered);
  }

  BlockStmt* visit(const uast::For* node) {
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    BlockStmt* body = createBlockWithStmts(node->stmts());
    bool zippered = node->iterand()->isZip();
    bool isForExpr = node->isExpressionLevel();

    return ForLoop::buildForLoop(indices, iteratorExpr, body, zippered,
                                 isForExpr);
  }

  // TODO: Can we reuse this for e.g. For/BracketLoop as well?
  Expr* convertForallLoopExpr(const uast::Forall* node) {
    assert(node->isExpressionLevel());
    assert(node->numStmts() == 1);

    // The pieces that we need for 'buildForallLoopExpr'.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    Expr* expr = nullptr;
    Expr* cond = nullptr;

    // TODO: We might need to see the parent to be able to handle this.
    bool maybeArrayType = false;
    bool zippered = node->iterand()->isZip();

    // Assert conditional has no else block, pull out the cond and expr
    // pieces of the conditional.
    if (node->stmt(0)->isConditional()) {
      INT_FATAL("TODO");
      return nullptr;
    } else {
      expr = toExpr(convertAST(node->stmt(0)));
    }

    return buildForallLoopExpr(indices, iteratorExpr, expr, cond,
                               maybeArrayType,
                               zippered);
  }

  Expr* visit(const uast::Forall* node) {
    if (node->isExpressionLevel()) {
      return convertForallLoopExpr(node);
    } else {
      INT_ASSERT(node->iterand());

      // These are the arguments that 'ForallStmt::build' requires.
      Expr* indices = convertLoopIndexDecl(node->index());
      Expr* iterator = toExpr(convertAST(node->iterand()));
      CallExpr* intents = convertWithClause(node->withClause(), node);
      BlockStmt* body = createBlockWithStmts(node->stmts());
      bool zippered = node->iterand()->isZip();
      bool serialOK = false;

      return ForallStmt::build(indices, iterator, intents, body, zippered,
                               serialOK);
    }
  }

  BlockStmt* visit(const uast::Foreach* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  /// Array, Domain, Range, Tuple ///

  CallExpr* visit(const uast::Array* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto expr : node->exprs()) {
      actualList->insertAtTail(toExpr(convertAST(expr)));
    }

    return new CallExpr("chpl__buildArrayExpr", actualList);
  }

  CallExpr* visit(const uast::Domain* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto expr : node->exprs()) {
      actualList->insertAtTail(toExpr(convertAST(expr)));
    }

    return new CallExpr("chpl__buildDomainExpr", actualList,
                        new SymExpr(gTrue));
  }

  CallExpr* visit(const uast::Range* node) {

    // These are the arguments 'buildBoundedRange' requires.
    Expr* low = nullptr;
    Expr* high = nullptr;
    bool openlow = false;
    bool openhigh = false;

    if (node->lowerBound()) {
      low = toExpr(convertAST(node->lowerBound()));
    }

    if (node->upperBound()) {
      high = toExpr(convertAST(node->upperBound()));
    }

    switch (node->opKind()) {
      case uast::Range::OPEN_HIGH: openhigh = true; break;
      case uast::Range::DEFAULT: break;
    }

    if (low && high) {
      return buildBoundedRange(low, high, openlow, openhigh);
    } else if (low) {
      return buildLowBoundedRange(low, openlow);
    } else if (high) {
      return buildHighBoundedRange(high, openhigh);
    } else {
      return buildUnboundedRange();
    }
  }

  // TODO: Get rid of the PRIM_ACTUALS_LIST here.
  Expr* visit(const uast::Tuple* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto expr : node->actuals()) {
      actualList->insertAtTail(toExpr(convertAST(expr)));
    }

    return new CallExpr("_build_tuple", actualList);
  }

  /// Literals ///

  Expr* visit(const uast::BoolLiteral* node) {
    return node->value() ? new SymExpr(gTrue) : new SymExpr(gFalse);
  }

  /// NumericLiterals ///

  Expr* visit(const uast::ImagLiteral* node) {
    SymExpr* se = buildImagLiteral(node->text().c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate && v->immediate->const_kind == NUM_KIND_IMAG);
    INT_ASSERT(v->immediate->real_value() == node->value());
    return se;
  }

  Expr* visit(const uast::IntLiteral* node) {
    SymExpr* se = buildIntLiteral(node->text().c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate && v->immediate->const_kind == NUM_KIND_INT);
    INT_ASSERT(v->immediate->int_value() == node->value());
    return se;
  }

  Expr* visit(const uast::RealLiteral* node) {
    SymExpr* se = buildRealLiteral(node->text().c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate && v->immediate->const_kind == NUM_KIND_REAL);
    INT_ASSERT(v->immediate->real_value() == node->value());
    return se;
  }

  Expr* visit(const uast::UintLiteral* node) {
    SymExpr* se = buildIntLiteral(node->text().c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate && v->immediate->const_kind == NUM_KIND_UINT);
    INT_ASSERT(v->immediate->uint_value() == node->value());
    return se;
  }

  /// StringLikeLiterals ///
  Expr* visit(const uast::BytesLiteral* node) {
    std::string quoted = quoteStringForC(node->str().toString());
    SymExpr* se = buildBytesLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_BYTES);
    return se;
  }

  Expr* visit(const uast::CStringLiteral* node) {
    std::string quoted = quoteStringForC(node->str().toString());
    SymExpr* se = buildCStringLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_C_STRING);
    return se;

  }

  Expr* visit(const uast::StringLiteral* node) {
    std::string quoted = quoteStringForC(node->str().toString());
    SymExpr* se = buildStringLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_STRING);
    return se;
  }

  /// Calls ///

  Expr* convertCalledExpression(const uast::Expression* node) {
    Expr* ret = nullptr;

    if (auto ident = node->toIdentifier()) {
      if (ident->name() == atomicStr) {
        ret = new UnresolvedSymExpr("chpl__atomicType");
      } else if (ident->name() == singleStr) {
        ret = new UnresolvedSymExpr("_singlevar");
      } else if (ident->name() == syncStr) {
        ret = new UnresolvedSymExpr("_syncvar");
      }
    }

    if (ret == nullptr) {
      ret = toExpr(convertAST(node));
      INT_ASSERT(ret);
    }

    return ret;
  }

  Expr* visit(const uast::FnCall* node) {
    const uast::Expression* calledExpression = node->calledExpression();
    INT_ASSERT(calledExpression);
    Expr* calledExpr = convertCalledExpression(calledExpression);
    INT_ASSERT(calledExpr);

    CallExpr* ret = nullptr;
    CallExpr* addArgsTo = nullptr;
    if (calledExpression->isNew()) {
      // we have (call PRIM_NEW (call C) mgmt)
      // and need to add the arguments to the (call C)
      CallExpr* primNew = toCallExpr(calledExpr);
      INT_ASSERT(primNew->isPrimitive(PRIM_NEW));
      CallExpr* typeCall = toCallExpr(primNew->get(1));
      INT_ASSERT(typeCall);
      ret = primNew;
      addArgsTo = typeCall;
    } else {
      ret = new CallExpr(calledExpr);
      addArgsTo = ret;
    }

    int nActuals = node->numActuals();
    for (int i = 0; i < nActuals; i++) {
      Expr* actual = convertAST(node->actual(i));
      INT_ASSERT(actual);
      if (node->isNamedActual(i)) {
        actual = buildNamedActual(node->actualName(i).c_str(), actual);
      }
      addArgsTo->insertAtTail(actual);
    }

    return ret;
  }

  CallExpr* convertDmappedOp(const uast::OpCall* node) {
    INT_ASSERT(node->numActuals() == 2);

    CallExpr* ret = new CallExpr("chpl__distributed");

    // Call takes actuals in reverse order.
    for (int i = node->numActuals()-1; i >= 0; i--) {
      Expr* actual = convertAST(node->actual(i));
      INT_ASSERT(actual);
      ret->insertAtTail(actual);
    }

    ret->insertAtTail(new SymExpr(gTrue));

    return ret;
  }

  CallExpr* convertRegularBinaryOrUnaryOp(const uast::OpCall* node) {
    CallExpr* ret = new CallExpr(node->op().c_str());
    int nActuals = node->numActuals();

    for (int i = 0; i < nActuals; i++) {
      Expr* actual = convertAST(node->actual(i));
      INT_ASSERT(actual);
      ret->insertAtTail(actual);
    }

    return ret;
  }

  Expr* visit(const uast::OpCall* node) {
    if (node->op() == dmappedStr) {
      return convertDmappedOp(node);
    } else if (node->op() == reduceAssignStr) {
      INT_ASSERT(node->numActuals() == 2);
      Expr* lhs = convertAST(node->actual(0));
      Expr* rhs = convertAST(node->actual(1));
      return buildAssignment(lhs, rhs, PRIM_REDUCE_ASSIGN);
    } else {
      return convertRegularBinaryOrUnaryOp(node);
    }
  }

  Expr* visit(const uast::PrimCall* node) {
    CallExpr* call = new CallExpr(PRIM_ACTUALS_LIST);
    SymExpr* se = buildStringLiteral(node->prim().c_str());
    call->insertAtTail(se);
    for (auto actual : node->actuals()) {
      call->insertAtTail(convertAST(actual));
    }
    return buildPrimitiveExpr(call);
  }

  // Note that this conversion is for the reduce expression, and not for
  // the reduce intent (see conversion for 'WithClause').
  Expr* visit(const uast::Reduce* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* visit(const uast::Zip* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);
    for (auto actual : node->actuals()) {
      Expr* conv = toExpr(convertAST(actual));
      actualList->insertAtTail(conv);
    }

    return new CallExpr(PRIM_ZIP, actualList);
  }

  /// Decls ///

  Expr* visit(const uast::MultiDecl* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  Expr* visit(const uast::TupleDecl* node) {
    BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);

    bool saveInTupleDecl = inTupleDecl;
    inTupleDecl = true;

    for (auto decl : node->decls()) {
      Expr* d = convertAST(decl);
      INT_ASSERT(d);
      if (DefExpr* def = toDefExpr(d)) {
        if (VarSymbol* var = toVarSymbol(def->sym)) {
          if (var->name[0] == '_' && var->name[1] == '\0') {
            var->name = astr("chpl_tuple_blank");
            var->cname = astr("chpl_tuple_blank");
          }
        }
      }
      block->insertAtTail(d);
    }

    Expr* typeExpr = convertExprOrNull(node->typeExpression());
    Expr* initExpr = convertExprOrNull(node->initExpression());

    BlockStmt* ret = buildTupleVarDeclStmt(block, typeExpr, initExpr);

    inTupleDecl = saveInTupleDecl;
    return ret;
  }

  /// NamedDecls ///

  Expr* visit(const uast::EnumElement* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  static RetTag convertRetTag(uast::Function::ReturnIntent returnIntent) {
    switch (returnIntent) {
      case uast::Function::DEFAULT_RETURN_INTENT:
        return RET_VALUE;
      case uast::Function::CONST:
        return RET_VALUE;
      case uast::Function::CONST_REF:
        return RET_CONST_REF;
      case uast::Function::REF:
        return RET_REF;
      case uast::Function::PARAM:
        return RET_PARAM;
      case uast::Function::TYPE:
        return RET_TYPE;
    }

    INT_FATAL("case not handled");
    return RET_VALUE;
  }

  static bool isAssignOp(UniqueString name) {
    return (name == "=" ||
            name == "+=" ||
            name == "-=" ||
            name == "*=" ||
            name == "/=" ||
            name == "%=" ||
            name == "**=" ||
            name == "&=" ||
            name == "|=" ||
            name == "^=" ||
            name == ">>=" ||
            name == "<<=");
  }

  Expr* visit(const uast::Function* node) {
    FnSymbol* fn = new FnSymbol("_");
    if (node->isInline()) {
      fn->addFlag(FLAG_INLINE);
    }
    if (node->isOverride()) {
      fn->addFlag(FLAG_OVERRIDE);
    }
    // TODO: add FLAG_NO_PARENS for parenless functions

    IntentTag thisTag = INTENT_BLANK;
    Expr* receiverType = nullptr;

    // Add the formals
    if (node->numFormals() > 0) {
      for (auto formal : node->formals()) {
        DefExpr* def = visit(formal);
        INT_ASSERT(def);
        if (formal->name() != thisStr) {
          buildFunctionFormal(fn, def); // adds it to the list
        } else if (!node->isPrimaryMethod()) {
          thisTag = convertFormalIntent(formal->intent());
          receiverType = convertExprOrNull(formal->typeExpression());
        }
      }
    }

    UniqueString name = node->name();

    fn = buildFunctionSymbol(fn, name.c_str(), thisTag, receiverType);

    if (isAssignOp(name)) {
      fn->addFlag(FLAG_ASSIGNOP);
    }

    RetTag retTag = convertRetTag(node->returnIntent());


    // TODO: handle specified cname for extern/export functions

    if (node->kind() == uast::Function::ITER) {
      if (fn->hasFlag(FLAG_EXTERN))
        USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
      fn->addFlag(FLAG_ITERATOR_FN);
    }

    if (node->kind() == uast::Function::OPERATOR) {
      fn->addFlag(FLAG_OPERATOR);
      if (fn->_this != NULL) {
        updateOpThisTagOrErr(fn);
        setupTypeIntentArg(toArgSymbol(fn->_this));
      }
    }

    Expr* retType = convertExprOrNull(node->returnType());
    Expr* whereClause = convertExprOrNull(node->whereClause());

    Expr* lifetimeConstraints = nullptr;
    if (node->numLifetimeClauses() > 0) {
      for (auto clause : node->lifetimeClauses()) {
        Expr* convertedClause = convertAST(clause);
        INT_ASSERT(convertedClause);

        if (lifetimeConstraints == nullptr) {
          lifetimeConstraints = convertedClause;
        } else {
          lifetimeConstraints =
            new CallExpr(",", lifetimeConstraints, convertedClause);
        }
      }
      INT_ASSERT(lifetimeConstraints);
    }

    BlockStmt* body = createBlockWithStmts(node->stmts());
    BlockStmt* decl = buildFunctionDecl(fn, retTag, retType,
                                        node->throws(), whereClause,
                                        lifetimeConstraints,
                                        body,
                                        /* docs */ nullptr);

    return decl;
  }

  DefExpr* visit(const uast::Module* node) {
    chpl::UniqueString ustr = node->name();
    const char* name = ustr.c_str();
    const char* path = context->filePathForId(node->id()).c_str();
    ModTag tag = MOD_USER; // TODO: distinguish internal/standard/etc
    bool priv = (node->visibility() == uast::Decl::PRIVATE);
    bool prototype = (node->kind() == uast::Module::PROTOTYPE ||
                      node->kind() == uast::Module::IMPLICIT);
    BlockStmt* body = createBlockWithStmts(node->stmts());

    ModuleSymbol* mod = buildModule(name,
                                    tag,
                                    body,
                                    path,
                                    priv,
                                    prototype,
                                    /* docs */ nullptr);

    return new DefExpr(mod);
  }

  /// VarLikeDecls ///

  static IntentTag convertFormalIntent(uast::Formal::Intent intent) {
    switch (intent) {
      case uast::Formal::DEFAULT_INTENT:
        return INTENT_BLANK;
      case uast::Formal::CONST:
        return INTENT_CONST;
      case uast::Formal::CONST_REF:
        return INTENT_CONST_REF;
      case uast::Formal::REF:
        return INTENT_REF;
      case uast::Formal::IN:
        return INTENT_IN;
      case uast::Formal::CONST_IN:
        return INTENT_CONST_IN;
      case uast::Formal::OUT:
        return INTENT_OUT;
      case uast::Formal::INOUT:
        return INTENT_INOUT;
      case uast::Formal::PARAM:
        return INTENT_PARAM;
      case uast::Formal::TYPE:
        return INTENT_TYPE;
    }

    INT_FATAL("case not handled");
    return INTENT_BLANK;
  }

  DefExpr* visit(const uast::Formal* node) {
    IntentTag intentTag = convertFormalIntent(node->intent());

    Expr* typeExpr = convertExprOrNull(node->typeExpression());
    Expr* initExpr = convertExprOrNull(node->initExpression());

    Expr* varargsVariable = nullptr; // TODO: handle varargs

    return buildArgDefExpr(intentTag, node->name().c_str(),
                           typeExpr, initExpr, varargsVariable);
  }

  ShadowVarPrefix convertTaskVarIntent(const uast::TaskVar* node) {
    switch (node->intent()) {
      case uast::TaskVar::VAR: return SVP_VAR;
      case uast::TaskVar::CONST: return SVP_CONST;
      case uast::TaskVar::CONST_REF: return SVP_CONST_REF;
      case uast::TaskVar::REF: return SVP_REF;
      case uast::TaskVar::IN: return SVP_IN;
      case uast::TaskVar::CONST_IN: return SVP_CONST_IN;
    }

    INT_FATAL("Should not reach here");
    return SVP_VAR;
  }

  Expr* visit(const uast::TaskVar* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  ShadowVarSymbol* convertTaskVar(const uast::TaskVar* node) {
    ShadowVarPrefix prefix = convertTaskVarIntent(node);
    Expr* nameExp = new UnresolvedSymExpr(node->name().c_str());
    Expr* type = convertExprOrNull(node->typeExpression());
    Expr* init = convertExprOrNull(node->initExpression());

    return ShadowVarSymbol::buildForPrefix(prefix, nameExp, type, init);
  }

  const char* tupleVariableName(const char* name) {
    if (inTupleDecl && name[0] == '_' && name[1] == '\0')
      return "chpl__tuple_blank";
    else
      return name;
  }

  void attachVarSymbolStorage(const uast::Variable* node, VarSymbol* vs) {
    switch (node->kind()) {
      case uast::Variable::VAR:
        vs->qual = QUAL_VAL;
        break;
      case uast::Variable::CONST:
        vs->addFlag(FLAG_CONST);
        vs->qual = QUAL_CONST;
        break;
      case uast::Variable::CONST_REF:
        vs->addFlag(FLAG_CONST);
        vs->addFlag(FLAG_REF_VAR);
        vs->qual = QUAL_CONST_REF;
        break;
      case uast::Variable::REF:
        vs->addFlag(FLAG_REF_VAR);
        vs->qual = QUAL_REF;
        break;
      case uast::Variable::PARAM:
        vs->addFlag(FLAG_PARAM);
        vs->qual = QUAL_PARAM;
        break;
      case uast::Variable::TYPE:
        vs->addFlag(FLAG_TYPE_VARIABLE);
        break;
      case uast::Variable::INDEX:
        assert(false && "Index variables should be handled elsewhere");
        break;
    }
  }

  CallExpr* convertArrayType(const uast::BracketLoop* node) {
    INT_ASSERT(node->isExpressionLevel());
    INT_ASSERT(node->numStmts() == 1);

    CallExpr* domActuals = new CallExpr(PRIM_ACTUALS_LIST);

    // Convert domain expressions into arguments for a PRIM_ACTUALS_LIST.
    if (const uast::Domain* dom = node->iterand()->toDomain()) {
      for (auto expr : dom->exprs()) {
        domActuals->insertAtTail(convertAST(expr));
      }
    } else {
      INT_FATAL("Not handled!");
    }

    CallExpr* ensureDomainExpr = new CallExpr("chpl__ensureDomainExpr",
                                              domActuals);
    Expr* subType = convertAST(node->stmt(0));
    CallExpr* ret = new CallExpr("chpl__buildArrayRuntimeType",
                                 ensureDomainExpr,
                                 subType);

    return ret;
  }

  Expr* visit(const uast::Variable* node) {
    auto stmts = new BlockStmt(BLOCK_SCOPELESS);
    auto varSym = new VarSymbol(tupleVariableName(node->name().c_str()));

    // Adjust the variable according to its kind
    attachVarSymbolStorage(node, varSym);

    if (node->isConfig()) {
      varSym->addFlag(FLAG_CONFIG);
    }

    Expr* typeExpr = nullptr;

    // If there is a bracket loop it is almost certainly an array type, so
    // special case it. Otherwise, just use the generic conversion call.
    if (const uast::Expression* te = node->typeExpression()) {
      if (const uast::BracketLoop* bkt = te->toBracketLoop()) {
        typeExpr = convertArrayType(bkt);
      } else {
        typeExpr = toExpr(convertAST(te));
      }
    }

    Expr* initExpr = convertExprOrNull(node->initExpression());

    auto defExpr = new DefExpr(varSym, initExpr, typeExpr);
    stmts->insertAtTail(defExpr);

    // Add a PRIM_END_OF_STATEMENT.
    if (fDocs == false && inTupleDecl == false) {
      CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
      stmts->insertAtTail(end);
    }

    return stmts;
  }

  /// TypeDecls

  Expr* visit(const uast::Enum* node) {
    INT_FATAL("TODO");
    return nullptr;
  }

  /// AggregateDecls

  Expr* visit(const uast::Class* node) {
    const char* name = node->name().c_str();
    const char* cname = name;
    Expr* inherit = convertExprOrNull(node->parentClass());
    BlockStmt* decls = createBlockWithStmts(node->declOrComments());
    Flag externFlag = FLAG_UNKNOWN;

    return buildClassDefExpr(name, cname, AGGREGATE_CLASS, inherit, decls,
                             externFlag, /* docs */ nullptr);
  }

  Expr* visit(const uast::Record* node) {
    const char* name = node->name().c_str();
    const char* cname = name; // TODO: cname could be set for extern record
    Expr* inherit = nullptr;
    BlockStmt* decls = createBlockWithStmts(node->declOrComments());
    Flag externFlag = FLAG_UNKNOWN; // TODO: add extern record support

    return buildClassDefExpr(name, cname, AGGREGATE_RECORD, inherit, decls,
                             externFlag, /* docs */ nullptr);
  }

  Expr* visit(const uast::Union* node) {
    const char* name = node->name().c_str();
    const char* cname = name; // TODO: cname could be set for extern union
    Expr* inherit = nullptr;
    BlockStmt* decls = createBlockWithStmts(node->declOrComments());
    Flag externFlag = FLAG_UNKNOWN; // TODO: add extern union support

    return buildClassDefExpr(name, cname, AGGREGATE_UNION, inherit, decls,
                             externFlag, /* docs */ nullptr);
  }
};

/// Generic conversion calling the above functions ///
Expr* Converter::convertAST(const uast::ASTNode* node) {
  astlocMarker markAstLoc(node->id());
  return node->dispatch<Expr*>(*this);
}

} // end anonymous namespace

ModuleSymbol* convertToplevelModule(chpl::Context* context,
                                    const chpl::uast::Module* mod) {
  astlocMarker markAstLoc(mod->id());
  Converter c(context);
  DefExpr* def = c.visit(mod);
  ModuleSymbol* ret = toModuleSymbol(def->sym);
  ModuleSymbol::addTopLevelModule(ret);
  return ret;
}
