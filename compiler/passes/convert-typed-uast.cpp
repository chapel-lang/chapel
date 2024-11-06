/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

/* This file supports conversion of uAST+types to the older AST used by
   the rest of the compiler. It uses recursive functions to perform
   the conversion.

   This pass is currently experimental. While it is optional, it should
   generate AST similar to what the production compiler emits after
   the callDestructors pass.
 */

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
#include "ResolveScope.h"
#include "TemporaryConversionThunk.h"
#include "TryStmt.h"
#include "WhileDoStmt.h"
#include "build.h"
#include "config.h"
#include "global-ast-vecs.h"
#include "metadata.h"
#include "optimizations.h"
#include "parser.h"
#include "resolution.h"
#include "stmt.h"
#include "view.h"

#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/global-strings.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/util/assertions.h"
#include "chpl/util/string-escapes.h"

#include "convert-help.h"

#include "llvm/ADT/SmallPtrSet.h"

#include <iostream>

// If defined, include debug output about TConverter
#define DEBUG_TRACE 1

using namespace chpl;
using namespace resolution;
using namespace uast;

// converts resolved uAST + types to typed AST
struct TConverter final : UastConverter {

  /// Nested Types ///
  using RV = ResolvedVisitor<TConverter>;

  // State required to mimic the desugaring of multi-declarations into
  // regular ones.
  // TODO: this is a copy from the untyped converter and needs to be
  // modified when multi-decls are supported in the typed converter.
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


  /// Fields ///
  Context* context = nullptr;

  // What is the module / function we are converting?
  const AstNode* symbol = nullptr; // Module* or Function*
  FnSymbol* curFnSymbol = nullptr; // corresponding to the above
  ModuleSymbol* curModuleSymbol = nullptr; // corresponding to the above
  Symbol* curRetVar = nullptr; // the return value variable (RVV)
                               // within the current function
  LabelSymbol* epilogueLabel = nullptr;

  // When converting an expression or a statement, where should we
  // put new nodes?
  AList* curAList = nullptr;

#ifdef DEBUG_TRACE
  static const bool trace = true;
#else
  static const bool trace = false;
#endif

  bool haveSetupModules = false;
  bool haveConvertedFunctions = false;
  // these are updated as we are converting different things
  ModTag topLevelModTag = MOD_USER;
  bool moduleFromLibraryFile = false;
  const ResolvedFunction* currentResolvedFunction = nullptr;

  // the untyped converter (temporarily, this is used for non-user code)
  owned<UastConverter> untypedConverter;

  // which modules / submodules to convert
  std::vector<ID> modulesToConvertVec;
  std::unordered_set<ID> modulesToConvert;

  // which functions to convert with types
  CalledFnsSet functionsToConvertWithTypes;

  // the main module
  ID mainModuleId;

  // keeps track of which block/formals list/actuals list we are currently
  // in the process of creating
  //
  // the Expr* pointer is used to set the parentExpr for anything added
  // to the AList.
  std::vector<std::pair<AList*, Expr*>> aListStack;
  // this block is where we store any result created at the top-level
  BlockStmt* scratchSpaceBlock = nullptr;

  // to keep track of symbols that have been converted & fixups needed

  std::unordered_map<ID, Symbol*> globalSyms; // fns, modules, mod-scope vars
  std::unordered_map<ID, Symbol*> localSyms; // function-local variables

  // this one handles functions in a way that allows for instantiation
  std::unordered_map<const ResolvedFunction*, FnSymbol*> fns;
  std::unordered_map<const ResolvedFunction*, FnSymbol*> newWrappers;

  // fixup lists
  std::vector<std::pair<SymExpr*, ID>> identFixups;
  std::vector<std::pair<SymExpr*, const ResolvedFunction*>> callFixups;

  // stores type fixups that are needed
  std::vector<Symbol*> typeFixups;

  // stores a mapping from chpl::Type* to Type*
  std::unordered_map<const types::Type*, Type*> convertedTypes;

  // so that TConverter can process one module (or function) at a time,
  // encountering a submodule, if it is to be converted, add it to this
  // list for processing just after the current module.
  std::vector<const uast::Module*> submodulesEncountered;

  /// Methods ///

  TConverter(Context* context) : context(context) {
    SET_LINENO(rootModule);
    scratchSpaceBlock = new BlockStmt();
    curAList = &scratchSpaceBlock->body;

    untypedConverter = createUntypedConverter(context);
  }

  ~TConverter();

  // supporting UastConverter methods
  void setModulesToConvert(const std::vector<ID>& vec) override {
    if (trace)
      printf("in setModulesToConvert with %i modules\n", (int)vec.size());

    modulesToConvert.clear();
    // save the vector
    modulesToConvertVec = vec;
    // add the modules to the set
    for (const ID& id : modulesToConvertVec) {
      if (trace)
        printf("Will convert module %s\n", id.str().c_str());

      modulesToConvert.insert(id);
    }

    // also tell the untyped converter about it
    untypedConverter->setModulesToConvert(vec);
  }

  void setFunctionsToConvertWithTypes(const CalledFnsSet& calledFns) override
  {
    if (trace)
      printf("in setFunctionsToConvertWithTypes with %i functions\n",
             (int)calledFns.size());

    functionsToConvertWithTypes = calledFns;
  }

  void setSymbolsToIgnore(std::unordered_set<ID> ignore) override {
    INT_FATAL("TConverter::setSymbolsToIgnore not implemented");
  }

  void setMainModule(ID mainModule) override {
    if (trace)
      printf("in setMainModule with %s\n", mainModule.str().c_str());

    mainModuleId = mainModule;
  }

  void useModuleWhenConverting(const chpl::ID& modId, ModuleSymbol* modSym) override {
    INT_FATAL("useModuleWhenConverting not expected for TConverter");
    globalSyms[modId] = modSym;
  }

  // This should generally not be called here as it implements untyped
  // conversion
  Expr* convertAST(const AstNode* node) override;

  Expr* convertExpr(const AstNode* node, RV& rv);
  Expr* convertExprOrNull(const AstNode* node, RV& rv) {
    if (node == nullptr) return nullptr;
    return convertExpr(node, rv);
  }

  ModuleSymbol* convertModule(const Module* mod);

  ModuleSymbol* convertToplevelModule(const Module* mod,
                                      ModTag modTag) override;

  void postConvertApplyFixups() override;

  void setupModulesToConvert();
  void convertFunctionsToConvert();

  ModuleSymbol* setupModule(ID modId);
  ModuleSymbol* findOrSetupModule(ID modId);

  void convertModuleInit(const Module* mod, ModuleSymbol* modSym);
  void convertFunction(const ResolvedFunction* r);
  FnSymbol* findOrConvertFunction(const ResolvedFunction* r);
  void convertNewWrapper(const ResolvedFunction* rInitFn);
  FnSymbol* findOrConvertNewWrapper(const ResolvedFunction* initFn);

  FnSymbol* convertFunctionForGeneratedCall(resolution::CallInfo ci,
                                            const uast::Module* inMod);
  void createMainFunctions() override;

  ArgSymbol* findOrCreateFormal(const Formal* node);
  VarSymbol* findOrCreateVar(const Variable* node);
  Symbol* findConvertedSym(const ID& id);
  Symbol* findOrCreateSym(const ID& id);

  AggregateType* findOrCreateClass(const types::BasicClassType* bct);
  AggregateType* findOrCreateRecord(const types::RecordType* rt);
  AggregateType* findOrCreateUnion(const types::UnionType* ut);
  Type* findConvertedType(const types::Type* t);
  Type* findOrCreateType(const types::Type* t);
  // convertType is the main entry point for these
  //  * checks if it was already converted, returning the saved result
  //  * updates the map of converted types in a way that allows
  //    for recursive types
  Type* convertType(const types::Type* t);

  //Symbol* findConvertedFn(const ResolvedFunction* rfn);
  void noteConvertedSym(const uast::AstNode* ast, Symbol* sym);

  // type conversion helpers
  // note: these do not check if the type has already been converted;
  // or update the 'convertedTypes' map; that happens in convertType.
  Type* helpConvertType(const types::Type* t);
  Type* helpConvertClassType(const types::ClassType* t);
  Type* helpConvertPtrType(const types::PtrType* t);
  Type* helpConvertEnumType(const types::EnumType* t);
  Type* helpConvertExternType(const types::ExternType* t);
  Type* helpConvertFunctionType(const types::FunctionType* t);
  void helpConvertFields(const types::CompositeType* ct,
                         const ResolvedFields& rf,
                         AggregateType* at);
  Type* helpConvertBasicClassType(const types::BasicClassType* t);
  Type* helpConvertRecordType(const types::RecordType* t);
  Type* helpConvertUnionType(const types::UnionType* t);
  Type* helpConvertTupleType(const types::TupleType* t);
  Type* helpConvertBoolType(const types::BoolType* t);
  Type* helpConvertComplexType(const types::ComplexType* t);
  Type* helpConvertImagType(const types::ImagType* t);
  Type* helpConvertIntType(const types::IntType* t);
  Type* helpConvertRealType(const types::RealType* t);
  Type* helpConvertUintType(const types::UintType* t);

  Symbol* convertParam(types::QualifiedType qt);

  // aListStack helpers
  void pushAList(AList* lst, Expr* expr) {
    aListStack.push_back({lst, expr});
    curAList = lst;
  }
  Expr* popAList() {
    CHPL_ASSERT(aListStack.size() > 0);

    Expr* ret = nullptr;
    ret = aListStack.back().second;

    aListStack.pop_back();
    if (aListStack.size() > 0)
      curAList = aListStack.back().first;
    else
      curAList = &scratchSpaceBlock->body;

    return ret;
  }

  BlockStmt* pushNewBlock() {
    auto newBlockStmt = new BlockStmt();
    pushAList(&newBlockStmt->body, newBlockStmt);
    return newBlockStmt;
  }
  BlockStmt* pushBlock(BlockStmt* block) {
    pushAList(&block->body, block);
    return block;
  }
  BlockStmt* popBlock() {
    return toBlockStmt(popAList());
  }

  void enterCallActuals(CallExpr* call) {
    pushAList(&call->argList, call);
  }
  CallExpr* exitCallActuals() {
    return toCallExpr(popAList());
  }

  void enterFormals(FnSymbol* fn) {
    pushAList(&fn->formals, nullptr);
  }
  void exitFormals(FnSymbol* fn) {
    popAList();
  }

