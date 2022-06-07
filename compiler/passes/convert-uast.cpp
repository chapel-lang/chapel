/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#include <iostream>

#include "convert-uast.h"

#include "CatchStmt.h"
#include "DeferStmt.h"
#include "DoWhileStmt.h"
#include "ForLoop.h"
#include "ForallStmt.h"
#include "IfExpr.h"
#include "TryStmt.h"
#include "WhileDoStmt.h"
#include "build.h"
#include "config.h"
#include "docsDriver.h"
#include "optimizations.h"
#include "parser.h"
#include "resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/global-strings.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/util/string-escapes.h"

// If this is set then variables/formals will have their "qual" field set
// now instead of later during resolution.
#define ATTACH_QUALIFIED_TYPES_EARLY 0

using namespace chpl;

namespace {

struct Converter {
  struct ModStackEntry {
    const uast::Module* mod;
    ModStackEntry(const uast::Module* mod)
      : mod(mod) {
    }
  };
  struct SymStackEntry {
    const uast::AstNode* ast;
    const resolution::ResolutionResultByPostorderID* resolved;
    SymStackEntry(const uast::AstNode* ast,
                  const resolution::ResolutionResultByPostorderID* resolved)
      : ast(ast), resolved(resolved) {
    }
  };

  chpl::Context* context = nullptr;
  bool inTupleDecl = false;
  ModTag topLevelModTag;
  // TODO: remove latestComment and builderResult once
  // chpldoc is implemented as a separate tool on uAST
  const char* latestComment = nullptr;
  const uast::BuilderResult& builderResult;
  std::vector<ModStackEntry> modStack;
  std::vector<SymStackEntry> symStack;

  std::unordered_map<const uast::AstNode*, BaseAST*> alreadyConverted;

  Converter(chpl::Context* context, ModTag topLevelModTag,
            const uast::BuilderResult& builderResult)
    : context(context),
      topLevelModTag(topLevelModTag),
      builderResult(builderResult) {}

  // general functions for converting
  Expr* convertAST(const uast::AstNode* node);
  Type* convertType(const types::QualifiedType qt);
  Symbol* convertParam(const types::QualifiedType qt);
  FnSymbol* convertFnOutOfOrder(ID fnId);

  // convertAST helpers
  void setVariableType(const uast::VarLikeDecl* v, Symbol* sym);
  void setResolvedCall(const uast::FnCall* call, CallExpr* ret);

  // type conversion helpers
  Type* convertClassType(const types::QualifiedType qt);
  Type* convertEnumType(const types::QualifiedType qt);
  Type* convertFunctionType(const types::QualifiedType qt);
  Type* convertBasicClassType(const types::QualifiedType qt);
  Type* convertRecordType(const types::QualifiedType qt);
  Type* convertTupleType(const types::QualifiedType qt);
  Type* convertUnionType(const types::QualifiedType qt);
  Type* convertBoolType(const types::QualifiedType qt);
  Type* convertComplexType(const types::QualifiedType qt);
  Type* convertImagType(const types::QualifiedType qt);
  Type* convertIntType(const types::QualifiedType qt);
  Type* convertRealType(const types::QualifiedType qt);
  Type* convertUintType(const types::QualifiedType qt);


  const char* consumeLatestComment() {
    const char* ret = latestComment;
    latestComment = nullptr;
    return ret;
  }

  static bool shouldResolve(UniqueString symbolPath) {
    // TODO: check for dead modules and don't resolve those
    //return symbolPath == "M" || symbolPath.startsWith("M.");
    return false;
  }
  static bool shouldResolve(ID symbolId) {
    return shouldResolve(symbolId.symbolPath());
  }
  static bool shouldResolve(const uast::AstNode* node) {
    return shouldResolve(node->id());
  }

  Expr* convertExprOrNull(const uast::AstNode* node) {
    if (node == nullptr)
      return nullptr;

    Expr* ret = convertAST(node);
    INT_ASSERT(ret);
    return ret;
  }

  const char* convertLinkageNameAstr(const uast::Decl* node) {
    if (auto linkageName = node->linkageName()) {
      if (auto linkageStr = linkageName->toStringLiteral()) {
        return astr(linkageStr->str());
      }
    }

    return nullptr;
  }

  Flag convertFlagForDeclLinkage(const uast::AstNode* node) {
    if (auto decl = node->toDecl()) {
      switch (decl->linkage()) {
        case uast::Decl::EXTERN: return FLAG_EXTERN;
        case uast::Decl::EXPORT: return FLAG_EXPORT;
        default: return FLAG_UNKNOWN;
      }
    }

    return FLAG_UNKNOWN;
  }

  static bool isBlockComment(const uast::Comment* node) {
    const auto& str = node->str();
    if (str.size() < 4) return false;
    if (str[0] != '/' || str[1] != '*') return false;
    INT_ASSERT(str[str.size()-1] == '/');
    INT_ASSERT(str[str.size()-2] == '*');
    return true;
  }

  // This code duplicates what was done in 'processBlockComment' for the old
  // scanner, minus all the state tracking for the scanner itself. It is
  // meant to prepare a comment for use with chpldoc.
  const char* trimBlockCommentForDocs(const uast::Comment* node) {
    INT_ASSERT(isBlockComment(node));
    INT_ASSERT(fDocs);

    const auto& com = node->str();
    int delimLen = strlen(fDocsCommentLabel);
    int delimIdx = (delimLen >= 2) ? 2 : 0;
    std::string str, line;
    bool badComment = false;
    int depth = 1;
    size_t idx = 2;   // Skip opening '/*'.
    int llc = 0;      // Last last char.
    int lc = 0;       // Last char.
    int c = 0;        // Current char.
    int d = 1;        // TODO: Better name?

    while (depth > 0) {
      llc = lc;
      lc = c;
      c = idx < com.size() ? com[idx++] : 0; // Advance the scanner.

      // Skip opening delimiter.
      if (idx < 2) {
        INT_ASSERT(c == '/' || c == '*');
        continue;
      }

      // For newlines, append entire line if past the delimiter.
      if (c == '\n') {
        if (delimIdx == delimLen) {
          str += line;
          str += '\n';
        }
        line.clear();

      // Maybe advance the delimiter, append character.
      } else {
        if ((delimIdx < delimLen) && (delimIdx != -1)) {
          if (c == fDocsCommentLabel[delimIdx]) {
            delimIdx++;
          } else {
            delimIdx = -1;
          }
        }

        line += c;
      }

      if (delimLen != 0 && c == fDocsCommentLabel[delimLen - d]) {
        d++;
      } else {
        d = 1;
      }

      // TODO: Eliminate depth tracking?
      if (lc == '*' && c == '/' && llc != '/') {
        if (delimIdx == delimLen && d != delimLen + 1) badComment = true;
        depth--;
        d = 1;
      } else if (lc == '/' && c == '*') {
        depth++;
      } else if (c == 0) {
        INT_FATAL("Should not reach here!");
      }
    }

    // Back up to not print delimiter again.
    if (line.size() >= 2) {
      line.resize(line.size() - 2);
    }

    // Even further for special delimiters.
    if (delimLen > 2 && delimIdx == delimLen) {
      line.resize(line.size() - (delimLen - 2));
    }

    if (delimIdx == delimLen) {
      str += line;

      if (delimLen > 2) {
        delimLen -= 2;
        str = str.substr(delimLen);
      }

      // TODO: What is this doing?
      auto loc = str.find("\\x09");
      while (loc != std::string::npos) {
        str = str.substr(0, loc) + str.substr(loc + 4);
        str.insert(loc, "\t");
        loc = str.find("\\x09");
      }
    }

    if (badComment) {
      auto loc = builderResult.commentToLocation(node);
      INT_ASSERT(!loc.isEmpty());
      USR_WARN(loc, "chpldoc comment not closed, "
                    "ignoring comment:%s\n",
                    str.c_str());
      return nullptr;
    }

    auto ret = str.size() ? astr(str) : nullptr;

    return ret;
  }

  Expr* visit(const uast::Comment* node) {
    if (!fDocs) return nullptr;
    INT_ASSERT(node->str().size() >= 2);
    latestComment = isBlockComment(node) ? trimBlockCommentForDocs(node)
                                         : nullptr;
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
      INT_ASSERT(attr->deprecationMessage().isEmpty());
    }

