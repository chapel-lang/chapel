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

#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/global-strings.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/util/assertions.h"
#include "chpl/util/string-escapes.h"

#include "convert-help.h"

#include "llvm/ADT/SmallPtrSet.h"

#include <iostream>

using namespace chpl;
using namespace resolution;
using namespace uast;

// converts resolved uAST + types to typed AST
struct TConverter final : UastConverter {

  /// Nested Types ///
  using RV = ResolvedVisitor<TConverter>;


  // helper for keeping track of converted functions
  struct ConvertedFunction {
    FnSymbol* fn;
    std::unordered_map<ID, Symbol*> vars;
  };

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


  /// Fields ///
  Context* context = nullptr;

  // What is the module / function we are converting?
  const AstNode* symbol = nullptr; // Module* or Function*

  // When converting a statement, where should we put new nodes?
  BlockStmt* curBlock = nullptr;

  bool trace = false;
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

  // keeps track of which block we are currently in the process of creating
  std::vector<BlockStmt*> blockStack;
  // this block is where we store any result created at the top-level
  BlockStmt* scratchSpaceBlock = nullptr;

  // to keep track of symbols that have been converted & fixups needed
  std::unordered_map<ID, ModuleSymbol*> modSyms;
  std::unordered_map<const ResolvedFunction*, ConvertedFunction> fns;
  std::unordered_map<ID, Symbol*> moduleScopeVars;

  std::vector<std::pair<SymExpr*, ID>> identFixups;
  std::vector<std::pair<SymExpr*, const ResolvedFunction*>> callFixups;

  // stores type fixups that are needed
  std::vector<Symbol*> typeFixups;

  // stores a mapping from chpl::Type* to Type*
  std::unordered_map<const types::Type*, Type*> convertedTypes;


  /// Methods ///

  TConverter(Context* context) : context(context) {
    SET_LINENO(rootModule);
    scratchSpaceBlock = new BlockStmt();
    curBlock = scratchSpaceBlock;

    untypedConverter = createUntypedConverter(context);
  }

  ~TConverter();

  // supporting UastConverter methods
  void setModulesToConvert(const std::vector<ID>& vec) override {
    modulesToConvert.clear();
    // save the vector
    modulesToConvertVec = vec;
    // add the modules to the set
    for (const ID& id : modulesToConvert) {
      modulesToConvert.insert(id);
    }

    // also tell the untyped converter about it
    untypedConverter->setModulesToConvert(vec);
  }

  void setFunctionsToConvertWithTypes(const CalledFnsSet& calledFns) override
  {
    functionsToConvertWithTypes = calledFns;
    // also tell the untyped converter about them so it can ignore them!
    untypedConverter->setFunctionsToConvertWithTypes(calledFns);
  }

  void useModuleWhenConverting(const chpl::ID& modId, ModuleSymbol* modSym) override {
    INT_FATAL("useModuleWhenConverting not expected for TConverter");
    modSyms[modId] = modSym;
  }

  Expr* convertAST(const AstNode* node) override;

  ModuleSymbol* convertToplevelModule(const Module* mod,
                                      ModTag modTag) override;

  void postConvertApplyFixups() override;


  // main entry points for converting
  void setupModulesToConvert();
  void convertFunctionsToConvert();

  ModuleSymbol* setupModule(ID modId);
  ModuleSymbol* findOrSetupModule(ID modId);

  void convertModuleInit(const Module* mod, ModuleSymbol* modSym);
  void convertFunction(const ResolvedFunction* r);

  // type conversion helpers
  Type* helpConvertType(const types::Type* t);
  Type* convertClassType(const types::ClassType* t);
  Type* convertCPtrType(const types::CPtrType* t);
  Type* convertEnumType(const types::EnumType* t);
  Type* convertExternType(const types::ExternType* t);
  Type* convertFunctionType(const types::FunctionType* t);
  Type* convertBasicClassType(const types::BasicClassType* t);
  Type* convertRecordType(const types::RecordType* t);
  Type* convertTupleType(const types::TupleType* t);
  Type* convertUnionType(const types::UnionType* t);
  Type* convertBoolType(const types::BoolType* t);
  Type* convertComplexType(const types::ComplexType* t);
  Type* convertImagType(const types::ImagType* t);
  Type* convertIntType(const types::IntType* t);
  Type* convertRealType(const types::RealType* t);
  Type* convertUintType(const types::UintType* t);

