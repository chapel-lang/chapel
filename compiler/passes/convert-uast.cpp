/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#include "CForLoop.h"
#include "CatchStmt.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "DoWhileStmt.h"
#include "ForLoop.h"
#include "ForallStmt.h"
#include "IfExpr.h"
#include "ImportStmt.h"
#include "LoopExpr.h"
#include "ParamForLoop.h"
#include "TemporaryConversionThunk.h"
#include "TryStmt.h"
#include "WhileDoStmt.h"
#include "build.h"
#include "config.h"
#include "global-ast-vecs.h"
#include "optimizations.h"
#include "parser.h"
#include "resolution.h"
#include "ResolveScope.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/global-strings.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/util/string-escapes.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/util/assertions.h"

#include "llvm/ADT/SmallPtrSet.h"

// If this is set then variables/formals will have their "qual" field set
// now instead of later during resolution.
#define ATTACH_QUALIFIED_TYPES_EARLY 0

using namespace chpl;

namespace {

struct ConvertedSymbolsMap {
  ID inSymbolId;
  ConvertedSymbolsMap* parentMap = nullptr;

  std::unordered_map<ID, Symbol*> syms;
  std::unordered_map<const resolution::TypedFnSignature*, FnSymbol*> fns;

  std::vector<std::pair<SymExpr*, ID>> identFixups;
  std::vector<std::pair<SymExpr*,
                        const resolution::TypedFnSignature*>> callFixups;

  ConvertedSymbolsMap() { }
  ConvertedSymbolsMap(ID id, ConvertedSymbolsMap* parentMap)
    : inSymbolId(id), parentMap(parentMap)
  { }

  // if it was already in the map, this will replace it
  void noteConvertedSym(const uast::AstNode* ast, Symbol* sym, bool trace);
  void noteConvertedFn(const resolution::TypedFnSignature* sig, FnSymbol* fn,
                       bool trace);
  Symbol* findConvertedSym(ID id, bool trace);
  FnSymbol* findConvertedFn(const resolution::TypedFnSignature* sig,
                            bool trace);
  void noteIdentFixupNeeded(SymExpr* se, ID id, ConvertedSymbolsMap* cur,
                            bool trace);
  void noteCallFixupNeeded(SymExpr* se,
                           const resolution::TypedFnSignature* sig,
                           ConvertedSymbolsMap* cur,
                           bool trace);

  ConvertedSymbolsMap* findMapContainingBoth(ID id1, ID id2);

  // find the map containing the passed ID as well as the current symbol ID
  ConvertedSymbolsMap* findMapContaining(ID id) {
    return findMapContainingBoth(id, inSymbolId);
  }

  void applyFixups(Context* context, const uast::AstNode* inAst, bool trace);
};

// TODO: replace this global variable with a field in Converter
// once we have a single Converter instance that converts a module
// and all of its dependencies.
// At that time, the global scope can be represented by symStack[0]
// and code containing a conditional to use gConvertedSyms can be removed.
static ConvertedSymbolsMap gConvertedSyms;

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
    owned<ConvertedSymbolsMap> convertedSyms;