    if (attr->isDeprecated()) {
      INT_ASSERT(!sym->hasFlag(FLAG_DEPRECATED));
      sym->addFlag(FLAG_DEPRECATED);

      auto msg = attr->deprecationMessage();
      if (!msg.isEmpty()) {
        sym->deprecationMsg = astr(msg);
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
    } else if (name == USTR("locale")) {
      return new SymExpr(dtLocale->symbol);
    } else if (name == USTR("uint")) {
      return new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol);
    } else if (name == USTR("int")) {
      return new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol);
    } else if (name == USTR("real")) {
      return new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol);
    } else if (name == USTR("complex")) {
      return new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol);
    } else if (name == USTR("align")) {
      return new UnresolvedSymExpr("chpl_align");
    } else if (name == USTR("by")) {
      return new UnresolvedSymExpr("chpl_by");
    } else if (name == USTR("_")) {
      return new UnresolvedSymExpr("chpl__tuple_blank");
    } else if (name == USTR("void")) {
      return new SymExpr(dtVoid->symbol);
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

  Expr* visit(const uast::Implements* node) {
    const char* name = astr(node->interfaceName());
    CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST);
    Expr* ret = nullptr;

    if (node->typeIdent()) {
      auto conv = convertAST(node->typeIdent());
      INT_ASSERT(conv);
      act->insertAtTail(conv);
    }

    Expr* conv = convertAST(node->interfaceExpr());
    if (auto call = toCallExpr(conv)) {
      for_actuals(actual, call) {
        actual->remove();
        act->insertAtTail(actual);
      }
    }

    if (node->isExpressionLevel()) {
      ret = IfcConstraint::build(name, act);
    } else {
      ret = ImplementsStmt::build(name, act, nullptr);
    }

    INT_ASSERT(ret);

    return ret;
  }

  /// SimpleBlockLikes ///

  BlockStmt*
  convertExplicitBlock(uast::AstListIteratorPair<uast::AstNode> stmts,
                       bool flattenTopLevelScopelessBlocks) {
    BlockStmt* ret = new BlockStmt();

    for (auto stmt: stmts) {
      astlocMarker markAstLoc(stmt->id());

      Expr* e = convertAST(stmt);
      if (!e) continue;

      bool inserted = false;
      if (flattenTopLevelScopelessBlocks) {
        if (auto childBlock = toBlockStmt(e)) {
          ret->appendChapelStmt(childBlock);
          inserted = true;
        }
      }

      if (!inserted) {
        ret->insertAtTail(e);
      }
    }

    return ret;
  }

  BlockStmt*
  convertImplicitBlock(uast::AstListIteratorPair<uast::AstNode> stmts) {
    BlockStmt* ret = nullptr;

    for (auto stmt: stmts) {
      astlocMarker markAstLoc(stmt->id());
      Expr* e = convertAST(stmt);
      if (!e) continue;
      if (ret) INT_FATAL("implicit block with multiple statements");
      ret = isBlockStmt(e) ? toBlockStmt(e) : buildChapelStmt(e);
    }

    return ret;
  }

  BlockStmt*
  createBlockWithStmts(uast::AstListIteratorPair<uast::AstNode> stmts,
                       uast::BlockStyle style,
                       bool flattenTopLevelScopelessBlocks=true) {
    BlockStmt* ret = nullptr;

    switch (style) {
      case uast::BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK:
      case uast::BlockStyle::EXPLICIT:
        ret = convertExplicitBlock(stmts, flattenTopLevelScopelessBlocks);
        break;
      case uast::BlockStyle::IMPLICIT:
        ret = convertImplicitBlock(stmts);
        break;
    }

    INT_ASSERT(ret);

    return ret;
  }

  Expr*
  singleExprFromStmts(uast::AstListIteratorPair<uast::AstNode> stmts) {
    Expr* ret = nullptr;

    for (auto stmt: stmts) {
      if (ret != nullptr) return nullptr;
      ret = convertAST(stmt);
    }

    INT_ASSERT(ret != nullptr);

    return ret;
  }

  Expr* visit(const uast::Begin* node) {
    auto byrefVars = convertWithClause(node->withClause(), node);
    auto stmt = createBlockWithStmts(node->stmts(), node->blockStyle());
    return buildBeginStmt(byrefVars, stmt);
  }

  BlockStmt* visit(const uast::Block* node) {
    return createBlockWithStmts(node->stmts(), node->blockStyle());
  }

  Expr* visit(const uast::Defer* node) {
    auto stmts = createBlockWithStmts(node->stmts(), node->blockStyle());
    return DeferStmt::build(stmts);
  }

  BlockStmt* visit(const uast::Local* node) {
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
    auto condition = convertExprOrNull(node->condition());
    if (condition) {
      return buildLocalStmt(condition, body);
    } else {
      return buildLocalStmt(body);
    }
  }

  Expr* visit(const uast::Manage* node) {
    auto managers = new BlockStmt();

    for (auto manager : node->managers()) {

      // TODO: This is deleted by the callee, odd convention...
      std::set<Flag>* flags = nullptr;
      Expr* managerExpr = nullptr;
      const char* resourceName = nullptr;

      if (auto as = manager->toAs()) {
        managerExpr = convertAST(as->symbol());

        auto var = as->rename()->toVariable();
        INT_ASSERT(var);
        INT_ASSERT(!var->initExpression() && !var->typeExpression());

        resourceName = astr(var->name());

        // TODO: I'm not sure what the best way to get flags is here.
        if (var->kind() != uast::Variable::INDEX) {
          flags = new std::set<Flag>;

          // TODO: Duplication here and with 'attachSymbolStorage',
          // consider cleaning up after parser is replaced.
          switch (var->kind()) {
            case uast::Variable::CONST:
              flags->insert(FLAG_CONST);
              break;
            case uast::Variable::CONST_REF:
              flags->insert(FLAG_REF_VAR);
              flags->insert(FLAG_CONST);
              break;
            case uast::Variable::PARAM:
              flags->insert(FLAG_PARAM);
              break;
            case uast::Variable::REF:
              flags->insert(FLAG_REF_VAR);
              break;
            case uast::Variable::TYPE:
              flags->insert(FLAG_TYPE_VARIABLE);
              break;
            default: break;
          }
        }
      } else {
        managerExpr = convertAST(manager);
      }

      INT_ASSERT(managerExpr);

      auto conv = buildManagerBlock(managerExpr, flags, resourceName);
      INT_ASSERT(conv);

      managers->insertAtTail(conv);
    }

    auto block = createBlockWithStmts(node->stmts(), node->blockStyle());

    auto ret = buildManageStmt(managers, block);
    INT_ASSERT(ret);

    return ret;
  }

  BlockStmt* visit(const uast::On* node) {
    Expr* expr = convertAST(node->destination());
    Expr* stmt = createBlockWithStmts(node->stmts(), node->blockStyle());
    return buildOnStmt(expr, stmt);
  }

  BlockStmt* visit(const uast::Serial* node) {
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
    auto condition = convertExprOrNull(node->condition());

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
    auto block = createBlockWithStmts(node->stmts(), node->blockStyle());

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
    } else if (member == USTR("align")) {
      return buildDotExpr(base, "chpl_align");
    } else if (member == USTR("by")) {
      return buildDotExpr(base, "chpl_by");
    } else {
      return buildDotExpr(base, member.c_str());
    }
  }

  Expr* visit(const uast::ExternBlock* node) {
    return buildExternBlockStmt(astr(node->code()));
  }

  Expr* visit(const uast::Require* node) {
    CallExpr* actuals = new CallExpr(PRIM_ACTUALS_LIST);
    for (auto expr : node->exprs()) {
      Expr* conv = convertAST(expr);
      INT_ASSERT(conv);
      actuals->insertAtTail(conv);
    }

    return buildRequireStmt(actuals);
  }

  // Copy the body of 'buildIncludeModule' since it is heavily tied to the
  // old parser's implementation.
  Expr* visit(const uast::Include* node) {
    const char* name = astr(node->name());
    bool isPrivate = node->visibility() == uast::Decl::PRIVATE;
    bool isPrototype = node->isPrototype();

    // Consume the comment but do not use it - module comment is used.
    auto comment = consumeLatestComment();
    (void) comment;

    auto& loc = chpl::parsing::locateAst(gContext, node);
    INT_ASSERT(!loc.isEmpty());
    auto path = astr(loc.path());
    ModuleSymbol* mod = parseIncludedSubmodule(name, path);
    INT_ASSERT(mod);

    if (isPrivate && !mod->hasFlag(FLAG_PRIVATE)) {
      mod->addFlag(FLAG_PRIVATE);
    }

    if (mod->hasFlag(FLAG_PRIVATE) && !isPrivate) {
      USR_FATAL_CONT(node->id(),
                     "cannot make a private module public through "
                     "an include statement");
      USR_PRINT(mod, "module declared private here");
    }

    if (isPrototype) {
      USR_FATAL_CONT(node->id(), "cannot apply prototype to module in "
                                 "include statement");
      USR_PRINT(mod, "put prototype keyword at module declaration here");
    }

    if (fWarnUnstable && mod->modTag == MOD_USER) {
      USR_WARN(node->id(), "module include statements are not yet stable "
                           "and may change");
    }

    return buildChapelStmt(new DefExpr(mod));
  }

  Expr* visit(const uast::Import* node) {
    const bool isPrivate = node->visibility() != uast::Decl::PUBLIC;
    auto ret = new BlockStmt(BLOCK_SCOPELESS);

    for (auto vc : node->visibilityClauses()) {
      ImportStmt* conv = nullptr;

      switch (vc->limitationKind()) {
        case uast::VisibilityClause::NONE: {
          INT_ASSERT(vc->numLimitations() == 0);

          // Handles case: 'import foo as bar'
          if (auto as = vc->symbol()->toAs()) {
            Expr* mod = convertAST(as->symbol());
            auto ident = as->rename()->toIdentifier();
            INT_ASSERT(ident);
            const char* rename = astr(ident->name());
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
          INT_FATAL("Not possible!");
          break;
      }

      INT_ASSERT(conv != nullptr);

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
    astlocMarker markAstLoc(node->id());

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
      default: INT_FATAL("Not handled!"); break;
    }

    INT_ASSERT(symManager);

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

  std::pair<Expr*, Expr*> convertAsForRename(const uast::As* node) {
    astlocMarker markAstLoc(node->id());

    Expr* one = toExpr(convertAST(node->symbol()));
    auto renameIdent = node->rename()->toIdentifier();
    assert(renameIdent);
    Expr* two = new UnresolvedSymExpr(renameIdent->name().c_str());
    return std::pair<Expr*, Expr*>(one, two);
  }

  Expr* visit(const uast::As* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  PotentialRename* convertRename(const uast::AstNode* node) {
    astlocMarker markAstLoc(node->id());

    PotentialRename* ret = new PotentialRename();

    if (auto as = node->toAs()) {
      ret->tag = PotentialRename::DOUBLE;
      ret->renamed = new std::pair<Expr*, Expr*>(convertAsForRename(as));
    } else {
      ret->tag = PotentialRename::SINGLE;
      ret->elem = toExpr(convertAST(node));
    }

    return ret;
  }

  BlockStmt* convertUsePossibleLimitations(const uast::Use* node) {
    astlocMarker markAstLoc(node->id());

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
      auto exprs = convertAsForRename(as);
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
    // special handling case when visibility is `only` and limitations list
    // is empty.
    // old parser expects an empty potential rename to indicate something like
    // use A only;
    if (vc->limitationKind() == uast::VisibilityClause::ONLY &&
        vc->numLimitations()==0) {
      PotentialRename* ret = new PotentialRename();
      ret->tag = PotentialRename::SINGLE;
      ret->elem = new UnresolvedSymExpr("");
      names->push_back(ret);
    }
    return buildUseStmt(mod, rename, names, except, privateUse);
  }

  BlockStmt* convertUseNoLimitations(const uast::Use* node) {
    astlocMarker markAstLoc(node->id());

    auto args = new std::vector<PotentialRename*>(); // TODO LEAK
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

  Expr* convertScanReduceOp(const uast::AstNode* node) {
    if (auto opIdent = node->toIdentifier()) {
      auto name = opIdent->name();
      if (name == USTR("+"))
        return new UnresolvedSymExpr("SumReduceScanOp");
      if (name == USTR("*"))
        return new UnresolvedSymExpr("ProductReduceScanOp");
      if (name == USTR("&&"))
        return new UnresolvedSymExpr("LogicalAndReduceScanOp");
      if (name == USTR("||"))
        return new UnresolvedSymExpr("LogicalOrReduceScanOp");
      if (name == USTR("&"))
        return new UnresolvedSymExpr("BitwiseAndReduceScanOp");
      if (name == USTR("|"))
        return new UnresolvedSymExpr("BitwiseOrReduceScanOp");
      if (name == USTR("^"))
        return new UnresolvedSymExpr("BitwiseXorReduceScanOp");
    }

    auto ret = convertAST(node);
    return ret;
  }

  // Note that there are two ways to translate this. In all cases the
  // contents get converted to a PRIM_ACTUALS_LIST. However, some of the
  // loop nodes (Forall/Foreach/BracketLoop) will want to call
  // 'addForallIntent' when adding converted children to the list, while
  // everything else will want to call 'addTaskIntent'.
  CallExpr* convertWithClause(const uast::WithClause* node,
                              const uast::AstNode* parent) {
    if (node == nullptr) return nullptr;

    astlocMarker markAstLoc(node->id());

    CallExpr* ret = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto expr : node->exprs()) {
      ShadowVarSymbol* svs = nullptr;

      // Normal conversion of TaskVar, reduce intents handled below.
      if (const uast::TaskVar* tv = expr->toTaskVar()) {
        svs = convertTaskVar(tv);
        INT_ASSERT(svs);

      // Handle reductions in with clauses explicitly here.
      } else if (const uast::Reduce* rd = expr->toReduce()) {
        astlocMarker markAstLoc(rd->id());

        Expr* ovar = convertAST(rd->iterand());
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
    const char* name = nullptr;
    if (auto target = node->target()) name = astr(target->name());
    return buildGotoStmt(GOTO_BREAK, name);
  }

  CatchStmt* visit(const uast::Catch* node) {
    auto errorVar = node->error();
    const char* name = errorVar ? astr(errorVar->name()) : nullptr;
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

    INT_ASSERT(ret != nullptr);

    return ret;
  }

  Expr* visit(const uast::Cobegin* node) {
    CallExpr* byrefVars = convertWithClause(node->withClause(), node);
    auto style = uast::BlockStyle::EXPLICIT;
    auto block = createBlockWithStmts(node->taskBodies(), style);
    return buildCobeginStmt(byrefVars, block);
  }

  Expr* visit(const uast::Let* node) {
    BlockStmt* decls = new BlockStmt(BLOCK_SCOPELESS);

    for (auto decl : node->decls()) {
      Expr* conv = nullptr;

      if (auto var = decl->toVariable()) {
        const bool useLinkageName = false;
        conv = convertVariable(var, useLinkageName);
      } else {
        // TODO: Might need to do something different on this path.
        conv = convertAST(decl);
      }
      if (!decl->isComment()) {
        INT_ASSERT(conv);
        decls->insertAtTail(conv);
      }
    }

    Expr* expr = convertAST(node->expression());
    INT_ASSERT(expr);

    return buildLetExpr(decls, expr);
  }

  /*
   * This helper checks if a conditional node has an assignment op in its
   * condition expression, and reproduces an error similar to that in the
   * old parser
   * NOTE: This checking should move to the dyno resolver in the future
   */
  bool checkAssignConditional(const uast::Conditional* node) {
    bool assignOp = false;
    if (node->condition()->isOpCall() ) {
      auto opCall = node->condition()->toOpCall();
      auto op = opCall->op();
      if (op == USTR("=")) {
        assignOp = true;
        USR_FATAL_CONT(convertAST(opCall->actual(0)),
                       "Assignment is illegal in a conditional");
        USR_PRINT(convertAST(opCall->actual(0)),
                  "Use == to check for equality in a conditional");
      } else if (op == USTR("+=") || op == USTR("-=") || op == USTR("*=")
                 || op == USTR("/=") || op == USTR("%=") || op == USTR("**=")
                 || op == USTR("&=") || op == USTR("|=") || op == USTR("^=")
                 || op == USTR(">>=")|| op == USTR("<<=")) {
        assignOp = true;
        USR_FATAL_CONT(convertAST(opCall->actual(0)),
                       "Assignment operation %s is illegal in a conditional",
                       op.c_str());
      }
    }
    return assignOp;
  }

  Expr* visit(const uast::Conditional* node) {
    INT_ASSERT(node->condition());

    /*
     * NOTE: we need to check for assignment in conditionals as the old parser
     * was handling this. In the future, this should move to the dyno resolver
     */
    if (checkAssignConditional(node)) USR_STOP();

    Expr* ret = nullptr;

    if (node->isExpressionLevel()) {
      auto cond = toExpr(convertAST(node->condition()));
      INT_ASSERT(cond);
      auto thenExpr = singleExprFromStmts(node->thenStmts());
      INT_ASSERT(thenExpr);
      auto elseExpr = singleExprFromStmts(node->elseStmts());
      INT_ASSERT(elseExpr);
      ret = new IfExpr(cond, thenExpr, elseExpr);

    } else {
      auto thenStyle = node->thenBlockStyle();
      Expr* thenBlock = nullptr;
      auto elseStyle = node->elseBlockStyle();
      Expr* elseBlock = nullptr;

      {
        astlocMarker markAstLoc(node->thenBlock()->id());
        thenBlock = createBlockWithStmts(node->thenStmts(), thenStyle);
      }

      if (node->hasElseBlock()) {
        astlocMarker markAstLoc(node->elseBlock()->id());
        elseBlock = createBlockWithStmts(node->elseStmts(), elseStyle);
      }

      Expr* cond = nullptr;

      // TODO: Can 'ifVars' happen in expression-level conditionals?
      if (auto ifVar = node->condition()->toVariable()) {
        INT_ASSERT(ifVar->kind() == uast::Variable::CONST ||
                   ifVar->kind() == uast::Variable::VAR);
        INT_ASSERT(ifVar->initExpression());
        // astr() varNameStr so it doesn't go out of scope when passed to
        // buildIfVar
        auto varNameStr = astr(ifVar->name());
        auto initExpr = toExpr(convertAST(ifVar->initExpression()));
        bool isConst = ifVar->kind() == uast::Variable::CONST;
        cond = buildIfVar(varNameStr, initExpr, isConst);
      } else {
        cond = toExpr(convertAST(node->condition()));
      }

      INT_ASSERT(cond);

      ret = buildIfStmt(cond, thenBlock, elseBlock);
    }

    INT_ASSERT(ret != nullptr);

    return ret;
  }

  BlockStmt* visit(const uast::Continue* node) {
    const char* name = nullptr;
    if (auto target = node->target()) name = astr(target->name());
    return buildGotoStmt(GOTO_CONTINUE, name);
  }

  Expr* visit(const uast::Label* node) {
    const char* name = astr(node->name());
    Expr* stmt = toExpr(convertAST(node->loop()));
    INT_ASSERT(stmt);
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
    auto block = createBlockWithStmts(node->stmts(), node->blockStyle());
    return buildSyncStmt(block);
  }

  CallExpr* visit(const uast::Throw* node) {
    CallExpr* ret = new CallExpr(PRIM_THROW);
    ret->insertAtTail(toExpr(convertAST(node->errorExpression())));
    return ret;
  }

  Expr* visit(const uast::Try* node) {
    if (node->isExpressionLevel()) {

      INT_ASSERT(node->numStmts() == 1);
      INT_ASSERT(!node->stmt(0)->isBlock());
      Expr* expr = convertAST(node->stmt(0));

      // Use this instead of 'TryStmt::build'.
      auto ret = node->isTryBang() ? tryBangExpr(expr) : tryExpr(expr);
      INT_ASSERT(ret);

      return ret;

    } else {
      bool tryBang = node->isTryBang();
      auto style = uast::BlockStyle::EXPLICIT;
      auto body = createBlockWithStmts(node->stmts(), style);
      BlockStmt* catches = new BlockStmt();
      bool isSyncTry = false; // TODO: When can this be true?

      for (auto handler : node->handlers()) {
        INT_ASSERT(handler->isCatch());
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
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
    return DoWhileStmt::build(condExpr, body);
  }

  BlockStmt* visit(const uast::While* node) {
    Expr* condExpr = nullptr;
    if (auto condVar = node->condition()->toVariable()) {
      condExpr = buildIfVar(condVar->name().c_str(),
                            toExpr(convertAST(condVar->initExpression())),
                            condVar->kind() == chpl::uast::Variable::CONST);
    } else {
      condExpr = toExpr(convertAST(node->condition()));
    }
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
    return WhileDoStmt::build(condExpr, body);
  }

  /// IndexableLoops ///

  // In the uAST, loop index variables are represented as Decl, but in the
  // old AST they are represented as expressions.
  Expr* convertLoopIndexDecl(const uast::Decl* node) {
    if (node == nullptr) return nullptr;

    astlocMarker markAstLoc(node->id());

    // Simple variables just get reverted to UnresolvedSymExpr.
    if (const uast::Variable* var = node->toVariable()) {
      auto name = var->name();
      if (auto remap = reservedWordRemapForIdent(name)) {
        return remap;
      } else {
        return new UnresolvedSymExpr(name.c_str());
      }
    // For tuples, recursively call 'convertLoopIndexDecl' on each element.
    } else if (const uast::TupleDecl* td = node->toTupleDecl()) {
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

  //
  // Note that that expressions that appear in type bindings, e.g.,
  // 'var x: [0..0] int' or 'type T = [0..0] int' use the
  // 'convertArrayType()' instead, as there is no ambiguity about
  // whether or not the bracket loop is a type.
  //
  bool isBracketLoopMaybeArrayType(const uast::BracketLoop* node) {
    if (!node->isExpressionLevel()) return false;
    if (node->iterand()->isZip()) return false;
    if (node->numStmts() != 1) return false;
    if (node->index() && node->stmt(0)->isConditional()) return false;
    return true;
  }

  Expr* convertBracketLoopExpr(const uast::BracketLoop* node) {
    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->isExpressionLevel());
    INT_ASSERT(node->numStmts() == 1);

    // The pieces that we need for 'buildForallLoopExpr'.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    Expr* expr = nullptr;
    Expr* cond = nullptr;
    bool maybeArrayType = isBracketLoopMaybeArrayType(node);
    bool zippered = node->iterand()->isZip();

    // Unpack things differently if body is a conditional.
    if (auto origCond = node->stmt(0)->toConditional()) {
      INT_ASSERT(origCond->numThenStmts() == 1);
      // a filter expression
      if (!origCond->hasElseBlock()) {
        cond = convertAST(origCond->condition());
        expr = singleExprFromStmts(origCond->thenStmts());
        INT_ASSERT(cond);
      } else {
        // not a filter
        INT_ASSERT(origCond->numElseStmts() == 1);
      }
    }

    if (!expr) {
      expr = singleExprFromStmts(node->stmts());
    }

    INT_ASSERT(expr != nullptr);

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
      auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
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
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
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
    bool maybeArrayType = false;
    bool zippered = node->iterand()->isZip();
    bool isForExpr = node->isExpressionLevel();

    if (node->isExpressionLevel()) {
      INT_ASSERT(node->numStmts() == 1);

      // Unpack things differently if body is a conditional.
      if (auto origCond = node->stmt(0)->toConditional()) {
        INT_ASSERT(origCond->numThenStmts() == 1);
        if (!origCond->hasElseBlock()) {
          body = singleExprFromStmts(origCond->thenStmts());
          cond = toExpr(convertAST(origCond->condition()));
        } else {
          INT_ASSERT(origCond->numElseStmts() == 1);
        }
      }

      if (!body) {
        body = singleExprFromStmts(node->stmts());
      }

      INT_ASSERT(body);

      ret = buildForLoopExpr(index, iteratorExpr, body, cond,
                             maybeArrayType,
                             zippered);

    // Param loops use the index variable name as 'const char*'.
    } else if (node->isParam()) {
      INT_ASSERT(node->index() && node->index()->isVariable());

      auto indexStr = astr(node->index()->toVariable()->name());
      body = createBlockWithStmts(node->stmts(), node->blockStyle());
      BlockStmt* block = toBlockStmt(body);
      INT_ASSERT(block);

      ret = buildParamForLoopStmt(indexStr, iteratorExpr, block);

    } else {
      body = createBlockWithStmts(node->stmts(), node->blockStyle());
      BlockStmt* block = toBlockStmt(body);
      INT_ASSERT(block);

      ret = ForLoop::buildForLoop(index, iteratorExpr, block, zippered,
                                  isForExpr);
    }

    INT_ASSERT(ret != nullptr);

    return ret;
  }

  // TODO: Can we reuse this for e.g. For/BracketLoop as well?
  Expr* convertForallLoopExpr(const uast::Forall* node) {
    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->isExpressionLevel());
    INT_ASSERT(node->numStmts() == 1);

    // The pieces that we need for 'buildForallLoopExpr'.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    Expr* expr = nullptr;
    Expr* cond = nullptr;
    bool maybeArrayType = false;
    bool zippered = node->iterand()->isZip();

    // Unpack things differently if body is a conditional.
    if (auto origCond = node->stmt(0)->toConditional()) {
      INT_ASSERT(origCond->numThenStmts() == 1);
      INT_ASSERT(!origCond->hasElseBlock());
      expr = singleExprFromStmts(origCond->thenStmts());
      cond = toExpr(convertAST(origCond->condition()));
      INT_ASSERT(cond);
    } else {
      expr = singleExprFromStmts(node->stmts());
    }

    INT_ASSERT(expr != nullptr);

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
      auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
      bool zippered = node->iterand()->isZip();
      bool serialOK = false;

      return ForallStmt::build(indices, iterator, intents, body, zippered,
                               serialOK);
    }
  }

  BlockStmt* visit(const uast::Foreach* node) {

    // Does not appear possible right now, from reading the grammar.
    INT_ASSERT(!node->isExpressionLevel());

    if (node->withClause()) {
      USR_FATAL_CONT(node->withClause()->id(), "foreach loops do not yet "
                                               "support task intents");
    }

    // The pieces that we need for 'buildForallLoopExpr'.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
    bool zippered = node->iterand()->isZip();
    bool isForExpr = node->isExpressionLevel();

    auto ret = ForLoop::buildForeachLoop(indices, iteratorExpr, body,
                                         zippered,
                                         isForExpr);

    return ret;
  }

  /// Array, Domain, Range, Tuple ///

  Expr* visit(const uast::Array* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);
    bool isAssociativeList = false;

    for (auto expr : node->exprs()) {
      bool hasConvertedThisIter = false;

      if (auto opCall = expr->toOpCall()) {
        if (opCall->op() == USTR("=>")) {
          isAssociativeList = true;
          INT_ASSERT(opCall->numActuals() == 2);
          Expr* lhs = convertAST(opCall->actual(0));
          Expr* rhs = convertAST(opCall->actual(1));
          actualList->insertAtTail(lhs);
          actualList->insertAtTail(rhs);
          hasConvertedThisIter = true;
        } else {
          if (isAssociativeList) INT_FATAL("Not possible!");
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
      ret = new CallExpr("chpl__buildArrayExpr", actualList);
    }

    INT_ASSERT(ret != nullptr);

    return ret;
  }

  Expr* visit(const uast::Domain* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);

    for (auto expr : node->exprs()) {
      actualList->insertAtTail(convertAST(expr));
    }

    Expr* ret = nullptr;

    if (node->usedCurlyBraces()) {
      ret = new CallExpr("chpl__buildDomainExpr", actualList,
                         new SymExpr(gTrue));
    } else {
      ret = new CallExpr("chpl__ensureDomainExpr", actualList);
    }

    INT_ASSERT(ret != nullptr);

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

  Expr* convertCalledKeyword(const uast::AstNode* node) {
    astlocMarker markAstLoc(node->id());

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
    astlocMarker markAstLoc(node->id());

    auto calledExpression = node->calledExpression();
    INT_ASSERT(calledExpression);
    CallExpr* ret = nullptr;

    if (auto kwSparse = calledExpression->toIdentifier()) {
      if (kwSparse->name() == USTR("sparse")) {
        INT_ASSERT(node->numActuals() == 1);

        if (auto innerCall = node->actual(0)->toFnCall()) {
          auto innerCalledExpression = innerCall->calledExpression();
          INT_ASSERT(innerCalledExpression);

          if (auto kwSubdomain = innerCalledExpression->toIdentifier()) {
            if (kwSubdomain->name() == USTR("subdomain")) {
              INT_ASSERT(innerCall->numActuals() == 1);

              ret = new CallExpr
                      ("chpl__buildSparseDomainRuntimeTypeForParentDomain");
              Expr* expr = convertAST(innerCall->actual(0));
              ret->insertAtTail(expr);
            }
          }
        }
      }
    }

    return ret;
  }

  Expr* visit(const uast::FnCall* node) {
    const uast::AstNode* calledExpression = node->calledExpression();
    INT_ASSERT(calledExpression);

    CallExpr* ret = nullptr;
    CallExpr* addArgsTo = nullptr;

    if (auto newExpression = calledExpression->toNew()) {
      CallExpr* newExprStart = convertNewManagement(newExpression);
      INT_ASSERT(newExprStart);

      auto nodeTypeExpr = newExpression->typeExpression();
      INT_ASSERT(nodeTypeExpr);

      // Try to convert a called keyword, if not then use defaults.
      Expr* typeExpr = convertCalledKeyword(nodeTypeExpr);
      bool isCalledKeyword = true;
      if (!typeExpr) {
        typeExpr = convertAST(nodeTypeExpr);
        isCalledKeyword = false;
      }

      INT_ASSERT(typeExpr);

      // Ensure type expression is always wrapped in a call. We can parse
      // something like 'new borrowed borrowed C', in which case we need
      // to insert the remaining arguments into the second 'borrowed'
      // call rather than just inserting them to the right of it.
      CallExpr* initCall = (isCalledKeyword && isCallExpr(typeExpr))
          ? toCallExpr(typeExpr)
          : new CallExpr(typeExpr);

      newExprStart->insertAtTail(initCall);

      ret = newExprStart;
      addArgsTo = initCall;

    // Special case 'sparse' since it's weird and can only appear one way.
    } else if (Expr* expr = convertSparseKeyword(node)) {
      return expr;

    // If a keyword produces a call, just use that instead of making one.
    } else if (Expr* expr = convertCalledKeyword(calledExpression)) {
      ret = isCallExpr(expr) ? toCallExpr(expr) : new CallExpr(expr);
      addArgsTo = ret;
    } else {
      ret = new CallExpr(convertAST(calledExpression));
      ret->square = node->callUsedSquareBrackets();
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

    setResolvedCall(node, ret);

    return ret;
  }

  Expr* convertDmappedOp(const uast::OpCall* node) {
    if (node->op() != USTR("dmapped")) return nullptr;

    astlocMarker markAstLoc(node->id());

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

    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->numActuals() == 1);
    Expr* expr = convertAST(node->actual(0));
    return new CallExpr(PRIM_TUPLE_EXPAND, expr);
  }

  Expr* convertReduceAssign(const uast::OpCall* node) {
    if (node->op() != USTR("reduce=")) return nullptr;

    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->numActuals() == 2);
    Expr* lhs = convertAST(node->actual(0));
    Expr* rhs = convertAST(node->actual(1));
    return buildAssignment(lhs, rhs, PRIM_REDUCE_ASSIGN);
  }

  Expr* convertToNilableChecked(const uast::OpCall* node) {
    if (node->op() != USTR("?")) return nullptr;

    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->numActuals() == 1);
    Expr* expr = convertAST(node->actual(0));
    if (auto call = toCallExpr(expr)) {
      if (call->isPrimitive(PRIM_NEW)) {
        INT_ASSERT(call->numActuals() <= 2);
        Expr* child = nullptr;
        if (call->numActuals() == 2) {
          INT_ASSERT(isNamedExpr(call->get(1)));
          child = call->get(2);
        } else if (call->numActuals() == 1) {
          child = call->get(1);
        } else {
          INT_FATAL(call, "unexpected form for new expression (no actuals)");
        }
        child->remove();
        auto toNilable = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, child);
        call->insertAtTail(toNilable);
        return call;
      }
    }
    return new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, expr);
  }

  Expr* convertLogicalAndAssign(const uast::OpCall* node) {
    if (node->op() != USTR("&&=")) return nullptr;

    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->numActuals() == 2);
    Expr* lhs = convertAST(node->actual(0));
    Expr* rhs = convertAST(node->actual(1));
    return buildLAndAssignment(lhs, rhs);
  }

  Expr* convertLogicalOrAssign(const uast::OpCall* node) {
    if (node->op() != USTR("||=")) return nullptr;

    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->numActuals() == 2);
    Expr* lhs = convertAST(node->actual(0));
    Expr* rhs = convertAST(node->actual(1));
    return buildLOrAssignment(lhs, rhs);
  }

  Expr* convertRegularBinaryOrUnaryOp(const uast::OpCall* node,
                                      const char* name=nullptr) {
    astlocMarker markAstLoc(node->id());

    const char* opName = name ? name : astr(node->op());
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

    INT_ASSERT(ret != nullptr);

    return ret;
  }

  Expr* visit(const uast::PrimCall* node) {
    CallExpr* call = new CallExpr(node->prim());
      for (auto actual : node->actuals()) {
        call->insertAtTail(convertAST(actual));
      }
    return call;
  }

  // Note that this conversion is for the reduce expression, and not for
  // the reduce intent (see conversion for 'WithClause').
  Expr* visit(const uast::Reduce* node) {
    INT_ASSERT(node->numActuals() == 2);
    Expr* opExpr = convertScanReduceOp(node->op());
    Expr* dataExpr = convertAST(node->iterand());
    bool zippered = node->iterand()->isZip();
    return buildReduceExpr(opExpr, dataExpr, zippered);
  }

  Expr* visit(const uast::Scan* node) {
    INT_ASSERT(node->numActuals() == 2);
    Expr* opExpr = convertScanReduceOp(node->op());
    Expr* dataExpr = convertAST(node->iterand());
    bool zippered = node->iterand()->isZip();
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
    auto comment = consumeLatestComment();

    for (auto decl : node->decls()) {
      INT_ASSERT(decl->linkage() == node->linkage());

      Expr* conv = nullptr;
      if (auto var = decl->toVariable()) {

        // Do not use the linkage name since multi-decls cannot be renamed.
        const bool useLinkageName = false;
        conv = convertVariable(var, useLinkageName);

        // Attach the doc comment if it exists.
        DefExpr* defExpr = toDefExpr(conv);
        INT_ASSERT(defExpr);
        auto varSym = toVarSymbol(defExpr->sym);
        INT_ASSERT(varSym);
        varSym->doc = comment;

      // Otherwise convert in a generic fashion.
      } else {
        conv = convertAST(decl);
      }

      INT_ASSERT(conv);
      ret->insertAtTail(conv);
    }

    if (!fDocs) {
      INT_ASSERT(!inTupleDecl);
      CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
      ret->insertAtTail(end);
    }

    return ret;
  }

  // Right now components are one of: Variable, Formal, TupleDecl.
  BlockStmt* convertTupleDeclComponents(const uast::TupleDecl* node,
                                        bool attachComments=false) {
    astlocMarker markAstLoc(node->id());

    auto comment = attachComments ? consumeLatestComment() : nullptr;
    BlockStmt* ret = new BlockStmt(BLOCK_SCOPELESS);

    const bool saveInTupleDecl = inTupleDecl;
    inTupleDecl = true;

    for (auto decl : node->decls()) {
      Expr* conv = nullptr;

      // Formals are converted into variables.
      if (auto formal = decl->toFormal()) {
        INT_ASSERT(formal->intent() == uast::Formal::DEFAULT_INTENT);
        INT_ASSERT(!formal->initExpression());
        INT_ASSERT(!formal->typeExpression());
        conv = new DefExpr(new VarSymbol(formal->name().c_str()));

        // Should not be attaching comments to tuple formals.
        INT_ASSERT(!attachComments);

      // Do not use the visitor because it produces a block statement.
      } else if (auto var = decl->toVariable()) {
        const bool useLinkageName = false;
        conv = convertVariable(var, useLinkageName);

        if (attachComments) {
          auto defExpr = toDefExpr(conv);
          INT_ASSERT(defExpr);
          auto varSym = toVarSymbol(defExpr->sym);
          INT_ASSERT(varSym);
          varSym->doc = comment;
        }

      // It must be a tuple.
      } else {
        INT_ASSERT(decl->isTupleDecl());
        conv = convertTupleDeclComponents(decl->toTupleDecl());
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
    const bool attachComments = true;
    auto tuple = convertTupleDeclComponents(node, attachComments);
    auto typeExpr = convertExprOrNull(node->typeExpression());
    auto initExpr = convertExprOrNull(node->initExpression());

    BlockStmt* ret = buildTupleVarDeclStmt(tuple, typeExpr, initExpr);

    // TODO: Shouldn't this be all symbols?
    DefExpr* tmpDef = toDefExpr(ret->body.first());
    attachSymbolStorage(node->intentOrKind(), tmpDef->sym);

    // Move the block info around like in 'buildVarDecls'.
    if (auto info = ret->blockInfoGet()) {
      INT_ASSERT(info->isNamed("_check_tuple_var_decl"));
      tmpDef->insertAfter(info);
      ret->blockInfoSet(NULL);
    }

    // Add a PRIM_END_OF_STATEMENT.
    if (!fDocs) {
      INT_ASSERT(!inTupleDecl);
      CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
      ret->insertAtTail(end);
    }

    return ret;
  }

  /// ForwardingDecl ///
  Expr* visit(const uast::ForwardingDecl* node) {
    // ForwardingDecl may contain a VisibilityClause, an Expression,
    // or a Variable declaration
    if (node->expr()->isVisibilityClause()){
      auto child = node->expr()->toVisibilityClause();
      bool except=false;
      if (child->limitationKind() == uast::VisibilityClause::ONLY) {
        except=false;
      }
      else if (child->limitationKind() == uast::VisibilityClause::EXCEPT) {
        except=true;
      }
      // convert the AstList of renames
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
    } else if (node->expr()->isIdentifier()
               || node->expr()->isFnCall()
               || node->expr()->isOpCall()) {
      return buildForwardingStmt(convertExprOrNull(node->expr()));
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

  const char* convertFunctionNameAndAstr(UniqueString name,
                                         uast::Function::Kind kind) {

    if (kind == uast::Function::LAMBDA) {
      static unsigned int nextId = 0;
      char buffer[100];

      /*
       Use sprintf to prevent buffer overflow if there are too many lambdas
       */
      if (snprintf(buffer, 100, "chpl_lambda_%i", nextId++) >= 100) {
        INT_FATAL("Too many lambdas.");
      }

      return astr(buffer);
    }

    const char* ret = nullptr;
    if (name == USTR("by")) {
      ret = "chpl_by";
    } else if (name == USTR("align")) {
      ret = "chpl_align";
    } else {
      ret = name.c_str();
    }

    INT_ASSERT(ret);

    // We have to uniquify the name now because it may be inlined (and thus
    // stack allocated).
    ret = astr(ret);

    return ret;
  }

  Expr* convertLifetimeClause(const uast::AstNode* node) {
    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->isOpCall() || node->isReturn());
    if (auto opCall = node->toOpCall()) {
      INT_ASSERT(opCall->numActuals()==2);
      auto lhsIdent = opCall->actual(0)->toIdentifier();
      auto rhsIdent = opCall->actual(1)->toIdentifier();
      INT_ASSERT(lhsIdent && rhsIdent);
      INT_ASSERT(opCall->op() == USTR("=") ||
                 opCall->op() == USTR("<") ||
                 opCall->op() == USTR(">") ||
                 opCall->op() == USTR("==")||
                 opCall->op() == USTR("<=")||
                 opCall->op() == USTR(">="));
      Expr* lhs = convertLifetimeIdent(lhsIdent);
      Expr* rhs = convertLifetimeIdent(rhsIdent);
      return new CallExpr(opCall->op().c_str(), lhs, rhs);
    } else if (auto ret = node->toReturn()) {
      INT_ASSERT(ret->value() && ret->value()->isIdentifier());
      auto ident = ret->value()->toIdentifier();

      Expr* val = convertLifetimeIdent(ident);
      return new CallExpr(PRIM_RETURN, val);

    } else {
      // should not arrive here, or else we missed something
      INT_FATAL("should not be reached");
      return nullptr;
    }
  }

  CallExpr* convertLifetimeIdent(const uast::Identifier* node) {
    astlocMarker markAstLoc(node->id());

    auto ident = node->toIdentifier();
    INT_ASSERT(ident);
    CallExpr* callExpr = new CallExpr(PRIM_LIFETIME_OF,
                                      convertExprOrNull(node));
    return callExpr;
  }

  FnSymbol* convertFunction(const uast::Function* node, const char* comment) {
    auto it = alreadyConverted.find(node);
    if (it != alreadyConverted.end()) {
      // already converted it, so return that
      return toFnSymbol(it->second);
    }

    // Decide if we want to resolve this function
    bool shouldResolveFunction = shouldResolve(node);

    const resolution::ResolutionResultByPostorderID* resolved = nullptr;
    const resolution::ResolvedFunction* resolvedFn = nullptr;
    const resolution::TypedFnSignature* initialSig = nullptr;
    const resolution::PoiScope* poiScope = nullptr;

    if (shouldResolveFunction) {
      auto uSig = resolution::UntypedFnSignature::get(context, node);
      initialSig = resolution::typedSignatureInitial(context, uSig);
      auto whereFalse =
        resolution::TypedFnSignature::WhereClauseResult::WHERE_FALSE;
      if (initialSig->whereClauseResult() != whereFalse) {
        if (!initialSig->needsInstantiation()) {
          // poiScope not needed for concrete functions
          poiScope = nullptr;
          resolvedFn = resolveFunction(context, initialSig, poiScope);
          if (resolvedFn) {
            resolved = &resolvedFn->resolutionById();
          }
        }
      }
    }

    // Also add to symStack
    // Add a SymStackEntry to the end of the symStack
    this->symStack.emplace_back(node, resolved);

    FnSymbol* fn = new FnSymbol("_");

    // Note that we have already converted this
    alreadyConverted.emplace(node, fn);

    // build up the userString as in old parser
    // needed to match up some error outputs
    // NOTE:
    // parentheses may have been discarded from the original user declaration,
    // and if so, we are not able to reconstruct them at this time
    std::stringstream ss;
    printFunctionSignature(ss, node);
    fn->userString = astr(ss.str());

    attachSymbolAttributes(node, fn);
    attachSymbolVisibility(node, fn);

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
            INT_ASSERT(!hasConvertedReceiver);
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
            INT_ASSERT(conv);
          }

        // A varargs formal.
        } else if (auto formal = decl->toVarArgFormal()) {
          INT_ASSERT(formal->name() != USTR("this"));
          conv = toDefExpr(convertAST(formal));
          INT_ASSERT(conv);

        // A tuple decl, where components are formals or tuple decls.
        } else if (auto formal = decl->toTupleDecl()) {
          auto castIntent = (uast::Formal::Intent)formal->intentOrKind();
          IntentTag tag = convertFormalIntent(castIntent);
          BlockStmt* tuple = convertTupleDeclComponents(formal);
          INT_ASSERT(tuple);

          Expr* type = convertExprOrNull(formal->typeExpression());
          Expr* init = convertExprOrNull(formal->initExpression());

          // TODO: Move this specialization into visitor? We can just
          // detect if components are formals.
          conv = buildTupleArgDefExpr(tag, tuple, type, init);
          INT_ASSERT(conv);
        } else {
          INT_FATAL("Not handled yet!");
        }

        // Attaches def to function's formal list.
        if (conv) {
          buildFunctionFormal(fn, conv);
        }
      }
    }

    const char* convName = convertFunctionNameAndAstr(node->name(),
                                                      node->kind());
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

    } else if (node->kind() == uast::Function::OPERATOR) {
      fn->addFlag(FLAG_OPERATOR);
      if (fn->_this != NULL) {
        updateOpThisTagOrErr(fn);
        setupTypeIntentArg(toArgSymbol(fn->_this));
      }

    } else if (node->kind() == uast::Function::LAMBDA) {
      fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);

      // TODO: move these to new frontend if they continue
      // to be relevant as FCFs are improved.
      if (retTag == RET_REF || retTag == RET_CONST_REF)
        USR_FATAL(fn, "'ref' return types are not allowed in lambdas");
      if (retTag == RET_PARAM)
        USR_FATAL(fn, "'param' return types are not allowed in lambdas");
      if (retTag == RET_TYPE)
        USR_FATAL(fn, "'type' return types are not allowed in lambdas");
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
        Expr* convertedClause = convertLifetimeClause(clause);
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

    if (node->body() && node->linkage() != uast::Decl::EXTERN) {

      // TODO: What about 'proc foo() return 0;'?
      auto style = uast::BlockStyle::EXPLICIT;
      body = createBlockWithStmts(node->stmts(), style);
    } else {
      if (node->numStmts()) {
        USR_FATAL_CONT("Extern functions cannot have a body");
      }
    }

    setupFunctionDecl(fn, retTag, retType, node->throws(),
                      whereClause,
                      lifetimeConstraints,
                      body,
                      /* docs */ comment);

    if (node->linkage() != uast::Decl::DEFAULT_LINKAGE) {
      Flag linkageFlag = convertFlagForDeclLinkage(node);
      Expr* linkageExpr = convertExprOrNull(node->linkageName());
      setupExternExportFunctionDecl(linkageFlag, linkageExpr, fn);
    }

    // Update the function symbol with any resolution results.
    if (resolved) {
      auto retType = resolution::returnType(context, initialSig, poiScope);
      fn->retType = convertType(retType);
    }

    // pop the function from the symStack
    INT_ASSERT(symStack.size() > 0 && symStack.back().ast == node);
    this->symStack.pop_back();

    return fn;
  }

  Expr* visit(const uast::Function* node) {
    auto comment = consumeLatestComment();
    FnSymbol* fn = convertFunction(node, comment);

    // For lambdas, return a DefExpr instead of a BlockStmt
    // containing a DefExpr because this is the pattern expected
    // by the production compiler.
    if (node->kind() == uast::Function::LAMBDA) {
      // leaves the BlockStmt ret and other DefExpr to be GC'd
      DefExpr* def = new DefExpr(fn);
      return def;
    }

    // Otherwise, return a block containing a DefExpr
    BlockStmt* ret = buildChapelStmt(new DefExpr(fn));
    return ret;
  }

  Expr* visit(const uast::Interface* node) {
    const char* name = astr(node->name());
    CallExpr* formals = new CallExpr(PRIM_ACTUALS_LIST);
    auto style = uast::BlockStyle::EXPLICIT;
    BlockStmt* body = createBlockWithStmts(node->stmts(), style);

    if (node->isFormalListPresent()) {
      for (auto formal : node->formals()) {
        if (auto ident = formal->toIdentifier()) {
          const char* name = astr(ident->name());
          auto formal = InterfaceSymbol::buildFormal(name, INTENT_TYPE);
          formals->insertAtTail(formal);
        } else {
          INT_FATAL("Expected identifier for interface formal");
        }
      }
    } else {
      INT_ASSERT(node->numFormals() == 0);
      DefExpr* formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
      formals->insertAtTail(formal);
    }

    auto isym = InterfaceSymbol::buildDef(name, formals, body);
    auto ret = buildChapelStmt(isym);

    return ret;
  }

  DefExpr* visit(const uast::Module* node) {
    auto comment = consumeLatestComment();

    auto it = alreadyConverted.find(node);
    if (it != alreadyConverted.end()) {
      // already converted it, so return that
      return toDefExpr(it->second);
    }

    // Decide if we want to resolve this module
    bool shouldResolveModule = shouldResolve(node);

    const resolution::ResolutionResultByPostorderID* resolved = nullptr;

    if (shouldResolveModule) {
      // Resolve the module
      const auto& tmp = resolution::resolveModule(context, node->id());
      resolved = &tmp;
    }

    // Push the current module name before descending into children.
    // Add a ModStackEntry to the end of the modStack
    this->modStack.emplace_back(node);
    // Also add to symStack
    // Add a SymStackEntry to the end of the symStack
    this->symStack.emplace_back(node, resolved);

    const char* name = astr(node->name());
    const char* path = astr(context->filePathForId(node->id()));

    // TODO (dlongnecke): For now, the tag is overridden by the caller.
    // See 'uASTAttemptToParseMod'. Eventually, it would be great if dyno
    // could note if a module is standard/internal/user.
    const ModTag tag = this->topLevelModTag;
    bool priv = (node->visibility() == uast::Decl::PRIVATE);
    bool prototype = (node->kind() == uast::Module::PROTOTYPE ||
                      node->kind() == uast::Module::IMPLICIT);
    auto style = uast::BlockStyle::EXPLICIT;

    currentModuleName = name;
    auto body = createBlockWithStmts(node->stmts(), style);

    // Pop the module after converting children.
    INT_ASSERT(modStack.size() > 0 && modStack.back().mod == node);
    this->modStack.pop_back();
    INT_ASSERT(symStack.size() > 0 && symStack.back().ast == node);
    this->symStack.pop_back();

    ModuleSymbol* mod = buildModule(name,
                                    tag,
                                    body,
                                    path,
                                    priv,
                                    prototype,
                                    comment);

    if (node->kind() == uast::Module::IMPLICIT) {
      mod->addFlag(FLAG_IMPLICIT_MODULE);
    }

    attachSymbolAttributes(node, mod);

    // Note that we have already converted this
    alreadyConverted.emplace(node, mod);

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

  Expr* convertTypeExpression(const uast::AstNode* node,
                              bool isFormalType=false) {
    if (!node) return nullptr;
    Expr* ret = nullptr;

    astlocMarker markAstLoc(node->id());

    if (auto bkt = node->toBracketLoop()) {
      ret = convertArrayType(bkt, isFormalType);
    } else {
      ret = convertAST(node);
    }

    INT_ASSERT(ret);

    return ret;
  }

  DefExpr* visit(const uast::Formal* node) {
    IntentTag intentTag = convertFormalIntent(node->intent());

    astlocMarker markAstLoc(node->id());

    Expr* typeExpr = convertTypeExpression(node->typeExpression(), true);
    Expr* initExpr = convertExprOrNull(node->initExpression());

    auto ret =  buildArgDefExpr(intentTag, node->name().c_str(),
                                typeExpr,
                                initExpr,
                                /*varargsVariable*/ nullptr);
    INT_ASSERT(ret->sym);

    attachSymbolAttributes(node, ret->sym);

    setVariableType(node, ret->sym);

    return ret;
  }

  ShadowVarPrefix convertTaskVarIntent(const uast::TaskVar* node) {
    astlocMarker markAstLoc(node->id());

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
    INT_ASSERT(ret->sym);

    attachSymbolAttributes(node, ret->sym);

    return ret;
  }

  ShadowVarSymbol* convertTaskVar(const uast::TaskVar* node) {
    astlocMarker markAstLoc(node->id());

    ShadowVarPrefix prefix = convertTaskVarIntent(node);
    Expr* nameExp = new UnresolvedSymExpr(node->name().c_str());
    Expr* type = convertTypeExpression(node->typeExpression());
    Expr* init = convertExprOrNull(node->initExpression());

    auto ret = ShadowVarSymbol::buildForPrefix(prefix, nameExp, type, init);

    INT_ASSERT(ret != nullptr);

    attachSymbolAttributes(node, ret);

    return ret;
  }

  const char* sanitizeVarName(const char* name) {
    if (inTupleDecl && name[0] == '_' && name[1] == '\0')
      return astr("chpl__tuple_blank");
    else
      return astr(name);
  }

  void attachSymbolStorage(const uast::Variable::Kind kind, Symbol* vs) {
    return attachSymbolStorage((uast::IntentList) kind, vs);
  }

  void attachSymbolStorage(const uast::TupleDecl::IntentOrKind iok,
                           Symbol* vs) {
    return attachSymbolStorage((uast::IntentList) iok, vs);
  }

  void attachSymbolStorage(const uast::IntentList kind, Symbol* vs) {
    auto qual = QUAL_UNKNOWN;

    switch (kind) {
      case uast::IntentList::VAR:
        qual = QUAL_VAL;
        break;
      case uast::IntentList::CONST_VAR:
        vs->addFlag(FLAG_CONST);
        qual = QUAL_CONST;
        break;
      case uast::IntentList::CONST_REF:
        vs->addFlag(FLAG_CONST);
        vs->addFlag(FLAG_REF_VAR);
        qual = QUAL_CONST_REF;
        break;
      case uast::IntentList::REF:
        vs->addFlag(FLAG_REF_VAR);
        qual = QUAL_REF;
        break;
      case uast::IntentList::PARAM:
        vs->addFlag(FLAG_PARAM);
        qual = QUAL_PARAM;
        break;
      case uast::IntentList::TYPE:
        vs->addFlag(FLAG_TYPE_VARIABLE);
        break;
      case uast::IntentList::INDEX:
        INT_FATAL("Index variables should be handled elsewhere");
        break;
      default:
        break;
    }

    if (ATTACH_QUALIFIED_TYPES_EARLY && qual != QUAL_UNKNOWN) {
      vs->qual = qual;
    }
  }

  CallExpr* convertArrayType(const uast::BracketLoop* node,
                             bool isFormalType=false) {
    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->isExpressionLevel());

    Expr* domActuals = nullptr;

    if (auto iterand = node->iterand()) {
      auto astForIterand = iterand;

      if (auto dom = iterand->toDomain()) {

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
          astForIterand = dom->expr(0);
        }
      }

      if (domActuals == nullptr) {
        auto expr = astForIterand;
        domActuals = convertAST(expr);

        // But wrap it if it is not a type query for a formal type.
        bool isFormalTypeQuery = isFormalType && expr->isTypeQuery();
        if (!isFormalTypeQuery) {
          domActuals = new CallExpr("chpl__ensureDomainExpr", domActuals);
        }
      }
    }

    INT_ASSERT(domActuals);

    Expr* subType = nullptr;
    if (node->numStmts()) {
      INT_ASSERT(node->numStmts() == 1);

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

    // This nonsense must be done so that the old builders can emit errors
    // about skyline arrays. TODO: Move check to 'dyno' rather than do this.
    if (node->index()) {
      auto convIndex = convertLoopIndexDecl(node->index());
      ret->insertAtTail(convIndex);
      ret->insertAtTail(subType->copy());
    }

    return ret;
  }

  // Returns a DefExpr that has not yet been inserted into the tree.
  DefExpr* convertVariable(const uast::Variable* node,
                           bool useLinkageName) {
    astlocMarker markAstLoc(node->id());

    auto varSym = new VarSymbol(sanitizeVarName(node->name().c_str()));
    const bool isTypeVar = node->kind() == uast::Variable::TYPE;

    // Adjust the variable according to its kind, e.g. 'const'/'type'.
    attachSymbolStorage(node->kind(), varSym);

    attachSymbolAttributes(node, varSym);

    attachSymbolVisibility(node, varSym);

    if (node->isConfig()) {
      varSym->addFlag(FLAG_CONFIG);
    }

    Flag linkageFlag = convertFlagForDeclLinkage(node);
    if (linkageFlag != FLAG_UNKNOWN) {
      varSym->addFlag(linkageFlag);
    }

    // TODO (dlongnecke): Move to new parser (or post-parsing walk).
    if (node->kind() == uast::Variable::PARAM &&
        linkageFlag == FLAG_EXTERN) {
      USR_FATAL(varSym, "external params are not supported");
    }

    // TODO (dlongnecke): Should be sanitized by the new parser.
    if (useLinkageName && node->linkageName()) {
      INT_ASSERT(linkageFlag != FLAG_UNKNOWN);
      varSym->cname = convertLinkageNameAstr(node);
    }

    Expr* typeExpr = nullptr;

    // If there is a bracket loop it is almost certainly an array type, so
    // special case it. Otherwise, just use the generic conversion call.
    if (const uast::AstNode* te = node->typeExpression()) {
      if (const uast::BracketLoop* bkt = te->toBracketLoop()) {
        typeExpr = convertArrayType(bkt);
      } else {
        typeExpr = toExpr(convertAST(te));
      }
    }

    Expr* initExpr = nullptr;

    if (const uast::AstNode* ie = node->initExpression()) {
      const uast::BracketLoop* bkt = ie->toBracketLoop();
      if (bkt && isTypeVar) {
          auto convArrayType = convertArrayType(bkt);
          initExpr = buildForallLoopExprFromArrayType(convArrayType);
        } else {
          initExpr = convertAST(ie);
        }
    } else {
      initExpr = convertExprOrNull(node->initExpression());
    }

    auto ret = new DefExpr(varSym, initExpr, typeExpr);

    // If the init expression of this variable is a domain and this
    // variable is not const, propagate that information by setting
    // 'definedConst' in the domain to false.
    setDefinedConstForDefExprIfApplicable(ret, &ret->sym->flags);

    // Fix up the AST based on the type, if it should be known
    setVariableType(node, varSym);

    return ret;
  }

  Expr* visit(const uast::Variable* node) {
    auto isTypeVar = node->kind() == uast::Variable::TYPE;
    auto stmts = new BlockStmt(BLOCK_SCOPELESS);
    auto comment = consumeLatestComment();

    auto defExpr = convertVariable(node, true);
    INT_ASSERT(defExpr);
    auto varSym = toVarSymbol(defExpr->sym);
    INT_ASSERT(varSym);

    // Attach the doc comment if it exists.
    varSym->doc = comment;

    stmts->insertAtTail(defExpr);

    // Special handling for extern type variables.
    if (isTypeVar) {
      if (node->linkage() == uast::Decl::EXTERN) {
        INT_ASSERT(!node->isConfig());
        INT_ASSERT(defExpr->sym && isVarSymbol(defExpr->sym));
        auto varSym = toVarSymbol(defExpr->sym);
        auto linkageName = node->linkageName() ? varSym->cname : nullptr;
        stmts = convertTypesToExtern(stmts, linkageName);
      }
    }

    // Add a PRIM_END_OF_STATEMENT.
    if (!fDocs && !inTupleDecl && !isTypeVar) {
      CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
      stmts->insertAtTail(end);
    }

    return stmts;
  }

  /// TypeDecls

  // Does not attach parent type.
  DefExpr* convertEnumElement(const uast::EnumElement* node) {
    astlocMarker markAstLoc(node->id());

    const char* name = astr(node->name());
    Expr* initExpr = convertExprOrNull(node->initExpression());
    auto ret = new DefExpr(new EnumSymbol(name), initExpr);
    attachSymbolAttributes(node, ret->sym);
    return ret;
  }

  Expr* visit(const uast::Enum* node) {
    auto comment = consumeLatestComment();
    auto enumType = new EnumType();

    // Attach any doc comment to the enum type.
    enumType->doc = comment;

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
    attachSymbolVisibility(node, enumTypeSym);

    enumType->symbol = enumTypeSym;

    auto ret = new BlockStmt(BLOCK_SCOPELESS);
    ret->insertAtTail(new DefExpr(enumTypeSym));

    return ret;
  }

  /// AggregateDecls

  AggregateTag convertAggregateDeclTag(const uast::AggregateDecl* node) {
    switch (node->tag()) {
      case uast::asttags::Class: return AGGREGATE_CLASS;
      case uast::asttags::Record: return AGGREGATE_RECORD;
      case uast::asttags::Union: return AGGREGATE_UNION;
      default: break;
    }
    INT_FATAL("Should not reach here!");
    return AGGREGATE_CLASS;
  }

  Expr* convertAggregateDecl(const uast::AggregateDecl* node) {
    auto comment = consumeLatestComment();
    const char* name = astr(node->name());
    const char* cname = name;
    Expr* inherit = nullptr;

    if (auto cls = node->toClass()) {
      inherit = convertExprOrNull(cls->parentClass());
    }

    if (node->linkageName()) {
      INT_ASSERT(node->linkage() != uast::Decl::DEFAULT_LINKAGE);
      cname = convertLinkageNameAstr(node);
    }

    auto style = uast::BlockStyle::EXPLICIT;
    auto decls = createBlockWithStmts(node->declOrComments(), style, false);
    Flag externFlag = convertFlagForDeclLinkage(node);
    auto tag = convertAggregateDeclTag(node);

    // Classes cannot be extern or export.
    if (node->isClass()) {
      INT_ASSERT(externFlag == FLAG_UNKNOWN);
    }

    auto ret = buildClassDefExpr(name, cname, tag, inherit,
                                 decls,
                                 externFlag,
                                 comment);
    INT_ASSERT(ret->sym);

    attachSymbolAttributes(node, ret->sym);
    attachSymbolVisibility(node, ret->sym);

    return ret;
  }

  Expr* visit(const uast::Class* node) {
    return convertAggregateDecl(node);
  }

  Expr* visit(const uast::Record* node) {
    return convertAggregateDecl(node);
  }

  Expr* visit(const uast::Union* node) {
    return convertAggregateDecl(node);
  }

  Expr* visit(const uast::EmptyStmt* node) {
    return new BlockStmt();
  }

};