  // helpers
  bool typeExistsAtRuntime(const types::Type* t);
  bool functionExistsAtRuntime(const ResolvedFunction* r);
  Expr* convertLifetimeClause(const AstNode* node, RV& rv);
  CallExpr* convertLifetimeIdent(const Identifier* node, RV& rv);
  // Only type expressions that are runtime types need to be converted
  // as code; otherwise we can forget the type expression and just use the type.
  // Accordingly, this returns 'nullptr' if the type expression isn't
  // a type with a runtime component.
  Expr* convertRuntimeTypeExpression(const AstNode* node, RV& rv);
  // note: the relevant calls and DefExpr are added to curAList;
  // this function returns the VarSymbol*
  VarSymbol* convertVariable(const uast::Variable* node,
                             RV& rv,
                             bool useLinkageName,
                             MultiDeclState* multiState = nullptr);
  void simplifyEpilogue(FnSymbol* fn);
  ::Qualifier convertQualifier(types::QualifiedType::Kind kind);
  IntentTag convertFormalIntentQt(types::QualifiedType::Kind kind);
  void setVariableType(const uast::VarLikeDecl* v, Symbol* sym, RV& rv);
  void convertActuals(CallExpr* c,
                      const Call* node,
                      const resolution::ResolvedExpression* re,
                      RV& rv);
  void handlePostCallActions(CallExpr* c,
                             const Call* node,
                             const resolution::ResolvedExpression* re,
                             RV& rv);


  // helpers we might want to bring back from convert-uast.cpp
  //Expr* resolvedIdentifier(const Identifier* node);
  //BlockStmt* convertExplicitBlock(AstListIteratorPair<AstNode> stmts,
  //                                bool flattenTopLevelScopelessBlocks);
  //BlockStmt* convertImplicitBlock(AstListIteratorPair<AstNode> stmts);
  //BlockStmt* createBlockWithStmts(AstListIteratorPair<AstNode> stmts,
             //                     BlockStyle style,
             //                     bool flattenTopLevelScopelessBlocks=true);
  //Expr* singleExprFromStmts(AstListIteratorPair<AstNode> stmts);
  // CallExpr* convertWithClause(const WithClause* node, const AstNode* parent);
  //CallExpr* convertNewManagement(const New* node);
  //Expr* convertScanReduceOp(const AstNode* node);
  //Expr* convertLoopIndexDecl(const Decl* node);
  //bool isBracketLoopMaybeArrayType(const BracketLoop* node);
  //Expr* convertBracketLoopExpr(const BracketLoop* node);
  //Expr* tryExtractFilterCond(const IndexableLoop* node, Expr*& cond);
  //Expr* convertForallLoopExpr(const Forall* node);
  //Expr* convertCalledKeyword(const AstNode* node, const Call* inCall);
  //Expr* convertSparseKeyword(const FnCall* node);
  //CallExpr* convertModuleDotCall(const FnCall* node);
  //Expr* convertDmappedOp(const OpCall* node);
  //Expr* convertTupleExpand(const OpCall* node);
  //Expr* convertReduceAssign(const OpCall* node);
  //Expr* convertToNilableChecked(const OpCall* node);
  //Expr* convertLogicalAndAssign(const OpCall* node);
  //Expr* convertLogicalOrAssign(const OpCall* node);
  //Expr* convertTupleAssign(const OpCall* node);
  //Expr* convertRegularBinaryOrUnaryOp(const OpCall* node,
  //                                    const char* name=nullptr);
  //BlockStmt* convertTupleDeclComponents(const TupleDecl* node);
  //FnSymbol* convertFunctionSignature(const FunctionSignature* node);
  //CallExpr* convertArrayType(const BracketLoop* node);
  //DefExpr* convertEnumElement(const EnumElement* node);
  //template <typename Iterable>
  //void convertInheritsExprs(const Iterable& iterable,
  //                          std::vector<Expr*>& inherits,
  //                          bool& inheritMarkedGeneric);
  //Expr* convertAggregateDecl(const AggregateDecl* node);
  //Expr* convertTypeExpression(const AstNode* node);
  //Expr* convertTypeExpressionOrNull(const AstNode* node);
  //ShadowVarSymbol* convertTaskVar(const TaskVar* node);

  // traversal cases to do nothing
  bool enter(const TypeDecl* ast, RV& rv) { return false; }
  void exit(const TypeDecl* ast, RV& rv) { }
  bool enter(const Use* ast, RV& rv) { return false; }
  void exit(const Use* ast, RV& rv) { }
  bool enter(const Import* ast, RV& rv) { return false; }
  void exit(const Import* ast, RV& rv) { }
  bool enter(const Require* ast, RV& rv) { return false; }
  void exit(const Require* ast, RV& rv) { }
  bool enter(const ExternBlock* ast, RV& rv) { return false; }
  void exit(const ExternBlock* ast, RV& rv) { }
  bool enter(const Implements* ast, RV& rv) { return false; }
  void exit(const Implements* ast, RV& rv) { }
  bool enter(const VisibilityClause* ast, RV& rv) { return false; }
  void exit(const VisibilityClause* ast, RV& rv) { }
  bool enter(const As* ast, RV& rv) { return false; }
  void exit(const As* ast, RV& rv) { }
  bool enter(const Attribute* ast, RV& rv) { return false; }
  void exit(const Attribute* ast, RV& rv) { }
  bool enter(const AttributeGroup* ast, RV& rv) { return false; }
  void exit(const AttributeGroup* ast, RV& rv) { }
  bool enter(const ErroneousExpression* ast, RV& rv) { return false; }
  void exit(const ErroneousExpression* ast, RV& rv) { }
  bool enter(const EmptyStmt* ast, RV& rv) { return false; }
  void exit(const EmptyStmt* ast, RV& rv) { }
  bool enter(const Formal* ast, RV& rv) { return false; }
  void exit(const Formal* ast, RV& rv) { }

  // traversal cases to do something
  bool enter(const Module* node, RV& rv);
  void exit(const Module* node, RV& rv);

  bool enter(const Function* node, RV& rv);
  void exit(const Function* node, RV& rv);

  bool enter(const Variable* node, RV& rv);
  void exit(const Variable* node, RV& rv);

  bool enter(const Literal* node, RV& rv);
  void exit(const Literal* node, RV& rv);

  bool enter(const Identifier* node, RV& rv);
  void exit(const Identifier* node, RV& rv);

  bool enter(const Return* node, RV& rv);
  void exit(const Return* node, RV& rv);

  bool enter(const Call* node, RV& rv);
  void exit(const Call* node, RV& rv);

  bool enter(const AstNode* node, RV& rv);
  void exit(const AstNode* node, RV& rv);
};

TConverter::~TConverter() { }

Expr* TConverter::convertAST(const AstNode* node) {
  // TODO: is this sufficient?
  return untypedConverter->convertAST(node);
}

Expr* TConverter::convertExpr(const AstNode* node, RV& rv) {
  // traverse to add it to curAList
  node->traverse(rv);

  CHPL_ASSERT(!curAList->empty());

  // remove the last thing from the current AList and return that
  return curAList->last()->remove();
}

ModuleSymbol* TConverter::convertModule(const Module* mod) {
  astlocMarker markAstLoc(mod->id());

  ModuleSymbol* modSym = findOrSetupModule(mod->id());

  // TODO: remove this block once the resolver is fully operational
  if (modSym->modTag != MOD_USER) {
    return untypedConverter->convertToplevelModule(mod, modSym->modTag);
  }

  // convert the module init while gathering submodules
  submodulesEncountered.clear();
  convertModuleInit(mod, modSym);

  // Additionally, convert any submodules encontered.
  // This is handled as a follow-on step in order to avoid storing
  // various pieces of TConverter state in a stack.
  for (const uast::Module* submod : submodulesEncountered) {
    ModuleSymbol* subSym = convertModule(submod);
    // add the DefExpr now (to match the untyped Converter)
    modSym->block->insertAtHead(new DefExpr(subSym));
  }
  submodulesEncountered.clear();

  return modSym;
}

ModuleSymbol* TConverter::convertToplevelModule(const Module* mod,
                                                ModTag modTag) {
  if (!haveSetupModules) {
    // create empty modules so we have somewhere to put the functions!
    setupModulesToConvert();
    haveSetupModules = true;
  }

  if (!haveConvertedFunctions) {
    // convert the functions!
    convertFunctionsToConvert();
    haveConvertedFunctions = true;
  }

  topLevelModTag = modTag;
  return convertModule(mod);
}

void TConverter::postConvertApplyFixups() {
  untypedConverter->postConvertApplyFixups();


  // TODO: apply fixups from this converter
}

void TConverter::setupModulesToConvert() {
  // create each module now since we might add a function to it
  // the order in which this is done is not particularly
  // important since they will be filled-in in module init order
  // via calls to convertToplevelModule.
  for (const ID& modId : modulesToConvertVec) {
    findOrSetupModule(modId);
  }
}

struct SortCalledFnsDeeperFirst {
  bool operator()(const std::pair<const ResolvedFunction*, CalledFnOrder>& a,
                  const std::pair<const ResolvedFunction*, CalledFnOrder>& b) {
    // reverse sort by depth
    if (a.second.depth > b.second.depth) return true;
    if (a.second.depth < b.second.depth) return false;

    // regular sort by index encountered in call graph
    return a.second.index < b.second.index;
  }
};

void TConverter::convertFunctionsToConvert() {
  // Sort the called functions
  // 1. in descending order by depth (higher depths first)
  // 2. by ID

  auto v = std::vector<std::pair<const ResolvedFunction*, CalledFnOrder>>(
                                       functionsToConvertWithTypes.begin(),
                                       functionsToConvertWithTypes.end());

  std::sort(v.begin(), v.end(), SortCalledFnsDeeperFirst());


  if (trace) {
    printf("Will convert functions in this order:\n");
    for (auto pair : v) {
      printf("  %s depth=%i index=%i\n", pair.first->id().str().c_str(),
             pair.second.depth, pair.second.index);
    }
  }

  for (auto pair : v) {
    convertFunction(pair.first);
  }

  // convert also main and create chpl_gen_main
  createMainFunctions();

  // tell the untyped converter about which functions to ignore
  // this is whatever functions we have converted already here
  std::unordered_set<ID> ignore;
  for (auto pair: fns) {
    const ResolvedFunction* fn = pair.first;
    ignore.insert(fn->id());
  }
  for (auto pair: globalSyms) {
    ignore.insert(pair.first);
  }
  untypedConverter->setSymbolsToIgnore(ignore);
}

