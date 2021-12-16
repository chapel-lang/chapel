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
#include "DeferStmt.h"
#include "DoWhileStmt.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "IfExpr.h"
#include "TryStmt.h"
#include "WhileDoStmt.h"
#include "build.h"
#include "docsDriver.h"

#include "chpl/uast/all-uast.h"
#include "chpl/util/string-escapes.h"
#include "chpl/queries/global-strings.h"

using namespace chpl;

namespace {

struct Converter {
  chpl::Context* context = nullptr;
  bool inTupleDecl = false;

  Converter(chpl::Context* context) : context(context) {}

  Expr* convertAST(const uast::ASTNode* node);

  Expr* convertExprOrNull(const uast::ASTNode* node) {
    if (node == nullptr)
      return nullptr;

    Expr* ret = convertAST(node);
    INT_ASSERT(ret);
    return ret;
  }

  Flag convertFlagForDeclLinkage(const uast::ASTNode* node) {
    if (auto decl = node->toDecl()) {
      switch (decl->linkage()) {
        case uast::Decl::EXTERN: return FLAG_EXTERN;
        case uast::Decl::EXPORT: return FLAG_EXPORT;
        default: return FLAG_UNKNOWN;
      }
    }

    return FLAG_UNKNOWN;
  }

  const char* astrFromStringLiteral(const uast::ASTNode* node) {
    if (auto strLit = node->toStringLiteral()) {
      const char* ret = astr(strLit->str().c_str());
      return ret;
    }

    return nullptr;
  }

  Expr* visit(const uast::Comment* node) {
    // old ast does not represent comments
    return nullptr;
  }