/// Generic conversion calling the above functions ///
Expr* Converter::convertAST(const uast::AstNode* node) {
  astlocMarker markAstLoc(node->id());
  return node->dispatch<Expr*>(*this);
}

static Qualifier convertQualifier(types::QualifiedType::Kind kind) {
  Qualifier q = QUAL_UNKNOWN;
  if      (kind == types::QualifiedType::VAR)       q = QUAL_VAL;
  else if (kind == types::QualifiedType::CONST_VAR) q = QUAL_CONST_VAL;
  else if (kind == types::QualifiedType::CONST_REF) q = QUAL_CONST_REF;
  else if (kind == types::QualifiedType::REF)       q = QUAL_REF;
  else if (kind == types::QualifiedType::IN)        q = QUAL_VAL;
  else if (kind == types::QualifiedType::CONST_IN)  q = QUAL_CONST_VAL;
  else if (kind == types::QualifiedType::OUT)       q = QUAL_VAL;
  else if (kind == types::QualifiedType::INOUT)     q = QUAL_VAL;
  else if (kind == types::QualifiedType::PARAM)     q = QUAL_PARAM;

  return q;
}

void Converter::setVariableType(const uast::VarLikeDecl* v, Symbol* sym) {
  if (symStack.size() > 0) {
    auto r = symStack.back().resolved;

    if (r != nullptr) {
      printf("SETTING VARIABLE TYPE!!\n");

      // Get the type of the variable itself
      auto resolvedVar = r->byAst(v);
      types::QualifiedType qt = resolvedVar.type();
      if (!qt.isUnknown()) {
        // Set a type for the variable
        sym->type = convertType(qt);

        // Set the Qualifier
        Qualifier q = convertQualifier(qt.kind());
        if (q != QUAL_UNKNOWN)
          sym->qual = q;

        // Set the param value for the variable in paramMap, if applicable
        if (sym->hasFlag(FLAG_MAYBE_PARAM) || sym->hasFlag(FLAG_PARAM)) {
          if (qt.hasParamPtr()) {
            Symbol* val = convertParam(qt);
            paramMap.put(sym, val);
          }
        }
      }
    }
  }
}