ModuleSymbol* TConverter::setupModule(ID modId) {
  astlocMarker markAstLoc(modId);

  UniqueString path;
  bool found = context->filePathForId(modId, path);
  INT_ASSERT(found);

  // compute the modTag
  ModTag modTag = getModuleTag(context, path);

  const AstNode* ast = parsing::idToAst(context, modId);
  if (ast == nullptr || !ast->isModule()) {
    INT_FATAL("module ID expected");
  }
  const Module* mod = ast->toModule();
  UniqueString unused;
  bool isFromLibraryFile = context->moduleIsInLibrary(modId, unused);

  const char* name = astr(mod->name());
  UniqueString pathUstr;
  UniqueString ignoredParentSymPath;

  bool priv = (mod->visibility() == Decl::PRIVATE);
  bool prototype = (mod->kind() == Module::PROTOTYPE ||
                    mod->kind() == Module::IMPLICIT);

  ModuleSymbol* modSym = buildModule(name,
                                     modTag,
                                     new BlockStmt(),
                                     astr(path),
                                     priv,
                                     prototype);

  if (mod->kind() == Module::IMPLICIT) {
    modSym->addFlag(FLAG_IMPLICIT_MODULE);
  }

  attachSymbolAttributes(context, mod, modSym, isFromLibraryFile);

  // Save the empty module for later filling-in
  globalSyms[modId] = modSym;

  // also tell the untyped converter about it
  untypedConverter->useModuleWhenConverting(modId, modSym);

  // This code does not add a DefExpr for the new module
  // * for toplevel modules, that happens in the code calling
  //   convertToplevelModule
  // * for submodules, that happens in convertModule for
  //   the containing module.

  return modSym;
}

ModuleSymbol* TConverter::findOrSetupModule(ID modId) {
  Symbol* sym = globalSyms[modId];
  if (sym == nullptr) {
    // It can happen that it wasn't in modulesToConvert but we need to
    // convert it anyway. E.g. with PrintModuleInitOrder.
    return setupModule(modId);
  }

  return toModuleSymbol(sym);
}

void TConverter::convertModuleInit(const Module* mod, ModuleSymbol* modSym) {
  if (trace)
    printf("Converting module init for %s[%i]\n",
           mod->id().str().c_str(), modSym->id);

  // create the module init function
  modSym->initFn = new FnSymbol(astr("chpl__init_", modSym->name));
  modSym->initFn->retType = dtVoid;

  modSym->initFn->addFlag(FLAG_MODULE_INIT);
  modSym->initFn->addFlag(FLAG_RESOLVED);
  modSym->initFn->addFlag(FLAG_RESOLVED_EARLY);
  modSym->initFn->addFlag(FLAG_INSERT_LINE_FILE_INFO);

  // add the init function to the module
  modSym->block->insertAtHead(new DefExpr(modSym->initFn));


  // If the module has the EXPORT_INIT flag then
  // propagate it to the module's init function
  if (modSym->hasFlag(FLAG_EXPORT_INIT) ||
      (fLibraryCompile && modSym->modTag == MOD_USER)) {
    modSym->initFn->addFlag(FLAG_EXPORT);
    modSym->initFn->addFlag(FLAG_LOCAL_ARGS);
  }

  // TODO: handle module deinit functions
  // TODO: call module init functions for other modules


  // convert module-level statements into module's init function

  // prepare to traverse
  ID id = mod->id();
  const ResolutionResultByPostorderID& resolved = resolveModule(context, id);
  pushBlock(modSym->initFn->body);
  symbol = mod;
  curFnSymbol = modSym->initFn;
  curModuleSymbol = modSym;
  moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);
  currentResolvedFunction = nullptr;

  // traverse
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rv(&rcval, symbol, *this, resolved);
  symbol->traverse(rv);

  // add the 'return _void' at the end of the module init function
  modSym->initFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  // tidy up after traversal
  symbol = nullptr;
  curFnSymbol = nullptr;
  curModuleSymbol = nullptr;
  popBlock();
}

void TConverter::convertFunction(const ResolvedFunction* r) {
  // don't bother with functions that don't need to exist at runtime
  if (!functionExistsAtRuntime(r)) {
    return;
  }

  if (trace)
    printf("Converting function %s\n", r->id().str().c_str());

  // figure out, in which block should we put the DefExpr for the new FnSymbol?

  // here we flatten all functions, so we just need to find the appropriate
  // module.
  ID parentModule = parsing::idToParentModule(context, r->id());
  ModuleSymbol* modSym = findOrSetupModule(parentModule);

  // prepare to visit
  const ResolutionResultByPostorderID& resolved = r->resolutionById();
  const AstNode* ast = parsing::idToAst(context, r->id());
  if (!ast || !ast->isFunction()) {
    CHPL_ASSERT(false && "expected Function");
    return;
  }
  const Function* fn = ast->toFunction();
  pushBlock(modSym->block);
  symbol = fn;
  curFnSymbol = nullptr; // to be set later
  curModuleSymbol = nullptr; // not doing a module init here
  currentResolvedFunction = r;
  moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);
  localSyms.clear();

  // traverse
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rv(&rcval, symbol, *this, resolved);
  symbol->traverse(rv); // note: this will set curSymbol

  // tidy up after traversal
  symbol = nullptr;
  curFnSymbol = nullptr;
  curModuleSymbol = nullptr;
  currentResolvedFunction = nullptr;
  popBlock();
  localSyms.clear();
}

FnSymbol* TConverter::findOrConvertFunction(const ResolvedFunction* r) {
  // if it was already converted, return that
  auto it = fns.find(r);
  if (it != fns.end()) {
    return it->second;
  }

  // otherwise, convert the function now
  convertFunction(r);
  FnSymbol* ret = fns[r];
  INT_ASSERT(ret);
  return ret;
}

void TConverter::convertNewWrapper(const ResolvedFunction* rInitFn) {
  // don't bother with functions that don't need to exist at runtime
  if (!functionExistsAtRuntime(rInitFn)) {
    return;
  }

  if (trace)
    printf("Converting _new wrapper for %s\n", rInitFn->id().str().c_str());

  // compute the containing Module and ModuleSymbol
  const Module* inModule = nullptr;
  ModuleSymbol* modSym = nullptr;
  {
    ID inModuleId = parsing::idToParentModule(context, rInitFn->id());
    if (const AstNode* inAst = parsing::idToAst(context, inModuleId)) {
      inModule = inAst->toModule();
    }
    INT_ASSERT(inModule);
    modSym = findOrSetupModule(inModuleId);
    INT_ASSERT(modSym);
  }

  // find or convert the init function
  FnSymbol* initFn = findOrConvertFunction(rInitFn);
  AggregateType* type = toAggregateType(initFn->_this->getValType());

  if (initFn->throwsError()) {
    // code below will need adjustment for this case
    CHPL_UNIMPL("_new that throws");
  }

  // build the new wrapper. this code is based upon buildNewWrapper.
  FnSymbol* fn = new FnSymbol(astrNew);

  BlockStmt* body = fn->body;
  VarSymbol* initTemp = newTemp("initTemp", type);
  CallExpr* innerInit = new CallExpr(initFn, gMethodToken, initTemp);
  VarSymbol* retVar = newTemp("ret", type);
  retVar->addFlag(FLAG_RVV);

  fn->addFlag(FLAG_NEW_WRAPPER);
  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->addFlag(FLAG_LAST_RESORT);
  fn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
  fn->addFlag(FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO);
  fn->addFlag(FLAG_LLVM_RETURN_NOALIAS);

  if (initFn->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
    fn->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
  }

  // Add the _new function just after the relevant initFn
  initFn->defPoint->insertAfter(new DefExpr(fn));

  // set up the formals and init call based on the init function
  SymbolMap initToNewMap;
  for_formals(formal, initFn) {
    if (formal != initFn->_this && formal->type != dtMethodToken) {
      ArgSymbol* newArg = formal->copy();
      initToNewMap.put(formal, newArg);
      fn->insertFormalAtTail(newArg);
      innerInit->insertAtTail(new SymExpr(newArg));
    }
  }
  update_symbols(fn, &initToNewMap);

  body->insertAtTail(new DefExpr(initTemp));

  // compute the size and md
  VarSymbol* size = newTemp("size", dtInt[INT_SIZE_64]);
  body->insertAtTail(new DefExpr(size));
  CallExpr* sizeofCall = new CallExpr(PRIM_SIZEOF_BUNDLE, type->symbol);
  body->insertAtTail(new CallExpr(PRIM_MOVE, size, sizeofCall));
  VarSymbol* md = newMemDesc(type); // int(16)

  // allocate and cast
  auto sizeQt = types::QualifiedType(types::QualifiedType::PARAM,
                                     types::IntType::get(context, 64));
  auto mdQt = types::QualifiedType(types::QualifiedType::PARAM,
                                   types::IntType::get(context, 16));
  auto ci = resolution::CallInfo(UniqueString::get(context, "chpl_here_alloc"),
                                 /* calledType */ types::QualifiedType(),
                                 /* isMethodCall */ false,
                                 /* hasQuestionArg */ false,
                                 /* isParenless */ false,
                                 {CallInfoActual(sizeQt),
                                  CallInfoActual(mdQt)});
  FnSymbol* allocateFn = convertFunctionForGeneratedCall(ci, inModule);

  VarSymbol* castTmp = newTemp("cast_tmp", dtCVoidPtr);
  body->insertAtTail(new DefExpr(castTmp));
  CallExpr* allocateCall = new CallExpr(allocateFn, size, md);
  body->insertAtTail(new CallExpr(PRIM_MOVE, castTmp, allocateCall));
  CallExpr* castCall = new CallExpr(PRIM_CAST, type, castTmp);
  body->insertAtTail(new CallExpr(PRIM_MOVE, initTemp, castCall));

  // run the initializer on the allocated class
  body->insertAtTail(innerInit);

  // return initTemp
  body->insertAtTail(new CallExpr(PRIM_MOVE, retVar, initTemp));
  body->insertAtTail(new CallExpr(PRIM_RETURN, retVar));

  printf("Generated _new\n");
  nprint_view(fn);
}

FnSymbol* TConverter::findOrConvertNewWrapper(const ResolvedFunction* rInitFn) {
  // if it was already converted, return that
  auto it = newWrappers.find(rInitFn);
  if (it != newWrappers.end()) {
    return it->second;
  }

  // otherwise, convert the function now
  convertNewWrapper(rInitFn);
  FnSymbol* ret = newWrappers[rInitFn];
  INT_ASSERT(ret);
  return ret;
}