  Expr* visit(const uast::Attributes* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  Flag convertPragmaToFlag(chpl::uast::PragmaTag pragma) {
    Flag ret = FLAG_UNKNOWN;
    switch (pragma) {
#define PRAGMA(name__, canParse__, parseStr__, desc__) \
      case chpl::uast::PRAGMA_ ## name__ : ret = FLAG_ ## name__; break;
#include "chpl/uast/PragmaList.h"
#undef PRAGMA
      default: break;
    }

    return ret;
  }

  void attachSymbolAttributes(const uast::Decl* node, Symbol* sym) {
    auto attr = node->attributes();

    if (!attr) return;

    if (!attr->isDeprecated()) {
      assert(attr->deprecationMessage().isEmpty());
    }

    if (attr->isDeprecated()) {
      assert(!sym->hasFlag(FLAG_DEPRECATED));
      sym->addFlag(FLAG_DEPRECATED);

      auto msg = attr->deprecationMessage();
      if (!msg.isEmpty()) {
        sym->deprecationMsg = astr(msg.c_str());
      }
    }

    for (auto pragma : attr->pragmas()) {
      Flag flag = convertPragmaToFlag(pragma);
      if (flag != FLAG_UNKNOWN) {
        sym->addFlag(flag);
      }
    }
  }

  void attachSymbolVisibility(const uast::Decl* node, Symbol* sym) {
    if (node->visibility() == uast::Decl::PRIVATE) {
      sym->addFlag(FLAG_PRIVATE);
    }
  }

  Expr* visit(const uast::ErroneousExpression* node) {
    return new CallExpr(PRIM_ERROR);
  }

  Expr* reservedWordRemapForIdent(UniqueString name) {
    if (name == USTR("?")) {
      return new SymExpr(gUninstantiated);
    } else if (name == USTR("unmanaged")) {
      return new SymExpr(dtUnmanaged->symbol);
    } else if (name == USTR("bytes")) {
      return new SymExpr(dtBytes->symbol);
    } else if (name == USTR("string")) {
      return new SymExpr(dtString->symbol);
    } else if (name == USTR("owned")) {
      return new UnresolvedSymExpr("_owned");
    } else if (name == USTR("shared")) {
      return new UnresolvedSymExpr("_shared");
    } else if (name == USTR("sync")) {
      return new UnresolvedSymExpr("_syncvar");
    } else if (name == USTR("single")) {
      return new UnresolvedSymExpr("_singlevar");
    } else if (name == USTR("domain")) {
      return new UnresolvedSymExpr("_domain");
    } else if (name == USTR("index")) {
      return new UnresolvedSymExpr("_index");
    } else if (name == USTR("nil")) {
      return new SymExpr(gNil);
    } else if (name == USTR("noinit")) {
      return new SymExpr(gNoInit);
    }

    return nullptr;
  }

  Expr* visit(const uast::Identifier* node) {
    Expr* ret = nullptr;

    auto name = node->name();

    if (auto remap = reservedWordRemapForIdent(name)) {
      ret = remap;
    } else {
      ret = new UnresolvedSymExpr(name.c_str());
    }

    return ret;
  }

  /// SimpleBlockLikes ///

  BlockStmt*
  createBlockWithStmts(uast::ASTListIteratorPair<uast::Expression> stmts) {
    BlockStmt* block = new BlockStmt();
    for (auto stmt: stmts) {
      Expr* e = convertAST(stmt);
      if (e) {
        block->insertAtTail(e);
      }
    }
    return block;
  }

  Expr*
  singleExprFromStmts(uast::ASTListIteratorPair<uast::Expression> stmts) {
    Expr* ret = nullptr;

    for (auto stmt: stmts) {
      if (ret != nullptr) return nullptr;
      ret = convertAST(stmt);
    }

    assert(ret != nullptr);

    return ret;
  }

  Expr* visit(const uast::Begin* node) {
    CallExpr* byrefVars = convertWithClause(node->withClause(), node);
    Expr* stmt = createBlockWithStmts(node->stmts());
    assert(stmt);
    return buildBeginStmt(byrefVars, stmt);
  }

  BlockStmt* visit(const uast::Block* node) {
    return createBlockWithStmts(node->stmts());
  }

  Expr* visit(const uast::Defer* node) {
    auto stmts = createBlockWithStmts(node->stmts());
    return DeferStmt::build(stmts);
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

  BlockStmt* visit(const uast::Delete* node) {
    auto actuals = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto expr : node->exprs()) {
      actuals->insertAtTail(convertAST(expr));
    }

    auto del = new CallExpr("chpl__delete", actuals);
    auto ret = new BlockStmt(BLOCK_SCOPELESS);
    ret->insertAtTail(del);

    return ret;
  }

  Expr* visit(const uast::Dot* node) {

    // These are the arguments that 'buildDotExpr' requires.
    BaseAST* base = toExpr(convertAST(node->receiver()));
    auto member = node->field();

    if (member == USTR("type")) {
      return new CallExpr(PRIM_TYPEOF, base);
    } else if (member == USTR("domain")) {
      return buildDotExpr(base, "_dom");
    } else {
      return buildDotExpr(base, member.c_str());
    }
  }

  Expr* visit(const uast::ExternBlock* node) {
    return buildExternBlockStmt(astr(node->code().c_str()));
  }

  Expr* visit(const uast::Require* node) {
    CallExpr* actuals = new CallExpr(PRIM_ACTUALS_LIST);
    for (auto expr : node->exprs()) {
      Expr* conv = convertAST(expr);
      assert(conv);
      actuals->insertAtTail(conv);
    }

    return buildRequireStmt(actuals);
  }

  Expr* visit(const uast::Import* node) {
    const bool isPrivate = node->visibility() != uast::Decl::PUBLIC;
    auto ret = new BlockStmt(BLOCK_SCOPELESS);

    for (auto vc : node->visibilityClauses()) {
      ImportStmt* conv = nullptr;

      switch (vc->limitationKind()) {
        case uast::VisibilityClause::NONE: {
          assert(vc->numLimitations() == 0);

          // Handles case: 'import foo as bar'
          if (auto as = vc->symbol()->toAs()) {
            Expr* mod = convertAST(as->symbol());
            const char* rename = as->rename()->name().c_str();
            conv = buildImportStmt(mod, rename);

          // Handles: 'import foo'
          } else {
            Expr* mod = convertAST(vc->symbol());
            conv = buildImportStmt(mod);
          }
        } break;

        // Handles: 'import foo.{a, b, c}'
        case uast::VisibilityClause::BRACES: {
          auto names = new std::vector<PotentialRename*>();
          Expr* mod = convertAST(vc->symbol());

          for (auto lmt : vc->limitations()) {
            auto rename = convertRename(lmt);
            names->push_back(rename);
          }

          conv = buildImportStmt(mod, names);
        } break;
        default:
          assert(0 == "Not possible!");
          break;
      }

      assert(conv != nullptr);

      ret->insertAtTail(conv);
    }

    setImportPrivacy(ret, isPrivate);

    return ret;
  }

  CallExpr* visit(const uast::New* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  CallExpr* convertNewManagement(const uast::New* node) {
    auto ret = new CallExpr(PRIM_NEW);

    if (node->management() == uast::New::DEFAULT_MANAGEMENT) {
      return ret;
    }

    Symbol* symManager = nullptr;
    switch (node->management()) {
      case uast::New::OWNED: symManager = dtOwned->symbol; break;
      case uast::New::SHARED: symManager = dtShared->symbol; break;
      case uast::New::UNMANAGED: symManager = dtUnmanaged->symbol; break;
      case uast::New::BORROWED: symManager = dtBorrowed->symbol; break;
      default: assert(0 == "Not handled!"); break;
    }

    assert(symManager);

    /*
    2238 | TNEW TUNMANAGED
    2239     { $$ = new CallExpr(PRIM_NEW,
    2240              new NamedExpr(astr_chpl_manager,
    2241                            new SymExpr(dtUnmanaged->symbol))); }
    */

    auto seManager = new SymExpr(symManager);
    auto managerExpr = new NamedExpr(astr_chpl_manager, seManager);
    ret->insertAtTail(managerExpr);

    return ret;
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
    if (op == USTR("+")) return new UnresolvedSymExpr("SumReduceScanOp");
    if (op == USTR("*")) return new UnresolvedSymExpr("ProductReduceScanOp");
    if (op == USTR("&&")) return new UnresolvedSymExpr("LogicalAndReduceScanOp");
    if (op == USTR("||")) return new UnresolvedSymExpr("LogicalOrReduceScanOp");
    if (op == USTR("&")) return new UnresolvedSymExpr("BitwiseAndReduceScanOp");
    if (op == USTR("|")) return new UnresolvedSymExpr("BitwiseOrReduceScanOp");
    if (op == USTR("^")) return new UnresolvedSymExpr("BitwiseXorReduceScanOp");
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

  BlockStmt* visit(const uast::Break* node) {
    const char* name = node->target() ? node->target()->name().c_str()
                                      : nullptr;
    return buildGotoStmt(GOTO_BREAK, name);
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

  Expr* visit(const uast::Cobegin* node) {
    CallExpr* byrefVars = convertWithClause(node->withClause(), node);
    BlockStmt* block = createBlockWithStmts(node->taskBodies());
    return buildCobeginStmt(byrefVars, block);
  }

  Expr* visit(const uast::Conditional* node) {
    assert(node->condition());

    Expr* ret = nullptr;

    if (node->isExpressionLevel()) {
      auto cond = toExpr(convertAST(node->condition()));
      assert(cond);
      auto thenExpr = singleExprFromStmts(node->thenStmts());
      assert(thenExpr);
      auto elseExpr = singleExprFromStmts(node->elseStmts());
      assert(elseExpr);
      ret = new IfExpr(cond, thenExpr, elseExpr);

    } else {
      auto thenBlock = createBlockWithStmts(node->thenStmts());
      assert(thenBlock);
      auto elseBlock = node->hasElseBlock()
            ? createBlockWithStmts(node->elseStmts())
            : nullptr;

      Expr* cond = nullptr;

      // TODO: Can 'ifVars' happen in expression-level conditionals?
      if (auto ifVar = node->condition()->toVariable()) {
        assert(ifVar->kind() == uast::Variable::CONST ||
               ifVar->kind() == uast::Variable::VAR);
        assert(ifVar->initExpression());
        auto varNameStr = ifVar->name().c_str();
        auto initExpr = toExpr(convertAST(ifVar->initExpression()));
        bool isConst = ifVar->kind() == uast::Variable::CONST;
        cond = buildIfVar(varNameStr, initExpr, isConst);
      } else {
        cond = toExpr(convertAST(node->condition()));
      }

      assert(cond);

      ret = buildIfStmt(cond, thenBlock, elseBlock);
    }

    assert(ret != nullptr);

    return ret;
  }

  BlockStmt* visit(const uast::Continue* node) {
    const char* name = node->target() ? node->target()->name().c_str()
                                      : nullptr;
    return buildGotoStmt(GOTO_CONTINUE, name);
  }

  Expr* visit(const uast::Label* node) {
    const char* name = node->name().c_str();
    Expr* stmt = toExpr(convertAST(node->loop()));
    assert(stmt);
    return buildLabelStmt(name, stmt);
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
    BlockStmt* block = createBlockWithStmts(node->stmts());
    return buildSyncStmt(block);
  }

  CallExpr* visit(const uast::Throw* node) {
    CallExpr* ret = new CallExpr(PRIM_THROW);
    ret->insertAtTail(toExpr(convertAST(node->errorExpression())));
    return ret;
  }

  Expr* visit(const uast::Try* node) {
    if (node->isExpressionLevel()) {

      assert(node->numStmts() == 1);
      assert(node->stmt(0)->isExpression() && !node->stmt(0)->isBlock());
      Expr* expr = convertAST(node->stmt(0));

      // Use this instead of 'TryStmt::build'.
      auto ret = node->isTryBang() ? tryBangExpr(expr) : tryExpr(expr);
      assert(ret);

      return ret;

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

  // TODO (dlongnecke): Just replace these with Identifier?
  DefExpr* visit(const uast::TypeQuery* node) {
    return new DefExpr(new VarSymbol(node->name().c_str()));
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

  // Deduced by looking at 'buildForallLoopExpr' calls in for_expr:
  bool isLoopMaybeArrayType(const uast::IndexableLoop* node) {
    return node->isBracketLoop() && node->index() &&
        !node->iterand()->isZip() &&
        !node->stmt(0)->isConditional();
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
    bool maybeArrayType = isLoopMaybeArrayType(node);
    bool zippered = node->iterand()->isZip();

      // Unpack things differently if body is a conditional.
      if (auto origCond = node->stmt(0)->toConditional()) {
        assert(origCond->numThenStmts() == 1);
        assert(!origCond->hasElseBlock());
        expr = singleExprFromStmts(origCond->thenStmts());
        cond = convertAST(origCond->condition());
        assert(cond);
      } else {
        expr = singleExprFromStmts(node->stmts());
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

  Expr* visit(const uast::For* node) {
    Expr* ret = nullptr;

    Expr* index = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    Expr* body = nullptr;
    Expr* cond = nullptr;
    bool maybeArrayType = isLoopMaybeArrayType(node);
    bool zippered = node->iterand()->isZip();
    bool isForExpr = node->isExpressionLevel();

    if (node->isExpressionLevel()) {
      assert(node->numStmts() == 1);

      // Unpack things differently if body is a conditional.
      if (auto origCond = node->stmt(0)->toConditional()) {
        assert(origCond->numThenStmts() == 1);
        assert(!origCond->hasElseBlock());
        body = singleExprFromStmts(origCond->thenStmts());
        cond = toExpr(convertAST(origCond->condition()));
      } else {
        body = singleExprFromStmts(node->stmts());
      }

      assert(body);

      ret = buildForLoopExpr(index, iteratorExpr, body, cond,
                             maybeArrayType,
                             zippered);

    // Param loops use the index variable name as 'const char*'.
    } else if (node->isParam()) {
      assert(node->index() && node->index()->isVariable());

      const char* indexStr = node->index()->toVariable()->name().c_str();
      body = createBlockWithStmts(node->stmts());
      BlockStmt* block = toBlockStmt(body);
      assert(block);

      ret = buildParamForLoopStmt(indexStr, iteratorExpr, block);

    } else {
      body = createBlockWithStmts(node->stmts());
      BlockStmt* block = toBlockStmt(body);
      assert(block);

      ret = ForLoop::buildForLoop(index, iteratorExpr, block, zippered,
                                  isForExpr);
    }

    assert(ret != nullptr);

    return ret;
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
    bool maybeArrayType = isLoopMaybeArrayType(node);
    bool zippered = node->iterand()->isZip();

      // Unpack things differently if body is a conditional.
      if (auto origCond = node->stmt(0)->toConditional()) {
        assert(origCond->numThenStmts() == 1);
        assert(!origCond->hasElseBlock());
        expr = singleExprFromStmts(origCond->thenStmts());
        cond = toExpr(convertAST(origCond->condition()));
        assert(cond);
      } else {
        expr = singleExprFromStmts(node->stmts());
      }

    assert(expr != nullptr);

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

    // Does not appear possible right now, from reading the grammar.
    assert(!node->isExpressionLevel());

    // The pieces that we need for 'buildForallLoopExpr'.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    BlockStmt* body = createBlockWithStmts(node->stmts());
    bool zippered = node->iterand()->isZip();
    bool isForExpr = node->isExpressionLevel();

    auto ret = ForLoop::buildForeachLoop(indices, iteratorExpr, body,
                                         zippered,
                                         isForExpr);

    return ret;
  }

  /// Array, Domain, Range, Tuple ///

  CallExpr* visit(const uast::Array* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto expr : node->exprs()) {
      actualList->insertAtTail(toExpr(convertAST(expr)));
    }

    return new CallExpr("chpl__buildArrayExpr", actualList);
  }

  Expr* visit(const uast::Domain* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);
    bool isAssociativeList = false;

    for (auto expr : node->exprs()) {
      bool hasConvertedThisIter = false;

      if (auto opCall = expr->toOpCall()) {
        if (opCall->op() == USTR("=>")) {
          isAssociativeList = true;
          assert(opCall->numActuals() == 2);
          Expr* lhs = convertAST(opCall->actual(0));
          Expr* rhs = convertAST(opCall->actual(1));
          actualList->insertAtTail(lhs);
          actualList->insertAtTail(rhs);
          hasConvertedThisIter = true;
        } else {
          if (isAssociativeList) assert(0 == "Not possible!");
        }
      }

      if (!hasConvertedThisIter) {
        actualList->insertAtTail(convertAST(expr));
      }
    }

    Expr* ret = nullptr;

    if (isAssociativeList) {
      ret = new CallExpr("chpl__buildAssociativeArrayExpr", actualList);
    } else {
      ret = new CallExpr("chpl__buildDomainExpr", actualList,
                         new SymExpr(gTrue));
    }

    assert(ret != nullptr);

    return ret;
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
    std::string quoted = quoteStringForC(node->str().str());
    SymExpr* se = buildBytesLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_BYTES);
    return se;
  }

  Expr* visit(const uast::CStringLiteral* node) {
    std::string quoted = quoteStringForC(node->str().str());
    SymExpr* se = buildCStringLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_C_STRING);
    return se;

  }

  Expr* visit(const uast::StringLiteral* node) {
    std::string quoted = quoteStringForC(node->str().str());
    SymExpr* se = buildStringLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_STRING);
    return se;
  }

  /// Calls ///

  Expr* convertCalledKeyword(const uast::Expression* node) {
    Expr* ret = nullptr;

    if (auto ident = node->toIdentifier()) {
      auto name = ident->name();
      if (name == USTR("atomic")) {
        ret = new UnresolvedSymExpr("chpl__atomicType");
      } else if (name == USTR("single")) {
        ret = new UnresolvedSymExpr("_singlevar");
      } else if (name == USTR("subdomain")) {
        ret = new CallExpr("chpl__buildSubDomainType");
      } else if (name == USTR("sync")) {
        ret = new UnresolvedSymExpr("_syncvar");
      } else if (name == USTR("index")) {
        ret = new CallExpr("chpl__buildIndexType");
      } else if (name == USTR("domain")) {
        auto base = "chpl__buildDomainRuntimeType";
        auto dist = new UnresolvedSymExpr("defaultDist");
        ret = new CallExpr(base, dist);
      } else if (name == USTR("unmanaged")) {
        ret = new CallExpr(PRIM_TO_UNMANAGED_CLASS_CHECKED);
      } else if (name == USTR("borrowed")) {
        ret = new CallExpr(PRIM_TO_BORROWED_CLASS_CHECKED);
      }

    // Unwrap 'foo.bytes' specially to avoid wrapping it in a third call.
    } else if (auto dot = node->toDot()) {
      auto member = dot->field();
      if (member == USTR("bytes")) {
        Expr* base = toExpr(convertAST(dot->receiver()));
        auto convDot = buildDotExpr(base, "chpl_bytes");
        ret = new CallExpr(convDot);
      }
    }

    return ret;
  }

  Expr* convertSparseKeyword(const uast::FnCall* node) {
    auto calledExpression = node->calledExpression();
    assert(calledExpression);
    CallExpr* ret = nullptr;

    if (auto kwSparse = calledExpression->toIdentifier()) {
      if (kwSparse->name() == USTR("sparse")) {
        assert(node->numActuals() == 1);

        if (auto innerCall = node->actual(0)->toFnCall()) {
          auto innerCalledExpression = innerCall->calledExpression();
          assert(innerCalledExpression);

          if (auto kwSubdomain = innerCalledExpression->toIdentifier()) {
            if (kwSubdomain->name() == USTR("subdomain")) {
              assert(innerCall->numActuals() == 1);

              ret = new CallExpr("chpl__buildSparseDomainRuntimeType");
              Expr* expr = convertAST(innerCall->actual(0));
              auto dot = buildDotExpr(expr->copy(), "defaultSparseDist");
              ret->insertAtTail(dot);
              ret->insertAtTail(expr);
            }
          }
        }
      }
    }

    return ret;
  }

  Expr* visit(const uast::FnCall* node) {
    const uast::Expression* calledExpression = node->calledExpression();
    INT_ASSERT(calledExpression);

    CallExpr* ret = nullptr;
    CallExpr* addArgsTo = nullptr;

    if (auto newExpression = calledExpression->toNew()) {
      CallExpr* newExprStart = convertNewManagement(newExpression);
      assert(newExprStart);

      // TODO: Need to check for special identifiers?
      Expr* typeExpr = convertAST(newExpression->typeExpression());

      auto initializerCall = new CallExpr(typeExpr);
      newExprStart->insertAtTail(initializerCall);

      ret = newExprStart;
      addArgsTo = initializerCall;

    // Special case 'sparse' since it's weird and can only appear one way.
    } else if (Expr* expr = convertSparseKeyword(node)) {
      return expr;

    // If a keyword produces a call, just use that instead of making one.
    } else if (Expr* expr = convertCalledKeyword(calledExpression)) {
      ret = isCallExpr(expr) ? toCallExpr(expr) : new CallExpr(expr);
      addArgsTo = ret;
    } else {
      ret = new CallExpr(convertAST(calledExpression));
      addArgsTo = ret;
    }

    // Convert and add the actual arguments.
    for (int i = 0; i < node->numActuals(); i++) {
      Expr* actual = convertAST(node->actual(i));
      INT_ASSERT(actual);
      if (node->isNamedActual(i)) {
        actual = buildNamedActual(node->actualName(i).c_str(), actual);
      }
      addArgsTo->insertAtTail(actual);
    }

    return ret;
  }

  Expr* convertDmappedOp(const uast::OpCall* node) {
    if (node->op() != USTR("dmapped")) return nullptr;

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

  Expr* convertTupleExpand(const uast::OpCall* node) {
    if (node->op() != USTR("...")) return nullptr;
    INT_ASSERT(node->numActuals() == 1);
    Expr* expr = convertAST(node->actual(0));
    return new CallExpr(PRIM_TUPLE_EXPAND, expr);
  }

  Expr* convertReduceAssign(const uast::OpCall* node) {
    if (node->op() != USTR("reduce=")) return nullptr;
    INT_ASSERT(node->numActuals() == 2);
    Expr* lhs = convertAST(node->actual(0));
    Expr* rhs = convertAST(node->actual(1));
    return buildAssignment(lhs, rhs, PRIM_REDUCE_ASSIGN);
  }

  Expr* convertToNilableChecked(const uast::OpCall* node) {
    if (node->op() != USTR("?")) return nullptr;
    INT_ASSERT(node->numActuals() == 1);
    Expr* expr = convertAST(node->actual(0));
    return new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, expr);
  }

  Expr* convertLogicalAndAssign(const uast::OpCall* node) {
    if (node->op() != USTR("&&=")) return nullptr;
    INT_ASSERT(node->numActuals() == 2);
    Expr* lhs = convertAST(node->actual(0));
    Expr* rhs = convertAST(node->actual(1));
    return buildLAndAssignment(lhs, rhs);
  }

  Expr* convertLogicalOrAssign(const uast::OpCall* node) {
    if (node->op() != USTR("||=")) return nullptr;
    INT_ASSERT(node->numActuals() == 2);
    Expr* lhs = convertAST(node->actual(0));
    Expr* rhs = convertAST(node->actual(1));
    return buildLOrAssignment(lhs, rhs);
  }

  Expr* convertRegularBinaryOrUnaryOp(const uast::OpCall* node,
                                      const char* name=nullptr) {
    const char* opName = name ? name : node->op().c_str();
    int nActuals = node->numActuals();
    CallExpr* ret = new CallExpr(opName);

    for (int i = 0; i < nActuals; i++) {
      Expr* actual = convertAST(node->actual(i));
      INT_ASSERT(actual);
      ret->insertAtTail(actual);
    }

    return ret;
  }

  Expr* visit(const uast::OpCall* node) {
    Expr* ret = nullptr;

    if (auto conv = convertDmappedOp(node)) {
      ret = conv;
    } else if (auto conv = convertTupleExpand(node)) {
      ret = conv;
    } else if (auto conv = convertReduceAssign(node)) {
      ret = conv;
    } else if (auto conv = convertToNilableChecked(node)) {
      ret = conv;
    } else if (auto conv = convertLogicalAndAssign(node)) {
      ret = conv;
    } else if (auto conv = convertLogicalOrAssign(node)) {
      ret = conv;
    } else if (node->op() == USTR("align")) {
      ret = convertRegularBinaryOrUnaryOp(node, "chpl_align");
    } else if (node->op() == USTR("by")) {
      ret = convertRegularBinaryOrUnaryOp(node, "chpl_by");
    } else {
      ret = convertRegularBinaryOrUnaryOp(node);
    }

    assert(ret != nullptr);

    return ret;
  }

  Expr* visit(const uast::PrimCall* node) {
    CallExpr* call = new CallExpr(PRIM_ACTUALS_LIST);
    SymExpr* se = buildStringLiteral(primTagToName(node->prim()));
    call->insertAtTail(se);
    for (auto actual : node->actuals()) {
      call->insertAtTail(convertAST(actual));
    }
    return buildPrimitiveExpr(call);
  }

  // Note that this conversion is for the reduce expression, and not for
  // the reduce intent (see conversion for 'WithClause').
  Expr* visit(const uast::Reduce* node) {
    assert(node->numActuals() == 1);
    Expr* opExpr = convertScanReduceOp(node->op());
    Expr* dataExpr = convertAST(node->actual(0));
    bool zippered = node->actual(0)->isZip();;
    return buildReduceExpr(opExpr, dataExpr, zippered);
  }

  Expr* visit(const uast::Scan* node) {
    assert(node->numActuals() == 1);
    Expr* opExpr = convertScanReduceOp(node->op());
    Expr* dataExpr = convertAST(node->actual(0));
    bool zippered = node->actual(0)->isZip();;
    return buildScanExpr(opExpr, dataExpr, zippered);
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
    BlockStmt* ret = new BlockStmt(BLOCK_SCOPELESS);

    // Ignore linkage name, the new parser should have emitted an error
    // because multi-decls cannot be renamed.
    (void) node->linkageName();

    for (auto decl : node->decls()) {
      assert(decl->linkage() == node->linkage());

      Expr* conv = nullptr;
      if (auto var = decl->toVariable()) {

        // Do not use the linkage name since multi-decls cannot be renamed.
        const bool useLinkageName = false;
        conv = convertVariable(var, useLinkageName);

      // Otherwise convert in a generic fashion.
      } else {
        conv = convertAST(decl);
      }

      assert(conv);
      ret->insertAtTail(conv);
    }

    if (!fDocs) {
      assert(!inTupleDecl);
      CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
      ret->insertAtTail(end);
    }

    return ret;
  }

  // Right now components are one of: Variable, Formal, TupleDecl.
  BlockStmt* convertTupleDeclComponents(const uast::TupleDecl* node) {
    BlockStmt* ret = new BlockStmt(BLOCK_SCOPELESS);

    const bool saveInTupleDecl = inTupleDecl;
    inTupleDecl = true;

    for (auto decl : node->decls()) {
      Expr* conv = nullptr;

      // Formals are converted into variables.
      if (auto formal = decl->toFormal()) {
        assert(formal->intent() == uast::Formal::DEFAULT_INTENT);
        assert(!formal->initExpression());
        assert(!formal->typeExpression());
        conv = new DefExpr(new VarSymbol(formal->name().c_str()));

      // Do not use the visitor because it produces a block statement.
      } else if (auto var = decl->toVariable()) {
        const bool useLinkageName = false;
        conv = convertVariable(var, useLinkageName);

      // It must be a tuple.
      } else {
        assert(decl->isTupleDecl());
        conv = convertAST(decl);
      }

      INT_ASSERT(conv);

      if (DefExpr* def = toDefExpr(conv)) {
        if (VarSymbol* var = toVarSymbol(def->sym)) {
          if (var->name[0] == '_' && var->name[1] == '\0') {

            // Old AST depends on this name to represent blank tuples.
            const char* blank = astr("chpl__tuple_blank");
            var->name = blank;
            var->cname = blank;
          }
        }
      }

      ret->insertAtTail(conv);
    }

    inTupleDecl = saveInTupleDecl;

    return ret;
  }

  // This builds a statement. Arguments use 'convertTupleDeclComponents'.
  BlockStmt* visit(const uast::TupleDecl* node) {
    auto tuple = convertTupleDeclComponents(node);
    auto typeExpr = convertExprOrNull(node->typeExpression());
    auto initExpr = convertExprOrNull(node->initExpression());

    BlockStmt* ret = buildTupleVarDeclStmt(tuple, typeExpr, initExpr);

    // Move the block info around like in 'buildVarDecls'.
    if (auto info = ret->blockInfoGet()) {
      INT_ASSERT(info->isNamed("_check_tuple_var_decl"));
      SymExpr* tuple = toSymExpr(info->get(1));
      tuple->symbol()->defPoint->insertAfter(info);
      ret->blockInfoSet(NULL);
    }

    // Add a PRIM_END_OF_STATEMENT.
    if (!fDocs) {
      assert(!inTupleDecl);
      CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
      ret->insertAtTail(end);
    }

    return ret;
  }

  /// ForwardingDecl ///
  Expr* visit(const uast::ForwardingDecl* node) {
    // ForwardingDecl may contain a VisibilityClause, an Expression,
    // or a Variable declartion
    if (node->expr()->isVisibilityClause()){
      auto child = node->expr()->toVisibilityClause();
      bool except=false;
      if (child->limitationKind() == uast::VisibilityClause::ONLY) {
        except=false;
      }
      else if (child->limitationKind() == uast::VisibilityClause::EXCEPT) {
        except=true;
      }
      // convert the ASTList of renames
      std::vector<PotentialRename*>* names = new std::vector<PotentialRename*>;
      for (auto lim:child->limitations()) {
        PotentialRename* name = convertRename(lim);
        names->push_back(name);
      }
      return buildForwardingStmt(convertExprOrNull(child->symbol()),
                                 names, except);
    } else if (node->expr()->isVariable()) {
        auto child = node->expr()->toVariable();
        return buildForwardingDeclStmt((BlockStmt*)visit(child));
    } else if (node->expr()->isFnCall()) {
      auto child = node->expr()->toFnCall();

      return buildForwardingStmt(convertExprOrNull(child));
    }

    INT_FATAL("Failed to convert ForwardingDecl");
    return nullptr;
  }

  /// NamedDecls ///

  Expr* visit(const uast::EnumElement* node) {
    INT_FATAL("Should not be called directly!");
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
    return (name == USTR("=") ||
            name == USTR("+=") ||
            name == USTR("-=") ||
            name == USTR("*=") ||
            name == USTR("/=") ||
            name == USTR("%=") ||
            name == USTR("**=") ||
            name == USTR("&=") ||
            name == USTR("|=") ||
            name == USTR("^=") ||
            name == USTR(">>=") ||
            name == USTR("<<="));
  }

  const char* convertFunctionNameAndAstr(UniqueString name) {
    const char* ret = nullptr;
    if (name == USTR("by")) {
      ret = "chpl_by";
    } else if (name == USTR("align")) {
      ret = "chpl_align";
    } else {
      ret = name.c_str();
    }

    assert(ret);

    // We have to uniquify the name now because it may be inlined (and thus
    // stack allocated).
    ret = astr(ret);

    return ret;
  }

  Expr* visit(const uast::Function* node) {
    FnSymbol* fn = new FnSymbol("_");

    attachSymbolAttributes(node, fn);

    if (node->isInline()) {
      fn->addFlag(FLAG_INLINE);
    }

    if (node->isOverride()) {
      fn->addFlag(FLAG_OVERRIDE);
    }

    if (node->isParenless()) {
      fn->addFlag(FLAG_NO_PARENS);
    }

    if (node->isMethod()) {
      fn->addFlag(FLAG_METHOD);
      if (node->isPrimaryMethod()) {
        fn->addFlag(FLAG_METHOD_PRIMARY);
      }
    }

    IntentTag thisTag = INTENT_BLANK;
    Expr* receiverType = nullptr;
    bool hasConvertedReceiver = false;

    // Add the formals
    if (node->numFormals() > 0) {
      for (auto decl : node->formals()) {
        DefExpr* conv = nullptr;

        // A "normal" formal.
        if (auto formal = decl->toFormal()) {

          // Special handling for implicit receiver formal.
          if (formal->name() == USTR("this")) {
            assert(!hasConvertedReceiver);
            hasConvertedReceiver = true;

            thisTag = convertFormalIntent(formal->intent());

            // TODO (dlongnecke): Error for new frontend!
            // "Type binding clauses are not supported..."
            if (node->isPrimaryMethod() && formal->typeExpression()) {
              receiverType = nullptr;
            } else {
              receiverType = convertExprOrNull(formal->typeExpression());
            }

          // Else convert it like normal.
          } else {
            conv = toDefExpr(convertAST(formal));
            assert(conv);
          }

        // A varargs formal.
        } else if (auto formal = decl->toVarArgFormal()) {
          assert(formal->name() != USTR("this"));
          conv = toDefExpr(convertAST(formal));
          assert(conv);

        // A tuple decl, where components are formals or tuple decls.
        } else if (auto formal = decl->toTupleDecl()) {
          auto castIntent = (uast::Formal::Intent)formal->intentOrKind();
          IntentTag tag = convertFormalIntent(castIntent);
          BlockStmt* tuple = convertTupleDeclComponents(formal);
          assert(tuple);

          Expr* type = convertExprOrNull(formal->typeExpression());
          Expr* init = convertExprOrNull(formal->initExpression());

          // TODO: Move this specialization into visitor? We can just
          // detect if components are formals.
          conv = buildTupleArgDefExpr(tag, tuple, type, init);
          assert(conv);
        } else {
          assert(0 == "Not handled yet!");
        }

        // Attaches def to function's formal list.
        if (conv) {
          buildFunctionFormal(fn, conv);
        }
      }
    }

    const char* convName = convertFunctionNameAndAstr(node->name());
    fn = buildFunctionSymbol(fn, convName, thisTag, receiverType);

    if (isAssignOp(node->name())) {
      fn->addFlag(FLAG_ASSIGNOP);
    }

    RetTag retTag = convertRetTag(node->returnIntent());

    if (node->kind() == uast::Function::ITER) {

      // TODO (dlongnecke): Move me to new frontend!
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

    Expr* retType = nullptr;
    if (auto retTypeExpr = node->returnType()) {
      if (auto arrayTypeExpr = retTypeExpr->toBracketLoop()) {
        retType = convertArrayType(arrayTypeExpr);
      } else {
        retType = convertAST(retTypeExpr);
      }
    }

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

    BlockStmt* body = nullptr;

    if (node->linkage() != uast::Decl::EXTERN) {
      body = createBlockWithStmts(node->stmts());

    // Clear the block statement for the body if function is extern.
    } else {
      if (node->numStmts()) {
        USR_FATAL_CONT("Extern functions cannot have a body");
      }
    }

    BlockStmt* ret = buildFunctionDecl(fn, retTag, retType, node->throws(),
                                       whereClause,
                                       lifetimeConstraints,
                                       body,
                                       /* docs */ nullptr);

    if (node->linkage() != uast::Decl::DEFAULT_LINKAGE) {
      Flag linkageFlag = convertFlagForDeclLinkage(node);
      Expr* linkageName = convertExprOrNull(node->linkageName());
      ret = buildExternExportFunctionDecl(linkageFlag, linkageName, ret);
    }

    return ret;
  }

  DefExpr* visit(const uast::Module* node) {
    chpl::UniqueString ustr = node->name();
    const char* name = ustr.c_str();
    const char* path = context->filePathForId(node->id()).c_str();

    // TODO (dlongnecke): For now, the tag is overridden by the caller.
    // See 'uASTAttemptToParseMod'. Eventually, it would be great if the
    // new frontend could note if a module is standard/internal/user.
    const ModTag tag = MOD_USER;
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

    if (node->kind() == uast::Module::IMPLICIT) {
      mod->addFlag(FLAG_IMPLICIT_MODULE);
    }

    attachSymbolAttributes(node, mod);

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

    Expr* typeExpr = nullptr;
    Expr* initExpr = convertExprOrNull(node->initExpression());

    if (auto te = node->typeExpression()) {
      if (auto bkt = te->toBracketLoop()) {
        typeExpr = convertArrayType(bkt);
      } else {
        typeExpr = convertAST(te);
      }
    }

    auto ret =  buildArgDefExpr(intentTag, node->name().c_str(),
                                typeExpr,
                                initExpr,
                                /*varargsVariable*/ nullptr);
    assert(ret->sym);

    attachSymbolAttributes(node, ret->sym);

    return ret;
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

  Expr* visit(const uast::VarArgFormal* node) {
    IntentTag intentTag = convertFormalIntent(node->intent());

    Expr* typeExpr = nullptr;
    Expr* initExpr = nullptr;

    INT_ASSERT(!node->initExpression());

    if (node->typeExpression()) {
      if (auto bkt = node->typeExpression()->toBracketLoop()) {
        typeExpr = convertArrayType(bkt);
      } else {
        typeExpr = convertAST(node->typeExpression());
      }
    }

    Expr* varargsVariable = convertExprOrNull(node->count());
    if (!varargsVariable) {
      varargsVariable = new SymExpr(gUninstantiated);
    }

    if (DefExpr* def = toDefExpr(varargsVariable)) {
      def->sym->addFlag(FLAG_PARAM);
    }

    auto ret = buildArgDefExpr(intentTag, node->name().c_str(),
                               typeExpr,
                               initExpr,
                               varargsVariable);
    assert(ret->sym);

    attachSymbolAttributes(node, ret->sym);

    return ret;
  }

  ShadowVarSymbol* convertTaskVar(const uast::TaskVar* node) {
    ShadowVarPrefix prefix = convertTaskVarIntent(node);
    Expr* nameExp = new UnresolvedSymExpr(node->name().c_str());
    Expr* type = convertExprOrNull(node->typeExpression());
    Expr* init = convertExprOrNull(node->initExpression());

    auto ret = ShadowVarSymbol::buildForPrefix(prefix, nameExp, type, init);

    assert(ret != nullptr);

    attachSymbolAttributes(node, ret);

    return ret;
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

    Expr* domActuals = new SymExpr(gNil);

    auto dom = node->iterand()->toDomain();
    INT_ASSERT(dom);

    // If there are no domain expressions, use 'nil'.
    if (!dom->numExprs()) {
      domActuals = new SymExpr(gNil);

    // Convert multiple domain expressions into a PRIM_ACTUALS_LIST.
    } else if (dom->numExprs() > 1) {
      CallExpr* actualsList = new CallExpr(PRIM_ACTUALS_LIST);
      domActuals = actualsList;

      for (auto expr : dom->exprs()) {
        actualsList->insertAtTail(convertAST(expr));
      }

      domActuals = new CallExpr("chpl__ensureDomainExpr", actualsList);

    // Use a single argument directly.
    } else {
      domActuals = convertAST(dom->expr(0));

      // But wrap it if it is not a type query.
      if (!dom->expr(0)->isTypeQuery()) {
        domActuals = new CallExpr("chpl__ensureDomainExpr", domActuals);
      }
    }

    INT_ASSERT(domActuals);

    Expr* subType = nullptr;
    if (node->numStmts()) {

      // Handle the possibility of nested array types.
      if (auto bkt = node->stmt(0)->toBracketLoop()) {
        subType = convertArrayType(bkt);
      } else {
        subType = convertAST(node->stmt(0));
      }

      INT_ASSERT(subType);
    }

    CallExpr* ret = new CallExpr("chpl__buildArrayRuntimeType",
                                 domActuals,
                                 subType);

    return ret;
  }

  // Returns a DefExpr that has not yet been inserted into the tree.
  DefExpr* convertVariable(const uast::Variable* node,
                           bool useLinkageName) {
    auto varSym = new VarSymbol(tupleVariableName(node->name().c_str()));

    // Adjust the variable according to its kind, e.g. 'const'/'type'.
    attachVarSymbolStorage(node, varSym);

    attachSymbolAttributes(node, varSym);

    attachSymbolVisibility(node, varSym);

    if (node->isConfig()) {
      varSym->addFlag(FLAG_CONFIG);
    }

    Flag linkageFlag = convertFlagForDeclLinkage(node);
    if (linkageFlag != FLAG_UNKNOWN) {
      varSym->addFlag(linkageFlag);
    }

    // TODO (dlongnecke): Should be sanitized by the new parser.
    if (useLinkageName) {
      if (auto linkageName = node->linkageName()) {
        assert(linkageFlag != FLAG_UNKNOWN);
        auto strLit = linkageName->toStringLiteral();
        assert(strLit);
        varSym->cname = astr(strLit->str().c_str());
      }
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

    auto ret = new DefExpr(varSym, initExpr, typeExpr);

    // Replace init expressions for config variables with values passed
    // in on the command-line, if necessary.
    if (node->isConfig()) {

      // TODO (dlongnecke): This call should be replaced by an equivalent
      // one from the new frontend.
      if (Expr* commandLineInit = lookupConfigVal(varSym)) {
        ret->init = commandLineInit;
      }
    }

    return ret;
  }

  Expr* visit(const uast::Variable* node) {
    auto stmts = new BlockStmt(BLOCK_SCOPELESS);

    auto defExpr = convertVariable(node, true);
    assert(defExpr);

    stmts->insertAtTail(defExpr);

    // Special handling for extern type variables.
    if (node->kind() == uast::Variable::TYPE) {
      if (node->linkage() == uast::Decl::EXTERN) {
        assert(!node->isConfig());
        stmts = convertTypesToExtern(stmts);
      }
    }

    // Add a PRIM_END_OF_STATEMENT.
    if (fDocs == false && inTupleDecl == false) {
      CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
      stmts->insertAtTail(end);
    }

    return stmts;
  }

  /// TypeDecls

  // Does not attach parent type.
  DefExpr* convertEnumElement(const uast::EnumElement* node) {
    const char* name = node->name().c_str();
    Expr* initExpr = convertExprOrNull(node->initExpression());
    auto ret = new DefExpr(new EnumSymbol(name), initExpr);
    return ret;
  }

  Expr* visit(const uast::Enum* node) {
    auto enumType = new EnumType();

    for (auto elem : node->enumElements()) {
      DefExpr* convElem = convertEnumElement(elem);
      convElem->sym->type = enumType;
      enumType->constants.insertAtTail(convElem);

      if (enumType->defaultValue == nullptr) {
        enumType->defaultValue = convElem->sym;
      }
    }

    auto enumTypeSym = new TypeSymbol(node->name().c_str(), enumType);

    attachSymbolAttributes(node, enumTypeSym);

    enumType->symbol = enumTypeSym;

    auto ret = new BlockStmt(BLOCK_SCOPELESS);
    ret->insertAtTail(new DefExpr(enumTypeSym));

    return ret;
  }

  /// AggregateDecls

  Expr* visit(const uast::Class* node) {
    const char* name = node->name().c_str();
    const char* cname = name;
    Expr* inherit = convertExprOrNull(node->parentClass());
    BlockStmt* decls = createBlockWithStmts(node->declOrComments());
    Flag externFlag = FLAG_UNKNOWN;

    auto ret = buildClassDefExpr(name, cname, AGGREGATE_CLASS, inherit,
                                 decls,
                                 externFlag,
                                 /*docs*/ nullptr);
    assert(ret->sym);

    attachSymbolAttributes(node, ret->sym);

    return ret;
  }

  Expr* visit(const uast::Record* node) {
    const char* name = node->name().c_str();
    const char* cname = name;
    Expr* inherit = nullptr;
    BlockStmt* decls = createBlockWithStmts(node->declOrComments());
    Flag linkageFlag = convertFlagForDeclLinkage(node);

    // TODO (dlongnecke): This should be sanitized by the new parser.
    if (node->linkageName()) {
      cname = astrFromStringLiteral(node->linkageName());
      assert(cname);
    }

    auto ret = buildClassDefExpr(name, cname, AGGREGATE_RECORD, inherit,
                                 decls,
                                 linkageFlag,
                                 /*docs*/ nullptr);
    assert(ret->sym);

    attachSymbolAttributes(node, ret->sym);

    return ret;
  }

  Expr* visit(const uast::Union* node) {
    const char* name = node->name().c_str();
    const char* cname = name;
    Expr* inherit = nullptr;
    BlockStmt* decls = createBlockWithStmts(node->declOrComments());
    Flag linkageFlag = convertFlagForDeclLinkage(node);

    // TODO (dlongnecke): This should be sanitized by the new parser.
    if (node->linkageName()) {
      cname = astrFromStringLiteral(node->linkageName());
      assert(cname);
    }

    auto ret = buildClassDefExpr(name, cname, AGGREGATE_UNION, inherit,
                                 decls,
                                 linkageFlag,
                                 /*docs*/ nullptr);
    assert(ret->sym);

    attachSymbolAttributes(node, ret->sym);

    return ret;
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
  return ret;
}