void Converter::setResolvedCall(const uast::FnCall* call, CallExpr* expr) {
  if (symStack.size() > 0) {
    auto r = symStack.back().resolved;

    if (r != nullptr) {
      const auto& resolvedExpr = r->byId(call->id());
      const auto& candidates = resolvedExpr.mostSpecific();
      int nBest = candidates.numBest();
      if (nBest == 0) {
        // nothing to do
      } else if (nBest > 1) {
        INT_FATAL("return intent overloading not yet handled");
      } else if (nBest == 1) {
        const resolution::TypedFnSignature* sig = candidates.only();
        auto usig = sig->untyped();
        if (usig->idIsFunction()) {
          FnSymbol* fn = convertFnOutOfOrder(usig->id());
          if (fn != nullptr) {
            // Do we need to remove the old baseExpr?
            expr->baseExpr = new SymExpr(fn);
            parent_insert_help(expr, expr->baseExpr);
          }
        }
      }
    }
  }
}

Type* Converter::convertType(const types::QualifiedType qt) {
  using namespace types;

  if (!qt.hasTypePtr())
    return dtUnknown;

  switch (qt.type()->tag()) {
    // builtin types with their own classes
    case typetags::AnyType:       return dtAny;
    case typetags::CStringType:   return dtStringC;
    case typetags::ErroneousType: return dtUnknown; // a lie
    case typetags::NilType:       return dtNil;
    case typetags::NothingType:   return dtNothing;
    case typetags::UnknownType:   return dtUnknown;
    case typetags::VoidType:      return dtVoid;

    // subclasses of BuiltinType

    // concrete builtin types
    case typetags::CFileType:     return dtFile;
    case typetags::CFnPtrType:    return dtCFnPtr;
    case typetags::CVoidPtrType:  return dtCVoidPtr;
    case typetags::OpaqueType:    return dtOpaque;
    case typetags::SyncAuxType:   return dtSyncVarAuxFields;
    case typetags::TaskIdType:    return dtTaskID;

    // generic builtin types
    case typetags::AnyBoolType:                  return dtAnyBool;
    case typetags::AnyBorrowedNilableType:       return dtBorrowedNilable;
    case typetags::AnyBorrowedNonNilableType:    return dtBorrowedNonNilable;
    case typetags::AnyBorrowedType:              return dtBorrowed;
    case typetags::AnyComplexType:               return dtAnyComplex;
    case typetags::AnyEnumType:                  return dtAnyEnumerated;
    case typetags::AnyImagType:                  return dtAnyImag;
    case typetags::AnyIntType:                   return dtIntegral; // a lie
    case typetags::AnyIntegralType:              return dtIntegral;
    case typetags::AnyIteratorClassType:         return dtIteratorClass;
    case typetags::AnyIteratorRecordType:        return dtIteratorRecord;
    case typetags::AnyManagementAnyNilableType:  return dtAnyManagementAnyNilable;
    case typetags::AnyManagementNilableType:     return dtAnyManagementNilable;
    case typetags::AnyManagementNonNilableType:  return dtAnyManagementNonNilable;
    case typetags::AnyNumericType:               return dtNumeric;
    case typetags::AnyOwnedType:                 return dtOwned;
    case typetags::AnyPodType:                   return dtAnyPOD;
    case typetags::AnyRealType:                  return dtAnyReal;
    case typetags::AnyRecordType:                return dtAnyRecord;
    case typetags::AnySharedType:                return dtShared;
    case typetags::AnyUintType:                  return dtIntegral; // a lie
    case typetags::AnyUninstantiatedType:        return dtUninstantiated;
    case typetags::AnyUnionType:                 return dtUnknown; // a lie
    case typetags::AnyUnmanagedNilableType:      return dtUnmanagedNilable;
    case typetags::AnyUnmanagedNonNilableType:   return dtUnmanagedNonNilable;
    case typetags::AnyUnmanagedType:             return dtUnmanaged;

    // declared types
    case typetags::ClassType:   return convertClassType(qt);
    case typetags::EnumType:   return convertEnumType(qt);
    case typetags::FunctionType:   return convertFunctionType(qt);

    case typetags::BasicClassType:   return convertBasicClassType(qt);
    case typetags::RecordType:   return convertRecordType(qt);
    case typetags::TupleType:   return convertTupleType(qt);
    case typetags::UnionType:   return convertUnionType(qt);

    // primitive types
    case typetags::BoolType:   return convertBoolType(qt);
    case typetags::ComplexType:   return convertComplexType(qt);
    case typetags::ImagType:   return convertImagType(qt);
    case typetags::IntType:   return convertIntType(qt);
    case typetags::RealType:   return convertRealType(qt);
    case typetags::UintType:   return convertUintType(qt);

    // implementation detail tags (should not be reachable)
    case typetags::START_BuiltinType:
    case typetags::END_BuiltinType:
    case typetags::START_DeclaredType:
    case typetags::END_DeclaredType:
    case typetags::START_CompositeType:
    case typetags::END_CompositeType:
    case typetags::START_PrimitiveType:
    case typetags::END_PrimitiveType:
    case typetags::NUM_TYPE_TAGS:
      INT_FATAL("should not be reachable");
      return dtUnknown;

    // intentionally no default --
    // want a C++ compiler error if a case is missing in the above
  }
  INT_FATAL("should not be reached");
  return nullptr;
}