// This runs the resolver to figure out which function to call &
// converts that function.
// It does not convert the call itself.
// It works as though the generated call were in the module initializer.
FnSymbol* TConverter::convertFunctionForGeneratedCall(resolution::CallInfo ci,
                                                      const uast::Module* inMod)
{
  auto modScope = scopeForModule(context, inMod->id());
  const PoiScope* poiScope = nullptr;
  auto scopeInfo = CallScopeInfo::forNormalCall(modScope, poiScope);
  auto r = resolveGeneratedCall(context, inMod, ci, scopeInfo);
  const auto& candidate = r.mostSpecific().only();
  const TypedFnSignature* sig = candidate.fn();
  INT_ASSERT(sig);
  INT_ASSERT(sig->untyped()->idIsFunction());

  chpl::resolution::ResolutionContext rcval(context);
  const ResolvedFunction* resolvedFn = resolveFunction(&rcval, sig, poiScope);
  INT_ASSERT(resolvedFn);

  return findOrConvertFunction(resolvedFn);
}

void TConverter::createMainFunctions() {
  const AstNode* ast = parsing::idToAst(context, mainModuleId);
  INT_ASSERT(ast);
  const Module* mainMod = ast->toModule();
  INT_ASSERT(mainMod);
  ModuleSymbol* mainModule = findOrSetupModule(mainModuleId);
  INT_ASSERT(mainModule);
  SET_LINENO(mainModule);

  // if 'proc main' was not provided, generate an empty 'proc main'.
  //
  // (if there was a main function there, we would have already converted it,
  //  since it needed to be included in the call graph).
  ID mainFnId = parsing::findProcMainInModule(context, mainModuleId);
  FnSymbol* mainFn = nullptr;
  if (mainFnId.isEmpty()) {
    // there wasn't a 'proc main' so we need to generate one.
    mainFn = new FnSymbol("main");
    mainFn->addFlag(FLAG_RESOLVED);
    mainFn->addFlag(FLAG_RESOLVED_EARLY);
    mainFn->retType = dtVoid;
    mainFn->body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    mainModule->block->insertAtTail(new DefExpr(mainFn));
  } else {
    // otherwise, we should have already converted 'proc main'
    // (since it was added to the call graph). Get the converted version.
    const ResolvedFunction* resolvedMain =
      resolveConcreteFunction(context, mainFnId);
    mainFn = toFnSymbol(fns[resolvedMain]);
    INT_ASSERT(mainFn);
  }

  // set the main module global var for later phases of compilation
  // and also the chplUserMain global.
  ModuleSymbol::setMainModule(mainModule);
  chplUserMain = mainFn;

  // adjust cname for 'proc main'
  chplUserMain->cname = astr("chpl_user_main");
  if (fIdBasedMunging && !mainModule->astloc.id().isEmpty()) {
    const char* cname = astr(mainModule->astloc.id().symbolPath().c_str(),
                             ".main");
    chplUserMain->cname = cname;
  }

  // generate chpl_gen_main
  // TODO: expand this after we have 'class C' and 'record R' converting
#if 0
  bool mainReturnsSomething = mainFn->retType != dtVoid;

  //
  // chpl_gen_main is the entry point for the compiler-generated code.
  // It invokes the user's code.
  //

  ArgSymbol* arg = new ArgSymbol(INTENT_CONST_REF, "_arg", dtMainArgument);

  chpl_gen_main          = new FnSymbol("chpl_gen_main");
  chpl_gen_main->retType = dtInt[INT_SIZE_64];
  chpl_gen_main->cname   = astr("chpl_gen_main");

  chpl_gen_main->insertFormalAtTail(arg);

  chpl_gen_main->addFlag(FLAG_EXPORT);  // chpl_gen_main is always exported.
  chpl_gen_main->addFlag(FLAG_LOCAL_ARGS);
  chpl_gen_main->addFlag(FLAG_COMPILER_GENERATED);
  chpl_gen_main->addFlag(FLAG_GEN_MAIN_FUNC);

  mainModule->block->insertAtTail(new DefExpr(chpl_gen_main));

  VarSymbol* main_ret = newTemp("ret", dtInt[INT_SIZE_64]);
  main_ret->addFlag(FLAG_RVV);
  VarSymbol* endCount = NULL;

  chpl_gen_main->insertAtTail(new DefExpr(main_ret));

  //
  // In --minimal-modules compilation mode, we won't have any
  // parallelism, so no need for end counts (or atomic/sync types to
  // support them).
  //
  if (fMinimalModules == false) {
    // figure out _endCountAlloc
    auto falseQt = types::QualifiedType(types::QualifiedType::PARAM,
                                        types::BoolType::get(context),
                                        types::BoolParam::get(context, false));
    auto ci = resolution::CallInfo(UniqueString::get(context, "_endCountAlloc"),
                                   /* calledType */ types::QualifiedType(),
                                   /* isMethodCall */ false,
                                   /* hasQuestionArg */ false,
                                   /* isParenless */ false,
                                   {CallInfoActual(falseQt)});
    FnSymbol* endCountAlloc = convertFunctionForGeneratedCall(ci, mainMod);

    endCount = newTemp("_endCount");

    chpl_gen_main->insertAtTail(new DefExpr(endCount));
    chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                             endCount,
                                             new CallExpr(endCountAlloc)));

    chpl_gen_main->insertAtTail(new CallExpr(PRIM_SET_DYNAMIC_END_COUNT, endCount));
  }

  chpl_gen_main->insertAtTail(new CallExpr("chpl_rt_preUserCodeHook"));

  // We have to initialize the main module explicitly.
  // It will initialize all the modules it uses, recursively.
  if (!fMultiLocaleInterop) {
    chpl_gen_main->insertAtTail(new CallExpr(mainModule->initFn));
    // also init other modules mentioned on command line
    forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
      if (mod->hasFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE) &&
          mod != mainModule) {
        chpl_gen_main->insertAtTail(new CallExpr(mod->initFn));
      }
    }

  } else {
    // Create an extern definition for the multilocale library server's main
    // function.  chpl_gen_main needs to call it in the course of its run, so
    // that we correctly set up the runtime.
    FnSymbol* chpl_mli_smain = new FnSymbol("chpl_mli_smain");
    chpl_mli_smain->addFlag(FLAG_EXTERN);
    chpl_mli_smain->addFlag(FLAG_LOCAL_ARGS);
    // Takes the connection information
    ArgSymbol* setup_conn = new ArgSymbol(INTENT_BLANK, "setup_conn",
                                          dtStringC);
    chpl_mli_smain->insertFormalAtTail(setup_conn);

    mainModule->block->insertAtTail(new DefExpr(chpl_mli_smain));

    VarSymbol* connection = newTemp("setup_conn");
    chpl_gen_main->insertAtTail(new DefExpr(connection));
    chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE, connection,
                                             new CallExpr("chpl_get_mli_connection",
                                                          arg)));
    chpl_gen_main->insertAtTail(new CallExpr("chpl_mli_smain", connection));
    //normalize(chpl_mli_smain);
  }

  bool main_ret_set = false;

  if (!fLibraryCompile && !fDynoGenStdLib) {
    SET_LINENO(chpl_gen_main);

    if (mainHasArgs == true) {
      VarSymbol* converted_args = newTemp("_main_args");

      converted_args->addFlag(FLAG_INSERT_AUTO_DESTROY);

      chpl_gen_main->insertAtTail(new DefExpr(converted_args));
      chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                               converted_args,
                                               new CallExpr("chpl_convert_args", arg)));

      if (mainReturnsSomething) {
        chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                                 main_ret,
                                                 new CallExpr("main", converted_args)));
        main_ret_set = true;

      } else {
        chpl_gen_main->insertAtTail(new CallExpr("main", converted_args));
      }

    } else {
      if (mainReturnsSomething) {
        chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                                 main_ret,
                                                 new CallExpr("main")));
        main_ret_set = true;

      } else {
        chpl_gen_main->insertAtTail(new CallExpr("main"));
      }
    }
  }

  if (!main_ret_set) {
    chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                             main_ret,
                                             new_IntSymbol(0, INT_SIZE_64)));
  }

  chpl_gen_main->insertAtTail(new CallExpr("chpl_rt_postUserCodeHook"));

  //
  // In --minimal-modules compilation mode, we won't be waiting on an
  // endcount (see comment above)
  //
  if (fMinimalModules == false) {
    chpl_gen_main->insertAtTail(new CallExpr("_waitEndCount", endCount));
    chpl_gen_main->insertAtTail(new CallExpr("chpl_deinitModules"));
  }

  chpl_gen_main->insertAtTail(new CallExpr(PRIM_RETURN, main_ret));
#endif
}

// Finds or creates a Formal
// The ArgSymbol is created on first request here, and filled in
// and the DefExpr created wherever it should be defined.
ArgSymbol* TConverter::findOrCreateFormal(const Formal* node) {
  auto it = localSyms.find(node->id());
  if (it != localSyms.end()) {
    return toArgSymbol(it->second);
  }

  IntentTag intentTag = convertFormalIntent(node->intent());
  astlocMarker markAstLoc(node->id());

  ArgSymbol* ret = new ArgSymbol(intentTag, astr(node->name()),
                                 /* typeExpr */ nullptr,
                                 /* initExpr */ nullptr,
                                 /* varargsVariable */ nullptr);
  localSyms[node->id()] = ret;
  return ret;
}

// Finds or creates a Variable
// The VarSymbol is created here on first request, and filled in
// and the DefExpr created wherever it should be defined.
VarSymbol* TConverter::findOrCreateVar(const Variable* node) {
  if (Symbol* sym = findConvertedSym(node->id())) {
    return toVarSymbol(sym);
  }

  // otherwise, create something
  // we need to decide if it should be a module-scope var or not
  // in order to update the appropriate map.
  bool isModuleScope = parsing::idIsModuleScopeVar(context, node->id());

  astlocMarker markAstLoc(node->id());
  VarSymbol* ret = new VarSymbol(astr(node->name()));
  if (isModuleScope) {
    globalSyms[node->id()] = ret;
  } else {
    localSyms[node->id()] = ret;
  }

  return ret;
}