  // general functions for converting types/params
  Type* convertType(const types::Type* t);
  Symbol* convertParam(types::QualifiedType qt);


  // blockStack helpers
  BlockStmt* pushNewBlock() {
    auto newBlockStmt = new BlockStmt();
    blockStack.push_back(newBlockStmt);
    curBlock = newBlockStmt;
    return curBlock;
  }
  BlockStmt* pushBlock(BlockStmt* block) {
    blockStack.push_back(block);
    curBlock = block;
    return curBlock;
  }
  void popBlock() {
    CHPL_ASSERT(blockStack.size() > 0);
    blockStack.pop_back();
    if (blockStack.size() > 0)
      curBlock = blockStack.back();
    else
      curBlock = scratchSpaceBlock;
  }

  // methods to help track what has been converted & handle fixups
  void noteConvertedVariable(const uast::AstNode* ast, Symbol* sym);
  void noteIdentFixupNeeded(SymExpr* se, ID id);
  void noteCallFixupNeeded(SymExpr* se, const ResolvedFunction* r);
  void noteTypeFixupNeeded(Symbol* sym);

  void noteAllContainedFixups(BaseAST* ast, int depth);

  // helper methods
  Expr* errNotDirectly() {
    // call this from a visit method to indicate it shouldn't be called
    // directly; rather the processing of this form should happen in
    // something that contains it, syntactically.
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  bool shouldConvertWithoutTypes(const ResolvedFunction* r) {
    return functionsToConvertWithTypes.count(r) == 0;
  }

  Expr* convertExprOrNull(const AstNode* node) {
    if (node == nullptr)
      return nullptr;

    Expr* ret = convertAST(node);
    INT_ASSERT(ret);
    return ret;
  }

  // helpers
  Expr* convertLifetimeClause(const AstNode* node);
  CallExpr* convertLifetimeIdent(const Identifier* node);

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
  //FnSymbol* convertFunction(const Function* node);
  //FnSymbol* convertFunctionSignature(const FunctionSignature* node);
  //CallExpr* convertArrayType(const BracketLoop* node);
  //VariableDefInfo convertVariable(const Variable* node,
  //                         bool useLinkageName,
  //                         MultiDeclState* multiState = nullptr);
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

  // traversal cases to do something
  bool enter(const Module* node, RV& rv);
  void exit(const Module* node, RV& rv);

  bool enter(const Function* ast, RV& rv);
  void exit(const Function* ast, RV& rv);

  bool enter(const AstNode* node, RV& rv);
  void exit(const AstNode* node, RV& rv);
};

TConverter::~TConverter() { }

Expr* TConverter::convertAST(const AstNode* node) {
  CHPL_UNIMPL("convertAST");
  return nullptr;
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

  astlocMarker markAstLoc(mod->id());

  ModuleSymbol* modSym = findOrSetupModule(mod->id());

  // TODO: remove this block once the resolver is fully operational
  if (modSym->modTag != MOD_USER) {
    return untypedConverter->convertToplevelModule(mod, modTag);
  }

  topLevelModTag = modTag;
  convertModuleInit(mod, modSym);

  return modSym;
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
  bool operator()(const std::pair<const ResolvedFunction*, int>& a,
                  const std::pair<const ResolvedFunction*, int>& b) {
    // reverse sort by depth
    if (a.second > b.second) return true;
    if (a.second < b.second) return false;

    // regular sort by ResolvedFunction
    return *a.first < *b.first;
  }
};

void TConverter::convertFunctionsToConvert() {
  // Sort the called functions
  // 1. in descending order by depth (higher depths first)
  // 2. by ID

  auto v = std::vector<std::pair<const ResolvedFunction*, int>>(
                                       functionsToConvertWithTypes.begin(),
                                       functionsToConvertWithTypes.end());

  std::sort(v.begin(), v.end(), SortCalledFnsDeeperFirst());


  printf("Will convert functions in this order:\n");
  for (auto pair : v) {
    printf("  %s depth=%i\n", pair.first->id().str().c_str(), pair.second);
  }

  for (auto pair : v) {
    convertFunction(pair.first);
  }
}

ModuleSymbol* TConverter::setupModule(ID modId) {
  astlocMarker markAstLoc(modId);

  UniqueString path;
  bool found = context->filePathForId(modId, path);
  INT_ASSERT(found);

  // compute the modTag
  ModTag modTag = MOD_USER;
  if (chpl::parsing::filePathIsInInternalModule(context, path)) {
    modTag = MOD_INTERNAL;
  } else if (chpl::parsing::filePathIsInStandardModule(context, path)) {
    modTag = MOD_STANDARD;
  } else if (chpl::parsing::filePathIsInBundledModule(context, path)) {
    // TODO: this considers code in modules/packages as MOD_STANDARD but
    // we would like this to be MOD_USER.
    // See also issue #24998.
    modTag = MOD_STANDARD;
  }

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
  modSyms[modId] = modSym;

  // also tell the untyped converter about it
  untypedConverter->useModuleWhenConverting(modId, modSym);

  // This code does not add a DefExpr for the new module
  // * for toplevel modules, that happens in the code calling
  //   convertToplevelModule
  // * for submodules, that happens in the visit function for a Module.
  //

  return modSym;
}

ModuleSymbol* TConverter::findOrSetupModule(ID modId) {
  ModuleSymbol* modSym = modSyms[modId];
  if (modSym == nullptr) {
    // It can happen that it wasn't in modulesToConvert but we need to
    // convert it anyway. E.g. with PrintModuleInitOrder.
    modSym = setupModule(modId);
  }

  return modSym;
}

void TConverter::convertModuleInit(const Module* mod, ModuleSymbol* modSym) {
  printf("Converting module init for %s[%i]\n",
         mod->id().str().c_str(), modSym->id);

  // create the module init function
  modSym->initFn = new FnSymbol(astr("chpl__init_", modSym->name));
  modSym->initFn->retType = dtVoid;

  modSym->initFn->addFlag(FLAG_MODULE_INIT);
  modSym->initFn->addFlag(FLAG_INSERT_LINE_FILE_INFO);

  // add the init function to the module
  modSym->block->insertAtHead(new DefExpr(modSym->initFn));

  // TODO: handle module deinit functions

  // convert module-level statements into module's init function

  // prepare to traverse
  ID id = mod->id();
  const ResolutionResultByPostorderID& resolved = resolveModule(context, id);
  pushBlock(modSym->initFn->body);
  const AstNode* oldSymbol = symbol;
  symbol = mod;
  moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);
  currentResolvedFunction = nullptr;