Type* Converter::convertClassType(const types::QualifiedType qt) {
  INT_FATAL("not implemented yet");
  return nullptr;
}

Type* Converter::convertEnumType(const types::QualifiedType qt) {
  INT_FATAL("not implemented yet");
  return nullptr;
}

Type* Converter::convertFunctionType(const types::QualifiedType qt) {
  INT_FATAL("not implemented yet");
  return nullptr;
}

Type* Converter::convertBasicClassType(const types::QualifiedType qt) {
  INT_FATAL("not implemented yet");
  return nullptr;
}

Type* Converter::convertRecordType(const types::QualifiedType qt) {
  const types::RecordType* t = qt.type()->toRecordType();
  if (t->isStringType()) {
    return dtString;
  } else if (t->isBytesType()) {
    return dtBytes;
  }

  INT_FATAL("not implemented yet");
  return nullptr;
}

Type* Converter::convertTupleType(const types::QualifiedType qt) {
  INT_FATAL("not implemented yet");
  return nullptr;
}

Type* Converter::convertUnionType(const types::QualifiedType qt) {
  INT_FATAL("not implemented yet");
  return nullptr;
}

// helper functions to convert a type to a size

static IF1_bool_type getBoolSize(const types::BoolType* t) {
  if (t->isDefaultWidth())
    return BOOL_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 8)  return BOOL_SIZE_8;
  else if (width == 16) return BOOL_SIZE_16;
  else if (width == 32) return BOOL_SIZE_32;
  else if (width == 64) return BOOL_SIZE_64;

  INT_FATAL("should not be reached");
  return BOOL_SIZE_DEFAULT;
}