// Looks for an already-converted Symbol. If none found, returns nullptr.
Symbol* TConverter::findConvertedSym(const ID& id) {
  { // check for local variables with that ID
    auto it = localSyms.find(id);
    if (it != localSyms.end()) {
      return it->second;
    }
  }
  { // check for global symbols with that ID
    auto it = globalSyms.find(id);
    if (it != globalSyms.end()) {
      return it->second;
    }
  }

  return nullptr;
}

Symbol* TConverter::findOrCreateSym(const ID& id) {
  Symbol* ret = findConvertedSym(id);
  if (ret != nullptr) {
    return ret;
  }

  // otherwise, convert it according to what it is
  const AstNode* ast = parsing::idToAst(context, id);
  if (auto formal = ast->toFormal()) {
    return findOrCreateFormal(formal);
  } else if (auto var = ast->toVariable()) {
    return findOrCreateVar(var);
  }

  CHPL_UNIMPL("unhandled case in findOrCreateSym");
  return nullptr;
}

void TConverter::noteConvertedSym(const uast::AstNode* ast, Symbol* sym) {
  if (currentResolvedFunction == nullptr) {
    globalSyms[ast->id()] = sym;
  } else {
    localSyms[ast->id()] = sym;
  }
}

Type* TConverter::findConvertedType(const types::Type* t) {
  auto it = convertedTypes.find(t);
  if (it != convertedTypes.end()) {
    return it->second;
  }

  return nullptr;
}

AggregateType*
TConverter::findOrCreateClass(const types::BasicClassType* bct) {
  AggregateType* ret = toAggregateType(findConvertedType(bct));
  if (ret != nullptr) {
    return ret;
  }

  if (AggregateType* dt = shouldWireWellKnownType(bct->name().c_str())) {
    return dt;
  }

  ret = new AggregateType(AGGREGATE_CLASS);
  convertedTypes[bct] = ret;
  return ret;
}

AggregateType*
TConverter::findOrCreateRecord(const types::RecordType* rt) {
  AggregateType* ret = toAggregateType(findConvertedType(rt));
  if (ret != nullptr) {
    return ret;
  }

  if (AggregateType* dt = shouldWireWellKnownType(rt->name().c_str())) {
    return dt;
  }

  ret = new AggregateType(AGGREGATE_RECORD);
  convertedTypes[rt] = ret;
  return ret;
}

AggregateType*
TConverter::findOrCreateUnion(const types::UnionType* ut) {
  AggregateType* ret = toAggregateType(findConvertedType(ut));
  if (ret != nullptr) {
    return ret;
  }

  ret = new AggregateType(AGGREGATE_UNION);
  convertedTypes[ut] = ret;
  return ret;
}

// Creates a type but if it's an AggregateType doesn't try to fill
// in the fields. That will be done later on convertType for that type.
// This strategy allows for recursion.
Type* TConverter::findOrCreateType(const types::Type* t) {
  Type* ret = findConvertedType(t);
  if (ret != nullptr) {
    return ret;
  }

  if (auto bct = t->toBasicClassType()) {
    return findOrCreateClass(bct);
  } else if (auto rt = t->toRecordType()) {
    return findOrCreateRecord(rt);
  } else if (auto ut = t->toUnionType()) {
    return findOrCreateUnion(ut);
  }

  // assumption: all recursion will be through the cases handled above

  ret = helpConvertType(t);
  convertedTypes[t] = ret;
  return ret;
}

// Converts a type. If it's an AggregateType, it will also fill in the
// fields, but only after creating a dummy AggregateType for the map
// that can be used in the case of recursive data structures.
Type* TConverter::convertType(const types::Type* t) {
  if (t == nullptr)
    return dtUnknown;

  // reuse one from the map if we have already converted it
  {
    auto it = convertedTypes.find(t);
    if (it != convertedTypes.end()) {
      return it->second;
    }
  }

  // convert the type
  Type* ret = helpConvertType(t);

  // save the result to the map
  convertedTypes[t] = ret;

  return ret;
}


Type* TConverter::helpConvertType(const types::Type* t) {
  using namespace types;

  if (t == nullptr)
    return dtUnknown;

  switch (t->tag()) {
    // builtin types with their own classes
    case typetags::AnyType:       return dtAny;
    case typetags::CStringType:   return dtStringC;
    case typetags::ErroneousType: return dtUnknown; // a lie
    case typetags::NilType:       return dtNil;
    case typetags::NothingType:   return dtNothing;
    case typetags::UnknownType:   return dtUnknown;
    case typetags::VoidType:      return dtVoid;

    // subclasses of IterableType
    case typetags::FnIteratorType: return dtUnknown; // a lie
    case typetags::LoopExprIteratorType: return dtUnknown; // a lie
    case typetags::PromotionIteratorType: return dtUnknown; // a lie

    // subclasses of BuiltinType

    // concrete builtin types
    case typetags::CFnPtrType:    return dtCFnPtr;
    case typetags::CVoidPtrType:  return dtCVoidPtr;
    case typetags::OpaqueType:    return dtOpaque;
    case typetags::SyncAuxType:   return dtSyncVarAuxFields;
    case typetags::TaskIdType:    return dtTaskID;

    // generic builtin types
    case typetags::AnyComplexType:               return dtAnyComplex;
    case typetags::AnyEnumType:                  return dtAnyEnumerated;
    case typetags::AnyImagType:                  return dtAnyImag;
    case typetags::AnyIntType:                   return dtIntegral; // a lie
    case typetags::AnyIntegralType:              return dtIntegral;
    case typetags::AnyIteratorClassType:         return dtIteratorClass;
    case typetags::AnyIteratorRecordType:        return dtIteratorRecord;
    case typetags::AnyThunkRecordType:           return dtThunkRecord;
    case typetags::AnyNumericType:               return dtNumeric;
    case typetags::AnyOwnedType:                 return dtOwned;
    case typetags::AnyPodType:                   return dtAnyPOD;
    case typetags::AnyRealType:                  return dtAnyReal;
    case typetags::AnyRecordType:                return dtAnyRecord;
    case typetags::AnySharedType:                return dtShared;
    case typetags::AnyUintType:                  return dtIntegral; // a lie
    case typetags::AnyUninstantiatedType:        return dtUninstantiated;
    case typetags::AnyUnionType:                 return dtUnknown; // a lie
    // declared types
    case typetags::ClassType:   return helpConvertClassType(t->toClassType());
    case typetags::EnumType:    return helpConvertEnumType(t->toEnumType());
    case typetags::ExternType:  return helpConvertExternType(t->toExternType());
    case typetags::FunctionType:return helpConvertFunctionType(t->toFunctionType());

    case typetags::ArrayType:
       CHPL_UNIMPL("convert array type");
       return dtUnknown; // TODO
    case typetags::DomainType:
       CHPL_UNIMPL("convert domain type");
      return dtUnknown; // TODO

    case typetags::BasicClassType:
      return helpConvertBasicClassType(t->toBasicClassType());

    case typetags::AnyClassType:    return dtAnyManagementNonNilable;
    case typetags::RecordType:      return helpConvertRecordType(t->toRecordType());
    case typetags::TupleType:       return helpConvertTupleType(t->toTupleType());
    case typetags::UnionType:       return helpConvertUnionType(t->toUnionType());

    // primitive types
    case typetags::BoolType:       return helpConvertBoolType(t->toBoolType());
    case typetags::ComplexType:    return helpConvertComplexType(t->toComplexType());
    case typetags::ImagType:       return helpConvertImagType(t->toImagType());
    case typetags::IntType:        return helpConvertIntType(t->toIntType());
    case typetags::RealType:       return helpConvertRealType(t->toRealType());
    case typetags::UintType:       return helpConvertUintType(t->toUintType());
    case typetags::CPtrType:       return helpConvertPtrType(t->toPtrType());
    case typetags::HeapBufferType: return helpConvertPtrType(t->toPtrType());

    // implementation detail tags (should not be reachable)
    case typetags::START_ManageableType:
    case typetags::END_ManageableType:
    case typetags::START_BuiltinType:
    case typetags::END_BuiltinType:
    case typetags::START_DeclaredType:
    case typetags::END_DeclaredType:
    case typetags::START_CompositeType:
    case typetags::END_CompositeType:
    case typetags::START_PrimitiveType:
    case typetags::END_PrimitiveType:
    case typetags::START_IteratorType:
    case typetags::END_IteratorType:
    case typetags::START_PtrType:
    case typetags::END_PtrType:
    case typetags::NUM_TYPE_TAGS:
      INT_FATAL("should not be reachable");
      return dtUnknown;

    // intentionally no default --
    // want a C++ compiler error if a case is missing in the above
  }
  INT_FATAL("should not be reached");
  return nullptr;
}

Type* TConverter::helpConvertClassType(const types::ClassType* t) {
  if (auto mt = t->manageableType()) {
    if (mt->isAnyClassType()) {
      // The production compiler represents these as special builtins
      auto dec = t->decorator();
      if (dec.isUnmanaged()) {
        if (dec.isNilable()) return dtUnmanagedNilable;
        if (dec.isNonNilable()) return dtUnmanagedNonNilable;
        return dtUnmanaged;
      } else if (dec.isBorrowed()) {
        if (dec.isNilable()) return dtBorrowedNilable;
        if (dec.isNonNilable()) return dtBorrowedNonNilable;
        return dtBorrowed;
      } else if (dec.isUnknownManagement()) {
        if (dec.isNilable()) return dtAnyManagementNilable;
        if (dec.isNonNilable()) return dtAnyManagementNonNilable;
        return dtAnyManagementAnyNilable;
      } else {
        // fall through
      }
    }
  }

  // convert the basic class type
  const types::BasicClassType* bct = t->basicClassType();
  Type* gotT = convertType(bct);
  AggregateType* at = toAggregateType(gotT);
  INT_ASSERT(at);

  Type* ret = dtUnknown;

  const types::RecordType* manager = t->managerRecordType(context);
  if (manager == nullptr) {
    ret = at; // unamanged / borrowed is just the class type at this point
  } else {
    // owned/shared should have had a substitution for chpl_t
    CHPL_ASSERT(!manager->substitutions().empty());
    // convert the managed class type
    ret = convertType(manager);
  }

  return ret;
}