  // traverse
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rv(&rcval, symbol, *this, resolved);
  symbol->traverse(rv);

  // tidy up after traversal
  symbol = oldSymbol;
  popBlock();
}

void TConverter::convertFunction(const ResolvedFunction* r) {
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
  const AstNode* oldSymbol = symbol;
  const ResolvedFunction* oldFn = currentResolvedFunction;
  symbol = fn;
  currentResolvedFunction = r;
  moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);

  // traverse
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rv(&rcval, symbol, *this, resolved);
  symbol->traverse(rv);

  // tidy up after traversal
  symbol = oldSymbol;
  currentResolvedFunction = oldFn;
  popBlock();
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
    case typetags::ClassType:   return convertClassType(t->toClassType());
    case typetags::EnumType:    return convertEnumType(t->toEnumType());
    case typetags::ExternType:  return convertExternType(t->toExternType());
    case typetags::FunctionType:return convertFunctionType(t->toFunctionType());

    case typetags::ArrayType:
       CHPL_UNIMPL("convert array type");
       return dtUnknown; // TODO
    case typetags::DomainType:
       CHPL_UNIMPL("convert domain type");
      return dtUnknown; // TODO

    case typetags::BasicClassType:
      return convertBasicClassType(t->toBasicClassType());

    case typetags::AnyClassType:    return dtAnyManagementNonNilable;
    case typetags::RecordType:      return convertRecordType(t->toRecordType());
    case typetags::TupleType:       return convertTupleType(t->toTupleType());
    case typetags::UnionType:       return convertUnionType(t->toUnionType());

    // primitive types
    case typetags::BoolType:     return convertBoolType(t->toBoolType());
    case typetags::ComplexType:  return convertComplexType(t->toComplexType());
    case typetags::ImagType:     return convertImagType(t->toImagType());
    case typetags::IntType:      return convertIntType(t->toIntType());
    case typetags::RealType:     return convertRealType(t->toRealType());
    case typetags::UintType:     return convertUintType(t->toUintType());
    case typetags::CPtrType:     return convertCPtrType(t->toCPtrType());

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
    case typetags::NUM_TYPE_TAGS:
      INT_FATAL("should not be reachable");
      return dtUnknown;

    // intentionally no default --
    // want a C++ compiler error if a case is missing in the above
  }
  INT_FATAL("should not be reached");
  return nullptr;
}