static IF1_complex_type getComplexSize(const types::ComplexType* t) {
  if (t->isDefaultWidth())
    return COMPLEX_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 64)  return COMPLEX_SIZE_64;
  else if (width == 128) return COMPLEX_SIZE_128;

  INT_FATAL("should not be reached");
  return COMPLEX_SIZE_DEFAULT;
}


static IF1_float_type getImagSize(const types::ImagType* t) {
  if (t->isDefaultWidth())
    return FLOAT_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 32) return FLOAT_SIZE_32;
  else if (width == 64) return FLOAT_SIZE_64;

  INT_FATAL("should not be reached");
  return FLOAT_SIZE_DEFAULT;
}

static IF1_int_type getIntSize(const types::IntType* t) {
  if (t->isDefaultWidth())
    return INT_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 8)  return INT_SIZE_8;
  else if (width == 16) return INT_SIZE_16;
  else if (width == 32) return INT_SIZE_32;
  else if (width == 64) return INT_SIZE_64;

  INT_FATAL("should not be reached");
  return INT_SIZE_DEFAULT;
}

static IF1_float_type getRealSize(const types::RealType* t) {
  if (t->isDefaultWidth())
    return FLOAT_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 32) return FLOAT_SIZE_32;
  else if (width == 64) return FLOAT_SIZE_64;

  INT_FATAL("should not be reached");
  return FLOAT_SIZE_DEFAULT;
}