    SymStackEntry(const uast::AstNode* ast,
                  const resolution::ResolutionResultByPostorderID* resolved,
                  ConvertedSymbolsMap* parentMap)
      : ast(ast), resolved(resolved), convertedSyms(nullptr) {
      convertedSyms = toOwned(new ConvertedSymbolsMap(ast->id(), parentMap));
    }
  };

  chpl::Context* context = nullptr;
  bool inTupleDecl = false;
  bool inTupleAssign = false;
  bool inImportOrUse = false;
  bool inForwardingDecl = false;
  bool canScopeResolve = false;
  bool trace = false;
  int delegateCounter = 0;

  ModTag topLevelModTag;
  std::vector<ModStackEntry> modStack;
  std::vector<SymStackEntry> symStack;

  /* When working within a method, field accesses need to be code generated
     as using 'this' rather than as SymExprs pointing to a field.
     To enable that, this stack tracks the Symbol* for the 'this' formal
     for a method currently being generated.
     This is different from symStack above because the process of converting
     ForwardingDecls will add a method that does not exist in the uAST. */
  std::vector<Symbol*> methodThisStack;


  Converter(chpl::Context* context, ModTag topLevelModTag)
    : context(context),
      topLevelModTag(topLevelModTag) { }

  // general functions for converting
  Expr* convertAST(const uast::AstNode* node);
  Type* convertType(const types::QualifiedType qt);
  Symbol* convertParam(const types::QualifiedType qt);

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

  // methods to help track what has been converted
  void noteConvertedSym(const uast::AstNode* ast, Symbol* sym);
  void noteConvertedFn(const resolution::TypedFnSignature* sig, FnSymbol* fn);
  Symbol* findConvertedSym(ID id);
  FnSymbol* findConvertedFn(const resolution::TypedFnSignature* sig);
  void noteIdentFixupNeeded(SymExpr* se, ID id);
  void noteCallFixupNeeded(SymExpr* se,
                           const resolution::TypedFnSignature* sig);
  void noteAllContainedFixups(BaseAST* ast, int depth);

  // symStack helpers
  void pushToSymStack(
       const uast::AstNode* ast,
       const resolution::ResolutionResultByPostorderID* resolved);
  void popFromSymStack(const uast::AstNode* ast, BaseAST* ret);
  const resolution::ResolutionResultByPostorderID* currentResolutionResult();

  bool shouldScopeResolve(ID symbolId) {
    if (canScopeResolve) {
      return fDynoScopeBundled ||
             !chpl::parsing::idIsInBundledModule(context, symbolId);
    }

    return false;
  }
  bool shouldScopeResolve(const uast::AstNode* node) {
    return shouldScopeResolve(node->id());
  }

  bool shouldResolve(UniqueString symbolPath) {
    return false;
  }
  bool shouldResolve(ID symbolId) {
    return shouldResolve(symbolId.symbolPath());
  }
  bool shouldResolve(const uast::AstNode* node) {
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
      auto linkageStr = linkageName->toStringLiteral();
      INT_ASSERT(linkageStr);
      auto ret = astr(linkageStr->value());
      return ret;
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

  Expr* visit(const uast::Comment* node) {
    return nullptr;
  }

  Expr* visit(const uast::AttributeGroup* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  Expr* visit(const uast::Attribute* node) {
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
    auto attr = node->attributeGroup();

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

    if (!attr->isUnstable()) {
      INT_ASSERT(attr->unstableMessage().isEmpty());
    }

    if (attr->isUnstable()) {
      INT_ASSERT(!sym->hasFlag(FLAG_UNSTABLE));
      sym->addFlag(FLAG_UNSTABLE);

      auto msg = attr->unstableMessage();
      if (!msg.isEmpty()) {
        sym->unstableMsg = astr(msg);
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

  Expr* resolvedIdentifier(const uast::Identifier* node) {
    // Don't try to resolve identifiers in use/import yet
    // (it messes up the current use/import build routines)
    if (inImportOrUse) {
      return nullptr;
    }

    // In forwarding declarations, don't convert things in "except" clauses
    // into SymExprs.
    if (inForwardingDecl) {
      return nullptr;
    }

    if (inTupleAssign && node->name() == USTR("_")) {
      // Don't resolve underscore node, just return chpl__tuple_blank.
      return new UnresolvedSymExpr("chpl__tuple_blank");
    }

    // Check for a resolution result that includes a target ID
    if (auto r = currentResolutionResult()) {
      const resolution::ResolvedExpression* rr = r->byAstOrNull(node);
      if (rr != nullptr) {
        auto id = rr->toId();
        if (id.isFabricatedId()) {
          // Right now, this only covers extern block elements
          // For those, return nullptr because we can't yet compute
          // the type of those.
          // TODO: compute the appropriate 'extern proc' etc and return that
          CHPL_ASSERT(id.fabricatedIdKind() == ID::ExternBlockElement);
          return nullptr;
        } else if (id.isEmpty() && node->name() == USTR("super")) {
          // The identifier is 'super' and doesn't refer to any variable
          // of that name, so it's a this.super call. Translate it as such.
          if (methodThisStack.empty()) {
            // TODO: probably too strict; what about field initializers?
            USR_FATAL(node->id(), "super cannot occur outside of a method");
          }
          Symbol* parentMethodConvertedThis = methodThisStack.back();
          auto thisExpr = new SymExpr(parentMethodConvertedThis);
          auto nameExpr = new_CStringSymbol(node->name().c_str());
          CallExpr* ret = new CallExpr(".", thisExpr, nameExpr);
          return ret;
        } else if (rr->isBuiltin()) {
          auto scope = ResolveScope::getScopeFor(theProgram->block);
          if (!scope) scope = ResolveScope::getRootModule();

          if (auto symbol = scope->lookupNameLocally(astr(node->name().c_str()), /* isUse */ false)) {
            return new SymExpr(symbol);
          }
        } else if (!id.isEmpty()) {
          // Don't resolve non-method, non-parenless function references.
          //
          // TODO: it's not quite clear why this is a problem; however, the
          // symptoms of not doing this check are that forall optimizations
          // pick the function identifier as something "captured" from outer
          // scope.
          if (parsing::idIsFunction(context, id) &&
              !parsing::idIsMethod(context, id) &&
              !parsing::idIsParenlessFunction(context, id)) {
            return nullptr;
          }

          // If we're referring to an associated type in an interface,
          // leave it unconverted for now because the compiler does some
          // mangling of the AST and breaks the "points-to" ID.
          auto toAst = parsing::idToAst(context, id);
          if (auto varLikeDecl = toAst->toVarLikeDecl()) {
            if (varLikeDecl->storageKind() == types::QualifiedType::TYPE) {
              auto toParentId = parsing::idToParentId(context, id);
              auto toParentAst = parsing::idToAst(context, toParentId);

              if (toParentAst->isInterface()) {
                // We're looking at an associated type.
                Symbol* sym = new TemporaryConversionSymbol(id);
                return new SymExpr(sym);
              }
            }
          }

          // figure out if it is field access
          // TODO: Once we are using types for 'this', this should check
          // that it is not a field access to some record/class unrelated
          // to the current 'this' type.
          bool isFieldAccess = false;
          Symbol* parentMethodConvertedThis = nullptr;
          if (parsing::idIsField(context, id) || parsing::idIsMethod(context, id)) {
            if (methodThisStack.size() > 0) {
              parentMethodConvertedThis = methodThisStack.back();
              isFieldAccess = true;
            }
          }

          // handle field access when only scope resolving
          if (!shouldResolve(node) && isFieldAccess) {
            // if we are just scope resolving, convert field
            // access to this.field using a string literal to
            // match production scope resolve
            Symbol* thisSym = parentMethodConvertedThis;
            INT_ASSERT(thisSym != nullptr);
            auto ast = parsing::idToAst(context, id);
            INT_ASSERT(ast);
            UniqueString name;
            if (auto var = ast->toVariable()) {
              name = var->name();
            } else {
              auto fn = ast->toFunction();
              INT_ASSERT(fn);
              name = fn->name();
            }
            auto str = new_CStringSymbol(name.c_str());
            CallExpr* ret = new CallExpr(".", thisSym, str);
            return ret;
          }

          // handle other Identifiers
          Symbol* sym = findConvertedSym(id);
          SymExpr* se = new SymExpr(sym);
          Expr* ret = se;

          if (parsing::idIsParenlessFunction(context, id)) {
            // it's a parenless function call so add a CallExpr
            ret = new CallExpr(se);
          }
          if (isFieldAccess) {
            INT_FATAL("resolving field access call not yet implemented");
            // TODO: convert it to a call to the field accessor
            // using the resolved TypedFnSignature from rr
          }

          // fixup, if any, will be noted in noteAllContainedFixups

          return ret;
        }
      }
    }

    return nullptr;
  }

  Expr* visit(const uast::Identifier* node) {
    // first, try to convert it using the resolution result
    if (Expr* e = resolvedIdentifier(node)) {
      return e;
    }

    // check for a reserved word
    auto name = node->name();
    if (auto remap = reservedWordRemapForIdent(name)) {
      return remap;
    }

    // otherwise use an UnresolvedSymExpr
    return new UnresolvedSymExpr(name.c_str());
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

  Expr* visit(const uast::Local* node) {
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
    auto condition = convertExprOrNull(node->condition());
    if (condition) {
      return buildThunk(buildConditionalLocalStmt, condition, body);
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

  Expr* visit(const uast::Include* node) {
    bool isIncPrivate = node->visibility() == uast::Decl::PRIVATE;

    const uast::Module* umod =
      parsing::getIncludedSubmodule(context, node->id());
    if (umod == nullptr) {
      return nullptr;
    }

    bool isModPrivate = umod->visibility() == uast::Decl::PRIVATE;
    const uast::BuilderResult* builderResult =
      parsing::parseFileContainingIdToBuilderResult(context, umod->id());
    INT_ASSERT(builderResult);

    UniqueString filePath;

    if (builderResult != nullptr) {
      filePath = builderResult->filePath();
    }

    // convert the included module

    // when converting the module,
    // make sure to use ID for included submodule
    // rather than the module include.
    astlocMarker markAstLoc(umod->id());

    ModuleSymbol* mod = convertModule(umod);
    INT_ASSERT(mod);

    // make some adjustments
    if (isIncPrivate && !isModPrivate) {
      mod->addFlag(FLAG_PRIVATE);
    }
    mod->addFlag(FLAG_INCLUDED_MODULE);

    if (fWarnUnstable && mod->modTag == MOD_USER) {
      USR_WARN(node->id(), "module include statements are not yet stable "
                           "and may change");
    }

    // allow production compiler to take action now that it is parsed
    noteParsedIncludedModule(mod, astr(filePath));

    // note that the converted 'module include' is the same as 'mod'
    noteConvertedSym(node, mod);

    return buildChapelStmt(new DefExpr(mod));
  }

  Expr* visit(const uast::Import* node) {
    inImportOrUse = true;
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

    inImportOrUse = false;
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
    CHPL_ASSERT(renameIdent);
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

    inImportOrUse = true;
    BlockStmt* ret = nullptr;
    if (node->numVisibilityClauses() == 1) {
      ret = convertUsePossibleLimitations(node);
    } else {
      ret = convertUseNoLimitations(node);
    }
    inImportOrUse = false;
    return ret;
  }

  Expr* visit(const uast::VisibilityClause* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  Expr* convertScanReduceOp(const uast::AstNode* node) {
    if (auto opIdent = node->toIdentifier()) {
      auto name = opIdent->name();
      // TODO: check for a resolution result for these
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

      if (name == USTR("max"))
        return new UnresolvedSymExpr("MaxReduceScanOp");
      if (name == USTR("maxloc"))
        return new UnresolvedSymExpr("maxloc");
      if (name == USTR("min"))
        return new UnresolvedSymExpr("MinReduceScanOp");
      if (name == USTR("minloc"))
        return new UnresolvedSymExpr("minloc");
      if (name == USTR("minmax"))
        return new UnresolvedSymExpr("minmax");
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
      bool isTaskVarDecl = false;

      // Normal conversion of TaskVar, reduce intents handled below.
      if (const uast::TaskVar* tv = expr->toTaskVar()) {
        svs = convertTaskVar(tv);
        INT_ASSERT(svs);

        // (const x) is a task intent, but (const x: int) and (const x = 1)
        // are task-private variable declarations.
        isTaskVarDecl = tv->initExpression() != nullptr ||
                        tv->typeExpression() != nullptr;
      // Handle reductions in with clauses explicitly here.
      } else if (const uast::ReduceIntent* rd = expr->toReduceIntent()) {
        astlocMarker markAstLoc(rd->id());

        Expr* ovar = new UnresolvedSymExpr(rd->name().c_str());
        Expr* riExpr = convertScanReduceOp(rd->op());
        svs = ShadowVarSymbol::buildFromReduceIntent(ovar, riExpr);
      } else {
        INT_FATAL("Not handled!");
      }

      INT_ASSERT(svs != nullptr);

      if (parent->isBracketLoop() || parent->isForall() ||
          parent->isForeach()) {
        noteConvertedSym(expr, svs);
        addForallIntent(ret, svs);
      } else {
        if (isTaskVarDecl) {
          noteConvertedSym(expr, svs);
        } else {
          auto r = symStack.back().resolved;
          if (r != nullptr) {
            if (auto rr = r->byAstOrNull(expr)) {
              noteConvertedSym(expr, findConvertedSym(rr->toId()));
            }
          }
        }
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

    if (canScopeResolve && errorVar != nullptr) {
      ret->createErrSym();
      DefExpr* def = toDefExpr(body->body.head);
      noteConvertedSym(errorVar, def->sym);
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
   * TODO(Resolver): This checking should move to the dyno resolver in the future
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

        DefExpr* def = toDefExpr(toCallExpr(cond)->get(1));
        noteConvertedSym(node->condition(), def->sym);
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
    VarSymbol* var = new VarSymbol(node->name().c_str());
    noteConvertedSym(node, var);
    return new DefExpr(var);
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
      DefExpr* def = toDefExpr(toCallExpr(condExpr)->get(1));
      noteConvertedSym(node->condition(), def->sym);
    } else {
      condExpr = toExpr(convertAST(node->condition()));
    }
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
    return WhileDoStmt::build(condExpr, body);
  }

  /// IndexableLoops ///

  // Help to convert loop index variables before loop bodies
  // so that the Symbols are available to refer to in SymExprs.
  // It returns a DefExpr or else a _build_tuple call containing others
  Expr* convertLoopIndexDecl(const uast::Decl* node) {
    if (node == nullptr) return nullptr;

    astlocMarker markAstLoc(node->id());

    // Simple variables get converted to DefExprs
    if (const uast::Variable* var = node->toVariable()) {
      if (var->name() == USTR("_")) {
        // don't try to create a DefExpr for '_' and instead use
        // an UnresolvedSymExpr for this case.
        return new UnresolvedSymExpr("chpl__tuple_blank");
      }

      return convertVariable(var, false);

    // For tuples, recursively call 'convertLoopIndexDecl' on each element.
    // to produce a CallExpr containing DefExprs
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
    return node->isMaybeArrayType();
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
  Expr* visit(const uast::Coforall* node) {
    INT_ASSERT(!node->isExpressionLevel());

    // These are the arguments that 'buildCoforallLoopStmt' requires.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iterator = toExpr(convertAST(node->iterand()));
    CallExpr* byref_vars = convertWithClause(node->withClause(), node);
    auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
    bool zippered = node->iterand()->isZip();

    return buildThunk(buildCoforallLoopStmt, indices, iterator, byref_vars, body, zippered);
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

      DefExpr* indexDef = toDefExpr(index);
      INT_ASSERT(indexDef && isVarSymbol(indexDef->sym));

      VarSymbol* indexVar = toVarSymbol(indexDef->sym);

      body = createBlockWithStmts(node->stmts(), node->blockStyle());
      BlockStmt* block = toBlockStmt(body);
      INT_ASSERT(block);

      ret = buildParamForLoopStmt(indexVar, iteratorExpr, block);

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

    // An 'if-expr' without an else is special pattern for the builder.
    if (auto noElseCond = node->stmt(0)->toConditional()) {
      if (!noElseCond->hasElseBlock()) {
        expr = singleExprFromStmts(noElseCond->thenStmts());
        cond = toExpr(convertAST(noElseCond->condition()));
        INT_ASSERT(cond);
      }
    }

    if (!expr) {
      INT_ASSERT(!cond);
      expr = singleExprFromStmts(node->stmts());
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

    // convert these for now, despite the error, so that symbols are converted.
    convertWithClause(node->withClause(), node);

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
    std::string quoted = escapeStringC(node->value().str());
    SymExpr* se = buildBytesLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_BYTES);
    return se;
  }

  Expr* visit(const uast::CStringLiteral* node) {
    std::string quoted = escapeStringC(node->value().str());
    SymExpr* se = buildCStringLiteral(quoted.c_str());
    VarSymbol* v = toVarSymbol(se->symbol());
    INT_ASSERT(v && v->immediate);
    INT_ASSERT(v->immediate->const_kind == CONST_KIND_STRING);
    INT_ASSERT(v->immediate->string_kind == STRING_KIND_C_STRING);
    return se;

  }

  Expr* visit(const uast::StringLiteral* node) {
    std::string quoted = escapeStringC(node->value().str());
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

  Expr* convertTupleAssign(const uast::OpCall* node) {
    if (node->op() != USTR("=") || node->numActuals() < 1
        || !node->actual(0)->isTuple()) return nullptr;

    INT_ASSERT(node->numActuals() == 2);
    inTupleAssign = true;
    Expr* lhs = convertAST(node->actual(0));
    inTupleAssign = false;
    INT_ASSERT(lhs);
    Expr* rhs = convertAST(node->actual(1));
    INT_ASSERT(rhs);

    const char* opName = astr(node->op());
    CallExpr* ret = new CallExpr(opName);
    ret->insertAtTail(lhs);
    ret->insertAtTail(rhs);

    return ret;
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
    } else if (auto conv = convertTupleAssign(node)) {
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

  Expr* visit(const uast::ReduceIntent* reduce) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
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

    for (auto decl : node->decls()) {
      INT_ASSERT(decl->linkage() == node->linkage());

      Expr* conv = nullptr;
      if (auto var = decl->toVariable()) {

        // Do not use the linkage name since multi-decls cannot be renamed.
        const bool useLinkageName = false;
        conv = convertVariable(var, useLinkageName);

        DefExpr* defExpr = toDefExpr(conv);
        INT_ASSERT(defExpr);
        auto varSym = toVarSymbol(defExpr->sym);
        INT_ASSERT(varSym);

      // Otherwise convert in a generic fashion.
      } else {
        conv = convertAST(decl);
      }

      INT_ASSERT(conv);
      ret->insertAtTail(conv);
    }

    INT_ASSERT(!inTupleDecl);
    CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
    ret->insertAtTail(end);

    return ret;
  }

  // Right now components are one of: Variable, Formal, TupleDecl.
  BlockStmt* convertTupleDeclComponents(const uast::TupleDecl* node) {
    astlocMarker markAstLoc(node->id());

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
        auto varSym = new VarSymbol(formal->name().c_str());
        conv = new DefExpr(varSym);
        noteConvertedSym(formal, varSym);

      // Do not use the visitor because it produces a block statement.
      } else if (auto var = decl->toVariable()) {
        const bool useLinkageName = false;
        conv = convertVariable(var, useLinkageName);

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
    auto tuple = convertTupleDeclComponents(node);
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
    INT_ASSERT(!inTupleDecl);
    CallExpr* end = new CallExpr(PRIM_END_OF_STATEMENT);
    ret->insertAtTail(end);

    return ret;
  }

  /// ForwardingDecl ///
  Expr* visit(const uast::ForwardingDecl* node) {
    // ForwardingDecl may contain a VisibilityClause and
    // then an Expression or a Variable declaration

    auto ret = new BlockStmt(BLOCK_SCOPELESS);

    UniqueString varName;

    // First, if we find a variable declaration, add that to the block
    if (auto var = node->expr()->toVariable()) {
      auto child = node->expr()->toVariable();
      BlockStmt* varBlock = (BlockStmt*)visit(child);
      // Remove the END_OF_STATEMENT marker, not used for fields
      Expr* last = varBlock->body.last();
      if (last && isEndOfStatementMarker(last)) {
        last->remove();
      }
      // Add the DefExpr for the VarSymbol to the ret Block
      for_alist(tmp, varBlock->body) {
        ret->insertAtTail(tmp->remove());
      }
      varName = var->name();
    }

    // Now construct the method (always a primary method) for the
    // forwarding part
    const char* name = astr("chpl_forwarding_expr", istr(++delegateCounter));
    if (!varName.isEmpty()) {
      name = astr(name, "_", varName.c_str());
    }

    FnSymbol* fn = new FnSymbol(name);

    fn->addFlag(FLAG_INLINE);
    fn->addFlag(FLAG_MAYBE_REF);
    fn->addFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS);
    fn->addFlag(FLAG_COMPILER_GENERATED);

    // compute the 'this' formal type
    const uast::AggregateDecl* decl = nullptr;
    INT_ASSERT(symStack.size() > 0);
    {
      SymStackEntry& last = symStack.back();
      INT_ASSERT(last.ast != nullptr);
      decl = last.ast->toAggregateDecl();
      INT_ASSERT(decl);
    }
    // TODO: use the resolved type for the contained declaration
    auto thisTypeExpr = new UnresolvedSymExpr(decl->name().c_str());

    // add a 'this' formal
    ArgSymbol* arg = new ArgSymbol(fn->thisTag, "this",
                                   dtUnknown, thisTypeExpr);
    fn->_this = arg;

    fn->insertFormalAtTail(new DefExpr(new ArgSymbol(INTENT_BLANK,
                                                 "_mt",
                                                 dtMethodToken)));
    fn->insertFormalAtTail(new DefExpr(arg));

    // note that we're in a forwarding declaration working with 'fn'
    // to get the field accesses to convert correctly
    methodThisStack.push_back(fn->_this);

    Expr* expr = nullptr;

    std::vector<PotentialRename*>* visNames = nullptr;
    bool except = false;
    if (auto vis = node->expr()->toVisibilityClause()) {
      // compute the visibility clauses
      if (vis->limitationKind() == uast::VisibilityClause::EXCEPT) {
        except = true;
      }
      // convert the AstList of renames
      visNames = new std::vector<PotentialRename*>;
      auto oldInForwardingDecl = inForwardingDecl;
      inForwardingDecl = true;
      for (auto lim : vis->limitations()) {
        PotentialRename* rename = convertRename(lim);
        visNames->push_back(rename);
      }
      inForwardingDecl = oldInForwardingDecl;
      // compute the forwarded-to expression
      expr = convertExprOrNull(vis->symbol());
    } else {
      // convert the forwarding expression & insert it into fn
      if (!varName.isEmpty()) {
        // forwarding var bla;
        expr = new UnresolvedSymExpr(varName.c_str());
      } else {
        // forwarding someExpression();
        expr = convertExprOrNull(node->expr());
      }
    }

    // insert the forwarding expression into the forwarding method
    fn->body->insertAtTail(new CallExpr(PRIM_RETURN, expr));

    // note, no longer working within forwarding method 'fn'
    methodThisStack.pop_back();

    // Add the forwarding target method to the ret Block
    DefExpr* fnDef = new DefExpr(fn);
    ret->insertAtTail(fnDef);

    // Create a ForwardingStmt to help the production resolver
    // It includes handling 'only' and 'except'
    ForwardingStmt* forwardingStmt = nullptr;
    if (node->expr()->isVisibilityClause()) {
      forwardingStmt = buildForwardingStmt(fnDef, visNames, except);
    } else {
      forwardingStmt = buildForwardingStmt(fnDef);
    }
    ret->insertAtTail(forwardingStmt);

    return ret;
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

  static const char*
  createAnonymousRoutineName(const uast::Function* node) {
    std::ignore = node;

    static const int maxDigits = 100;
    static unsigned int nextId = 0;
    static const char* prefix = "chpl_anon";
    char buf[maxDigits];

    if ((nextId + 1) == 0) INT_FATAL("Overflow for lambda ID number");

    auto kind = astr(uast::Function::kindToString(node->kind()));

    // Use sprintf to prevent buffer overflow if there are too many lambdas.
    int n = snprintf(buf, (size_t) maxDigits, "%s_%s_%i", prefix, kind,
                     nextId++);
    if (n > (int) maxDigits) INT_FATAL("Too many lambdas.");

    auto ret = astr(buf);
    return ret;
  }

  static const char*
  convertFunctionNameAndAstr(const uast::Function* node) {
    auto name = node->name();
    auto kind = node->kind();

    if (node->isAnonymous()) return createAnonymousRoutineName(node);

    if (name.isEmpty()) {
      INT_ASSERT(kind == uast::Function::PROC);
      return nullptr;
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

  // build up the userString as in old parser
  // needed to match up some error outputs
  // NOTE: parentheses may have been discarded from the original user
  // declaration, and if so, we are not able to reconstruct them at
  // this time
  const char* constructUserString(const uast::Function* node) {
    std::stringstream ss;
    printFunctionSignature(ss, node);
    auto ret = astr(ss.str());
    return ret;
  }

  const char* constructUserString(const uast::FunctionSignature* node) {
    std::stringstream ss;
    printFunctionSignature(ss, node);
    auto ret = astr(ss.str());
    return ret;
  }

  FnSymbol* convertFunction(const uast::Function* node) {
    // Decide if we want to resolve this function
    bool shouldResolveFunction = shouldResolve(node);
    bool shouldScopeResolveFunction = shouldResolveFunction ||
                                      shouldScopeResolve(node);

    const resolution::ResolutionResultByPostorderID* resolved = nullptr;
    const resolution::ResolvedFunction* resolvedFn = nullptr;
    const resolution::TypedFnSignature* initialSig = nullptr;
    const resolution::PoiScope* poiScope = nullptr;

    if (shouldResolveFunction || shouldScopeResolveFunction) {
      if (shouldResolveFunction) {
        resolvedFn = resolution::resolveConcreteFunction(context, node->id());
      } else {
        resolvedFn =
          resolution::scopeResolveFunction(context, node->id());
      }
      if (resolvedFn) {
        resolved = &resolvedFn->resolutionById();
      }
    }

    // Also add to symStack
    // Add a SymStackEntry to the end of the symStack
    pushToSymStack(node, resolved);

    FnSymbol* fn = new FnSymbol("_");

    // Note that we have already converted this function
    noteConvertedSym(node, fn);
    if (resolvedFn)
      noteConvertedFn(resolvedFn->signature(), fn);

    fn->userString = constructUserString(node);

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
    ArgSymbol* convertedReceiver = nullptr;

    // Add the formals
    if (node->numFormals() > 0) {
      for (auto decl : node->formals()) {
        DefExpr* conv = nullptr;

        // A "normal" formal.
        if (auto formal = decl->toFormal()) {
          conv = toDefExpr(convertAST(formal));
          INT_ASSERT(conv);

          // Special handling for implicit receiver formal.
          if (formal->name() == USTR("this")) {
            INT_ASSERT(convertedReceiver == nullptr);

            thisTag = convertFormalIntent(formal->intent());

            convertedReceiver = toArgSymbol(conv->sym);
            INT_ASSERT(convertedReceiver);
            methodThisStack.push_back(convertedReceiver);

            conv->sym->addFlag(FLAG_ARG_THIS);

            if (thisTag == INTENT_TYPE) {
              setupTypeIntentArg(convertedReceiver);
            }
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
          // Note the formal is converted so we can wire up SymExprs later
          noteConvertedSym(decl, conv->sym);
        }
      }
    }

    const char* convName = convertFunctionNameAndAstr(node);

    // used to be buildFunctionSymbol
    fn->cname = fn->name = astr(convName);

    if (convertedReceiver) {
      fn->thisTag = thisTag;
      fn->_this = convertedReceiver;
      fn->setMethod(true);
      ArgSymbol* mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
      fn->insertFormalAtHead(new DefExpr(mt));
      if (node->isPrimaryMethod()) {
        fn->addFlag(FLAG_METHOD_PRIMARY);
      }
    }

    if (fn->name == astrDeinit)
      fn->addFlag(FLAG_DESTRUCTOR);

    if (isAssignOp(node->name())) {
      fn->addFlag(FLAG_ASSIGNOP);
    }

    RetTag retTag = convertRetTag(node->returnIntent());

    if (node->kind() == uast::Function::ITER) {
      fn->addFlag(FLAG_ITERATOR_FN);

    } else if (node->kind() == uast::Function::OPERATOR) {
      fn->addFlag(FLAG_OPERATOR);
      if (fn->_this != NULL) {
        updateOpThisTagOrErr(fn);
        setupTypeIntentArg(toArgSymbol(fn->_this));
      }

    } else if (node->isAnonymous()) {
      fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
      fn->addFlag(FLAG_ANONYMOUS_FN);
      if (node->kind() == uast::Function::LAMBDA) {
        fn->addFlag(FLAG_LEGACY_LAMBDA);
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

    if (node->body()) {
      INT_ASSERT(node->linkage() != uast::Decl::EXTERN);

      // TODO: What about 'proc foo() return 0;'?
      auto style = uast::BlockStyle::EXPLICIT;
      body = createBlockWithStmts(node->stmts(), style);
    }

    setupFunctionDecl(fn, retTag, retType, node->throws(),
                      whereClause,
                      lifetimeConstraints,
                      body);

    if (node->linkage() != uast::Decl::DEFAULT_LINKAGE) {
      Flag linkageFlag = convertFlagForDeclLinkage(node);
      Expr* linkageExpr = convertExprOrNull(node->linkageName());

      // This thing sets the 'cname' if it's a string literal, attaches
      // some flags, sets the return type to 'void' if no type is
      // specified, and attaches a dummy formal for the C name (?).
      setupExternExportFunctionDecl(linkageFlag, linkageExpr, fn);
    }

    // Update the function symbol with any resolution results.
    if (shouldResolveFunction) {
      auto retType = resolution::returnType(context, initialSig, poiScope);
      fn->retType = convertType(retType);
    }

    // pop the function from the symStack
    popFromSymStack(node, fn);
    if (convertedReceiver) {
      methodThisStack.pop_back();
    }

    return fn;
  }

  // TODO: Wire up the resolution/scope-resolve stuff as for functions.
  FnSymbol* convertFunctionSignature(const uast::FunctionSignature* node) {
    FnSymbol* fn = new FnSymbol(nullptr);

    fn->userString = constructUserString(node);

    if (node->isParenless()) fn->addFlag(FLAG_NO_PARENS);
    if (node->thisFormal() != nullptr) {
      fn->addFlag(FLAG_METHOD);
    }

    IntentTag thisTag = INTENT_BLANK;
    ArgSymbol* convertedReceiver = nullptr;

    // Add the formals
    if (node->numFormals() > 0) {
      for (auto decl : node->formals()) {
        DefExpr* conv = nullptr;

        // A "normal" formal.
        if (auto formal = decl->toFormal()) {
          conv = toDefExpr(convertAST(formal));
          INT_ASSERT(conv);

          // Special handling for implicit receiver formal.
          if (formal->name() == USTR("this")) {
            INT_ASSERT(convertedReceiver == nullptr);

            thisTag = convertFormalIntent(formal->intent());

            convertedReceiver = toArgSymbol(conv->sym);
            INT_ASSERT(convertedReceiver);

            conv->sym->addFlag(FLAG_ARG_THIS);

            if (thisTag == INTENT_TYPE) {
              setupTypeIntentArg(convertedReceiver);
            }

          // E.g., a formal like 'proc(_: int)'.
          } else if (formal->isExplicitlyAnonymous()) {
            conv->sym->addFlag(FLAG_ANONYMOUS_FORMAL);
            INT_ASSERT(!strcmp(conv->sym->name, "_"));
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
        } else if (auto anon = decl->toAnonFormal()) {
          INT_FATAL("Not possible - should have been handled by frontend");
          conv = toDefExpr(convertAST(anon));
          INT_ASSERT(conv);
        } else {
          INT_FATAL("Not handled yet!");
        }

        // Attaches def to function's formal list.
        if (conv) {
          buildFunctionFormal(fn, conv);
          // Note the formal is converted so we can wire up SymExprs later
          noteConvertedSym(decl, conv->sym);
        }
      }
    }

    // Should not be possible - other cases should be using the
    // 'convertFunction' routine for now, even if they store
    // the info using a signature under the covers.
    INT_ASSERT(node->kind() == uast::Function::PROC);

    // The name is not relevant as this will not participate in
    // function resolution in the typical way - this symbol is only
    // a vehicle for its formals and return type.
    // auto convName = astr(uast::Function::kindToString(node->kind()));
    fn->cname = nullptr;

    if (convertedReceiver) {
      fn->thisTag = thisTag;
      fn->_this = convertedReceiver;
      fn->setMethod(true);
      auto mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
      fn->insertFormalAtHead(new DefExpr(mt));
    }

    RetTag retTag = convertRetTag(node->returnIntent());
    auto nodeRetType = node->returnType();
    Expr* retType = (nodeRetType && nodeRetType->isBracketLoop())
            ? convertArrayType(nodeRetType->toBracketLoop())
            : convertExprOrNull(nodeRetType);

    // TODO: I'd like to get rid of these build calls (if Michael
    // has not already gotten rid of them on main), as there's not
    // too much in them.
    setupFunctionDecl(fn, retTag, retType, node->throws(),
                      /*whereClause*/ nullptr,
                      /*lifetimeConstraints*/ nullptr,
                      /*body*/ nullptr);

    return fn;
  }

  Expr* visit(const uast::AnonFormal* node) {
    auto intent = convertFormalIntent(node->intent());
    Expr* typeExpr = nullptr;

    if (auto te = node->typeExpression()) {
      typeExpr = convertAST(te);
      INT_ASSERT(typeExpr);
    }

    auto convFormal = new ArgSymbol(intent, nullptr, dtUnknown, typeExpr);
    convFormal->addFlag(FLAG_ANONYMOUS_FORMAL);

    auto ret = new DefExpr(convFormal);
    return ret;
  }

  Expr* visit(const uast::FunctionSignature* node) {
    FnSymbol* fn = convertFunctionSignature(node);
    fn->addFlag(FLAG_ANONYMOUS_FN);
    fn->addFlag(FLAG_NO_FN_BODY);
    INT_ASSERT(fn->isAnonymous() && fn->isSignature());
    auto ret = new DefExpr(fn);
    return ret;
  }

  Expr* visit(const uast::Function* node) {
    FnSymbol* fn = nullptr;
    Expr* ret = nullptr;

    fn = convertFunction(node);

    // For anonymous functions, return a DefExpr instead of a BlockStmt
    // containing a DefExpr because this is the pattern expected
    // by the production compiler. Otherwise, return a block containing
    // a DefExpr.
    if (node->isAnonymous()) {
      DefExpr* def = new DefExpr(fn);
      ret = def;
    } else {
      BlockStmt* stmt = buildChapelStmt(new DefExpr(fn));
      ret = stmt;
    }

    return ret;
  }

  Expr* visit(const uast::Interface* node) {
    const char* name = astr(node->name());
    CallExpr* formals = new CallExpr(PRIM_ACTUALS_LIST);
    auto style = uast::BlockStyle::EXPLICIT;
    BlockStmt* body = createBlockWithStmts(node->stmts(), style);

    for (auto ast : node->formals()) {
      if (auto formal = ast->toFormal()) {
        const char* name = astr(formal->name());
        auto ifcFormal = InterfaceSymbol::buildFormal(name, INTENT_TYPE);
        formals->insertAtTail(ifcFormal);
        noteConvertedSym(formal, ifcFormal->sym);
      } else {
        INT_FATAL("Interface formal is not represented by a formal AST node!");
      }
    }

    auto isym = InterfaceSymbol::buildDef(name, formals, body);

    // associated types declarations in buildDef are transformed from
    // variables to TypeSymbols. Iterate the type symbol definitions
    // on the Dyno end and re-run noteConvertedSym to make sure they
    // refer to the newly-inserted TypeSymbols and not the now-deleted
    // variables.
    const auto& isymAssociatedTypes =
      toInterfaceSymbol(isym->sym)->associatedTypes;
    for (auto stmt : node->stmts()) {
      auto varLikeDecl = stmt->toVarLikeDecl();
      if (varLikeDecl == nullptr) continue;
      if (varLikeDecl->storageKind() != types::QualifiedType::TYPE) continue;

      auto assocTypeName = varLikeDecl->name();
      noteConvertedSym(varLikeDecl,
                       isymAssociatedTypes.at(assocTypeName.c_str())->symbol);
    }

    auto ret = buildChapelStmt(isym);
    noteConvertedSym(node, isym->sym);

    return ret;
  }

  ModuleSymbol* convertModule(const uast::Module* node) {
    // Decide if we want to resolve this module
    bool shouldResolveModule = shouldResolve(node);
    bool shouldScopeResolveModule = shouldResolveModule ||
                                    shouldScopeResolve(node);

    const resolution::ResolutionResultByPostorderID* resolved = nullptr;

    if (shouldResolveModule || shouldScopeResolveModule) {
      // Resolve the module
      if (shouldResolveModule) {
        const auto& tmp = resolution::resolveModule(context, node->id());
        resolved = &tmp;
      } else {
        const auto& tmp = resolution::scopeResolveModule(context, node->id());
        resolved = &tmp;
      }
    }

    // Push the current module name before descending into children.
    // Add a ModStackEntry to the end of the modStack
    this->modStack.emplace_back(node);

    // Also add to symStack
    pushToSymStack(node, resolved);

    const char* name = astr(node->name());
    UniqueString pathUstr;
    UniqueString ignoredParentSymPath;
    bool foundPath =
      context->filePathForId(node->id(), pathUstr, ignoredParentSymPath);
    (void)foundPath; // avoid unused variable warning
    CHPL_ASSERT(foundPath);
    const char* path = astr(pathUstr);

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

    ModuleSymbol* mod = buildModule(name,
                                    tag,
                                    body,
                                    path,
                                    priv,
                                    prototype);

    if (node->kind() == uast::Module::IMPLICIT) {
      mod->addFlag(FLAG_IMPLICIT_MODULE);
    }

    attachSymbolAttributes(node, mod);

    // Note the module is converted so we can wire up SymExprs later
    noteConvertedSym(node, mod);

    // Pop the module after converting children.
    INT_ASSERT(modStack.size() > 0 && modStack.back().mod == node);
    this->modStack.pop_back();
    popFromSymStack(node, mod);

    return mod;
  }
  DefExpr* visit(const uast::Module* node) {
    ModuleSymbol* mod = convertModule(node);
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

    // noteConvertedSym should be called when handling the enclosing Function

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
    // TODO: can we avoid this UnresolvedSymExpr ?
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
    return attachSymbolStorage((uast::Qualifier) kind, vs);
  }

  void attachSymbolStorage(const uast::TupleDecl::IntentOrKind iok,
                           Symbol* vs) {
    return attachSymbolStorage((uast::Qualifier) iok, vs);
  }

  void attachSymbolStorage(const uast::Qualifier kind, Symbol* vs) {
    auto qual = QUAL_UNKNOWN;

    switch (kind) {
      case uast::Qualifier::VAR:
        qual = QUAL_VAL;
        break;
      case uast::Qualifier::CONST_VAR:
        vs->addFlag(FLAG_CONST);
        qual = QUAL_CONST;
        break;
      case uast::Qualifier::CONST_REF:
        vs->addFlag(FLAG_CONST);
        vs->addFlag(FLAG_REF_VAR);
        qual = QUAL_CONST_REF;
        break;
      case uast::Qualifier::REF:
        vs->addFlag(FLAG_REF_VAR);
        qual = QUAL_REF;
        break;
      case uast::Qualifier::PARAM:
        vs->addFlag(FLAG_PARAM);
        qual = QUAL_PARAM;
        break;
      case uast::Qualifier::TYPE:
        vs->addFlag(FLAG_TYPE_VARIABLE);
        break;
      case uast::Qualifier::INDEX:
        vs->addFlag(FLAG_INDEX_VAR);
        break;
      default:
        break;
    }

    if (ATTACH_QUALIFIED_TYPES_EARLY && qual != QUAL_UNKNOWN) {
      vs->qual = qual;
    }
  }

  static bool isEnsureDomainExprCall(Expr* expr) {
    if (auto call = toCallExpr(expr)) {
      return call->isNamed("chpl__ensureDomainExpr");
    }
    return false;
  }

  CallExpr* convertArrayType(const uast::BracketLoop* node,
                             bool isFormalType=false) {
    astlocMarker markAstLoc(node->id());

    INT_ASSERT(node->isExpressionLevel());

    const uast::TypeQuery* lastTypeQuery = nullptr;
    int numTypeQueries = 0;
    Expr* domActuals = nullptr;
    bool isEmptyDomain = false;

    if (auto iterand = node->iterand()) {

      // Most domains can be converted, but some require special attention.
      if (auto dom = iterand->toDomain()) {

        // If there are no domain expressions, use 'nil'. TODO: 'dtAny'?
        if (!dom->numExprs()) {
          domActuals = new SymExpr(gNil);
          isEmptyDomain = true;

        // Otherwise, check for and sanitize type queries.
        } else {
          for (int i = 0; i < dom->numExprs(); i++) {
            if (auto tq = dom->expr(i)->toTypeQuery()) {
              numTypeQueries += 1;
              lastTypeQuery = tq;
            }
          }
        }
      } else if (auto tq = iterand->toTypeQuery()) {
        numTypeQueries = 1;
        lastTypeQuery = tq;
      }

      CHPL_ASSERT(numTypeQueries <= 1);

      // If there is a type query, extract it from the domain.
      if (lastTypeQuery) {
        CHPL_ASSERT(isFormalType);
        CHPL_ASSERT(!domActuals);
        domActuals = convertAST(lastTypeQuery);
      }

      // Make sure we have something to work with.
      domActuals = !domActuals ? convertAST(iterand) : domActuals;

      if (!isEnsureDomainExprCall(domActuals) && numTypeQueries == 0 &&
          !isEmptyDomain) {
        domActuals = new CallExpr("chpl__ensureDomainExpr", domActuals);
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

          // Use this builder because it is performing checks for skyline
          // arrays amongst other things (that are too arcane for me).
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

    // Note the variable is converted so we can wire up SymExprs later
    noteConvertedSym(node, varSym);

    return ret;
  }

  Expr* visit(const uast::Variable* node) {
    auto isTypeVar = node->kind() == uast::Variable::TYPE;
    auto stmts = new BlockStmt(BLOCK_SCOPELESS);

    auto defExpr = convertVariable(node, true);
    INT_ASSERT(defExpr);
    auto varSym = toVarSymbol(defExpr->sym);
    INT_ASSERT(varSym);

    stmts->insertAtTail(defExpr);

    // Special handling for extern type variables.
    if (isTypeVar) {
      if (node->linkage() == uast::Decl::EXTERN) {
        INT_ASSERT(!node->isConfig());
        INT_ASSERT(defExpr->sym && isVarSymbol(defExpr->sym));
        auto varSym = toVarSymbol(defExpr->sym);
        auto linkageName = node->linkageName() ? varSym->cname : nullptr;
        stmts = convertTypesToExtern(stmts, linkageName);

        // fix up gConvertedSyms since convertTypesToExtern
        // replaced the DefExpr/Symbol
        INT_ASSERT(stmts->body.last() && isDefExpr(stmts->body.last()));
        auto newDef = toDefExpr(stmts->body.last());
        noteConvertedSym(node, newDef->sym);
      }
    }

    // Add a PRIM_END_OF_STATEMENT.
    if (!inTupleDecl && !isTypeVar) {
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

    // Note the enum element is converted so we can wire up SymExprs later
    noteConvertedSym(node, ret->sym);

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
    attachSymbolVisibility(node, enumTypeSym);

    enumType->symbol = enumTypeSym;

    auto ret = new BlockStmt(BLOCK_SCOPELESS);
    ret->insertAtTail(new DefExpr(enumTypeSym));

    // Note the enum type is converted so we can wire up SymExprs later
    noteConvertedSym(node, enumTypeSym);

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

    const resolution::ResolutionResultByPostorderID* resolved = nullptr;
    if (shouldScopeResolve(node)) {
      resolved = &resolution::scopeResolveAggregate(context, node->id());
    }
    pushToSymStack(node, resolved);

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
                                 externFlag);
    INT_ASSERT(ret->sym);

    attachSymbolAttributes(node, ret->sym);
    attachSymbolVisibility(node, ret->sym);

    // Note the type is converted so we can wire up SymExprs later
    noteConvertedSym(node, ret->sym);

    popFromSymStack(node, ret->sym);

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
  if (auto r = currentResolutionResult()) {
    // Get the type of the variable itself
    const resolution::ResolvedExpression* rr = r->byAstOrNull(v);
    if (rr != nullptr) {
      types::QualifiedType qt = rr->type();
      if (!qt.isUnknown()) {
        printf("SETTING VARIABLE TYPE!!\n");

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
  if (auto r = currentResolutionResult()) {
    const resolution::ResolvedExpression* rr = r->byAstOrNull(call);
    if (rr != nullptr) {
      const auto& candidates = rr->mostSpecific();
      int nBest = candidates.numBest();
      if (nBest == 0) {
        // nothing to do
      } else if (nBest > 1) {
        INT_FATAL("return intent overloading not yet handled");
      } else if (nBest == 1) {
        const resolution::TypedFnSignature* sig = candidates.only();
        Symbol* fn = findConvertedFn(sig);
        if (fn == nullptr) {
          // we will fix it later
          fn = new TemporaryConversionSymbol(sig);
        }

        // TODO: Do we need to remove the old baseExpr?
        SymExpr* se = new SymExpr(fn);
        expr->baseExpr = se;
        parent_insert_help(expr, expr->baseExpr);

        // fixup, if any, will noted in noteAllContainedFixups
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

    case typetags::ArrayType: return dtUnknown;
    case typetags::BasicClassType:   return convertBasicClassType(qt);
    case typetags::DomainType:   return dtUnknown;
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

void Converter::noteConvertedSym(const uast::AstNode* ast, Symbol* sym) {
  if (!canScopeResolve) return;

  if (symStack.size() > 0) {
    symStack.back().convertedSyms->noteConvertedSym(ast, sym, trace);
  } else {
    gConvertedSyms.noteConvertedSym(ast, sym, trace);
  }
}

void Converter::noteConvertedFn(const resolution::TypedFnSignature* sig,
                                FnSymbol* fn) {
  if (!canScopeResolve) return;

  if (symStack.size() > 0) {
    symStack.back().convertedSyms->noteConvertedFn(sig, fn, trace);
  } else {
    gConvertedSyms.noteConvertedFn(sig, fn, trace);
  }
}

Symbol* Converter::findConvertedSym(ID id) {
  if (!canScopeResolve) return nullptr;

  if (symStack.size() > 0) {
    return symStack.back().convertedSyms->findConvertedSym(id, trace);
  } else {
    return gConvertedSyms.findConvertedSym(id, trace);
  }
}

FnSymbol* Converter::findConvertedFn(const resolution::TypedFnSignature* sig) {
  if (!canScopeResolve) return nullptr;

  if (symStack.size() > 0) {
    return symStack.back().convertedSyms->findConvertedFn(sig, trace);
  } else {
    return gConvertedSyms.findConvertedFn(sig, trace);
  }
}

void Converter::noteIdentFixupNeeded(SymExpr* se, ID id) {
  if (!canScopeResolve) return;

  ConvertedSymbolsMap* m = nullptr;
  ConvertedSymbolsMap* cur = &gConvertedSyms;
  if (symStack.size() > 0) {
    // figure out where to put the fixup
    cur = symStack.back().convertedSyms.get();
    m = cur->findMapContaining(id);
  }

  if (m == nullptr) {
    m = &gConvertedSyms;
  }

  m->noteIdentFixupNeeded(se, id, cur, trace);
}

void Converter::noteCallFixupNeeded(SymExpr* se,
                                    const resolution::TypedFnSignature* sig) {
  if (!canScopeResolve) return;

  ConvertedSymbolsMap* m = nullptr;
  ConvertedSymbolsMap* cur = &gConvertedSyms;
  if (symStack.size() > 0) {
    cur = symStack.back().convertedSyms.get();
    m = cur->findMapContaining(sig->untyped()->id());
  }

  if (m == nullptr) {
    m = &gConvertedSyms;
  }

  m->noteCallFixupNeeded(se, sig, cur, trace);
}

static std::string computeMapName(ID inSymbolId) {
  if (inSymbolId.isEmpty())
    return "global";
  else
    return inSymbolId.str();
}

static std::string astName(const uast::AstNode* ast) {
  if (ast == nullptr) {
    return "null";
  } else if (auto named = ast->toNamedDecl()) {
    return named->name().str();
  } else {
    return "unknown";
  }
}

void Converter::noteAllContainedFixups(BaseAST* ast, int depth) {
  // Traverse over 'sym' but don't go in to nested submodules/fns/aggregates
  // since we will have already gathered from those and we don't want
  // this to be quadratic in time.
  //
  // Gather the fixups that need to be done.
  // This is a separate traversal so that the build functions
  // can copy the AST freely.

  if (depth > 0) {
    if (isModuleSymbol(ast)) {
      return;
    }
    if (auto fn = toFnSymbol(ast)) {
      if (!fn->hasFlag(FLAG_COMPILER_NESTED_FUNCTION)) {
        // ignore functions that are created from building expressions
        // but aren't represented in the uAST
        return;
      }
    }
    if (TypeSymbol* ts = toTypeSymbol(ast)) {
      if (isAggregateType(ts->type)) {
        return;
      }
    }
  }

  AST_CHILDREN_CALL(ast, noteAllContainedFixups, depth+1);

  if (SymExpr* se = toSymExpr(ast)) {
    if (auto tcs = toTemporaryConversionSymbol(se->symbol())) {
      if (tcs->sig != nullptr) {
        noteCallFixupNeeded(se, tcs->sig);
      } else {
        noteIdentFixupNeeded(se, tcs->symId);
      }
    }
  }
}

void Converter::pushToSymStack(
     const uast::AstNode* ast,
     const resolution::ResolutionResultByPostorderID* resolved) {
  ConvertedSymbolsMap* parentMap = nullptr;
  if (symStack.size() > 0) {
    auto backMap = symStack.back().convertedSyms.get();
    auto backAst = parsing::idToAst(context, backMap->inSymbolId);

    // If we're inside a nested function, then we should use the parent
    // function's ConvertedSymbolsMap as the parentMap.
    //
    // If we're inside an aggregate, then we should use that aggregate's
    // map as the parent so that we can find the right fields. This can
    // happen for methods declared inside a class or record.
    if (backAst->toFunction() ||
        backAst->isAggregateDecl()) {
      parentMap = backMap;
    } else {
      // Find the current top-level module from symStack and consider it the
      // parent.
      // We could track things in a more granular way but we might need to
      // access something like A.B.C.D (where A, B, C are modules) later.
      parentMap = symStack.front().convertedSyms.get();
    }
  } else {
    parentMap = &gConvertedSyms;
  }
  if (trace) {
    printf("Entering %s %s with parent %s\n",
           astName(ast).c_str(), ast->id().str().c_str(),
           computeMapName(parentMap->inSymbolId).c_str());
  }
  symStack.emplace_back(ast, resolved, parentMap);
}


void Converter::popFromSymStack(const uast::AstNode* ast, BaseAST* ret) {
  if (ret != nullptr) {
    noteAllContainedFixups(ret, 0);
  }

  if (symStack.size() > 0) {
    CHPL_ASSERT(symStack.back().ast == ast);
    symStack.back().convertedSyms->applyFixups(context, ast, trace);
  } else {
    CHPL_ASSERT(false && "stack error");
  }
  if (trace) {
    int id = 0;
    if (ret != nullptr) id = ret->id;

    printf("Exiting %s %s [%i]\n",
           astName(ast).c_str(), ast->id().str().c_str(), id);
  }
  symStack.pop_back();
}
const resolution::ResolutionResultByPostorderID*
Converter::currentResolutionResult() {
  const resolution::ResolutionResultByPostorderID* r = nullptr;
  if (symStack.size() > 0) {
    r = symStack.back().resolved;
  }
  return r;
}

void ConvertedSymbolsMap::noteConvertedSym(const uast::AstNode* ast,
                                           Symbol* sym,
                                           bool trace) {
  if (trace) {
    printf("Converted sym %s %s to %s[%i] and noting it in %s\n",
           astName(ast).c_str(), ast->id().str().c_str(),
           sym->name, sym->id,
           computeMapName(inSymbolId).c_str());
  }

  syms[ast->id()] = sym;
}

void ConvertedSymbolsMap::noteConvertedFn(
                                const resolution::TypedFnSignature* sig,
                                FnSymbol* fn,
                                bool trace) {
  if (trace) {
    printf("Converted fn %s %s to %s[%i] and noting it in %s\n",
           sig->untyped()->name().c_str(),
           sig->untyped()->id().str().c_str(),
           fn->name, fn->id,
           computeMapName(inSymbolId).c_str());
  }

  fns.emplace(sig, fn);
}

void ConvertedSymbolsMap::noteIdentFixupNeeded(SymExpr* se, ID id,
                                               ConvertedSymbolsMap* cur,
                                               bool trace) {
  if (trace) {
    printf("Noting fixup needed [%i] for mention of %s within %s in map for %s\n",
           se->id,
           id.str().c_str(),
           computeMapName(cur->inSymbolId).c_str(),
           computeMapName(this->inSymbolId).c_str());
  }

  identFixups.emplace_back(se, id);
}

void ConvertedSymbolsMap::noteCallFixupNeeded(SymExpr* se,
                                const resolution::TypedFnSignature* sig,
                                ConvertedSymbolsMap* cur,
                                bool trace) {
  if (trace) {
    printf("Noting fixup needed [%i] for mention of %s within %s in map for %s\n",
           se->id,
           sig->untyped()->id().str().c_str(),
           computeMapName(cur->inSymbolId).c_str(),
           computeMapName(this->inSymbolId).c_str());
  }

  callFixups.emplace_back(se, sig);
}

Symbol* ConvertedSymbolsMap::findConvertedSym(ID id, bool trace) {
  for (ConvertedSymbolsMap* cur = this;
       cur != nullptr;
       cur = cur->parentMap) {
    auto it = cur->syms.find(id);
    if (it != cur->syms.end()) {
      Symbol* ret = it->second;
      // already converted it, so return that
      if (trace) {
        printf("Found sym %s %s in %s\n",
               ret->name, id.str().c_str(), inSymbolId.str().c_str());
      }
      // convert references to classes as anymanaged
      // e.g. 'C' in 'typeFn(C)' refers to anymanaged C rather than borrowed C
      //
      // The call to `getDecoratedClass` used to try and insert a defPoint for
      // the DecoratedClass's TypeSymbol when first creating it. This doesn't
      // work if the AggregateType that represents the class isn't already in
      // the tree.
      //
      // We can run into this situation when scope-resolving an AggregateType
      // that contains a reference to itself. Now, getDecoratedClass will not
      // try to insert a defPoint when the original AggregateType is not in the
      // tree. We will insert these defPoints manually later in
      // `postConvertApplyFixups`.
      if (TypeSymbol* ts = toTypeSymbol(ret)) {
        if (AggregateType* at = toAggregateType(ts->type)) {
          if (at->isClass() && isClassLikeOrManaged(at)) {
            Type* useType =
              at->getDecoratedClass(ClassTypeDecorator::GENERIC_NONNIL);
            ret = useType->symbol;
          }
        }
      }
      return ret;
    }
  }

  if (trace) {
    printf("Could not find sym %s in %s or parents\n",
           id.str().c_str(), inSymbolId.str().c_str());
  }

  return new TemporaryConversionSymbol(id);
}

FnSymbol* ConvertedSymbolsMap::findConvertedFn(
                                  const resolution::TypedFnSignature* sig,
                                  bool trace) {
  for (ConvertedSymbolsMap* cur = this;
       cur != nullptr;
       cur = cur->parentMap) {
    auto it = cur->fns.find(sig);
    if (it != cur->fns.end()) {
      FnSymbol* fn = it->second;
      // already converted it, so return that
      if (trace) {
        printf("Found fn %s %s in %s\n",
               sig->untyped()->name().c_str(),
               sig->untyped()->id().str().c_str(),
               computeMapName(inSymbolId).c_str());
      }
      return fn;
    }
  }

  if (trace) {
    printf("Could not find fn %s in %s or parents\n",
           sig->untyped()->id().str().c_str(),
           computeMapName(inSymbolId).c_str());
  }

  return nullptr;
}

ConvertedSymbolsMap* ConvertedSymbolsMap::findMapContainingBoth(ID id1,
                                                                ID id2) {
  for (ConvertedSymbolsMap* cur = this;
       cur != nullptr && !cur->inSymbolId.isEmpty();
       cur = cur->parentMap) {
    if (cur->inSymbolId.contains(id1) &&
        cur->inSymbolId.contains(id2)) {
      return cur;
    }
  }

  return nullptr;
}

void ConvertedSymbolsMap::applyFixups(chpl::Context* context,
                                      const uast::AstNode* inAst,
                                      bool trace) {

  // Note: we should be able to minimize the fixups needed
  // by converting the modules in the initialization order
  llvm::SmallPtrSet<SymExpr*, 4> fixedUp;

  // Fix up any SymExprs needing to be re-targeted
  for (const auto& p : identFixups) {
    SymExpr* se = p.first;
    ID target = p.second;

    // Already fixed up by following the symExprs linked list on a
    // TemporaryConversionSymbol (see below). Skip here.
    if (fixedUp.count(se) > 0) continue;

    auto tcsymbol = se->symbol();
    INT_ASSERT(isTemporaryConversionSymbol(tcsymbol));

    Symbol* sym = findConvertedSym(target, /* trace */ false);
    if (isTemporaryConversionSymbol(sym)) {
      INT_FATAL("could not find target symbol for sym fixup for %s within %s",
                target.str().c_str(), inSymbolId.str().c_str());
    }

    se->setSymbol(sym);
    // Not all symExprs are noted as fixups (due to lowering and AST
    // transformations), so visit the temporary conversion symbol's recorded
    // symExprs to try handle these stragglers.
    //
    // This is a workaround; ideally, we'd not perform as many AST
    // transformations, and not need to walk all the SymExprs for each
    // tcsymbol.
    for_SymbolSymExprs(se, tcsymbol) {
      se->setSymbol(sym);
      fixedUp.insert(se);
    }
  }
  // clear gIdentFixups since these have now been processed
  identFixups.clear();

  // Fix up any CallExprs that need to have their calledExpr re-targeted
  for (const auto& p : callFixups) {
    SymExpr* se = p.first;
    const resolution::TypedFnSignature* target = p.second;

    INT_ASSERT(isTemporaryConversionSymbol(se->symbol()));

    FnSymbol* fn = findConvertedFn(target, /* trace */ false);
    if (fn == nullptr) {
      INT_FATAL("could not find target function for call fixup %s within %s",
                 target->untyped()->name().c_str(),
                 inSymbolId.str().c_str());
    }
    se->setSymbol(fn);
  }
  // clear gFnCallFixups since these have now been processed
  callFixups.clear();

  // copy the syms and fns maps to the parent map if the symbols
  // within could be visible elsewhere
  if (parentMap && inAst) {
    if (inAst->isModule() || inSymbolId == inAst->id()) {
      if (trace) {
        printf("Copying symbols from %s to parent %s\n",
               computeMapName(inSymbolId).c_str(),
               computeMapName(parentMap->inSymbolId).c_str());
      }
      parentMap->syms.insert(syms.begin(), syms.end());
      parentMap->fns.insert(fns.begin(), fns.end());
    }
  }
}


} // end anonymous namespace

ModuleSymbol*
convertToplevelModule(chpl::Context* context,
                      const chpl::uast::Module* mod,
                      ModTag modTag) {
  astlocMarker markAstLoc(mod->id());
  Converter c(context, modTag);

  c.canScopeResolve = fDynoScopeResolve;
  c.trace = fDynoDebugTrace;

  ModuleSymbol* ret = c.convertModule(mod);
  return ret;
}

void postConvertApplyFixups(chpl::Context* context) {
  gConvertedSyms.applyFixups(context, nullptr, /* trace */ false);

  // Add defPoints that 'getDecoratedClass' was prevented from inserting when
  // the original AggregateType was no longer in the tree.
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (auto dct = toDecoratedClassType(ts->type)) {
      if (isAlive(ts) == false) {
        SET_LINENO(ts);
        auto at = dct->getCanonicalClass();
        DefExpr* defDec = new DefExpr(ts);
        at->symbol->defPoint->insertAfter(defDec);
      }
    }
  }

  // Fix method receivers
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->_this == nullptr) continue; // not a method

    if (fn->_this->type == dtUnknown) {
      Expr* expr = toArgSymbol(fn->_this)->typeExpr->body.only();
      if (SymExpr* receiver = toSymExpr(expr)) {
        if (auto dct = toDecoratedClassType(receiver->symbol()->type)) {
          SET_LINENO(receiver);
          receiver->replace(new SymExpr(dct->getCanonicalClass()->symbol));
        }
      }
    }
  }

  forv_Vec(TemporaryConversionThunk, thunk, gTemporaryConversionThunks) {
    if (thunk->inTree()) {
      SET_LINENO(thunk);
      thunk->replace(thunk->force());
    }
  }

  // Ensure no SymExpr referring to TemporaryConversionSymbol is still in tree
  if (fVerify) {
    forv_Vec(SymExpr, se, gSymExprs) {
      if (isTemporaryConversionSymbol(se->symbol())) {
        INT_ASSERT(!se->inTree());
      }
    }
    forv_Vec(TemporaryConversionThunk, thunk, gTemporaryConversionThunks) {
      INT_ASSERT(!thunk->inTree());
    }
  }

  // clear out the globals to save space
  // (in future, these will be fields in Converter,
  //  and they can be cleared out without any special action if the
  //  Converter is destroyed after it is no longer needed)
  gConvertedSyms.syms.clear();
  gConvertedSyms.fns.clear();
}