Type* TConverter::convertClassType(const types::ClassType* t) {
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

  CHPL_UNIMPL("Unhandled class type");
  return nullptr;
}

Type* TConverter::convertCPtrType(const types::CPtrType* t) {
  // find the C pointer type to instantiate
  AggregateType* base = t->isConst() ? dtCPointerConst : dtCPointer;

  // handle 'c_ptr' and 'c_ptrConst' without an element type
  if (t->eltType() == nullptr) {
    return base;
  }

  auto qt = types::QualifiedType(types::QualifiedType::TYPE, t);

  if (base->numFields() == 0) {
    // the proper AST for CPointer hasn't been created yet,
    // and we need it to proceed, so return a temporary conversion symbol.
    return new TemporaryConversionType(qt);
  }

  // otherwise, convert the element type
  Type* convertedEltT = convertType(t->eltType());
  if (isTemporaryConversionType(convertedEltT)) {
    // return a temporary conversion symbol since something is missing
    return new TemporaryConversionType(t);
  }

  // instantiate it with the element type
  // note: getInstantiation should re-use existing instantiations
  int index = 1;
  Expr* insnPoint = nullptr; // TODO: set this to something?
  AggregateType* ret =
    base->getInstantiation(convertedEltT->symbol, index, insnPoint);

  return ret;
}

Type* TConverter::convertEnumType(const types::EnumType* t) {
  CHPL_UNIMPL("convertEnumType");
  return nullptr;
}

Type* TConverter::convertExternType(const types::ExternType* t) {
  CHPL_UNIMPL("convertExternType");
  return nullptr;
}

Type* TConverter::convertFunctionType(const types::FunctionType* t) {
  CHPL_UNIMPL("convertExternType");
  return nullptr;
}

Type* TConverter::convertBasicClassType(const types::BasicClassType* t) {
  CHPL_UNIMPL("convertExternType");
  return nullptr;
}

Type* TConverter::convertRecordType(const types::RecordType* t) {
  if (t->isStringType()) {
    return dtString;
  } else if (t->isBytesType()) {
    return dtBytes;
  }

  std::string msg = "unhandled record type: ";
  msg += t == nullptr ? "(null)" : t->name().str();
  CHPL_UNIMPL(msg.c_str());
  return nullptr;
}

Type* TConverter::convertTupleType(const types::TupleType* t) {
  CHPL_UNIMPL("convertTupleType");
  return nullptr;
}

Type* TConverter::convertUnionType(const types::UnionType* t) {
  CHPL_UNIMPL("convertUnionType");
  return nullptr;
}