static IF1_int_type getUintSize(const types::UintType* t) {
  if (t->isDefaultWidth())
    return INT_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 8)  return INT_SIZE_8;
  else if (width == 16) return INT_SIZE_16;
  else if (width == 32) return INT_SIZE_32;
  else if (width == 64) return INT_SIZE_64;

  INT_FATAL("should not be reached");
  return INT_SIZE_DEFAULT;
}


Type* Converter::convertBoolType(const types::QualifiedType qt) {
  const types::BoolType* t = qt.type()->toBoolType();
  return dtBools[getBoolSize(t)];
}

Type* Converter::convertComplexType(const types::QualifiedType qt) {
  const types::ComplexType* t = qt.type()->toComplexType();
  return dtComplex[getComplexSize(t)];
}

Type* Converter::convertImagType(const types::QualifiedType qt) {
  const types::ImagType* t = qt.type()->toImagType();
  return dtImag[getImagSize(t)];
}

Type* Converter::convertIntType(const types::QualifiedType qt) {
  const types::IntType* t = qt.type()->toIntType();
  return dtInt[getIntSize(t)];
}

Type* Converter::convertRealType(const types::QualifiedType qt) {
  const types::RealType* t = qt.type()->toRealType();
  return dtReal[getRealSize(t)];
}