Type* TConverter::helpConvertPtrType(const types::PtrType* t) {
  // find the pointer type to instantiate
  AggregateType* base = nullptr;
  if (auto ct = t->toCPtrType()) {
    base = ct->isConst() ? dtCPointerConst : dtCPointer;
  } else {
    INT_ASSERT(t->toHeapBufferType());
    base = dtHeapBuffer;
  }

  if (base->symbol == nullptr) {
    const char* name = nullptr;
    if (base == dtCPointer) {
      name = "c_ptr";
    } else if (base == dtCPointerConst) {
      name = "c_ptrConst";
    } else {
      name = "_ddata";
    }
    base->symbol = new TypeSymbol(name, base);
  }

  if (base->numFields() == 0) {
    // fill in the 'type eltType' field
    VarSymbol* v = new VarSymbol(astr("eltType"), dtAny);
    v->qual = QUAL_UNKNOWN;
    v->makeField();
    base->fields.insertAtTail(new DefExpr(v));

    // add to globalSyms so it can be ignored in the untyped converter
    const char* symbolPath = nullptr;
    if (base == dtCPointer) {
      symbolPath = "CTypes.c_ptr";
    } else if (base == dtCPointerConst) {
      symbolPath = "CTypes.c_ptrConst";
    } else {
      symbolPath = "ChapelBase._ddata";
    }

    auto id = ID(UniqueString::get(context, symbolPath));
    globalSyms[id] = base->symbol;
  }

  // handle ptr without an element type
  if (t->eltType() == nullptr) {
    return base;
  }

  // convert the element type
  Type* convertedEltT = convertType(t->eltType());

  // instantiate it with the element type
  // note: getInstantiation should re-use existing instantiations
  int index = 1;
  Expr* insnPoint = nullptr; // TODO: does this need to be set?
  AggregateType* ret =
    base->getInstantiation(convertedEltT->symbol, index, insnPoint);

  return ret;
}

Type* TConverter::helpConvertEnumType(const types::EnumType* t) {
  CHPL_UNIMPL("convertEnumType");
  return nullptr;
}

Type* TConverter::helpConvertExternType(const types::ExternType* t) {
  CHPL_UNIMPL("convertExternType");
  return nullptr;
}

Type* TConverter::helpConvertFunctionType(const types::FunctionType* t) {
  CHPL_UNIMPL("convertExternType");
  return nullptr;
}

void TConverter::helpConvertFields(const types::CompositeType* ct,
                                   const ResolvedFields& rf,
                                   AggregateType* at) {
  int nFields = rf.numFields();
  for (int i = 0; i < nFields; i++) {
    types::QualifiedType qt = rf.fieldType(i);
    if (qt.isType() && !typeExistsAtRuntime(qt.type())) {
      // a 'type' field can be left out at this point
      continue;
    } else if (qt.isParam()) {
      // a 'param' field can be left out at this point
      continue;
    }
    Type* ft = convertType(qt.type());
    UniqueString name = rf.fieldName(i);
    VarSymbol* v = new VarSymbol(astr(name), ft);
    v->qual = convertQualifier(qt.kind());
    v->makeField();
    at->fields.insertAtTail(new DefExpr(v));
  }
}

Type* TConverter::helpConvertBasicClassType(const types::BasicClassType* bct) {
  printf("helpConvertBasicClassType\n");
  bct->dump();
  printf("\n");

  const ResolvedFields& rf =
    fieldsForTypeDecl(context, bct, DefaultsPolicy::USE_DEFAULTS);

  AggregateType* ct = findOrCreateClass(bct);
  if (ct->symbol == nullptr) {
    ct->symbol = new TypeSymbol(astr(bct->name()), ct);
  }

  // add the DefExpr to the module containing the type
  ID inModuleId = parsing::idToParentModule(context, bct->id());
  ModuleSymbol* inModule = nullptr;
  if (inModuleId.isEmpty()) {
    inModule = rootModule;
  } else {
    inModule = findOrSetupModule(inModuleId);
    INT_ASSERT(inModule);
  }
  inModule->block->insertAtTail(new DefExpr(ct->symbol));

  // convert the superclass as a field 'super'
  if (auto parentClassType = bct->parentClassType()) {
    Type* pt = convertType(parentClassType);
    VarSymbol* v = new VarSymbol("super", pt);
    v->qual = QUAL_VAL;
    v->makeField();
    ct->fields.insertAtTail(new DefExpr(v));
  }

  // convert the fields
  helpConvertFields(bct, rf, ct);

  printf("helpConvertBasicClassType result\n");
  nprint_view(ct);
  printf("\n");

  return ct;
}

Type* TConverter::helpConvertRecordType(const types::RecordType* t) {
  const ResolvedFields& rf =
    fieldsForTypeDecl(context, t, DefaultsPolicy::USE_DEFAULTS);

  AggregateType* at = findOrCreateRecord(t);
  if (at->symbol == nullptr) {
    at->symbol = new TypeSymbol(astr(t->name()), at);
  }

  // add the DefExpr to the module containing the type
  ID inModuleId = parsing::idToParentModule(context, t->id());
  ModuleSymbol* inModule = findOrSetupModule(inModuleId);
  inModule->block->insertAtTail(new DefExpr(at->symbol));

  // convert the fields
  helpConvertFields(t, rf, at);

  return at;
}

Type* TConverter::helpConvertUnionType(const types::UnionType* t) {
  const ResolvedFields& rf =
    fieldsForTypeDecl(context, t, DefaultsPolicy::USE_DEFAULTS);

  AggregateType* at = findOrCreateUnion(t);
  if (at->symbol == nullptr) {
    at->symbol = new TypeSymbol(astr(t->name()), at);
  }

  // add the DefExpr to the module containing the type
  ID inModuleId = parsing::idToParentModule(context, t->id());
  ModuleSymbol* inModule = findOrSetupModule(inModuleId);
  inModule->block->insertAtTail(new DefExpr(at->symbol));

  // convert the fields
  helpConvertFields(t, rf, at);

  return at;
}

Type* TConverter::helpConvertTupleType(const types::TupleType* t) {
  CHPL_UNIMPL("convertTupleType");
  return nullptr;
}