Type* TConverter::convertBoolType(const types::BoolType* t) {
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

Type* TConverter::convertComplexType(const types::ComplexType* t) {
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

Type* TConverter::convertImagType(const types::ImagType* t) {
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

Type* TConverter::convertIntType(const types::IntType* t) {
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

Type* TConverter::convertRealType(const types::RealType* t) {
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

Type* TConverter::convertUintType(const types::UintType* t) {
  return dtUInt[getUintSize(t)];
}

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




void TConverter::noteConvertedVariable(const uast::AstNode* ast, Symbol* sym) {

  if (currentResolvedFunction == nullptr) {
    moduleScopeVars[ast->id()] = sym;
  } else {
    fns[currentResolvedFunction].vars[ast->id()] = sym;
  }
}

Expr* TConverter::convertLifetimeClause(const uast::AstNode* node) {
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

CallExpr* TConverter::convertLifetimeIdent(const uast::Identifier* node) {
  astlocMarker markAstLoc(node->id());

  auto ident = node->toIdentifier();
  INT_ASSERT(ident);
  CallExpr* callExpr = new CallExpr(PRIM_LIFETIME_OF, convertExprOrNull(node));
  return callExpr;
}

bool TConverter::enter(const Module* node, RV& rv) {
  printf("enter %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  if (modulesToConvert.count(node->id()) == 0) {
    // this module should not be converted.
    return false;
  }

  if (node == symbol) {
    // we are visiting it to convert the module initialization function
    // so proceed with the traversal
    return true;
  } else {
    // it is a submodule.
    // Set up to convert the submodule's module init function
    // with a separate call to traverse.
    ModuleSymbol* modSym = findOrSetupModule(node->id());
    convertModuleInit(node, modSym);

    // add the DefExpr now (to match the untyped Converter)
    ID parentModuleId = parsing::idToParentModule(context, node->id());
    ModuleSymbol* parentModule = findOrSetupModule(parentModuleId);
    parentModule->block->insertAtHead(new DefExpr(modSym));

    return false;
  }

  return true;
}
void TConverter::exit(const Module* node, RV& rv) {
  printf("exit %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Function* node, RV& rv) {
  printf("enter %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));


  if (node != symbol) {
    // It's a function, but not the one we are working on.
    // Stop the traversal. This Function will be handled when
    // convertFunction is called.
    return false;
  }

  printf("Really converting Function\n");

  astlocMarker markAstLoc(node->id());

  CHPL_ASSERT(currentResolvedFunction != nullptr);

  FnSymbol* fn = new FnSymbol("_");
  curBlock->insertAtTail(new DefExpr(fn));

  // note the correspondence between the ResolvedFunction & what it converts to
  // (for calls, including recursive calls)
  fns[currentResolvedFunction].fn = fn;

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
  if (currentResolvedFunction->signature()->instantiatedFrom() != nullptr) {
    // generic instantiations are "invisible"
    fn->addFlag(FLAG_INVISIBLE_FN);
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
        }

      // A varargs formal.
      } else if (auto formal = decl->toVarArgFormal()) {
        INT_ASSERT(formal->name() != USTR("this"));
        conv = toDefExpr(convertAST(formal));
        INT_ASSERT(conv);

      // A tuple decl, where components are formals or tuple decls.
      /*} else if (auto formal = decl->toTupleDecl()) {
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

      // Attaches def to function's formal list.
      if (conv) {
        buildFunctionFormal(fn, conv);
        // Note the formal is converted so we can wire up SymExprs later
        noteConvertedVariable(decl, conv->sym);
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
  Expr* retType = nullptr;
  Expr* whereClause = nullptr;

  // TODO: handle runtime types for return type

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
    body = new BlockStmt();
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

  fn->retType = convertType(currentResolvedFunction->returnType().type());

  // visit the body to convert
  if (body) {
    pushBlock(body);
    node->body()->traverse(rv);
  }

  return false;
}
void TConverter::exit(const Function* node, RV& rv) {
  printf("exit %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  popBlock();
}

bool TConverter::enter(const AstNode* node, RV& rv) {
  printf("enter %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
  return true;
}
void TConverter::exit(const AstNode* node, RV& rv) {
  printf("exit %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

chpl::owned<UastConverter> createTypedConverter(chpl::Context* context) {
  return toOwned(new TConverter(context));
}