Type* Converter::convertUintType(const types::QualifiedType qt) {
  const types::UintType* t = qt.type()->toUintType();
  return dtUInt[getUintSize(t)];
}

Symbol* Converter::convertParam(const types::QualifiedType qt) {

  const types::Param* p = qt.param();
  const types::Type* t = qt.type();
  INT_ASSERT(p && t);

  if (auto bp = p->toBoolParam()) {
    const types::BoolType* bt = t->toBoolType();
    return new_BoolSymbol(bp->value(), getBoolSize(bt));
  } else if (auto cp = p->toComplexParam()) {
    const types::ComplexType* ct = t->toComplexType();
    types::Param::ComplexDouble tmp = cp->value();
    char buf[64];
    // compute the hexadecimal string form for the number
    snprintf(buf, sizeof(buf), "%a+%ai", tmp.re, tmp.im);
    return new_ComplexSymbol(buf, tmp.re, tmp.im, getComplexSize(ct));
  } else if (auto ip = p->toIntParam()) {
    const types::IntType* it = t->toIntType();
    return new_IntSymbol(ip->value(), getIntSize(it));
  } else if (p->isNoneParam()) {
    return gNone;
  } else if (auto rp = p->toRealParam()) {
    char buf[64];
    // compute the hexadecimal string form for the number
    snprintf(buf, sizeof(buf), "%a", rp->value());

    if (auto rt = t->toRealType()) {
      return new_RealSymbol(buf, getRealSize(rt));
    } else if (auto it = t->toImagType()) {
      return new_ImagSymbol(buf, getImagSize(it));
    }
  } else if (auto sp = p->toStringParam()) {
    if (t->isStringType()) {
      return new_StringSymbol(sp->value().c_str());
    } else if (t->isCStringType()) {
      return new_CStringSymbol(sp->value().c_str());
    } else if (t->isBytesType()) {
      return new_BytesSymbol(sp->value().c_str());
    }
  } else if (auto up = p->toUintParam()) {
    const types::UintType* t = qt.type()->toUintType();
    return new_UIntSymbol(up->value(), getUintSize(t));
  }

  INT_FATAL("should not be reached");
  return nullptr;
}

static ID getModuleId(Context* context, ID id) {
  while (!id.isEmpty()) {
    auto tag = parsing::idToTag(context, id);
    if (tag == uast::asttags::Module)
      return id;
    id = id.parentSymbolId(context);
  }

  return id;
}

FnSymbol* Converter::convertFnOutOfOrder(ID fnId) {
  // Compute the module
  ID moduleId = getModuleId(context, fnId);
  auto ast = parsing::idToAst(context, moduleId);
  INT_ASSERT(ast && ast->isModule());
  auto mod = ast->toModule();
  // Compute the function
  ast = parsing::idToAst(context, fnId);
  INT_ASSERT(ast && ast->isFunction());
  auto fn = ast->toFunction();
  // Add the module containing the function to the modStack if
  // it is not already there.
  bool changeModStack = true;
  if (modStack.size() > 0 && modStack.back().mod == mod)
    changeModStack = false;

  if (changeModStack) {
    modStack.emplace_back(mod);
  }

  FnSymbol* ret = nullptr;
  // note: visit will add fn to the symStack
  Expr* got = visit(fn);
  if (auto def = toDefExpr(got)) {
    ret = toFnSymbol(def->sym);
  } else if (auto block = toBlockStmt(got)) {
    if (auto def = toDefExpr(block->body.last())) {
      ret = toFnSymbol(def->sym);
    }
  }

  // pop
  if (changeModStack) {
    modStack.pop_back();
  }

  return ret;
}

} // end anonymous namespace

ModuleSymbol*
convertToplevelModule(chpl::Context* context,
                      const chpl::uast::Module* mod,
                      ModTag modTag,
                      const chpl::uast::Comment* comment,
                      const chpl::uast::BuilderResult& builderResult) {
  astlocMarker markAstLoc(mod->id());
  Converter c(context, modTag, builderResult);

  // Maybe prepare a toplevel comment to attach to the module.
  if (comment) {
    auto convComment = c.visit(comment);
    INT_ASSERT(convComment == nullptr);
  }

  DefExpr* def = c.visit(mod);
  ModuleSymbol* ret = toModuleSymbol(def->sym);
  return ret;
}
