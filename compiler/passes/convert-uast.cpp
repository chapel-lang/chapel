/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
#include "metadata.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/global-strings.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/util/string-escapes.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/util/assertions.h"
#include "stmt.h"

#include "convert-help.h"

#include "llvm/ADT/SmallPtrSet.h"

// If this is set then variables/formals will have their "qual" field set
// now instead of later during resolution.
#define ATTACH_QUALIFIED_TYPES_EARLY false

using namespace chpl;

struct Converter final : UastConverter {
  struct ModStackEntry {
    const uast::Module* mod = nullptr;
    // If we detect a module use and the module is already converted, store it here.
    std::vector<ModuleSymbol*> usedModules;
    // If we detect a module use and the module is not converted, store the ID here.
    std::vector<ID> usedModuleIds;
    bool isFromLibraryFile = false;
    ModStackEntry(const uast::Module* mod, bool isFromLibraryFile)
      : mod(mod), isFromLibraryFile(isFromLibraryFile) {
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
  bool inTupleAssign = false;
  bool inImportOrUse = false;
  bool inForwardingDecl = false;
  bool inTypeExpression = false;
  bool canScopeResolve = false;
  bool trace = false;
  int delegateCounter = 0;

  ModTag topLevelModTag;

  // which modules / submodules to convert
  std::unordered_set<chpl::ID> modulesToConvert;
  std::unordered_set<chpl::ID> symbolsToIgnore;

  // to keep track of symbols that have been converted & fixups needed
  std::unordered_map<ID, ModuleSymbol*> modSyms;
  std::unordered_map<ID, Symbol*> syms;
  std::unordered_map<const resolution::TypedFnSignature*, FnSymbol*> fns;

  std::vector<std::pair<SymExpr*, ID>> identFixups;
  std::vector<std::pair<ModuleSymbol*, ID>> moduleFixups;

  std::vector<ModStackEntry> modStack;
  std::vector<SymStackEntry> symStack;

  /* When working within a method, field accesses need to be code generated
     as using 'this' rather than as SymExprs pointing to a field.
     To enable that, this stack tracks the Symbol* for the 'this' formal
     for a method currently being generated.
     This is different from symStack above because the process of converting
     ForwardingDecls will add a method that does not exist in the uAST. */
  std::vector<Symbol*> methodThisStack;

  /* Some actions in the production scope resolver (particularly using a module)
     cause a search up the chain of scopes until they encounter a Block node.
     Rather than implementing this search, just keep track of a stack of
     scope-producing nodes. To properly keep track of them, code in the
     converter that would normally call `new BlockExpr()`, creating a new
     scope-producing block but not tracking it, would need to instead call
     pushScopefulBlock, making the new block appear on this stack. */
  std::vector<BlockStmt*> blockStack;


  Converter(chpl::Context* context)
    : context(context),
      topLevelModTag(MOD_USER)
  {
    canScopeResolve = fDynoScopeResolve;
    trace = fDynoDebugTrace;
  }

  // supporting UastConverter methods
  void setModulesToConvert(const std::vector<ID>& vec) override {
    modulesToConvert.clear();
    // add them to the set
    for (const ID& id : vec) {
      modulesToConvert.insert(id);
    }
  }

  void setMainModule(ID mainModule) override {
    // no action needed here
  }

  void setFunctionsToConvertWithTypes(const resolution::CalledFnsSet& calledFns) override
  {
    // no action needed here
  }

  void setSymbolsToIgnore(std::unordered_set<chpl::ID> ignore) override {
    symbolsToIgnore.swap(ignore);
  }

  void useModuleWhenConverting(const chpl::ID& modId, ModuleSymbol* modSym) override {
    modSyms[modId] = modSym;
  }

  ModuleSymbol*
  convertToplevelModule(const chpl::uast::Module* mod, ModTag modTag) override;

  void postConvertApplyFixups() override;

  void createMainFunctions() override {
    // no action needed here
  }

  // general functions for converting
  Expr* convertAST(const uast::AstNode* node) override;

  // methods to help track what has been converted
  void noteConvertedSym(const uast::AstNode* ast, Symbol* sym);
  void noteConvertedFn(const resolution::TypedFnSignature* sig, FnSymbol* fn);
  Symbol* findConvertedSym(ID id, bool neverTrace=false);
  void noteIdentFixupNeeded(SymExpr* se, ID id);
  void noteModuleFixupNeeded(ModuleSymbol* m, ID id);
  void noteCallFixupNeeded(SymExpr* se,
                           const resolution::TypedFnSignature* sig);

  void noteAllContainedFixups(BaseAST* ast, int depth);

  // symStack helpers
  void pushToSymStack(
       const uast::AstNode* ast,
       const resolution::ResolutionResultByPostorderID* resolved);
  void popFromSymStack(const uast::AstNode* ast, BaseAST* ret);
  const resolution::ResolutionResultByPostorderID* currentResolutionResult();

  // blockStack helpers
  BlockStmt* pushScopefulBlock() {
    auto newBlockStmt = new BlockStmt();
    blockStack.push_back(newBlockStmt);
    return newBlockStmt;
  }
  void popScopefulBlock() {
    CHPL_ASSERT(blockStack.size() > 0);
    blockStack.pop_back();
  }
  void storeReferencedMod(Symbol* referencedMod) {
    CHPL_ASSERT(blockStack.size() > 0);
    CHPL_ASSERT(modStack.size() > 0);
    if (auto modSym = toModuleSymbol(referencedMod)) {
      blockStack.back()->modRefsAdd(modSym);
      modStack.back().usedModules.push_back(modSym);
    } else if (auto tcs = toTemporaryConversionSymbol(referencedMod)) {
      blockStack.back()->modRefsAdd(tcs);
      modStack.back().usedModuleIds.push_back(tcs->symId);
    } else {
      CHPL_ASSERT(false && "Only module symbols and temporary conversion symbols should be stored in a BlockStmt's modRefs!");
    }
  }

  bool shouldScopeResolve(ID symbolId) {
    if (canScopeResolve) {
      return fDynoScopeBundled || topLevelModTag == MOD_USER;
    }

    return false;
  }
  bool shouldScopeResolve(const uast::AstNode* node) {
    return shouldScopeResolve(node->id());
  }

  bool isFromLibraryFile() {
    return modStack.size() > 0 && modStack.back().isFromLibraryFile;
  }

  Expr* convertExprOrNull(const uast::AstNode* node) {
    if (node == nullptr)
      return nullptr;

    Expr* ret = convertAST(node);
    INT_ASSERT(ret);
    return ret;
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

  Expr* visit(const uast::ErroneousExpression* node) {
    return new CallExpr(PRIM_ERROR);
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
          if (isFieldAccess) {
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
            CHPL_UNIMPL("resolving field access call not yet implemented");
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
    if (inImportOrUse) {
      if (auto remap = reservedWordToInternalName(name)) {
        return remap;
      }
    } else {
      if (auto remap = reservedWordRemapForIdent(name)) {
        return remap;
      }
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
    BlockStmt* ret = pushScopefulBlock();

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

    popScopefulBlock();
    return ret;
  }

  BlockStmt*
  convertImplicitBlock(uast::AstListIteratorPair<uast::AstNode> stmts) {
    BlockStmt* ret = nullptr;

    for (auto stmt: stmts) {
      astlocMarker markAstLoc(stmt->id());
      Expr* e = convertAST(stmt);
      if (!e) continue;
      if (ret) CHPL_UNIMPL("implicit block with multiple statements");
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
    auto managers = pushScopefulBlock();

    for (auto manager : node->managers()) {

      // TODO: This is deleted by the callee, odd convention...
      std::set<Flag>* flags = nullptr;
      Expr* managerExpr = nullptr;
      const uast::Variable* storedResourceVar = nullptr;
      const char* resourceName = nullptr;

      if (auto as = manager->toAs()) {
        managerExpr = convertAST(as->symbol());

        storedResourceVar = as->rename()->toVariable();
        INT_ASSERT(storedResourceVar);
        INT_ASSERT(!storedResourceVar->initExpression() && !storedResourceVar->typeExpression());

        resourceName = astr(storedResourceVar->name());

        // TODO: I'm not sure what the best way to get flags is here.
        if (storedResourceVar->kind() != uast::Variable::INDEX) {
          flags = new std::set<Flag>;

          // TODO: Duplication here and with 'attachSymbolStorage',
          // consider cleaning up after parser is replaced.
          switch (storedResourceVar->kind()) {
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

      Symbol* storedResource;
      auto conv = buildManagerBlock(managerExpr, flags, resourceName, storedResource);
      if (storedResourceVar) {
        // If we had an 'as <whatever>', the production builder better have
        // created a Symbol* for it.
        CHPL_ASSERT(storedResource);
        noteConvertedSym(storedResourceVar, storedResource);
      }
      INT_ASSERT(conv);

      managers->insertAtTail(conv);
    }
    popScopefulBlock(); // No longer in the "managers" block

    auto block = createBlockWithStmts(node->stmts(), node->blockStyle());

    auto ret = buildManageStmt(managers, block, topLevelModTag);
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
    auto block = createBlockWithStmts(node->body()->stmts(), node->blockStyle());

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

  bool isDotOnModule(const uast::Dot* node,
                     ID& outModuleId,
                     ID& outTargetId,
                     types::QualifiedType& outTargetType) {
    outModuleId = ID();
    outTargetId = ID();
    outTargetType = types::QualifiedType();

    auto r = currentResolutionResult();
    if (!r) return false;

    if (auto rr = r->byAstOrNull(node->receiver())) {
      if (rr->type().kind() == types::QualifiedType::MODULE) {
        outModuleId = rr->toId();
      }
    }
    if (outModuleId.isEmpty()) return false;

    if (auto rr = r->byAstOrNull(node)) {
      outTargetId = rr->toId();
      outTargetType = rr->type();
    }

    return true;
  }

  Expr* convertModuleDot(const uast::Dot* node) {
    ID moduleId, targetId;
    types::QualifiedType targetType;
    if (!isDotOnModule(node, moduleId, targetId, targetType) ||
        targetId.isEmpty()) return nullptr;
    if (targetId.isFabricatedId()) {
      CHPL_ASSERT(targetId.fabricatedIdKind() == ID::ExternBlockElement);
      return nullptr;
    }
    storeReferencedMod(findConvertedSym(moduleId));

    // If it's just a variable, turn it into a direct reference to said
    // variable, bypassing a ('.' M x) expression.
    auto convertedSymbol = findConvertedSym(targetId);
    Expr* ret = new SymExpr(convertedSymbol);

    // If it's a parenless function call it.
    if (targetType.kind() == types::QualifiedType::PARENLESS_FUNCTION) {
      ret = new CallExpr(ret);
    }

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
      if (auto ret = convertModuleDot(node)) {
        return ret;
      }
      if (inImportOrUse) {
        // Skip "special" things like .locale handling if we're in
        // something like `import M.locale`, which _should_ be valid for
        // importing tertiary methods.

        return new CallExpr(".", base, new_CStringSymbol(member.c_str()));
      }
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

    auto parentId = parsing::idToParentId(context, node->id());
    auto parentAst = parsing::idToAst(context, parentId);
    bool atModuleScope = parentAst->isModule();

    return buildRequireStmt(actuals, atModuleScope);
  }

  Expr* visit(const uast::Include* node) {
    bool isIncPrivate = node->visibility() == uast::Decl::PRIVATE;

    const uast::Module* umod =
      parsing::getIncludedSubmodule(context, node->id());
    if (umod == nullptr) {
      return nullptr;
    }

    // skip any submodules that do not need to be converted /  are dead
    if (modulesToConvert.count(umod->id()) == 0) {
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

  Expr* visit(const uast::Init* node) {
    // target should always be "this", aka the method receiver.
    auto toInit = convertAST(node->target());
    return new CallExpr(new CallExpr(".", toInit, new_CStringSymbol("chpl__initThisType")));
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
      default: CHPL_UNIMPL("Unhandled new expression"); break;
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

        if(parent->toForeach()) {
          USR_FATAL(node->id(), "reduce intents can not be used in foreach loops");
        }

        Expr* ovar = new UnresolvedSymExpr(rd->name().c_str());
        Expr* riExpr = convertScanReduceOp(rd->op());
        svs = ShadowVarSymbol::buildFromReduceIntent(ovar, riExpr);
      } else {
        CHPL_UNIMPL("Unhandled with clause");
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
        conv = convertVariable(var, useLinkageName).entireExpr;
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
    BlockStmt* whenStmts = pushScopefulBlock();

    for (auto when : node->whenStmts()) {
      whenStmts->insertAtTail(toExpr(convertAST(when)));
    }
    popScopefulBlock();

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
      BlockStmt* catches = pushScopefulBlock();
      bool isSyncTry = false; // TODO: When can this be true?

      for (auto handler : node->handlers()) {
        INT_ASSERT(handler->isCatch());
        auto conv = toExpr(convertAST(handler));
        catches->insertAtTail(conv);
      }
      popScopefulBlock();

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
    return DoWhileStmt::build(condExpr, body,
                              extractLlvmAttributesAndRejectOthers(context, node));
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
    return WhileDoStmt::build(condExpr, body,
                              extractLlvmAttributesAndRejectOthers(context, node));
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

      return convertVariable(var, false).requireDefOnly();

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
      CHPL_UNIMPL("Unhandled Decl");
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
      if (inTypeExpression) return convertArrayType(node);
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

      auto loopAttributes = LoopAttributeInfo::fromExplicitLoop(context, node);
      loopAttributes.applyToLoop(*this, indices, body);
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

  Expr* tryExtractFilterCond(const uast::IndexableLoop* node, Expr*& cond) {
    INT_ASSERT(node->isExpressionLevel());
    INT_ASSERT(node->numStmts() == 1);

    Expr* ret = nullptr;
    // Unpack things differently if body is a conditional.
    if (auto origCond = node->stmt(0)->toConditional()) {
      INT_ASSERT(origCond->numThenStmts() == 1);
      if (!origCond->hasElseBlock()) {
        ret = singleExprFromStmts(origCond->thenStmts());
        cond = toExpr(convertAST(origCond->condition()));
      } else {
        INT_ASSERT(origCond->numElseStmts() == 1);
      }
    }

    if (!ret) {
      ret = singleExprFromStmts(node->stmts());
    }

    INT_ASSERT(ret);
    return ret;
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
      body = tryExtractFilterCond(node, cond);

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

      auto loopAttributes = LoopAttributeInfo::fromExplicitLoop(context, node);
      loopAttributes.applyToLoop(*this, index, block);
      ret = ForLoop::buildForLoop(index, iteratorExpr, block, zippered,
                                  isForExpr, extractLlvmAttributesAndRejectOthers(context, node));
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
    expr = tryExtractFilterCond(node, cond);

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

      auto loopAttributes = LoopAttributeInfo::fromExplicitLoop(context, node);
      loopAttributes.applyToLoop(*this, indices, body);
      return ForallStmt::build(indices, iterator, intents, body, zippered,
                               serialOK);
    }
  }

  Expr* visit(const uast::Foreach* node) {
    Expr* ret = nullptr;

    // The pieces that we need for 'buildForallLoopExpr'.
    Expr* indices = convertLoopIndexDecl(node->index());
    Expr* iteratorExpr = toExpr(convertAST(node->iterand()));
    CallExpr* intents = convertWithClause(node->withClause(), node);
    Expr* cond = nullptr;
    bool zippered = node->iterand()->isZip();
    bool isForExpr = node->isExpressionLevel();
    bool maybeArrayType = false;

    if (node->isExpressionLevel()) {
      auto body = tryExtractFilterCond(node, cond);

      ret = buildForeachLoopExpr(indices, iteratorExpr, body, cond,
                                 maybeArrayType, zippered);
    } else {
      auto body = createBlockWithStmts(node->stmts(), node->blockStyle());
      auto loopAttributes = LoopAttributeInfo::fromExplicitLoop(context, node);
      loopAttributes.applyToLoop(*this, indices, body);
      ret = ForLoop::buildForeachLoop(indices, iteratorExpr, intents, body,
                                      zippered,
                                      isForExpr, std::move(loopAttributes.llvmMetadata));
    }

    return ret;
  }

  /// Array, Domain, Range, Tuple ///

  void convertArrayRow(const uast::ArrayRow* node, CallExpr* actualList) {
    for (auto expr : node->exprs()) {
      if (expr->isArrayRow()) {
        convertArrayRow(expr->toArrayRow(), actualList);
      } else {
        actualList->insertAtTail(convertAST(expr));
      }
    }
  }

  Expr* visit(const uast::Array* node) {
    CallExpr* actualList = new CallExpr(PRIM_ACTUALS_LIST);
    Expr* shapeList = nullptr;
    bool isAssociativeList = false;

    if (!node->isMultiDim()) {
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
            if (isAssociativeList) CHPL_UNIMPL("Invalid associative list");
          }
        }

        if (!hasConvertedThisIter) {
          actualList->insertAtTail(convertAST(expr));
        }
      }

    } else {
      CallExpr* shapeActualList = new CallExpr(PRIM_ACTUALS_LIST);
      shapeActualList->insertAtTail(
        new SymExpr(new_IntSymbol(node->numExprs(), INT_SIZE_64)));
      const uast::AstNode* cur = node->expr(0);
      while (cur->isArrayRow()) {
        auto row = cur->toArrayRow();
        shapeActualList->insertAtTail(
          new SymExpr(new_IntSymbol(row->numExprs(), INT_SIZE_64)));
        cur = row->expr(0);
      }
      shapeList = new CallExpr("_build_tuple", shapeActualList);

      for (auto expr : node->exprs()) {
        convertArrayRow(expr->toArrayRow(), actualList);
      }
    }

    Expr* ret = nullptr;
    if (!node->isMultiDim()) {
      INT_ASSERT(shapeList == nullptr);
      if (isAssociativeList) {
        ret = new CallExpr("chpl__buildAssociativeArrayExpr", actualList);
      } else {
        ret = new CallExpr("chpl__buildArrayExpr", actualList);
      }
    } else {
      INT_ASSERT(shapeList != nullptr);
      ret = new CallExpr("chpl__buildNDArrayExpr", shapeList, actualList);
    }

    INT_ASSERT(ret != nullptr);

    return ret;
  }

  Expr* visit(const uast::ArrayRow* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
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

  Expr* convertCalledKeyword(const uast::AstNode* node,
                             const uast::Call* inCall) {
    astlocMarker markAstLoc(node->id());

    Expr* ret = nullptr;

    // check to see if the call actuals are just a ?
    bool justQuestionMark = false;
    if (inCall->numActuals() == 1) {
      if (auto ident = inCall->actual(0)->toIdentifier()) {
        if (ident->name() == USTR("?")) {
          justQuestionMark = true;
        }
      }
    }

    if (auto ident = node->toIdentifier()) {
      auto name = ident->name();

      if (name == USTR("atomic")) {
        ret = new UnresolvedSymExpr("chpl__atomicType");
      } else if (name == USTR("subdomain")) {
        ret = new CallExpr("chpl__buildSubDomainType");
      } else if (name == USTR("sync")) {
        ret = new UnresolvedSymExpr("_syncvar");
      } else if (name == USTR("index")) {
        ret = new CallExpr("chpl__buildIndexType");
      } else if (name == USTR("domain")) {
        if (justQuestionMark) {
          ret = new UnresolvedSymExpr("_domain");
        } else {
          auto base = "chpl__buildDomainRuntimeType";
          auto dist = new UnresolvedSymExpr("defaultDist");
          ret = new CallExpr(base, dist);
        }
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

  CallExpr* convertModuleDotCall(const uast::FnCall* node) {
    ID moduleId, targetId;
    types::QualifiedType targetType;
    auto dot = node->calledExpression()->toDot();
    if (!dot || !isDotOnModule(dot, moduleId, targetId, targetType)) return nullptr;

    // Don't know how to convert fabricated IDs yet.
    if (targetId.isFabricatedId()) {
      CHPL_ASSERT(targetId.fabricatedIdKind() == ID::ExternBlockElement);
      return nullptr;
    }

    if (!targetId.isEmpty() && targetType.kind() != types::QualifiedType::FUNCTION) {
      // an empty ID indicates that it's an overloaded function or otherwise unknown,
      // and a function type indicates... a function. Otherwise, it's something
      // else (like a class name) and shouldn't be turned into a call in this
      // manner.
      return nullptr;
    }

    auto moduleForCall = findConvertedSym(moduleId);
    storeReferencedMod(moduleForCall);
    return new CallExpr(new UnresolvedSymExpr(astr(dot->field())), gModuleToken, moduleForCall);
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
      Expr* typeExpr = convertCalledKeyword(nodeTypeExpr, node);
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
    } else if (Expr* expr = convertCalledKeyword(calledExpression, node)) {
      ret = isCallExpr(expr) ? toCallExpr(expr) : new CallExpr(expr);
      addArgsTo = ret;
    } else if (CallExpr* callExpr = convertModuleDotCall(node)) {
      ret = callExpr;
      addArgsTo = callExpr;
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
          CHPL_UNIMPL("unexpected form for new expression (no actuals)");
          return nullptr;
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


    MultiDeclState desugaringState;
    if (node->destination()) desugaringState.localeTemp = newTemp("chpl__localeTemp");

    // Field multi-decl desugaring happens later in build.cpp and produces
    // different code; don't do redundant work here.
    bool isField = parsing::idIsField(context, node->id());
    if (!isField) {
      // post-parse checks should rule this out
      CHPL_ASSERT(!node->destination());
    }

    // Iterate in reverse just in case this is a remote variable declaration
    // and we need to mimic the desugaring of multi-decls.
    //
    // We need to mimic this here because remote variables are desugared early.
    for (int i = node->numDeclOrComments() - 1; i >= 0; i--) {
      auto child = node->declOrComment(i);
      auto decl = child->toDecl();
      if (!decl) continue;

      INT_ASSERT(decl->linkage() == node->linkage());

      Expr* conv = nullptr;
      if (auto var = decl->toVariable()) {

        // Do not use the linkage name since multi-decls cannot be renamed.
        const bool useLinkageName = false;
        auto convAll = convertVariable(var, useLinkageName,
                                       isField ? nullptr : &desugaringState);
        conv = convAll.entireExpr;

        DefExpr* defExpr = convAll.variableDef;
        INT_ASSERT(defExpr);
        auto varSym = toVarSymbol(defExpr->sym);
        INT_ASSERT(varSym);

      // Otherwise convert in a generic fashion.
      } else {
        // post-parse checks should rule this out
        INT_ASSERT(!desugaringState.localeTemp);

        // tuple decls between variable decls interrupt multi-decl desugaring:
        //
        //    var x, (y,z) = ..., w: int;
        //
        // the 'x' does not get type 'int'.
        desugaringState.reset();
        conv = convertAST(decl);
      }

      INT_ASSERT(conv);
      ret->insertAtHead(conv);
    }

    if (auto dest = node->destination()) {
      auto destExpr = convertAST(dest);
      ret->insertAtHead(new DefExpr(desugaringState.localeTemp, destExpr));
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
        conv = convertVariable(var, useLinkageName).entireExpr;

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
    attachSymbolStorage(node->intentOrKind(), tmpDef->sym, ATTACH_QUALIFIED_TYPES_EARLY);

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

    // We never pushed the forwarding function onto the symbol stack, so
    // we don't need to exit. Exiting normally notes fixups, though, so
    // do that now.
    noteAllContainedFixups(fn, 0);

    return ret;
  }

  /// NamedDecls ///

  Expr* visit(const uast::EnumElement* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
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
      CHPL_UNIMPL("Unhandled lifetime clause");
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

  FnSymbol* convertFunction(const uast::Function* node) {
    // Decide if we want to resolve this function
    bool shouldScopeResolveFunction = shouldScopeResolve(node);

    const resolution::ResolutionResultByPostorderID* resolved = nullptr;
    const resolution::ResolvedFunction* resolvedFn = nullptr;

    if (shouldScopeResolveFunction) {
      resolvedFn =
        resolution::scopeResolveFunction(context, node->id());
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

    attachSymbolAttributes(context, node, fn, isFromLibraryFile());
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
          CHPL_UNIMPL("Unhandled formal");
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

    if (fIdBasedMunging && node->linkage() == uast::Decl::DEFAULT_LINKAGE &&
        // ignore things like chpl_taskAddCoStmt
        !fn->hasFlag(FLAG_ALWAYS_RESOLVE)) {
      CHPL_ASSERT(node->id().postOrderId() == -1);
      fn->cname = astr(node->id().symbolPath());
    }

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

    Expr* retType = convertTypeExpressionOrNull(node->returnType());
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
          CHPL_UNIMPL("Unhandled formal in function signature");
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
    Expr* retType = convertTypeExpressionOrNull(nodeRetType);

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
    // don't convert functions we were asked to ignore
    if (symbolsToIgnore.count(node->id()) != 0) return nullptr;

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
    bool shouldScopeResolveModule = shouldScopeResolve(node);

    const resolution::ResolutionResultByPostorderID* resolved = nullptr;

    if (shouldScopeResolveModule) {
      // Resolve the module
      const auto& tmp = resolution::scopeResolveModule(context, node->id());
      resolved = &tmp;
    }

    // Push the current module name before descending into children.
    // Add a ModStackEntry to the end of the modStack
    UniqueString unused;
    bool isFromLibraryFile = context->moduleIsInLibrary(node->id(), unused);
    this->modStack.push_back(ModStackEntry(node, isFromLibraryFile));

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

    const ModTag tag = this->topLevelModTag;
    bool priv = (node->visibility() == uast::Decl::PRIVATE);
    bool prototype = (node->kind() == uast::Module::PROTOTYPE ||
                      node->kind() == uast::Module::IMPLICIT);
    auto style = uast::BlockStyle::EXPLICIT;

    currentModuleName = name;
    auto body = createBlockWithStmts(node->stmts(), style);


    ModuleSymbol* mod = nullptr;
    auto it = modSyms.find(node->id());
    if (it != modSyms.end()) {
      mod = it->second;
      // append the newly converted statements to the module's block
      for_alist(expr, body->body) {
        mod->block->insertAtTail(expr->remove());
      }
    } else {
      mod = buildModule(name, tag, body, path, priv, prototype);

      if (node->kind() == uast::Module::IMPLICIT) {
        mod->addFlag(FLAG_IMPLICIT_MODULE);
      }

      attachSymbolAttributes(context, node, mod, isFromLibraryFile);
    }

    // Note the module is converted so we can wire up SymExprs later
    noteConvertedSym(node, mod);

    // Pop the module after converting children, and note the modules used
    // within.
    INT_ASSERT(modStack.size() > 0 && modStack.back().mod == node);
    for (auto usedMod : modStack.back().usedModules) {
      mod->moduleUseAdd(usedMod);
    }
    for (auto modId : modStack.back().usedModuleIds) {
      noteModuleFixupNeeded(mod, modId);
    }
    this->modStack.pop_back();
    popFromSymStack(node, mod);

    return mod;
  }
  DefExpr* visit(const uast::Module* node) {
    // skip any submodules that are dead
    if (modulesToConvert.count(node->id()) == 0) {
      return nullptr;
    }

    ModuleSymbol* mod = convertModule(node);
    return new DefExpr(mod);
  }

  /// VarLikeDecls ///

  Expr* convertTypeExpression(const uast::AstNode* node) {
    INT_ASSERT(node != nullptr);

    astlocMarker markAstLoc(node->id());

    bool oldInTypeExpression = inTypeExpression;
    inTypeExpression = true;
    Expr* ret = convertAST(node);
    inTypeExpression = oldInTypeExpression;

    INT_ASSERT(ret);

    return ret;
  }

  Expr* convertTypeExpressionOrNull(const uast::AstNode* node) {
    if (!node) return nullptr;
    return convertTypeExpression(node);
  }

  DefExpr* visit(const uast::Formal* node) {
    IntentTag intentTag = convertFormalIntent(node->intent());

    astlocMarker markAstLoc(node->id());

    Expr* typeExpr = convertTypeExpressionOrNull(node->typeExpression());
    Expr* initExpr = convertExprOrNull(node->initExpression());

    auto ret =  buildArgDefExpr(intentTag, node->name().c_str(),
                                typeExpr,
                                initExpr,
                                /*varargsVariable*/ nullptr);
    INT_ASSERT(ret->sym);

    attachSymbolAttributes(context, node, ret->sym, isFromLibraryFile());

    // noteConvertedSym should be called when handling the enclosing Function

    return ret;
  }

  Expr* visit(const uast::TaskVar* node) {
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  Expr* visit(const uast::VarArgFormal* node) {
    IntentTag intentTag = convertFormalIntent(node->intent());

    Expr* typeExpr = convertTypeExpressionOrNull(node->typeExpression());
    Expr* initExpr = nullptr;

    INT_ASSERT(!node->initExpression());

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

    attachSymbolAttributes(context, node, ret->sym, isFromLibraryFile());

    return ret;
  }

  ShadowVarSymbol* convertTaskVar(const uast::TaskVar* node) {
    astlocMarker markAstLoc(node->id());

    ShadowVarPrefix prefix = convertTaskVarIntent(node);
    // TODO: can we avoid this UnresolvedSymExpr ?
    Expr* nameExp = new UnresolvedSymExpr(node->name().c_str());
    Expr* type = convertTypeExpressionOrNull(node->typeExpression());
    Expr* init = convertExprOrNull(node->initExpression());

    auto ret = ShadowVarSymbol::buildForPrefix(prefix, nameExp, type, init);

    INT_ASSERT(ret != nullptr);

    attachSymbolAttributes(context, node, ret, isFromLibraryFile());

    return ret;
  }

  CallExpr* convertArrayType(const uast::BracketLoop* node) {
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

  // When converting variables etc. with @assertOnGpu or @blockSize,
  // we don't just create a DefExpr; we also create an enclosing block which
  // contains calls to primitives that implement @assertOnGpu and @blockSize.
  //
  // This data structure contains pointers to both.
  struct VariableDefInfo {
    DefExpr* variableDef;
    Expr* entireExpr;

    /**
      Helper for code that calls 'convertVariable' but doesn't expect to handle
      blocks with additional primitives, which can be introduced by that call
      for GPU attributes that need to be propagated to init expressions.
     */
    Expr* requireDefOnly() const {
      CHPL_ASSERT(entireExpr == variableDef);
      return variableDef;
    }
  };

  // State required to mimic the desugaring of multi-declarations into
  // regular ones.
  struct MultiDeclState {
    // The result of computing the target locale, stored in a temp, used
    // for remote variables.
    Symbol* localeTemp = nullptr;

    Symbol* typeTemp = nullptr;
    Symbol* prev = nullptr;
    Expr* prevTypeExpr = nullptr;
    Expr* prevInitExpr = nullptr;

    void reset() {
      typeTemp = nullptr;
      prev = nullptr;
      prevTypeExpr = nullptr;
      prevInitExpr = nullptr;
    }

    // For remote variables, these helpers don't modify the def point, since
    // def point is quite different (it's an invocation of a remote variable
    // wrapper builder with extra arguments).

    void replaceTypeExpr(Expr* newExpr) {
      if (localeTemp) {
        prevTypeExpr->replace(newExpr);
      } else {
        prev->defPoint->exprType = newExpr;
      }
      prevTypeExpr = nullptr;
    }

    void replaceInitExpr(Expr* newExpr) {
      if (localeTemp) {
        prevInitExpr->replace(newExpr);
      } else {
        prev->defPoint->init = newExpr;
      }
    }
  };

  // Returns a DefExpr that has not yet been inserted into the tree.
  // For children of remote multi-decls, parentDestination is the destination
  // of the outer multiDecl.
  VariableDefInfo convertVariable(const uast::Variable* node,
                           bool useLinkageName,
                           MultiDeclState* multiState = nullptr) {
    astlocMarker markAstLoc(node->id());

    bool isStatic = false;
    Expr* staticSharingKind = nullptr;
    if (auto ag = node->attributeGroup()) {
      if (auto attr = ag->getAttributeNamed(USTR("functionStatic"))) {
        if (!node->initExpression()) {
          USR_FATAL(node->id(), "function-static variables must have an initializer.");
        }
        // post-parse checks rule this out.
        CHPL_ASSERT(!node->destination());
        isStatic = true;

        if (attr->numActuals() > 0) {
          staticSharingKind = convertAST(attr->actual(0));
        }
      }
    }

    bool isRemote = node->destination() != nullptr ||
                    (multiState != nullptr && multiState->localeTemp != nullptr);
    auto block = (isRemote || multiState) ? new BlockStmt(BLOCK_SCOPELESS) : nullptr;

    auto varSym = new VarSymbol(sanitizeVarName(node->name().c_str(), inTupleDecl));
    const bool isTypeVar = node->kind() == uast::Variable::TYPE;

    if (fIdBasedMunging && node->linkage() == uast::Decl::DEFAULT_LINKAGE) {
      // is it a module-scope variable?
      bool moduleScopeVar = false;
      const uast::Module* mod = nullptr;
      if (symStack.size() > 0 && modStack.size() > 0) {
        const uast::AstNode* sym = symStack.back().ast;
        mod = modStack.back().mod;
        if (mod == sym) {
          // it's not in a function/type/etc.
          // is it within a block or within the module directly?
          moduleScopeVar = true;
          // TODO: make this a parsing query
          for (auto ast = parsing::parentAst(context, node);
               ast != nullptr && ast != mod;
               ast = parsing::parentAst(context, ast)) {
            if (ast->isTupleDecl() || ast->isMultiDecl()) {
              // these are OK and still declare a top-level variable
            } else {
              moduleScopeVar = false;
            }
          }
        }
      }
      // adjust the cname for module-scope variables
      if (moduleScopeVar && mod) {
        varSym->cname = astr(mod->id().symbolPath().c_str(),
                             ".",
                             varSym->name);
      }
    }

    uast::Variable::Kind symbolKind = node->kind();
    if (isRemote) {
      // Remote variables get desugared early (now!), but they get turned
      // into references to remote memory. So, we need the symbol storage
      // kind to be REF, not VAR.
      switch(symbolKind) {
        case uast::Variable::VAR:
          symbolKind = uast::Variable::REF;
          break;
        case uast::Variable::CONST:
          symbolKind = uast::Variable::CONST_REF;
          break;
        default:
          // post-parse checks rule this out.
          CHPL_ASSERT(false && "unsupported remote variable kind");
      }
    }

    // Adjust the variable according to its kind, e.g. 'const'/'type'.
    attachSymbolStorage(symbolKind, varSym, ATTACH_QUALIFIED_TYPES_EARLY);

    attachSymbolAttributes(context, node, varSym, isFromLibraryFile());

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

    Expr* destinationExpr = convertExprOrNull(node->destination());
    Expr* typeExpr = convertTypeExpressionOrNull(node->typeExpression());
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

      if (isStatic) {
        auto initExprCall = new CallExpr(PRIM_STATIC_FUNCTION_VAR, initExpr);
        initExpr = initExprCall;

        if (staticSharingKind) {
          initExprCall->insertAtTail(staticSharingKind);
        }
      }
    } else {
      initExpr = convertExprOrNull(node->initExpression());
    }

    if ((!typeExpr && !initExpr) && multiState) {
      // Need to draw type expr and init expr from previous variable.
      CHPL_ASSERT(block);

      if (!multiState->typeTemp && multiState->prevTypeExpr) {
        auto newTypeTemp = newTemp("type_tmp");
        newTypeTemp->addFlag(FLAG_TYPE_VARIABLE);
        multiState->typeTemp = newTypeTemp;

        auto prevTypeExpr = multiState->prevTypeExpr;
        multiState->replaceTypeExpr(new SymExpr(newTypeTemp));

        // Create the 'def point' (constructor adds it to newTypeTemp->defPoint)
        std::ignore = new DefExpr(newTypeTemp, prevTypeExpr);
      }
      if (auto typeTemp = multiState->typeTemp) {
        // Once we create the type temp we clear the type expressions so that
        // there's no risk of copying it.
        CHPL_ASSERT(multiState->prevTypeExpr == nullptr);

        // If the type symbol was defined by a previously-visited variable,
        // and since we visit in reverse, its current definition will be
        // after us, which is not good. Move it up to before this symbol.
        auto typeDefPoint = typeTemp->defPoint;
        if (typeDefPoint->list) typeDefPoint->remove();
        block->insertAtHead(typeDefPoint);

        typeExpr = new SymExpr(typeTemp);
      }
      if (auto prevInitExpr = multiState->prevInitExpr) {
        Expr* replaceWith;
        if (prevInitExpr->isNoInitExpr()) {
          replaceWith = prevInitExpr->copy();
        } else if (typeExpr) {
          replaceWith = new CallExpr("chpl__readXX", new SymExpr(varSym));
        } else {
          replaceWith = new SymExpr(varSym);
        }
        multiState->replaceInitExpr(replaceWith);
        initExpr = prevInitExpr;
      }

      multiState->prev = varSym;
    } else if (multiState) {
      CHPL_ASSERT(block);
      multiState->prevTypeExpr = typeExpr;
      multiState->prevInitExpr = initExpr;
      multiState->typeTemp = nullptr;
      multiState->prev = varSym;
    }

    DefExpr* def = nullptr;
    VariableDefInfo ret;
    if (isRemote) {
      CHPL_ASSERT(block);
      auto wrapper = new VarSymbol(astr("chpl_wrapper_", varSym->name));
      auto wrapperCall = new CallExpr("chpl__buildRemoteWrapper");
      wrapperCall->insertAtTail(destinationExpr ? destinationExpr : new SymExpr(multiState->localeTemp));

      if (typeExpr) wrapperCall->insertAtTail(typeExpr);
      if (initExpr) wrapperCall->insertAtTail(new CallExpr(PRIM_CREATE_THUNK, initExpr));
      auto wrapperDef = new DefExpr(wrapper, wrapperCall);

      auto wrapperGet = new CallExpr(".", new SymExpr(wrapper), new_CStringSymbol("get"));
      def = new DefExpr(varSym, new CallExpr(wrapperGet));
      varSym->addFlag(FLAG_REMOTE_VARIABLE);

      block->insertAtTail(wrapperDef);
      block->insertAtTail(def);
      ret = VariableDefInfo { def, block };
    } else {
      def = new DefExpr(varSym, initExpr, typeExpr);
      Expr* entireExpr = def;
      if (block) {
        entireExpr = block;
        block->insertAtTail(def);
      }

      ret = VariableDefInfo { def, entireExpr };
    }

    auto loopFlags = LoopAttributeInfo::fromVariableDeclaration(context, node);
    if (!loopFlags.empty()) {
      auto block = new BlockStmt(BLOCK_SCOPELESS);
      block->insertAtTail(new CallExpr(PRIM_GPU_ATTRIBUTE_BLOCK));
      block->insertAtTail(ret.entireExpr);

      if (auto primBlock = loopFlags.createPrimitivesBlock(*this)) {
        block->insertAtTail(primBlock);
      }

      ret.entireExpr = block;
    }

    // If the init expression of this variable is a domain and this
    // variable is not const, propagate that information by setting
    // 'definedConst' in the domain to false.
    setDefinedConstForDefExprIfApplicable(def, &def->sym->flags);

    // Note the variable is converted so we can wire up SymExprs later
    noteConvertedSym(node, varSym);

    return ret;
  }

  Expr* visit(const uast::Variable* node) {
    auto isTypeVar = node->kind() == uast::Variable::TYPE;
    auto stmts = new BlockStmt(BLOCK_SCOPELESS);

    auto info = convertVariable(node, true);
    INT_ASSERT(info.entireExpr && info.variableDef);
    auto varSym = toVarSymbol(info.variableDef->sym);
    INT_ASSERT(varSym);

    stmts->insertAtTail(info.entireExpr);

    // Special handling for extern type variables.
    if (isTypeVar) {
      if (node->linkage() == uast::Decl::EXTERN) {
        INT_ASSERT(!node->isConfig());
        INT_ASSERT(info.variableDef->sym && isVarSymbol(info.variableDef->sym));
        auto varSym = toVarSymbol(info.variableDef->sym);
        auto linkageName = node->linkageName() ? varSym->cname : nullptr;
        stmts = convertTypesToExtern(stmts, linkageName);

        // fix up convertedSyms since convertTypesToExtern
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
    attachSymbolAttributes(context, node, ret->sym, isFromLibraryFile());

    // Note the enum element is converted so we can wire up SymExprs later
    noteConvertedSym(node, ret->sym);

    return ret;
  }

  Expr* visit(const uast::Enum* node) {
    const resolution::ResolutionResultByPostorderID* resolved = nullptr;
    if (shouldScopeResolve(node)) {
      resolved = &resolution::scopeResolveEnum(context, node->id());
    }
    pushToSymStack(node, resolved);

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

    attachSymbolAttributes(context, node, enumTypeSym, isFromLibraryFile());
    attachSymbolVisibility(node, enumTypeSym);

    enumType->symbol = enumTypeSym;

    auto ret = new BlockStmt(BLOCK_SCOPELESS);
    ret->insertAtTail(new DefExpr(enumTypeSym));

    // Note the enum type is converted so we can wire up SymExprs later
    noteConvertedSym(node, enumTypeSym);

    popFromSymStack(node, enumTypeSym);

    return ret;
  }

  /// AggregateDecls

  std::string inheritanceExprToStringForErr(const uast::AstNode* identOrDot) {
    if (auto ident = identOrDot->toIdentifier()) {
      return ident->name().c_str();
    } else {
      auto dot = identOrDot->toDot();
      CHPL_ASSERT(dot);
      auto receiverStr = inheritanceExprToStringForErr(dot->receiver());
      return receiverStr + "." + dot->field().c_str();
    }
  }

  template <typename Iterable>
  void convertInheritsExprs(const Iterable& iterable,
                            std::vector<Expr*>& inherits,
                            bool& inheritMarkedGeneric) {
    for (auto inheritExpr : iterable) {
      bool thisInheritMarkedGeneric = false;
      auto* ident =
        uast::Class::getUnwrappedInheritExpr(inheritExpr, thisInheritMarkedGeneric);

      // Always convert the target expression so that we note used modules
      // as needed. We won't necessarily use the resulting expression;
      // see the comment below.
      auto converted = convertExprOrNull(ident);
      inheritMarkedGeneric |= thisInheritMarkedGeneric;

      auto results = currentResolutionResult();
      if (!(results != nullptr || ident->isIdentifier())) {
        USR_FATAL_CONT(inheritExpr->id(),
                       "only simple inheritance expressions are supported "
                       "without Dyno scope resolution");
      }

      // If scope resolution is enabled, then we should already know what
      // the inherit expression is referring to. Use that information,
      // and error if it's not there, since the Dyno scope resolver should
      // be fully online.
      if (results) {
        if (auto result = results->byAstOrNull(ident)) {
          auto toId = result->toId();
          if (!toId.isEmpty()) {
            auto converted = findConvertedSym(toId);
            CHPL_ASSERT(converted &&
                        "non-null 'results' implies scope resolution is enabled");
            inherits.push_back(new SymExpr(converted));
            continue;
          }
        }

        // Couldn't find the target, emit an error message
        USR_FATAL_CONT(inheritExpr->id(),
                       "could not find parent class or target interface '%s' "
                       "for inheritance expression",
                       inheritanceExprToStringForErr(ident).c_str());
      }

      // Couldn't find the target, so translate it literally and hand it off
      // to the production scope resolver.
      //
      // This can happen if:
      //   * we simply didn't perform scope resolution: results == nullptr.
      //     In this case, we may have issued an error message (if the
      //     inheritance expression is not simple, which production doesn't
      //     support). If the identifier is simple, though, we can get by
      //     without Dyno's scope resolution info.
      //   * we did perform scope resolution, but didn't get information
      //     for this inheritance expression. In that case, we issued an error
      //     message ("could not find parent class").
      if (converted) {
        inherits.push_back(converted);
      }
    }
  }

  Expr* convertAggregateDecl(const uast::AggregateDecl* node) {

    const resolution::ResolutionResultByPostorderID* resolved = nullptr;
    if (shouldScopeResolve(node)) {
      resolved = &resolution::scopeResolveAggregate(context, node->id());
    }
    pushToSymStack(node, resolved);

    const char* name = astr(node->name());
    const char* cname = name;
    bool inheritMarkedGeneric = false;

    std::vector<Expr*> inherits;
    if (auto ad = node->toAggregateDecl()) {
      convertInheritsExprs(ad->inheritExprs(), inherits, inheritMarkedGeneric);
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

    auto ret = buildClassDefExpr(name, cname, tag,
                                 inherits,
                                 decls,
                                 externFlag,
                                 topLevelModTag);
    INT_ASSERT(ret->sym);

    attachSymbolAttributes(context, node, ret->sym, isFromLibraryFile());
    attachSymbolVisibility(node, ret->sym);
    if (inheritMarkedGeneric) {
      ret->sym->addFlag(FLAG_SUPERCLASS_MARKED_GENERIC);
    }

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

static std::string astName(const uast::AstNode* ast) {
  if (ast == nullptr) {
    return "null";
  } else if (auto named = ast->toNamedDecl()) {
    return named->name().str();
  } else {
    return "unknown";
  }
}

void Converter::noteConvertedSym(const uast::AstNode* ast, Symbol* sym) {
  if (!canScopeResolve) return;

  if (trace) {
    printf("Converted sym %s %s to %s[%i]\n",
           astName(ast).c_str(), ast->id().str().c_str(),
           sym->name, sym->id);
  }

  syms[ast->id()] = sym;
}

void Converter::noteConvertedFn(const resolution::TypedFnSignature* sig,
                                FnSymbol* fn) {
  if (!canScopeResolve) return;

  if (trace) {
    printf("Converted fn %s %s to %s[%i]\n",
           sig->untyped()->name().c_str(),
           sig->untyped()->id().str().c_str(),
           fn->name, fn->id);
  }

  fns.emplace(sig, fn);
}

Symbol* Converter::findConvertedSym(ID id, bool neverTrace) {
  if (!canScopeResolve) return nullptr;

  bool doTrace = trace && !neverTrace;

  auto it = syms.find(id);
  if (it != syms.end()) {
    Symbol* ret = it->second;
    // already converted it, so return that
    if (doTrace) {
      printf("Found sym %s %s\n", ret->name, id.str().c_str());
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

  if (doTrace) {
    printf("Could not find sym %s\n", id.str().c_str());
  }

  return new TemporaryConversionSymbol(id);
}

void Converter::noteIdentFixupNeeded(SymExpr* se, ID id) {
  if (!canScopeResolve) return;

  if (trace) {
    printf("Noting fixup needed [%i] for mention of %s\n",
           se->id,
           id.str().c_str());
  }

  identFixups.emplace_back(se, id);
}

void Converter::noteModuleFixupNeeded(ModuleSymbol* mod, ID id) {
  if (!canScopeResolve) return;

  if (trace) {
    printf("Noting fixup needed [%i] for mention of %s\n",
           mod->id, id.str().c_str());
  }

  moduleFixups.emplace_back(mod, id);
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
      // stop if we get to a nested module
      return;
    }
    if (auto fn = toFnSymbol(ast)) {
      if (!fn->hasFlag(FLAG_COMPILER_NESTED_FUNCTION)) {
        // stop if we get to a function that isn't compiler-generated
        return;
      }
    }
    if (TypeSymbol* ts = toTypeSymbol(ast)) {
      if (isAggregateType(ts->type)) {
        // stop if we get to a nested class/record/union
        return;
      }
    }
  }

  AST_CHILDREN_CALL(ast, noteAllContainedFixups, depth+1);

  if (SymExpr* se = toSymExpr(ast)) {
    if (auto tcs = toTemporaryConversionSymbol(se->symbol())) {
      if (!tcs->symId.isEmpty()) {
        noteIdentFixupNeeded(se, tcs->symId);
      }
    }
  }
}

void Converter::pushToSymStack(
     const uast::AstNode* ast,
     const resolution::ResolutionResultByPostorderID* resolved) {
  if (trace) {
    printf("Entering %s %s\n", astName(ast).c_str(), ast->id().str().c_str());
  }
  symStack.emplace_back(ast, resolved);
}


void Converter::popFromSymStack(const uast::AstNode* ast, BaseAST* ret) {
  if (ret != nullptr) {
    noteAllContainedFixups(ret, 0);
  }

  if (symStack.size() > 0) {
    CHPL_ASSERT(symStack.back().ast == ast);
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

ModuleSymbol*
Converter::convertToplevelModule(const chpl::uast::Module* mod,
                                 ModTag modTag) {
  astlocMarker markAstLoc(mod->id());

  topLevelModTag = modTag;
  ModuleSymbol* ret = convertModule(mod);
  return ret;
}

void Converter::postConvertApplyFixups() {
  // apply fixups that we have tracked

  SET_LINENO(rootModule); // avoid "no line number available" masking other errs

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

    Symbol* sym = findConvertedSym(target, /* neverTrace */ true);
    if (isTemporaryConversionSymbol(sym)) {
      INT_FATAL(se, "could not find target symbol for sym fixup for %s",
                target.str().c_str());
    }

    se->setSymbol(sym);
    fixedUp.insert(se);

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
  identFixups.clear();

  for (const auto& p : moduleFixups) {
    ModuleSymbol* m = p.first;
    const ID& target = p.second;

    Symbol* sym = findConvertedSym(target, /* neverTrace */ true);
    auto usedM = toModuleSymbol(sym);
    if (!usedM) {
      INT_FATAL(m, "could not find target symbol for module fixup for %s",
                target.str().c_str());
    }

    m->moduleUseAdd(usedM);
  }
  moduleFixups.clear();

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
  // (could be removed & wait for the Converter to be destroyed)
  syms.clear();
  fns.clear();
}


// keep the linker happy
UastConverter::~UastConverter() { }

owned<UastConverter> createUntypedConverter(chpl::Context* context) {
  return toOwned(new Converter(context));
}