Type* TConverter::helpConvertBoolType(const types::BoolType* t) {
  return dtBool;
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

Type* TConverter::helpConvertComplexType(const types::ComplexType* t) {
  return dtComplex[getComplexSize(t)];
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

Type* TConverter::helpConvertImagType(const types::ImagType* t) {
  return dtImag[getImagSize(t)];
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

Type* TConverter::helpConvertIntType(const types::IntType* t) {
  return dtInt[getIntSize(t)];
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

Type* TConverter::helpConvertRealType(const types::RealType* t) {
  return dtReal[getRealSize(t)];
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

Type* TConverter::helpConvertUintType(const types::UintType* t) {
  return dtUInt[getUintSize(t)];
}

// note: new_IntSymbol etc already returns existing if already created
Symbol* TConverter::convertParam(types::QualifiedType qt) {
  const types::Param* p = qt.param();
  const types::Type* t = qt.type();
  INT_ASSERT(p && t);

  if (auto bp = p->toBoolParam()) {
    return new_BoolSymbol(bp->value());
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

bool TConverter::typeExistsAtRuntime(const types::Type* t) {
  /* TODO */
  return false;
}

bool TConverter::functionExistsAtRuntime(const ResolvedFunction* r) {
  const auto& qt = r->returnType();
  if (qt.isType() && !typeExistsAtRuntime(qt.type())) {
    // a 'type' actual can be left out at this point
    return false;
  } else if (qt.isParam()) {
    // a 'param' actual can be left out at this point
    return false;
  }

  // otherwise, there is something to do.
  return true;
}

Expr* TConverter::convertLifetimeClause(const uast::AstNode* node, RV& rv) {
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
    Expr* lhs = convertLifetimeIdent(lhsIdent, rv);
    Expr* rhs = convertLifetimeIdent(rhsIdent, rv);
    return new CallExpr(opCall->op().c_str(), lhs, rhs);
  } else if (auto ret = node->toReturn()) {
    INT_ASSERT(ret->value() && ret->value()->isIdentifier());
    auto ident = ret->value()->toIdentifier();

    Expr* val = convertLifetimeIdent(ident, rv);
    return new CallExpr(PRIM_RETURN, val);

  } else {
    // should not arrive here, or else we missed something
    CHPL_UNIMPL("Unhandled lifetime clause");
    return nullptr;
  }
}

CallExpr* TConverter::convertLifetimeIdent(const uast::Identifier* node, RV& rv)
{
  astlocMarker markAstLoc(node->id());

  auto ident = node->toIdentifier();
  INT_ASSERT(ident);
  CallExpr* callExpr = new CallExpr(PRIM_LIFETIME_OF,
                                    convertExprOrNull(node, rv));
  return callExpr;
}

Expr* TConverter::convertRuntimeTypeExpression(const AstNode* node, RV& rv) {
  // TODO: implement this
  return nullptr;
}

VarSymbol* TConverter::convertVariable(const uast::Variable* node,
                                       RV& rv,
                                       bool useLinkageName,
                                       MultiDeclState* multiState) {
  astlocMarker markAstLoc(node->id());

  bool isStatic = false;
  if (auto ag = node->attributeGroup()) {
    if (ag->getAttributeNamed(USTR("functionStatic"))) {
      CHPL_UNIMPL("function-static variables");
    }
  }

  if (node->name() == USTR("_")) {
    // no need to convert _
    return nullptr;
  }

  bool isRemote = node->destination() != nullptr ||
                  (multiState != nullptr && multiState->localeTemp != nullptr);

  auto varSym = findOrCreateVar(node);
  const bool isTypeVar = node->kind() == uast::Variable::TYPE;
  bool moduleScopeVar = false;
  const uast::Module* inModule = symbol->toModule();

  if (inModule != nullptr) {
    moduleScopeVar = parsing::idIsModuleScopeVar(context, node->id());
  }

  if (fIdBasedMunging && node->linkage() == uast::Decl::DEFAULT_LINKAGE) {
    // is it a module-scope variable?
    // adjust the cname for module-scope variables
    const uast::Module* mod = symbol->toModule();
    if (moduleScopeVar && mod) {
      varSym->cname = astr(mod->id().symbolPath().c_str(),
                           ".",
                           varSym->name);
    }
  }
  uast::Variable::Kind symbolKind = node->kind();
  if (isRemote) {
    CHPL_UNIMPL("remote variable");
  }

  // Adjust the variable according to its kind, e.g. 'const'/'type'.
  attachSymbolStorage(symbolKind, varSym, /* setQual */ true);

  attachSymbolAttributes(context, node, varSym, moduleFromLibraryFile);

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
  //Expr* destinationExpr = convertExprOrNull(node->destination(), rv);
  Expr* typeExpr = convertRuntimeTypeExpression(node->typeExpression(), rv);
  Expr* initExpr = nullptr;

  if (const uast::AstNode* ie = node->initExpression()) {
    const uast::BracketLoop* bkt = ie->toBracketLoop();
    if (bkt && isTypeVar) {
      CHPL_UNIMPL("convert array type");
    } else {
      initExpr = convertExpr(ie, rv);
    }

    if (isStatic) {
      CHPL_UNIMPL("function-static variables");
    }
  } else {
    initExpr = convertExprOrNull(node->initExpression(), rv);
  }

  if ((!typeExpr && !initExpr) && multiState) {
    CHPL_UNIMPL("multi-vars");
  }

  DefExpr* def = nullptr;
  if (isRemote) {
    CHPL_UNIMPL("remote variable");
  } else {
    def = new DefExpr(varSym);
    if (!moduleScopeVar) {
      curAList->insertAtTail(def);
    } else {
      // module variables need to be stored outside of module init fn
      INT_ASSERT(curModuleSymbol);
      curModuleSymbol->block->insertAtTail(def);
    }

    CallExpr* move = nullptr;
    if (varSym->isRef()) {
      move = new CallExpr(PRIM_MOVE, varSym,
                                     new CallExpr(PRIM_ADDR_OF, initExpr));
    } else {
      move = new CallExpr(PRIM_MOVE, varSym, initExpr);
    }
    curAList->insertAtTail(move);
  }

  auto loopFlags = LoopAttributeInfo::fromVariableDeclaration(context, node);
  if (!loopFlags.empty()) {
    CHPL_UNIMPL("loop attribute info");
    // curAList->insertAtTail(new CallExpr(PRIM_GPU_ATTRIBUTE_BLOCK));
    //if (auto primBlock = loopFlags.createPrimitivesBlock(*this)) {
    //  curAList->insertAtTail(primBlock);
    //}
  }
  // If the init expression of this variable is a domain and this
  // variable is not const, propagate that information by setting
  // 'definedConst' in the domain to false.
  setDefinedConstForDefExprIfApplicable(def, &def->sym->flags);

  // Fix up the AST based on the type, if it should be known
  setVariableType(node, varSym, rv);

  // Note the variable is converted so we can wire up SymExprs later
  noteConvertedSym(node, varSym);

  return varSym;
}

// this does a micro-optimization to replace
//   GOTO(epilogue)
//   epilogue:
// with relying on the pass-through
void TConverter::simplifyEpilogue(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_NO_FN_BODY)) return;

  if (Expr* labelDef = epilogueLabel->defPoint) {
    if (GotoStmt* g = toGotoStmt(labelDef->prev)) {
      if (g->gotoTag == GOTO_RETURN) {
        // remove the GOTO and rely on fall-through
        g->remove();
      }
    }
  }

  // if the epilogue label is now unused, remove it
  if (epilogueLabel->firstSymExpr() == nullptr) {
    epilogueLabel->defPoint->remove();
  }
}

::Qualifier TConverter::convertQualifier(types::QualifiedType::Kind kind) {
  ::Qualifier q = QUAL_UNKNOWN;
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


IntentTag TConverter::convertFormalIntentQt(types::QualifiedType::Kind kind)
{
  IntentTag t = INTENT_BLANK;

  if      (kind == types::QualifiedType::VAR)            t = INTENT_IN;
  else if (kind == types::QualifiedType::CONST_VAR)      t = INTENT_IN;
  else if (kind == types::QualifiedType::CONST_REF)      t = INTENT_CONST_REF;
  else if (kind == types::QualifiedType::REF)            t = INTENT_REF;
  else if (kind == types::QualifiedType::IN)             t = INTENT_IN;
  else if (kind == types::QualifiedType::CONST_IN)       t = INTENT_CONST_IN;
  else if (kind == types::QualifiedType::OUT)            t = INTENT_OUT;
  else if (kind == types::QualifiedType::INOUT)          t = INTENT_INOUT;
  else if (kind == types::QualifiedType::PARAM)          t = INTENT_PARAM;
  else if (kind == types::QualifiedType::TYPE)           t = INTENT_TYPE;
  else if (kind == types::QualifiedType::DEFAULT_INTENT) t = INTENT_BLANK;

  return t;
}

void TConverter::setVariableType(const uast::VarLikeDecl* v,
                                 Symbol* sym,
                                 RV& rv) {
  printf("setVariableType %s\n", v->id().str().c_str());

  // Get the type of the variable itself
  if (const resolution::ResolvedExpression* rr = rv.byAstOrNull(v)) {
    types::QualifiedType qt = rr->type();
    if (!qt.isUnknown()) {
      printf("qt is \n");
      qt.dump();

      // Set a type for the variable
      sym->type = convertType(qt.type());

      // Set the Qualifier
      auto q = convertQualifier(qt.kind());
      if (q != QUAL_UNKNOWN)
        sym->qual = q;

      // If it's a Formal / ArgSymbol, also set the intent and originalIntent
      ArgSymbol* arg = toArgSymbol(sym);
      const uast::Formal* fml = v->toFormal();
      if (arg != nullptr && fml != nullptr) {
        arg->originalIntent = convertFormalIntent(fml->intent());
        arg->intent = convertFormalIntentQt(qt.kind());
      }

      // Set the param value for the variable in paramMap, if applicable
      /*if (sym->hasFlag(FLAG_MAYBE_PARAM) || sym->hasFlag(FLAG_PARAM)) {
        if (qt.hasParamPtr()) {
          Symbol* val = convertParam(qt);
          paramMap.put(sym, val);
        }
      }*/
    }
  }
}

void TConverter::convertActuals(CallExpr* c,
                                const Call* node,
                                const resolution::ResolvedExpression* re,
                                RV& rv) {
  if (node->isPrimCall()) {
    // use a simple strategy to convert prim calls
    // (assuming no named argument passing)
    // TODO: do some associated actions need to be handled here?
    enterCallActuals(c);
    for (auto actual : node->actuals()) {
      c->insertAtTail(convertExpr(actual, rv));
    }
    exitCallActuals();
    return;
  }

  // otherwise
  enterCallActuals(c);

  // gather the actuals in a way that is consistent with other resolution parts
  std::vector<const AstNode*> actualAsts;
  auto ci = resolution::CallInfo::create(context, node, rv.byPostorder(),
                                         /* raiseErrors */ false,
                                         &actualAsts);

  // consider the actual for each Formal
  const auto& candidate = re->mostSpecific().only();
  auto& formalActualMap = candidate.formalActualMap();
  for (const FormalActual& fa : formalActualMap.byFormals()) {
    const types::QualifiedType& qt = fa.formalType();
    if (qt.isType() && !typeExistsAtRuntime(qt.type())) {
      // a 'type' actual can be left out at this point
    } else if (qt.isParam()) {
      // a 'param' actual can be left out at this point
    } else {
      if (fa.hasDefault()) {
        CHPL_UNIMPL("default arguments");
      } else {
        // it's something that exists at runtime, so include it
        Expr* actualExpr = convertExpr(actualAsts[fa.actualIdx()], rv);
        c->insertAtTail(actualExpr);
      }
      // TODO handle associated actions for 'in' 'out', deinit tmp, etc
    }
  }

  // done generating actuals
  exitCallActuals();


}

void TConverter::handlePostCallActions(CallExpr* c,
                                       const Call* node,
                                       const resolution::ResolvedExpression* re,
                                       RV& rv) {
}


bool TConverter::enter(const Module* node, RV& rv) {
  if (trace)
    printf("enter module %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  if (modulesToConvert.count(node->id()) == 0) {
    // this module should not be converted.
    if (trace) printf("Not in modules to convert\n");
    return false;
  }

  if (node == symbol) {
    // we are visiting it to convert the module initialization function
    // so proceed with the traversal
    if (trace) printf("Proceeding with module converting\n");
    return true;
  } else {
    if (trace) printf("Recording a submodule\n");
    submodulesEncountered.push_back(node);

    return false;
  }
  return true;
}
void TConverter::exit(const Module* node, RV& rv) {
  if (trace)
    printf("exit module %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Function* node, RV& rv) {
  if (trace)
    printf("enter function %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));


  if (node != symbol) {
    // It's a function, but not the one we are working on.
    // Stop the traversal. This Function will be handled when
    // convertFunction is called.
    return false;
  }

  if (trace) printf("Really converting Function %s\n", node->name().c_str());

  astlocMarker markAstLoc(node->id());

  CHPL_ASSERT(currentResolvedFunction != nullptr);

  FnSymbol* fn = new FnSymbol(astr(node->name()));
  curAList->insertAtTail(new DefExpr(fn));
  curFnSymbol = fn; // for setting child node's parentSymbol

  // note the correspondence between the ResolvedFunction & what it converts to
  // (for calls & done here to support recursive calls)
  fns[currentResolvedFunction] = fn;

  fn->userString = constructUserString(node);

  attachSymbolAttributes(context, node, fn, moduleFromLibraryFile);
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

  fn->addFlag(FLAG_RESOLVED);
  fn->addFlag(FLAG_RESOLVED_EARLY);

  if (currentResolvedFunction->signature()->instantiatedFrom() != nullptr) {
    // generic instantiations are "invisible"
    fn->addFlag(FLAG_INVISIBLE_FN);
  }

  IntentTag thisTag = INTENT_BLANK;
  ArgSymbol* convertedReceiver = nullptr;


  // Convert the formals
  if (node->numFormals() > 0) {
    // create the formals first & put them in a map
    if (trace) printf("Converting formals\n");
    enterFormals(fn);
    // TODO: gather the formals we want to create
    // to un-pack tuples etc.
    for (auto decl : node->formals()) {
      astlocMarker markAstLoc(decl->id());

      // Fill in each ArgSymbol and add the DefExpr
      if (auto fml = decl->toFormal()) {
        Expr* typeExpr = nullptr;
        typeExpr = convertRuntimeTypeExpression(fml->typeExpression(), rv);
        if (typeExpr) {
          CHPL_UNIMPL("runtime type formal");
        }

        ArgSymbol* arg = findOrCreateFormal(fml);
        DefExpr* def = new DefExpr(arg);

        attachSymbolAttributes(context, node, def->sym, moduleFromLibraryFile);

        setVariableType(fml, arg, rv);

        fn->insertFormalAtTail(def);

        // Special handling for implicit receiver formal.
        if (fml->name() == USTR("this")) {
          INT_ASSERT(convertedReceiver == nullptr);

          convertedReceiver = arg;
          INT_ASSERT(convertedReceiver);

          arg->addFlag(FLAG_ARG_THIS);

          if (thisTag == INTENT_TYPE) {
            setupTypeIntentArg(convertedReceiver);
          }
        }
      } else if (decl->isVarArgFormal()) {
        // A varargs formal.
        CHPL_UNIMPL("varargs formal");
        // TODO: do these even exist after call destructors?

      } else if (decl->toTupleDecl()) {
        // A tuple decl, where components are formals or tuple decls.
        CHPL_UNIMPL("Unhandled tuple formal");
        /*
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
        */
      } else {
        CHPL_UNIMPL("Unhandled formal");
      }
    }
    exitFormals(fn);
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
  Expr* retType = convertRuntimeTypeExpression(node->returnType(), rv);
  Expr* whereClause = nullptr;

  Expr* lifetimeConstraints = nullptr;
  if (node->numLifetimeClauses() > 0) {
    if (trace) printf("Converting lifetime clause\n");
    // create a new AList for the lifetime clause
    pushNewBlock();

    for (auto clause : node->lifetimeClauses()) {
      Expr* convertedClause = convertLifetimeClause(clause, rv);
      INT_ASSERT(convertedClause);

      if (lifetimeConstraints == nullptr) {
        lifetimeConstraints = convertedClause;
      } else {
        lifetimeConstraints =
          new CallExpr(",", lifetimeConstraints, convertedClause);
      }
    }
    INT_ASSERT(lifetimeConstraints);

    popBlock();
  }

  setupFunctionDecl(fn, retTag, retType, node->throws(),
                    whereClause,
                    lifetimeConstraints,
                    // pass a body or not to set FLAG_NO_FN_BODY appropriately
                    node->body() ? fn->body : nullptr);

  if (node->linkage() != uast::Decl::DEFAULT_LINKAGE) {
    Flag linkageFlag = convertFlagForDeclLinkage(node);
    Expr* linkageExpr = convertExprOrNull(node->linkageName(), rv);

    // This thing sets the 'cname' if it's a string literal, attaches
    // some flags, sets the return type to 'void' if no type is
    // specified, and attaches a dummy formal for the C name (?).
    setupExternExportFunctionDecl(linkageFlag, linkageExpr, fn);
  }

  fn->retType = convertType(currentResolvedFunction->returnType().type());

  // visit the body to convert
  if (node->body()) {
    if (trace) printf("Converting body into %i\n", fn->body->id);
    pushBlock(fn->body);

    if (fn->retType != dtVoid) {
      // construct the RVV
      curRetVar = newTemp("ret", fn->retType);
      curRetVar->addFlag(FLAG_RVV);

      if (fn->retTag == RET_PARAM)      curRetVar->addFlag(FLAG_PARAM);
      if (fn->retTag == RET_TYPE)       curRetVar->addFlag(FLAG_TYPE_VARIABLE);
      if (fn->hasFlag(FLAG_MAYBE_TYPE)) curRetVar->addFlag(FLAG_MAYBE_TYPE);

      fn->insertAtHead(new DefExpr(curRetVar));
    }

    // construct the epilogue label
    epilogueLabel = new LabelSymbol(astr("_end_", fn->name));
    epilogueLabel->addFlag(FLAG_EPILOGUE_LABEL);
    // note: label is added to the AST later

    for (auto stmt : node->body()->stmts()) {
      astlocMarker markAstLoc(stmt->id());
      stmt->traverse(rv);
    }

    // add the epilogue label to the AST
    fn->insertAtTail(new DefExpr(epilogueLabel));

    // TODO: add deinit calls for the end-of-block actions for the function here

    // add the single return
    if (fn->retType == dtVoid) {
      fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    } else {
      fn->insertAtTail(new CallExpr(PRIM_RETURN, curRetVar));
    }

    popBlock();
  }

  simplifyEpilogue(fn);

  if (trace) {
    printf("converted to: ");
    nprint_view(fn);
    printf("\n");
  }

  // clear return variables so they can't be confused
  curRetVar = nullptr;
  epilogueLabel = nullptr;

  return false;
}
void TConverter::exit(const Function* node, RV& rv) {
  if (trace) printf("exit function %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Variable* node, RV& rv) {
  if (trace) printf("enter variable %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  VarSymbol* varSym = convertVariable(node, rv, true);
  INT_ASSERT(varSym);
  // note: convertVariable should have added the DefExpr and any associated
  // stuff to curAList.

  // Special handling for extern type variables.
  const bool isTypeVar = node->kind() == uast::Variable::TYPE;
  if (isTypeVar) {
    if (node->linkage() == uast::Decl::EXTERN) {
      CHPL_UNIMPL("extern type vars");
      /* TODO
      INT_ASSERT(!node->isConfig());
      INT_ASSERT(info.variableDef->sym && isVarSymbol(info.variableDef->sym));
      auto varSym = toVarSymbol(info.variableDef->sym);
      auto linkageName = node->linkageName() ? varSym->cname : nullptr;
      stmts = convertTypesToExtern(stmts, linkageName);

      // fix up convertedSyms since convertTypesToExtern
      // replaced the DefExpr/Symbol
      INT_ASSERT(stmts->body.last() && isDefExpr(stmts->body.last()));
      auto newDef = toDefExpr(stmts->body.last());
      noteConvertedSym(node, newDef->sym);*/
    }
  }

  return false;
}
void TConverter::exit(const Variable* node, RV& rv) {
  if (trace) printf("exit variable %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}



bool TConverter::enter(const Literal* node, RV& rv) {
  if (trace) printf("enter literal %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));


  Expr* se = untypedConverter->convertAST(node);
  curAList->insertAtTail(se);

  return false;
}
void TConverter::exit(const Literal* node, RV& rv) {
  if (trace) printf("exit literal %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Identifier* node, RV& rv) {
  if (trace) printf("enter identifier %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  const ResolvedExpression* re = rv.byAstOrNull(node);
  if (re != nullptr) {
    auto id = re->toId();
    if (!id.isEmpty()) {
      Symbol* sym = findOrCreateSym(id);
      SymExpr* se = new SymExpr(sym);
      curAList->insertAtTail(se);
      return false;
    }
  }

  CHPL_UNIMPL("identifier case not handled");

  return false;
}
void TConverter::exit(const Identifier* node, RV& rv) {
  if (trace) printf("exit identifier %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}


bool TConverter::enter(const Return* node, RV& rv) {
  if (trace) printf("enter return %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  CallExpr* ret = new CallExpr(PRIM_RETURN);

  curAList->insertAtTail(ret);
  enterCallActuals(ret);

  return true;
}
void TConverter::exit(const Return* node, RV& rv) {
  if (trace) printf("exit return %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  CallExpr* ret = exitCallActuals();

  // normalize returns to use the Return Value Variable (RVV)
  if (node->value() != nullptr) {
    Expr* retExpr = ret->get(1)->remove();
    CallExpr* move = nullptr;
    if (curFnSymbol->returnsRefOrConstRef()) {
      move = new CallExpr(PRIM_MOVE,
                          curRetVar, new CallExpr(PRIM_ADDR_OF, retExpr));
    } else {
      move = new CallExpr(PRIM_MOVE, curRetVar, retExpr);
    }
    ret->insertBefore(move);
  }

  // replace with GOTO(epilogue)
  ret->replace(new GotoStmt(GOTO_RETURN, epilogueLabel));
}

bool TConverter::enter(const Call* node, RV& rv) {
  if (trace) printf("enter call %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  if (node->id().str() == "dd@3") {
    gdbShouldBreakHere();
  }

  const resolution::ResolvedExpression* re = rv.byAstOrNull(node);

  if (auto primCall = node->toPrimCall()) {
    // there should not be associated actions for primitive calls
    CHPL_ASSERT(!re || !re->hasAssociatedActions());

    CallExpr* ret = new CallExpr(primCall->prim());
    curAList->insertAtTail(ret);

    convertActuals(ret, node, re, rv);
    handlePostCallActions(ret, node, re, rv);
    return false;
  }

  if (re == nullptr) {
    CHPL_UNIMPL("call with no ResolvedExpression");
    return false;
  }

  bool generateNew = false;
  const auto& candidate = re->mostSpecific().only();
  const TypedFnSignature* sig = candidate.fn();
  if (sig == nullptr) {
    // For 'new unmanaged C(...)' it should generate a call to a '_new'
    // function. The actuals are passed along to it.
    for (const auto& action : re->associatedActions()) {
      if (action.action() == AssociatedAction::NEW_INIT) {
        sig = action.fn();
        generateNew = true;
      }
    }
    if (sig == nullptr) {
      CHPL_UNIMPL("call with no most specific function");
      return false;
    }
  }

  const ResolvedFunction* resolvedFn = nullptr;
  if (sig->untyped()->idIsFunction()) {
    // TODO: do we need to handle nested functions differently here?
    chpl::resolution::ResolutionContext rcval(context);
    const PoiScope* poiScope = re->poiScope();
    resolvedFn = resolveFunction(&rcval, sig, poiScope);
  }

  if (resolvedFn == nullptr) {
    return false;
  }

  // Consider the return type of the called function.
  // Do we need to emit this call at all?
  if (!functionExistsAtRuntime(resolvedFn)) {
    // don't emit the call, but instead emit a SymExpr for the relevant
    // type or param.
    const auto& qt = resolvedFn->returnType();
    SymExpr* ret = nullptr;
    if (qt.isType()) {
      Type* t = convertType(qt.type());
      ret = new SymExpr(t->symbol);
    } else {
      Symbol* p = convertParam(qt);
      ret = new SymExpr(p);
    }
    curAList->insertAtTail(ret);
    return false;
  }

  Symbol* calledFn = nullptr;
  if (generateNew) {
    calledFn = findOrConvertNewWrapper(resolvedFn);
  } else {
    calledFn = findOrConvertFunction(resolvedFn);
  }

  // set up the call
  CallExpr* ret = new CallExpr(calledFn);
  curAList->insertAtTail(ret);

  convertActuals(ret, node, re, rv);

  return false;
}

void TConverter::exit(const Call* node, RV& rv) {
  if (trace) printf("exit call %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const AstNode* node, RV& rv) {
  if (trace) printf("enter ast %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
  return true;
}
void TConverter::exit(const AstNode* node, RV& rv) {
  if (trace) printf("exit ast %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

chpl::owned<UastConverter> createTypedConverter(chpl::Context* context) {
  return toOwned(new TConverter(context));
}
