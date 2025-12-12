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
#include "chpl/resolution/BranchSensitiveVisitor.h"
#include "chpl/resolution/can-pass.h"
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

// If defined as 1, dump debug output from the converter to stdout.
#define TC_DEBUG_TRACE 0

// Wrapper around 'CHPL_UNIMPL' that also calls 'debuggerBreakHere()'.
#define TC_UNIMPL(msg__) do { \
  CHPL_UNIMPL(msg__); \
  debuggerBreakHere(); \
} while (0)


using namespace chpl;
using namespace resolution;
using namespace uast;


//
// The 'typed converter' uses the fully resolved uAST provided by the 'dyno'
// frontend in order to generate typed AST used by the rest of the compiler
// that is equivalent to the AST produced at the end of the 'callDestructors'
// pass (with a few deliberate exceptions). It uses recursive functions to
// perform the conversion.
//
// One of the goals of this pass is to (for the functions generated here)
// completely sidestep the use of the legacy normalizer and resolver.
// This means that when deciding to use a legacy function, extra care should
// be taken to make sure that the function does not normalize or resolve.
//
// Right now this pass operates on a function-by-function basis. The untyped
// converter will be invoked on functions in standard/internal modules
// that are not explicitly represented in the call graph. Additionally, the
// functions and types that are generated here are always fully instantiated,
// and generated functions will not participate in legacy resolution.
//
// See private issue #6907 for a meta issue documenting this effort.
//
// A NOTE ABOUT 'FLAG_RESOLVED_EARLY'
//
// Note that the symbols produced by this pass will be marked with the flag
// 'FLAG_RESOLVED_EARLY'. What impact this has on the rest of the compiler
// depends on the symbol that is marked:
//
// -- Generally, for all symbols, it means that the symbol will _NOT_ be
//    normalized, as this pass does its best to ensure a normal form (based
//    on AST shape after the 'callDestructors' pass).
//
// -- Generally, for 'FnSymbol', this means that the symbol will not be
//    considered as a candidate by the old resolver when performing e.g.,
//    call resolution.
//
// -- Generally, for 'TypeSymbol' and 'Type', this means that the type will
//    be interpreted differently by the rest of the compiler. The types
//    that are generated here are considered to be concrete and
//    fully resolved, so the old compiler must be made aware of that when
//    inspecting them. In terms of number of adjustments we need to make to
//    the old compiler, types may end up being the most invasive.
//
// -- Generally, for default functions, we continue to let the old compiler
//    generate all of them eagerly. Technically, the frontend enables us to
//    lazily generate only the functions that are needed, but we cannot take
//    advantage of that until this pass generates all old AST.
//
struct TConverter final : UastConverter,
                          chpl::resolution::BranchSensitiveVisitor<chpl::resolution::DefaultFrame, ResolvedVisitor<TConverter>&> {

  /// ------------ ///
  /// Nested Types ///
  /// ------------ ///

  // The typed converter declares itself to be a 'resolved visitor'.
  using RV = ResolvedVisitor<TConverter>;

  // This struct is used to implement a debug printing macro below.
  struct DebugPrinter {
    const char* function_ = nullptr;
    int line_ = -1;

    DebugPrinter(const char* function, int line)
      : function_(function), line_(line) {
    }

    void operator()(TConverter* tc, const char* fmt, ...) {
      va_list vl;
      va_start(vl, fmt);
      vdebugf(tc, function_, line_, fmt, vl);
      va_end(vl);
    }

    static void vdebugf(TConverter* tc, const char* function, int line,
                        const char* fmt,
                        va_list vl) {
      std::printf("[%s:%d] ", function, line);
      std::vprintf(fmt, vl);
    }
  };

  // A "debug printf" macro that only prints if 'TC_DEBUG_TRACE' is non-zero.
  #if defined(TC_DEBUG_TRACE) && TC_DEBUG_TRACE != 0
    #define TC_DEBUGF DebugPrinter(__FUNCTION__, __LINE__)
  #else
    // Discard arguments if unused to prevent any 'unused variable' warnings.
    #define TC_DEBUGF(tc__, fmt__, ...) do { \
      if constexpr(false) std::ignore = std::make_tuple(__VA_ARGS__); \
    } while (0)
  #endif

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

  class ActualConverter;

  // This state is used to drive conversion of a module or function.
  // It is swapped in or out of the converter as needed in order to
  // convert different functions.
  struct ConvertedSymbolState {

    // The uAST function or module that we are currently converting.
    const AstNode* symbol = nullptr;

    // When converting functions, this is the AST for the 'FnSymbol*'.
    FnSymbol* fnSymbol = nullptr;

    // The AST for the module containing the symbol we are converting.
    ModuleSymbol* moduleSymbol = nullptr;

    // In functions, this AST is for the return variable (if it exists).
    Symbol* retVar = nullptr;

    // In functions, this AST is for the epilogue jump label (if it exists).
    LabelSymbol* epilogueLabel = nullptr;

    // When converting a uAST function, this is the 'ResolvedFunction*'.
    const ResolvedFunction* resolvedFunction = nullptr;

    // This will only be set when converting a call that has default values.
    ActualConverter* actualConverter = nullptr;

    // Set to 'true' if this module was loaded from a library file.
    bool moduleFromLibraryFile = false;

    // A mapping from 'dyno' symbols to old AST symbols for local variables.
    // TODO: This won't work for variables declared within a 'param' loop!
    std::unordered_map<ID, Symbol*> localSyms;

    // The current AST list, its owner, and if it is for a statement.
    std::vector<std::tuple<AList*, Expr*, bool>> aListStack;

    // To fetch the previous statement children list.
    std::vector<size_t> stmtListIdxStack;

    // Used to quickly determine if we are in a user context or not.
    ModTag topLevelModTag = MOD_USER;

    /// ------------------ ///
    /// aListStack helpers ///
    /// ------------------ ///

    void pushAList(AList* alist, Expr* owner, bool isStmtList) {
      int idx = (int) aListStack.size();
      aListStack.push_back({alist, owner, isStmtList});
      if (isStmtList) {
        stmtListIdxStack.push_back(idx);
      }
    }

    Expr* popAList() {
      INT_ASSERT(aListStack.size() > 0);

      Expr* ret = nullptr;
      auto [alist, owner, isStmtList] = aListStack.back();
      aListStack.pop_back();
      std::ignore = alist;
      ret = owner;

      if (isStmtList) {
        INT_ASSERT(!stmtListIdxStack.empty());
        stmtListIdxStack.pop_back();
      }

      return ret;
    }

    std::tuple<AList*, Expr*, bool>& fetchListStackEntry(bool fetchStmtList) {
      size_t idx = 0;

      if (fetchStmtList) {
        INT_ASSERT(!stmtListIdxStack.empty());
        idx = stmtListIdxStack.back();
        INT_ASSERT(0 <= idx && idx < aListStack.size());
      }

      auto& ret = fetchStmtList ? aListStack[idx] : aListStack.back();
      return ret;
    }

    AList* stmtList() {
      bool fetchStmtList = true;
      auto& tup = fetchListStackEntry(fetchStmtList);
      INT_ASSERT(std::get<bool>(tup) == fetchStmtList);
      return std::get<AList*>(tup);
    }

    AList* lastList() {
      bool fetchStmtList = false;
      auto& tup = fetchListStackEntry(fetchStmtList);
      return std::get<AList*>(tup);
    }

    void insertIntoStmtAtHead(Expr* e) {
      stmtList()->insertAtHead(e);
    }

    void insertIntoStmtAtTail(Expr* e) {
      stmtList()->insertAtTail(e);
    }

    void insertAtHead(Expr* e) {
      lastList()->insertAtHead(e);
    }

    void insertAtTail(Expr* e) {
      lastList()->insertAtTail(e);
    }
  };

  // When a formal has a default-argument value that is used, this type
  // will help to construct the value and pass it in at the callsite.
  class ActualConverter {
   public:
    // The first component is the converted expression. The second is
    // the actual temp, created on demand if needed while converting
    // default arguments.
    //
    // If the first slot is 'nullptr' then no actual was created for it,
    // which is possible if the actual was elided (e.g., the formal is
    // param or a type).
    //
    // If the second slot is 'nullptr' then no actual temp was needed.
    using ActualInfo = std::tuple<Expr*, Symbol*>;

   private:
    // The typed converter.
    TConverter* tc_ = nullptr;

    // Does this call require defaults at all?
    bool callRequiresDefaults_ = false;

    // The state of the called function. This is swapped on and off
    // the converter as needed in order to interleave conversion of
    // default argument values with conversion of call actuals.
    ConvertedSymbolState calledFnState_;

    // The signature of the called function.
    const TypedFnSignature* tfs_ = nullptr;

    // The uAST representing the call's location.
    const AstNode* astForCall_ = nullptr;

    // Collected actual uASTs. Do not have to be children of 'astForCall'.
    const std::vector<const AstNode*> actualAsts_;

    // The map driving conversion of the call actuals.
    const FormalActualMap& fam_;

    // The resolved visitor for the symbol containing the call.
    RV& rv_;

    // Mapping from a formal's ID to the 'FormalActual' slot.
    std::unordered_map<ID, Symbol*> formalIdToActualSymbol_;

    // Used to insert actual temps if needed.
    AList* stmtInsertionPoint_ = nullptr;

    // Extra setup done only if there are default arguments.
    void prepareCalledFnConversionState();

    // Convert a single actual considering 'fa'.
    Expr* convertActual(const FormalActual& fa);

    Expr* convertActualWithArg(const FormalActual& fa);
    Expr* convertActualUsingDefault(const FormalActual& fa);

   public:
    ActualConverter(TConverter* tc,
                    const AstNode* astForCall,
                    const std::vector<const AstNode*>& actualAsts,
                    const resolution::TypedFnSignature* tfs,
                    const FormalActualMap& fam,
                    RV& rv);

    // Insert converted actuals into the given 'CallExpr'.
    void convertAndInsertActuals(CallExpr* call, bool skipReceiver);

    int expandTupleHelper(CallExpr* call,
                          const AstNode* actualAst,
                          Expr* tupleExpr);

    // When converting a default argument, this is called to replace a
    // reference to a formal with a reference to an actual if needed.
    Symbol* interceptFormalUse(const ID& id);
  };

  /// ------ ///
  /// Fields ///
  /// ------ ///

  bool genDefaultFieldAccessorWrapperFns = false;
  bool genDefaultArgumentValueWrapperFns = false;

  #if defined(TC_DEBUG_TRACE) && TC_DEBUG_TRACE != 0
    static constexpr bool trace = true;
  #else
    static constexpr bool trace = false;
  #endif

  // The 'dyno' compiler context.
  Context* context = nullptr;

  // State for the current function or module being converted.
  ConvertedSymbolState cur;

  bool haveSetupModules = false;
  bool haveConvertedFunctions = false;

  // the untyped converter (temporarily, this is used for non-user code)
  owned<UastConverter> untypedConverter;

  // which modules / submodules to convert
  std::vector<ID> modulesToConvertVec;
  std::unordered_set<ID> modulesToConvert;

  // which functions to convert with types
  CalledFnsSet functionsToConvertWithTypes;

  // the main module
  ID mainModuleId;

  // this block is where we store any result created at the top-level
  BlockStmt* scratchSpaceBlock = nullptr;

  // TODO: Can we just use 'scratchSpaceBlock' for this?
  AList* globalInsertionPoint = nullptr;

  //
  // to keep track of symbols that have been converted & fixups needed
  //

  // Stores functions, modules, module-scope vars, and does not need to
  // be cleaned up when 'ConvertedSymbolState' is changed.
  std::map<ID, Symbol*> globalSyms;

  // this one handles functions in a way that allows for instantiation
  std::unordered_map<const ResolvedFunction*, FnSymbol*> fns;
  std::unordered_map<const ResolvedFunction*, FnSymbol*> newWrappers;
  std::unordered_map<const types::Type*, FnSymbol*> chplTupleInit;

  // Stores a mapping from chpl::Type* to (bool, Type*), where the boolean
  // indicates whether the type has been converted yet. If 'false', then it is
  // int he process of being converted.
  std::unordered_map<const types::Type*, std::pair<bool, Type*>> convertedTypes;

  // so that TConverter can process one module (or function) at a time,
  // encountering a submodule, if it is to be converted, add it to this
  // list for processing just after the current module.
  std::vector<const uast::Module*> submodulesEncountered;

  // Handles for standard/internal modules (these are not global
  // variables in case we'd like to remove those somewhow later).
  ModuleSymbol* modChapelBase = nullptr;
  ModuleSymbol* modChapelTuple = nullptr;
  ModuleSymbol* modIO = nullptr;

  /// ------- ///
  /// Methods ///
  /// ------- ///

  TConverter(Context* context) : context(context) {
    SET_LINENO(rootModule);
    globalInsertionPoint = &theProgram->block->body;
    untypedConverter = createUntypedConverter(context);
    scratchSpaceBlock = new BlockStmt();
    setupEssentialModuleGlobalVars();
  }

  ~TConverter();

  void setupEssentialModuleGlobalVars() {
    using Entry = std::tuple<ModuleSymbol**, ModuleSymbol**, const char*>;

    std::vector<Entry> v {
      Entry { &modChapelBase,  &baseModule,    "ChapelBase"  },
      Entry { &modChapelTuple, nullptr,        "ChapelTuple" },
      Entry { &modIO,          &ioModule,      "IO"          },
    };

    for (auto [ptr1, ptr2, str] : v) {
      INT_ASSERT(ptr1);
      if (*ptr1) continue;

      auto name = ustr(str);

      // Since this occurs before the call graph is built, make sure the
      // file path for the module has been mapped by parsing it.
      auto uastMod = parsing::getToplevelModule(context, name);

      // The module should exist and be top-level. TODO: Minimal modules?
      INT_ASSERT(uastMod);

      auto mod = findOrSetupModule(ID(name));
      INT_ASSERT(mod);

      // TODO: Handle collisions if necessary...
      INT_ASSERT(mod->modTag == MOD_INTERNAL || mod->modTag == MOD_STANDARD);

      // Set the 'TConverter' field pointer.
      *ptr1 = mod;

      // Set the global variable pointer if it exists.
      if (ptr2) *ptr2 = mod;
    }
  }

  // supporting UastConverter methods
  void setModulesToConvert(const std::vector<ID>& vec) override {
    TC_DEBUGF(this, "have %i modules\n", (int)vec.size());

    modulesToConvert.clear();
    // save the vector
    modulesToConvertVec = vec;
    // add the modules to the set
    for (const ID& id : modulesToConvertVec) {
      TC_DEBUGF(this, "Will convert module %s\n", id.str().c_str());
      modulesToConvert.insert(id);
    }

    // also tell the untyped converter about it
    untypedConverter->setModulesToConvert(vec);
  }

  void setFunctionsToConvertWithTypes(const CalledFnsSet& calledFns) override {
    TC_DEBUGF(this, "have %i functions\n", (int)calledFns.size());
    functionsToConvertWithTypes = calledFns;
  }

  void setSymbolsToIgnore(std::unordered_set<ID> ignore) override {
    INT_FATAL("TConverter::setSymbolsToIgnore not implemented");
  }
  void eraseSymbolToIgnore(ID ignore) override {
    INT_FATAL("TConverter::eraseSymbolToIgnore not implemented");
  }

  void setMainModule(ID mainModule) override {
    TC_DEBUGF(this, "have %s\n", mainModule.str().c_str());
    mainModuleId = mainModule;
  }

  void useModuleWhenConverting(const chpl::ID& modId,
                               ModuleSymbol* modSym) override {
    INT_FATAL("useModuleWhenConverting not expected for TConverter");
    globalSyms[modId] = modSym;
  }

  // This should not be called. To use untyped conversion, call below.
  Expr* convertAST(const AstNode* node) override {
    INT_ASSERT(false && "Should not be called here!");
    return nullptr;
  }

  Expr* convertAST(const AstNode* node, ModTag modTag) override {
    INT_ASSERT(false && "Should not be called here!");
    return nullptr;
  }

  Expr* convertAstUntyped(const AstNode* node) {
    return untypedConverter->convertAST(node);
  }

  Expr* convertExpr(const AstNode* node, RV& rv,
                    types::QualifiedType* outQt=nullptr);

  Expr* convertExprOrNull(const AstNode* node, RV& rv,
                          types::QualifiedType* outQt=nullptr) {
    if (node == nullptr) return nullptr;
    return convertExpr(node, rv, outQt);
  }

  UniqueString ustr(const char* str) {
    return UniqueString::get(context, str);
  }

  UniqueString ustr(const std::string& str) {
    return UniqueString::get(context, str);
  }

  // BranchSensitiveVisitor methods
  const chpl::types::Param* determineWhenCaseValue(const uast::AstNode* ast,
                                                   RV& rv) override {
    if (auto action = rv.byAst(ast).getAction(AssociatedAction::COMPARE)) {
      return action->type().param();
    }

    return nullptr;
  }
  const types::Param* determineIfValue(const uast::AstNode* ast,
                                       RV& rv) override {
    if (auto rr = rv.byAst(ast); rr.type().kind() == types::QualifiedType::PARAM) {
      return rr.type().param();
    }
    return nullptr;
  }
  void traverseNode(const uast::AstNode* ast, RV& rv) override {
    ast->traverse(rv);
  }



  /// -------------------------- ///
  /// Symbol and Type Conversion ///
  /// -------------------------- ///

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
  FnSymbol* convertNewWrapper(const ResolvedFunction* initFn);
  ArgSymbol* convertFormal(const Formal* node, RV& rv);
  void convertAndInsertVarArgs(const VarArgFormal* node, RV& rv);

  FnSymbol* findOrConvertFunction(const ResolvedFunction* r);
  FnSymbol* findOrConvertNewWrapper(const ResolvedFunction* initFn);
  FnSymbol* findOrConvertTupleInit(const types::TupleType* tt);

  // This gadget will resolve a _frontend-visible_ function using 'ci'
  // and produce the lowered 'FnSymbol*' to use in code generation.
  // If the function does not exist in the original source code or the
  // frontend is not otherwise aware of it, then it will not be found!
  //
  // If 'outRf' is a non-null pointer then it will be set with info
  // about the function that was used.
  FnSymbol*
  convertFunctionForGeneratedCall(const resolution::CallInfo& ci,
                                  const uast::AstNode* inAst,
                                  const ResolvedFunction** outRf=nullptr);

  void createMainFunctions() override;

  ArgSymbol* findOrCreateFormal(const Formal* node);
  VarSymbol* findOrCreateVar(const Variable* node);
  VarSymbol* findOrCreateHiddenVarArg(const VarArgFormal* node, RV& rv);
  Symbol* findConvertedSym(const ID& id);
  Symbol* findOrCreateSym(const ID& id, RV& rv);

  /* Look up a type in the converted types map. Does not convert. */
  Type* findConvertedType(const types::Type* t);
  bool typeIsConverted(const types::Type* t);

  // Converts a type. If it's an AggregateType, it will also fill in the
  // fields, but only after creating a dummy AggregateType for the map
  // that can be used in the case of recursive data structures.
  Type* convertType(const types::Type* t);

  void noteConvertedSym(const uast::AstNode* ast, Symbol* sym) override;
  void noteConvertedFn(const resolution::TypedFnSignature* sig, FnSymbol* fn) override;

  Symbol* convertParam(const types::QualifiedType& qt);

  // note: the relevant calls and DefExpr are added to the current statement
  // children list. The variable is not yet inserted into the tree, though.
  // This function returns the 'VarSymbol*'.
  Symbol* convertVariable(const uast::Variable* node,
                          RV& rv,
                          bool useLinkageName,
                          MultiDeclState* multiState = nullptr,
                          Expr* initExprOverride = nullptr);

  /// --------------------- ///
  /// Tree Mutation Helpers ///
  /// --------------------- ///

  // When called, will insert 'e' as a child of the current statement AST.
  void insertStmt(Expr* e) {
    cur.insertIntoStmtAtTail(e);
  }

  // When called, will insert 'e' as a child of the the immediate parent AST.
  // This can be a statement or expression.
  void insertExpr(Expr* e) {
    cur.insertAtTail(e);
  }

  BlockStmt* pushNewBlock() {
    auto newBlockStmt = new BlockStmt();
    bool isStmtList = true;
    cur.pushAList(&newBlockStmt->body, newBlockStmt, isStmtList);
    return newBlockStmt;
  }

  BlockStmt* pushBlock(BlockStmt* block) {
    bool isStmtList = true;
    cur.pushAList(&block->body, block, isStmtList);
    return block;
  }

  BlockStmt* popBlock() {
    return toBlockStmt(cur.popAList());
  }

  void enterCallActuals(CallExpr* call) {
    bool isStmtList = false;
    cur.pushAList(&call->argList, call, isStmtList);
  }

  CallExpr* exitCallActuals() {
    return toCallExpr(cur.popAList());
  }

  void enterFormals(FnSymbol* fn) {
    bool isStmtList = false;
    cur.pushAList(&fn->formals, nullptr, isStmtList);
  }

  void exitFormals(FnSymbol* fn) {
    cur.popAList();
  }

  /// ---------- ///
  /// Predicates ///
  /// ---------- ///

  bool shouldElideFormal(const TypedFnSignature* tfs, int formalIdx);
  bool typeExistsAtRuntime(const types::Type* t);
  bool functionExistsAtRuntime(const ResolvedFunction* r);

  /// --------------------- ///
  /// Expression Conversion ///
  /// --------------------- ///


  Expr* placeholder(const char* function, int line) {
    if constexpr(trace) {
      DebugPrinter(function, line)(this, "Generating placeholder AST!");
    }

    auto var = new_IntSymbol(0, INT_SIZE_64);
    auto ret = new SymExpr(var);
    return ret;
  }

  // If you are converting and reach an error case, use this macro to generate
  // _some_ expression instead of nothing at all.
  //
  // In most cases, the frontend should have emitted errors for malformed
  // programs when it was used to construct the call-graph, which occurs
  // before the typed-converter traversal runs. However, in some cases errors
  // will not be caught until the converter executes. This can occur when the
  // converter injects calls when generating that the frontend did not
  // account for.
  //
  // In such cases, we want to continue generating as much AST as possible
  // without calling '..._FATAL' or crashing unexpectedly, so call this
  // macro instead. It returns an integer literal, which is fine because
  // the frontend will emit errors, so the correctness of the generated AST
  // does not matter.
  #define TC_PLACEHOLDER(tc__) (tc__->placeholder(__FUNCTION__, __LINE__))

  Expr* elided(const char* function, int line, const AstNode* node) {
    if constexpr(trace) {
      DebugPrinter(function, line)(this, "Eliding call at %s!", node->id().str().c_str());
    }

    return new CallExpr(PRIM_NOOP);
  }

  #define TC_ELIDED(tc__, node) (tc__->elided(__FUNCTION__, __LINE__, node))

  // Helper that constructs AST to produce the default value for a type.
  //
  // The argument 'pin' is an uAST node used to ground any generated calls
  // needed to resolve and construct the default value. If it is 'nullptr',
  // then the currently converted symbol will be used instead. In addition
  // to providing context about the default value, 'pin' is also used to
  // retrieve information such as the POI for any necessary calls.
  //
  // The argument 'rv' is the 'ResolvedVisitor' used to convert the current
  // symbol in the typed converter. It must be provided and must make sense
  // for the uAST 'pin' (if that was provided).
  Expr* defaultValueForType(const types::Type* t, const AstNode* pin, RV& rv);

  // Create a new temporary. The type used for it must be supplied.
  Symbol* makeNewTemp(const types::QualifiedType& qt, bool insertDef=true);

  // Store 'e' in a temporary if it does not already refer to one. The type
  // for the temporary must be provided and cannot easily be retrieved from
  // the converted expression.
  //
  // TODO: Can we provide a custom function to retrieve 'e->qualType()'
  // without performing legacy resolution? Then we can remove the 'qt'
  // requirement, which would mean types no longer need to map 1-to-1.
  SymExpr* storeInTempIfNeeded(Expr* e, const types::QualifiedType& qt);
  SymExpr* insertDerefTemp(SymExpr* se, const types::QualifiedType& qt);

  Expr* convertLifetimeClause(const AstNode* node, RV& rv);

  CallExpr* convertLifetimeIdent(const Identifier* node, RV& rv);

  // Only type expressions that are runtime types need to be converted
  // as code; otherwise we can forget the type expression and just use the type.
  // Accordingly, this returns 'nullptr' if the type expression isn't
  // a type with a runtime component.
  Expr* convertRuntimeTypeExpression(const AstNode* node, RV& rv);

  void simplifyEpilogue(FnSymbol* fn);

  ::Qualifier convertQualifier(types::QualifiedType::Kind kind);

  IntentTag convertFormalIntentQt(types::QualifiedType::Kind kind);

  void setVariableType(const uast::VarLikeDecl* v, Symbol* sym, RV& rv);

  // Try to convert a primitive call.
  Expr* convertPrimCallOrNull(const Call* node, RV& rv);
  SymExpr* insertClassConversion(types::QualifiedType to,
                              types::QualifiedType from,
                              const AstNode* astForErr,
                              Expr* fromExpr,
                              RV& rv);

  void convertImplicitInit(const types::CompositeType* ct, ID id, RV& rv);

  // Convert move-init expressions
  Expr* convertMoveInitAssignOrNull(const Call* node, RV& rv);
  Expr* convertEnumIntegralCastOrNull(const Call* node, RV& rv);

  // Try to convert a 'new' expression (which is considered a special
  // form of call) into a call to a middle-end only '_new' wrapper.
  Expr* convertNewCallOrNull(const Call* node, RV& rv);

  // Try to convert a tuple value e.g., '(x, y)'.
  Expr* convertTupleCallOrNull(const Call* node, RV& rv);

  // Try to convert a regular named call that maps to a 'TypedFnSignature'.
  // This branch is taken if the 'ResolvedExpression' for 'node' maps
  // cleanly to a single 'TypedFnSignature'. This branch may still be taken
  // if the call does not map to a signature but is an intrinsic (a call to
  // a function that the compiler has opted to implement directly), or if
  // the call appears to be a call to a named function but is actually
  // handled directly as a language feature (e.g., field initialization).
  Expr* convertNamedCallOrNull(const Call* node, RV& rv);

  // Try to convert a field access expression. Accepts both 'Identifier' and
  // 'Dot' uAST and can also handle implicit method receivers. This branch
  // will only consider expressions which semantically appear to be field
  // access. In some cases, the resolved expression may map to a parenless
  // call (to e.g., a compiler-generated field accessor). Whether or not the
  // accessor is invoked depends on how the converter is configured.
  Expr* convertFieldAccessOrNull(const AstNode* node, RV& rv);

  // Try to convert a parenless call. This branch accepts both 'Identifier'
  // as well as 'Dot' uAST and can also handle implicit method receivers.
  // This branch is only invoked if there is a clear mapping between the
  // given expression and a called function. It will not handle field access.
  Expr* convertParenlessCallOrNull(const AstNode* node, RV& rv);

  // Try to convert field init points e.g., 'this.x = x' in an initializer.
  Expr* convertFieldInitOrNull(const Call* node,
                               const ResolvedExpression* re,
                               const std::vector<const AstNode*>& actualAsts,
                               const resolution::CallInfo& ci,
                               RV& rv);

  // Try to convert the pattern 'tuple[0]'.
  Expr* convertIntrinsicTupleIndexingOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv);

  // The frontend may decide to handle casts for several types intrinsically,
  // such as casts for primitive types or strings. If the cast expression
  // produces a 'QualifiedType' but does not have a 'TypedFnSignature' set,
  // then it is probably an intrinsic.
  Expr* convertIntrinsicCastOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv);

  // Logical short-circuiting for the bool '||' and '&&' operators.
  Expr* convertIntrinsicLogicalOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv);

  // Primitive assignment between extern types
  Expr* convertExternAssignmentOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv);

  // Support '...' tuple expansion
  Expr* convertTupleExpand(const Call* node,
                           const ResolvedExpression* re,
                           const std::vector<const AstNode*>& actualAsts,
                           const resolution::CallInfo& ci,
                           RV& rv);

  // Support for e.g., ``(a, b) = fn()``
  Expr* convertGroupedAssign(const Call* node,
                             const ResolvedExpression* re,
                             const std::vector<const AstNode*>& actualAsts,
                             const resolution::CallInfo& ci,
                             RV& rv);

  // Try to elide a call to a specific signature at compile-time. If so,
  // generate and return a NOOP. Sets 'outRf' to the results of the
  // lookup process for 'sig' if 'outRf' is a non-null pointer.
  Expr* paramElideCallOrNull(const TypedFnSignature* sig,
                             const PoiScope* poiScope,
                             const ResolvedFunction** outRf=nullptr);

  // Loop over actuals and convert them. Does not handle pass-by-name.
  template <typename Iterable>
  void convertAndInsertPrimCallActuals(CallExpr* call,
                                       const Iterable& actuals,
                                       RV& rv);

  // For a given AST, convert and insert actuals into the lowered call 'c'.
  //
  // This process is driven by the 'FormalActualMap' 'fam', so 'fam' must
  // have the correct shape to ensure proper conversion (it is responsible
  // for handling pass-by-name).
  //
  // The vector 'actualAsts' should contain the ASTs that are apparent in
  // the call. Implicit receivers are handled by considering the signature
  // 'tfs' and may rely on the currently converted symbol being set (e.g.,
  // 'cur.fnSymbol').
  //
  // Actual conversion handles generation of default-argument values, but
  // this is only possible if a 'TypedFnSignature' 'tfs' is supplied for
  // the function being called.
  void convertAndInsertActuals(CallExpr* c,
                               const AstNode* node,
                               const std::vector<const AstNode*>& actualAsts,
                               const resolution::TypedFnSignature* tfs,
                               const FormalActualMap& fam,
                               RV& rv,
                               bool skipReceiver=false);

  // This handles tasks such as e.g., inserting 'deinit' calls at the
  // end of the current statement. TODO: Need to wire this up to occur
  // at end of statement and block.
  void handlePostCallActions(CallExpr* c,
                             const Call* node,
                             const resolution::ResolvedExpression* re,
                             RV& rv);

  // These functions help the converter generate field accesses. Note that
  // these are "plain-old" field accesses and not calls to parenless field
  // accessor functions.
  Expr* codegenGetField(const AstNode* recvAst, const char* fieldName, RV& rv,
                        types::QualifiedType* outFieldQt=nullptr);
  Expr* codegenGetField(const AstNode* recvAst, int fieldIndex, RV& rv,
                        types::QualifiedType* outFieldQt=nullptr);
  Expr* codegenGetField(Expr* recv, const types::QualifiedType& qtRecv,
                        const char* fieldName, RV& rv,
                        types::QualifiedType* outFieldQt=nullptr);
  Expr* codegenGetField(Expr* recv, const types::QualifiedType& qtRecv,
                        int fieldIndex, RV& rv,
                        types::QualifiedType* outFieldQt=nullptr);

  Expr* codegenImplicitThis(RV& rv);

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
  bool enter(const New* ast, RV& rv) { return false; }
  void exit(const New* ast, RV& rv) { }

  // traversal cases to do something
  bool enter(const Module* node, RV& rv);
  void exit(const Module* node, RV& rv);

  bool enter(const Function* node, RV& rv);
  void exit(const Function* node, RV& rv);

  bool enter(const Variable* node, RV& rv);
  void exit(const Variable* node, RV& rv);

  bool enter(const TupleDecl* node, RV& rv);
  void exit(const TupleDecl* node, RV& rv);

  bool enter(const Literal* node, RV& rv);
  void exit(const Literal* node, RV& rv);

  bool enter(const Dot* node, RV& rv);
  void exit(const Dot* node, RV& rv);

  bool enter(const Identifier* node, RV& rv);
  void exit(const Identifier* node, RV& rv);

  bool enter(const Return* node, RV& rv);
  void exit(const Return* node, RV& rv);

  bool enter(const Call* node, RV& rv);
  void exit(const Call* node, RV& rv);

  bool enter(const Conditional* node, RV& rv);
  void exit(const Conditional* node, RV& rv);

  bool enter(const ExternBlock* node, RV& rv);
  void exit(const ExternBlock* node, RV& rv);

  bool enter(const Select* node, RV& rv);
  void exit(const Select* node, RV& rv);

  bool enter(const Block* node, RV& rv);
  void exit(const Block* node, RV& rv);

  bool enter(const Range* node, RV& rv);
  void exit(const Range* node, RV& rv);

  bool enter(const While* node, RV& rv);
  void exit(const While* node, RV& rv);

  bool enter(const For* node, RV& rv);
  void exit(const For* node, RV& rv);

  bool enter(const Init* node, RV& rv);
  void exit(const Init* node, RV& rv);

  bool enter(const AstNode* node, RV& rv);
  void exit(const AstNode* node, RV& rv);
};

TConverter::~TConverter() { }

// TODO: Do we need this recursion or can we snip the entire sub-tree if the
// root is a non-runtime type? That would speed things up, but need to see.
static const types::Type*
typeIfAllSubExprsAreStaticTypes(TConverter* tc, const AstNode* node,
                          TConverter::RV& rv) {
  if (auto re = rv.byAstOrNull(node)) {
    auto& qt = re->type();
    if (qt.isType() && qt.hasTypePtr() && !qt.isUnknownOrErroneous()) {
      auto ret = qt.type();
      if (tc->typeExistsAtRuntime(ret)) return nullptr;
      for (auto ast : node->children()) {
        if (!typeIfAllSubExprsAreStaticTypes(tc, ast, rv)) return nullptr;
      }
      return ret;
    }
  }
  return nullptr;
}

Expr* TConverter::convertExpr(const AstNode* node, RV& rv,
                              types::QualifiedType* outQt) {
  astlocMarker markAstLoc(node->id());

  Expr* ret = nullptr;

  // Look for cases where we can evaluate immediately without traversing.
  if (auto re = rv.byAstOrNull(node)) {
    auto& qt = re->type();

    if (qt.isParam()) {
      // We can fold all 'param' expressions away immediately.
      auto sym = convertParam(qt);
      INT_ASSERT(sym);
      ret = new SymExpr(sym);

    } else if (auto t = typeIfAllSubExprsAreStaticTypes(this, node, rv)) {
      // The entire sub-tree is a non-runtime type, so we can convert it.
      auto convType = convertType(t);
      INT_ASSERT(convType);
      ret = new SymExpr(convType->symbol);
    }
  }

  if (!ret) {
    // Otherwise, traverse the AST. Results will be stored in the last list.
    node->traverse(rv);
    INT_ASSERT(!cur.lastList()->empty());
    ret = cur.lastList()->last()->remove();
  }

  // We should always have some AST at this point.
  INT_ASSERT(ret);

  if (outQt) {
    // Set the expression's type if it was requested.
    if (auto re = rv.byAstOrNull(node)) *outQt = re->type();
  }

  return ret;
}

ModuleSymbol* TConverter::convertModule(const Module* mod) {
  astlocMarker markAstLoc(mod->id());

  ModuleSymbol* modSym = findOrSetupModule(mod->id());

  // TODO: remove this block once the resolver is fully operational
  if (modSym->modTag != MOD_USER) {
    // Search for module scope variables in internal/standard modules and
    // convert them using the typed converter. Otherwise they would not be
    // converted because we currently use the untyped converter for these
    // modules.
    //
    // We identify these variables by checking for module-scope variables
    // without a defPoint. This indicates that we needed a SymExpr for a use
    // of a variable, but haven't actually converted it yet.
    //
    // For non-extern module-scope variables we will create a copy of the
    // variable with a "_dyno_" prefix and a function that initializes the
    // value. We do this to get along with the production resolver more easily,
    // by allowing it to resolve the original variable normally, while the
    // type-converted AST can use the copy. We create a function to handle
    // initialization because otherwise normalization of the module would
    // unhelpfully modify the resolved AST, and normalization skips resolved
    // functions.
    for (auto pair: globalSyms) {
      auto sym = pair.second;
      if (!parsing::idIsModule(context, pair.first) &&
          mod->id().contains(pair.first) &&
          sym->defPoint == nullptr &&
          parsing::idIsModuleScopeVar(context, pair.first)) {
        auto node = parsing::idToAst(context, pair.first)->toVariable();

        // Found a global variable referred to by some other code
        // For now, this should only happen with internal or standard modules.
        INT_ASSERT(modSym->modTag != MOD_USER);

        // Extern variables need to use the same name.
        if (node->linkage() != uast::Variable::EXTERN) {
          sym->name = astr("_dyno_", sym->name);
          sym->addFlag(FLAG_NO_INIT);

          // Let the untyped converter convert the variable normally, so that
          // it can be used with code still relying on the production resolver.
          untypedConverter->eraseSymbolToIgnore(pair.first);
        } else {
          untypedConverter->noteConvertedSym(node, sym);
        }

        // Create an initialization function where the variable's converted
        // initialization AST will be inserted.
        FnSymbol* initFn = new FnSymbol((astr("_init_", sym->name)));
        initFn->retType = dtVoid;

        ConvertedSymbolState saved = cur;
        cur = {};
        cur.moduleSymbol = modSym;
        cur.symbol = mod;
        cur.moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);
        pushBlock(initFn->body);

        ResolutionContext rcval(context);
        auto rr = resolveModuleStmt(context, pair.first);
        ResolvedVisitor<TConverter> rv(&rcval, cur.symbol, *this, rr);
        convertVariable(node, rv, true);
        popBlock();

        cur = saved;

        // Extern variables don't require initialization, and only need to be
        // converted to get the correct type.
        if (node->linkage() != uast::Variable::EXTERN) {
          modSym->block->insertAtTail(new DefExpr(initFn));
          modSym->block->insertAtTail(new CallExpr(initFn));
        }
      }
    }

    return untypedConverter->convertToplevelModule(mod, modSym->modTag);
  }

  // convert the module init while gathering submodules
  submodulesEncountered.clear();
  convertModuleInit(mod, modSym);

  // Additionally, convert any submodules encountered.
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

  cur.topLevelModTag = modTag;
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
    TC_DEBUGF(this, "Will convert functions in this order:\n");
    for (auto pair : v) {
      TC_DEBUGF(this, "  %s depth=%i index=%i\n",
                pair.first->id().str().c_str(),
                pair.second.depth,
                pair.second.index);
    }
  }

  for (auto pair : v) {
    if (fns.find(pair.first) == fns.end()) {
      convertFunction(pair.first);
    }
  }

  // Create 'chpl_gen_main()' as well as an empty 'main()' function if needed.
  // If the user defined 'main(), then it will have been added to the call
  // graph and converted in the above loop. If we are just resolving then do
  // not bother with this step.
  if (!fDynoResolveOnly) createMainFunctions();

  // tell the untyped converter about which functions to ignore
  // this is whatever functions we have converted already here
  std::unordered_set<ID> ignore;
  for (auto pair: fns) {
    const ResolvedFunction* fn = pair.first;
    // Allow methods to be resolved by the production resolver, so that we can
    // avoid having to match calling conventions while the converter is in
    // development.
    if (parsing::idIsInBundledModule(context, fn->id()) &&
        fn->signature()->isMethod()) continue;

    // Need to leave generic functions alone, so production can use them
    if (fn->signature()->instantiatedFrom() != nullptr) continue;

    ignore.insert(fn->id());

    auto ast = parsing::idToAst(context, fn->id());
    untypedConverter->noteConvertedSym(ast,
                                 pair.second);
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

  if (parsing::idIsToplevelModule(context, modId)) {
    // Make sure that the module is inserted into 'theProgram'. This
    // call does nothing if the module is already inserted.
    ModuleSymbol::addTopLevelModule(modSym);
  }

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
  TC_DEBUGF(this, "Converting module init for %s[%i]\n",
            mod->id().str().c_str(), modSym->id);

  // create the module init function
  modSym->initFn = new FnSymbol(astr("chpl__init_", modSym->name));
  modSym->initFn->retType = dtVoid;

  modSym->initFn->addFlag(FLAG_MODULE_INIT);
  modSym->initFn->addFlag(FLAG_RESOLVED);
  modSym->initFn->addFlag(FLAG_RESOLVED_EARLY);
  modSym->initFn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
  modSym->initFn->setNormalized(true);

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
  const ResolutionResultByPostorderID& rr = resolveModule(context, id);

  // don't actually convert for --dyno-resolve-only
  if (fDynoResolveOnly) return;

  ConvertedSymbolState saved = cur;
  cur = {};

  pushBlock(modSym->initFn->body);
  cur.symbol = mod;
  cur.fnSymbol = modSym->initFn;
  cur.moduleSymbol = modSym;
  cur.moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);
  cur.resolvedFunction = nullptr;

  // traverse
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rv(&rcval, cur.symbol, *this, rr);
  cur.symbol->traverse(rv);

  // add the 'return _void' at the end of the module init function
  modSym->initFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  // Swap off after traversal.
  std::swap(saved, cur);
}

void TConverter::convertFunction(const ResolvedFunction* rf) {
  // don't bother with functions that don't need to exist at runtime
  if (!functionExistsAtRuntime(rf)) return;

  // don't actually convert for --dyno-resolve-only
  if (fDynoResolveOnly) return;

  if (auto it = fns.find(rf); it != fns.end()) {
    return;
  }

  auto& id = rf->id();
  TC_DEBUGF(this, "Converting function %s\n", id.str().c_str());

  // figure out, in which block should we put the DefExpr for the new FnSymbol?

  // here we flatten all functions, so we just need to find the appropriate
  // module.
  ID parentModule = parsing::idToParentModule(context, id);
  ModuleSymbol* modSym = findOrSetupModule(parentModule);

  // prepare to visit
  const ResolutionResultByPostorderID& rr = rf->resolutionById();
  const AstNode* ast = parsing::idToAst(context, id);
  if (!ast || !ast->isFunction()) {
    INT_ASSERT(false && "expected Function");
    return;
  }

  const Function* fn = ast->toFunction();

  // Clear the current symbol markers (caller should have saved them).
  cur = {};
  cur.symbol = fn;
  cur.resolvedFunction = rf;
  cur.moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);

  pushBlock(modSym->block);

  // Traversing the symbol will use the typed converter as a visitor.
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rv(&rcval, cur.symbol, *this, rr);
  cur.symbol->traverse(rv);

  popBlock();
}

FnSymbol* TConverter::findOrConvertFunction(const ResolvedFunction* rf) {
  // if it was already converted, return that
  auto it = fns.find(rf);
  if (it != fns.end()) {
    return it->second;
  }

  // otherwise, convert the function

  // save state
  // this needs to happen when looking for a function called by
  // some code that wasn't necessarily planned for in the call graph
  // e.g. _cond_test
  ConvertedSymbolState saved = cur;

  // convert the function and return the result
  convertFunction(rf);
  FnSymbol* ret = fns[rf];
  INT_ASSERT(ret);

  // restore state
  cur = saved;

  return ret;
}

FnSymbol* TConverter::convertNewWrapper(const ResolvedFunction* rInitFn) {
  // don't bother with functions that don't need to exist at runtime
  if (!functionExistsAtRuntime(rInitFn)) return nullptr;

  TC_DEBUGF(this, "Converting _new wrapper for %s\n", rInitFn->id().str().c_str());

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

  // Converting the init function might have already generated the _new
  // wrapper, if encountered recursively
  if (auto it = newWrappers.find(rInitFn); it != newWrappers.end()) {
    return it->second;
  }

  AggregateType* type = toAggregateType(initFn->_this->getValType());

  if (initFn->throwsError()) {
    // code below will need adjustment for this case
    TC_UNIMPL("_new that throws");
  }

  // build the new wrapper. this code is based upon buildNewWrapper.
  FnSymbol* fn = new FnSymbol(astrNew);

  BlockStmt* body = fn->body;
  VarSymbol* initTemp = newTemp("initTemp", type);
  CallExpr* innerInit = new CallExpr(initFn, initTemp);
  VarSymbol* retVar = newTemp("ret", type);
  retVar->addFlag(FLAG_RVV);
  body->insertAtTail(new DefExpr(retVar));

  fn->setMethod(true);
  fn->addFlag(FLAG_NEW_WRAPPER);
  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->addFlag(FLAG_LAST_RESORT);
  fn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
  fn->addFlag(FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO);
  fn->addFlag(FLAG_LLVM_RETURN_NOALIAS);

  if (initFn->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
    fn->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
  }
  fn->addFlag(FLAG_RESOLVED);
  fn->addFlag(FLAG_RESOLVED_EARLY);
  fn->setNormalized(true);

  fn->retType = convertType(rInitFn->signature()->formalType(0).type());

  // Add the _new function just after the relevant initFn
  initFn->defPoint->insertAfter(new DefExpr(fn));

  // set up the formals and init call based on the init function
  SymbolMap initToNewMap;
  for_formals(formal, initFn) {
    if (formal != initFn->_this) {
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
  auto ci = resolution::CallInfo(ustr("chpl_here_alloc"),
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
  CallExpr* castCall = new CallExpr(PRIM_CAST, type->symbol, castTmp);
  body->insertAtTail(new CallExpr(PRIM_MOVE, initTemp, castCall));

  // run the initializer on the allocated class
  body->insertAtTail(innerInit);

  // return initTemp
  body->insertAtTail(new CallExpr(PRIM_MOVE, retVar, initTemp));
  body->insertAtTail(new CallExpr(PRIM_RETURN, retVar));

  return fn;
}

FnSymbol* TConverter::findOrConvertNewWrapper(const ResolvedFunction* rInitFn) {
  // if it was already converted, return that
  auto it = newWrappers.find(rInitFn);
  if (it != newWrappers.end()) {
    return it->second;
  }

  // otherwise, convert the function now
  FnSymbol* ret = convertNewWrapper(rInitFn);
  INT_ASSERT(ret);
  if (auto it = newWrappers.find(rInitFn); it != newWrappers.end()) {
    INT_ASSERT(it->second == ret);
  } else {
    newWrappers[rInitFn] = ret;
  }

  return ret;
}

// TODO: Migrate tuples to just use 'init' instead.
FnSymbol* TConverter::findOrConvertTupleInit(const types::TupleType* tt) {
  auto it = chplTupleInit.find(tt);
  if (it != chplTupleInit.end()) return it->second;

  Type* newType = convertType(tt);

  // Otherwise, construct it.
  FnSymbol* ret = new FnSymbol("chpl__tuple_init");

  auto _this = new ArgSymbol(INTENT_REF, "this", newType);
  _this->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(_this);
  ret->_this = _this;

  // NOTE: The original code inserts this and adds a substitution, but
  // I don't think we need to bother with this (also, why does the
  // original 'chpl__tuple_init' code do this in the first place? Isn't
  // the substitution for the type sufficient enough?).
  /*
  Type* sizeType = dtInt[INT_SIZE_DEFAULT];
  ArgSymbol* sizeArg = new ArgSymbol(INTENT_IN, "size", sizeType);
  sizeArg->addFlag(FLAG_INSTANTIATED_PARAM);
  ret->insertFormalAtTail(sizeArg);
  */

  int size = tt->numElements();
  for (int i = 0; i < size; i++) {
    const char* name = astr("x", istr(i));
    auto qtElement = tt->elementType(i);
    auto eltType = convertType(qtElement.type());

    // Use the 'in' intent to ensure that we can safely move the formal.
    ArgSymbol* arg = new ArgSymbol(INTENT_IN, name, eltType);
    ret->insertFormalAtTail(arg);

    // Mark the arg as 'no auto destroy' since we will be moving it.
    arg->addFlag(FLAG_NO_AUTO_DESTROY);

    // Move the formal into the field.
    ret->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                   _this,
                                   new_CStringSymbol(name),
                                   new SymExpr(arg)));
  }

  ret->addFlag(FLAG_RESOLVED);
  ret->addFlag(FLAG_RESOLVED_EARLY);
  ret->addFlag(FLAG_ALLOW_REF);
  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->addFlag(FLAG_LAST_RESORT);
  ret->addFlag(FLAG_INLINE);
  ret->addFlag(FLAG_INVISIBLE_FN);
  ret->addFlag(FLAG_INIT_TUPLE);
  ret->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
  ret->addFlag(FLAG_PARTIAL_TUPLE);
  ret->retTag = RET_VALUE;
  ret->retType = newType;

  ret->substitutions.copy(newType->substitutions);
  CallExpr* primReturn = new CallExpr(PRIM_RETURN, _this);
  ret->insertAtTail(primReturn);

  // TODO: Do we need to set this?
  BlockStmt* instantiationPoint = nullptr;
  ret->setInstantiationPoint(instantiationPoint);

  // Insert the definition into the tuple module.
  modChapelTuple->block->insertAtHead(new DefExpr(ret));

  // Emplace it and return it.
  auto& slot = chplTupleInit[tt];
  INT_ASSERT(slot == nullptr);
  slot = ret;

  return ret;
}

// This runs the resolver to figure out which function to call &
// converts that function.
// It does not convert the call itself.
// It works as though the generated call were in the module initializer.
FnSymbol*
TConverter::convertFunctionForGeneratedCall(const resolution::CallInfo& ci,
                                            const uast::AstNode* in,
                                            const ResolvedFunction** outRf) {
  auto scope = scopeForId(context, in->id());
  const PoiScope* poiScope = nullptr;
  auto scopeInfo = CallScopeInfo::forNormalCall(scope, poiScope);
  auto rc = createDummyRC(context);
  auto r = resolveGeneratedCall(&rc, in, ci, scopeInfo);
  const auto& candidate = r.mostSpecific().only();
  const TypedFnSignature* sig = candidate.fn();
  INT_ASSERT(sig);
  INT_ASSERT(sig->untyped()->idIsFunction());

  chpl::resolution::ResolutionContext rcval(context);
  const ResolvedFunction* resolvedFn = resolveFunction(&rcval, sig, poiScope);
  INT_ASSERT(resolvedFn);

  if (outRf) *outRf = resolvedFn;

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

  // If this ID is non-empty then it means the user defined 'main()'.
  ID mainFnId = parsing::findProcMainInModule(context, mainModuleId);

  FnSymbol* mainFn = nullptr;
  if (mainFnId.isEmpty()) {
    // there wasn't a 'proc main' so we need to generate one.
    mainFn = new FnSymbol("main");
    mainFn->addFlag(FLAG_RESOLVED);
    mainFn->addFlag(FLAG_RESOLVED_EARLY);
    mainFn->setNormalized(true);
    mainFn->retType = dtVoid;
    mainFn->body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    mainModule->block->insertAtTail(new DefExpr(mainFn));
  } else {
    // Otherwise, we already converted 'main()' via traversal of the
    // call-graph before this point. Get the converted version using
    // the ID we got above.
    auto rf = resolveConcreteFunction(context, mainFnId);
    mainFn = toFnSymbol(fns[rf]);
    INT_ASSERT(mainFn);
  }

  ModuleSymbol::setMainModule(mainModule);
  chplUserMain = mainFn;

  // adjust cname for 'proc main'
  chplUserMain->cname = astr("chpl_user_main");
  if (fIdBasedMunging && !mainModule->astloc.id().isEmpty()) {
    const char* cname = astr(mainModule->astloc.id().symbolPath().c_str(),
                             ".main");
    chplUserMain->cname = cname;
  }

  // TODO: Let's write this in the module code!
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

    // TODO: add converter or QualifiedType methods to more
    // easily construct a QualifiedType for common values like param false.
    auto falseQt = types::QualifiedType::makeParamBool(context, false);
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
  auto it = cur.localSyms.find(node->id());
  if (it != cur.localSyms.end()) {
    return toArgSymbol(it->second);
  }

  IntentTag intentTag = convertFormalIntent(node->intent());
  astlocMarker markAstLoc(node->id());

  ArgSymbol* ret = new ArgSymbol(intentTag, astr(node->name()),
                                 /* typeExpr */ dtUnknown,
                                 /* initExpr */ nullptr,
                                 /* varargsVariable */ nullptr);
  cur.localSyms[node->id()] = ret;
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
    cur.localSyms[node->id()] = ret;
  }

  return ret;
}

//
// For the following call where 'x', 'y', and 'z' are passed to a single
// varargs formal 'a'...
//
// proc foo(a: ...?n) {}
// var x, y, z: int;
// foo(x, y, z);
//
// (...The compiler generates the following signature for 'foo'...)
//
// proc foo(_e0: int, _e1: int, _e2: int) {}
//
// When the original varargs formal 'x' is used in certain ways that require
// tuple semantics, this function will create a "hidden" temporary tuple out
// of the formals '_e0'..'_e2' that represents 'x'.
//
// This helper function should be called when the "hidden" varargs tuple is
// required, as the first call will create and initialize the temporary.
//
VarSymbol*
TConverter::findOrCreateHiddenVarArg(const VarArgFormal* node, RV& rv) {
  if (Symbol* sym = findConvertedSym(node->id())) return toVarSymbol(sym);

  astlocMarker markAstLoc(node->id());
  VarSymbol* ret = new VarSymbol(astr(node->name()));
  cur.localSyms[node->id()] = ret;

  // Fill in the variable, insert it, and initialize it. This is different
  // from the other cases above because the hidden vararg variable is not
  // filled in at declaration, but rather when a specific use of the formal
  // triggers it to exist.

  // Add flags as per: 'buildTupleVariable'
  if (node->intent() == Formal::TYPE) ret->addFlag(FLAG_TYPE_VARIABLE);
  if (auto ag = node->attributeGroup()) {
    if (!ag->hasPragma(PRAGMA_NO_AUTO_DESTROY)) {
      // TODO: Can remove, we handle inserting deinit calls?
      ret->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }
  }

  auto re = rv.byAstOrNull(node);
  INT_ASSERT(re);

  // Next, set the type and initialize it using the vararg formals.
  auto& qt = re->type();
  auto tt = qt.type() ? qt.type()->toTupleType() : nullptr;
  if (tt) {
    ret->type = convertType(tt);

    if (qt.kind() == types::QualifiedType::CONST_IN ||
        qt.kind() == types::QualifiedType::IN) {
      TC_UNIMPL("Manual move-initialization for varargs tuple");
      return ret;
    }

    auto init = findOrConvertTupleInit(tt);
    auto call = new CallExpr(init);
    int numViewed = 0;

    // TODO: Why does 'chpl__tuple_init' take 'this' only to 'return this'?!
    call->insertAtTail(new SymExpr(ret));

    // TODO: Need to figure out a strategy for injecting auto-copies.
    // TODO: Either dyno handles 'init' or we need to insert naive
    // copy calls here. Probably the latter.
    INT_ASSERT(cur.fnSymbol);
    for_formals(arg, cur.fnSymbol) {
      if (!arg->hasFlag(FLAG_EXPANDED_VARARGS)) continue;
      call->insertAtTail(new SymExpr(arg));
      numViewed += 1;
    }

    INT_ASSERT(numViewed == tt->numElements());

    auto move = new CallExpr(PRIM_MOVE, new SymExpr(ret), call);
    cur.fnSymbol->body->insertAtHead(move);
    cur.fnSymbol->body->insertAtHead(new DefExpr(ret));
  }

  return ret;
}

// Looks for an already-converted Symbol. If none found, returns nullptr.
Symbol* TConverter::findConvertedSym(const ID& id) {
  { // check for local variables with that ID
    auto it = cur.localSyms.find(id);
    if (it != cur.localSyms.end()) {
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

// TODO: convert this to a visitor if there end up being a lot of cases
Symbol* TConverter::findOrCreateSym(const ID& id, RV& rv) {
  Symbol* ret = findConvertedSym(id);
  if (ret != nullptr) {
    return ret;
  }

  auto ast = parsing::idToAst(context, id);
  auto parent = ast ? parsing::parentAst(context, ast) : nullptr;
  INT_ASSERT(ast);

  if (cur.symbol && cur.symbol->isFunction() &&
      cur.symbol == parent &&
      cur.actualConverter) {
    INT_ASSERT(ast->isFormal() || ast->isVarArgFormal() ||
               ast->isTupleDecl());
    // It's a formal. Since the 'actualConverter' field is set, then we
    // are converting a default argument and need to let the actual
    // converter intercept the formal to replace it with an actual.
    return cur.actualConverter->interceptFormalUse(id);
  }

  if (auto varargs = ast->toVarArgFormal()) {
    // TODO: Copy logic to determine if we need to create a hidden vararg
    // tuple variable at all, see: 'needVarArgTupleAsWhole'...
    // -OR-...Should we just always insert it and let dead-variable-elim
    // remove it if unused?
    return findOrCreateHiddenVarArg(varargs, rv);

  } else if (auto formal = ast->toFormal()) {
    return findOrCreateFormal(formal);

  } else if (auto var = ast->toVariable()) {
    return findOrCreateVar(var);
  }

  return nullptr;
}

void TConverter::noteConvertedSym(const uast::AstNode* ast, Symbol* sym) {
  if (cur.resolvedFunction == nullptr) {
    globalSyms[ast->id()] = sym;
  } else {
    cur.localSyms[ast->id()] = sym;
  }
}

void TConverter::noteConvertedFn(const resolution::TypedFnSignature* sig, FnSymbol* fn) {
  CHPL_ASSERT(false && "currently unused");
}

Type* TConverter::findConvertedType(const types::Type* t) {
  auto it = convertedTypes.find(t);
  if (it != convertedTypes.end()) {
    return it->second.second;
  }

  return nullptr;
}

bool TConverter::typeIsConverted(const types::Type* t) {
  auto it = convertedTypes.find(t);
  if (it != convertedTypes.end()) {
    return it->second.first;
  }

  return false;
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

struct ConvertTypeHelper {
  TConverter* tc_ = nullptr;

  Context* context() { return tc_->context; }

  void helpConvertFields(const types::CompositeType* ct,
                         const ResolvedFields& rf,
                         AggregateType* at,
                         AggregateType* instantiatedFrom) {
    AggregateType* cur = instantiatedFrom;
    int nFields = rf.numFields();
    for (int i = 0; i < nFields; i++) {
      bool wasGeneric = false;
      if (instantiatedFrom) {
        wasGeneric = ct->substitution(rf.fieldDeclId(i)) != types::QualifiedType();
        //auto field = instantiatedFrom->getField(i+1);
        //bool ignoredHasDefault = false;
      }

      types::QualifiedType qt = rf.fieldType(i);
      Type* ft = nullptr;
      if (rf.isGeneric()) {
        // Leave the field type as unknown for generic types.
        // Otherwise, converting the type might result in incorrect errors
        // about the field type being ambiguously generic.
        ft = dtUnknown;
      } else {
        ft = tc_->convertType(qt.type());
      }

      // NOTE: During the intermediate stage of development of this conversion
      // pass, we need to preserve field initialization expressions in case
      // they are used in production's resolution later.
      //
      // For example, strings are used by both the compiler and the typed
      // converter.
      auto declID = rf.fieldDeclId(i);
      const AstNode* declAst = parsing::idToAst(tc_->context, declID);
      const VarLikeDecl* field = declAst->toVarLikeDecl();
      if (!field) {
        INT_ASSERT(declAst->isForwardingDecl());
        field = declAst->toForwardingDecl()->expr()->toVarLikeDecl();
      }

      Expr* initExpr = nullptr;
      Expr* typeExpr = nullptr;
      if (!instantiatedFrom && parsing::idIsInBundledModule(tc_->context, declID)) {
        // Preserve the untyped AST for internal/standard types in case the
        // production resolver needs it later. This does not apply to
        // instantiations, which the production resolver will not handle.
        //
        // TODO: Remove this when the production resolver is no longer used.
        if (field->initExpression()) {
          initExpr = tc_->untypedConverter->convertAST(field->initExpression());
        }
        if (field->typeExpression()) {
          typeExpr = tc_->untypedConverter->convertAST(field->typeExpression());
        }
      }

      UniqueString name = rf.fieldName(i);
      VarSymbol* v = new VarSymbol(astr(name));

      if (qt.isType()) {
        v->addFlag(FLAG_TYPE_VARIABLE);
      } else if (qt.isParam()) {
        v->addFlag(FLAG_PARAM);
      }

      if (!rf.isGeneric()) {
        v->type = ft;
        v->qual = tc_->convertQualifier(qt.kind());

        Symbol* sub = nullptr;
        if (qt.isType()) {
          at->substitutions.put(v, ft->symbol);

          sub = ft->symbol;
        } else if (qt.isParam()) {
          auto param = tc_->convertParam(qt);
          at->substitutions.put(v, param);
          paramMap.put(v, param);
          sub = param;
        }

        if (sub && cur->genericFields.size() != 1) {
          INT_ASSERT(wasGeneric);
          cur = cur->getInstantiation(sub, i+1, at->symbol->instantiationPoint);
          cur->foundGenericFields = true;
        }
      }

      v->makeField();
      at->fields.insertAtTail(new DefExpr(v, initExpr, typeExpr));
    }

    // The final instantiation, 'at', already exists. So we need to fiddle with
    // the logic here and replace the latest 'cur' with 'at'
    if (instantiatedFrom) {
      cur->addInstantiation(at);
    }
  }

  void helpConvertCompositeType(const types::CompositeType* ct,
                                const ResolvedFields& rf,
                                AggregateType* at) {
    auto gct = ct->instantiatedFromCompositeType();
    if (gct && !tc_->typeIsConverted(gct)) {
      tc_->convertType(gct);
    }
    auto instantiatedFrom = toAggregateType(tc_->findConvertedType(gct));

    // First convert the fields in a regular manner.
    helpConvertFields(ct, rf, at, instantiatedFrom);

    auto ts = at->symbol;
    INT_ASSERT(ts);

    if (!rf.isGeneric()) {
      ts->addFlag(FLAG_RESOLVED_EARLY);
      at->resolveStatus = RESOLVED;
      at->foundGenericFields = true;

      if (instantiatedFrom) {
        at->instantiatedFrom = instantiatedFrom;
        at->renameInstantiation();
      }
    } else {
      // This is a generic type created for the purpose of serving as a root
      // instantiation, which other parts of production assume exists.
      //
      // TODO: Remove this when the production resolver is no longer used.
      at->processGenericFields();
      at->resolveStatus = UNRESOLVED;
      at->markAsGeneric();
    }

    if (auto& id = ct->id()) {
      if (auto ast = parsing::idToAst(context(), id)) {
        bool isFromLibrary = tc_->cur.moduleFromLibraryFile;
        auto decl = ast->toAggregateDecl();
        INT_ASSERT(decl);

        // Attach pragmas and attributes.
        attachSymbolAttributes(context(), decl, ts, isFromLibrary);

        // And attach a linkage flag as well.
        auto linkageFlag = convertFlagForDeclLinkage(decl);
        if (linkageFlag != FLAG_UNKNOWN) {
          ts->addFlag(linkageFlag);
        }

        // Attach untyped conversion results for methods and nested types
        for (auto stmt : decl->declOrComments()) {
          if (auto decl = stmt->toNamedDecl();
              decl && !decl->isVarLikeDecl() &&
              parsing::idIsInBundledModule(context(), decl->id())) {
            if (instantiatedFrom != nullptr) {
              if (decl->name() == USTR("init") ||
                  decl->name() == USTR("init=") ||
                  decl->name() == USTR("deinit")) {
                // Don't generate initializers for instantiations.
                continue;
              }
            }

            auto expr = tc_->untypedConverter->convertAST(decl);
            INT_ASSERT(expr);
            at->addDeclarations(expr);
          } else if (parsing::idIsInBundledModule(context(), stmt->id()) &&
                     stmt->isForwardingDecl() &&
                     at->instantiatedFrom == nullptr) {
            auto expr = toBlockStmt(tc_->untypedConverter->convertAST(stmt));
            INT_ASSERT(expr);
            at->addDeclarations(expr->body.last());
          }
        }
      }
    }

    auto rc = createDummyRC(context());
    if (types::Type::isPod(&rc, ct)) {
      ts->addFlag(FLAG_POD);
    } else {
      ts->addFlag(FLAG_NOT_POD);
    }
  }

  DefExpr* insertTypeIntoModule(ID idToInsertAt, Type* converted) {
    if (auto def = converted->symbol->defPoint) return def;

    ID inModuleId = idToInsertAt;
    if (!parsing::idIsModule(context(), inModuleId)) {
      inModuleId = parsing::idToParentModule(context(), inModuleId);
    }

    auto mod = inModuleId ? tc_->findOrSetupModule(inModuleId)
                          : rootModule;
    mod->block->insertAtTail(new DefExpr(converted->symbol));

    return converted->symbol->defPoint;
  }

  Type* visit(const types::Type* t) {
    // Check to make sure implementation detail tags did not slip through.
    switch (t->tag()) {
      case types::typetags::START_ManageableType:
      case types::typetags::END_ManageableType:
      case types::typetags::START_BuiltinType:
      case types::typetags::END_BuiltinType:
      case types::typetags::START_DeclaredType:
      case types::typetags::END_DeclaredType:
      case types::typetags::START_CompositeType:
      case types::typetags::END_CompositeType:
      case types::typetags::START_PrimitiveType:
      case types::typetags::END_PrimitiveType:
      case types::typetags::START_IteratorType:
      case types::typetags::END_IteratorType:
      case types::typetags::START_PtrType:
      case types::typetags::END_PtrType:
      case types::typetags::NUM_TYPE_TAGS:
        INT_FATAL("should not be reachable");
        return dtUnknown;
      default: break;
    }

    std::string msg;
    msg += "Converting type tag '";
    msg += chpl::types::typetags::tagToString(t->tag());
    msg += "'";
    TC_UNIMPL(msg.c_str());
    return dtUnknown;
  }

  // builtin types with their own classes
  Type* visit(const types::AnyType* t) { return dtAny; }
  Type* visit(const types::CStringType* t) { return dtStringC; }
  Type* visit(const types::ErroneousType* t) { return dtUnknown; } // a lie
  Type* visit(const types::NilType* t) { return dtNil; }
  Type* visit(const types::NothingType* t) { return dtNothing; }
  Type* visit(const types::UnknownType* t) { return dtUnknown; }
  Type* visit(const types::VoidType* t) { return dtVoid; }

  // subclasses of IterableType (these return lies)
  Type* visit(const types::FnIteratorType* t) { return dtUnknown; }
  Type* visit(const types::LoopExprIteratorType* t) { return dtUnknown; }
  Type* visit(const types::PromotionIteratorType* t) { return dtUnknown; }

  // concrete builtin types
  Type* visit(const types::CFnPtrType* t) { return dtCFnPtr; }
  Type* visit(const types::CVoidPtrType* t) { return dtCVoidPtr; }
  Type* visit(const types::OpaqueType* t) { return dtOpaque; }
  Type* visit(const types::SyncAuxType* t) { return dtSyncVarAuxFields; }
  Type* visit(const types::TaskIdType* t) { return dtTaskID; }

  // generic builtin types
  Type* visit(const types::AnyComplexType* t) { return dtAnyComplex; }
  Type* visit(const types::AnyEnumType* t) { return dtAnyEnumerated; }
  Type* visit(const types::AnyImagType* t) { return dtAnyImag; }
  Type* visit(const types::AnyIntType* t) { return dtIntegral; } // a lie
  Type* visit(const types::AnyIntegralType* t) { return dtIntegral; }
  Type* visit(const types::AnyIteratorClassType* t) { return dtIteratorClass; }
  Type* visit(const types::AnyIteratorRecordType* t) { return dtIteratorRecord; }
  Type* visit(const types::AnyThunkRecordType* t) { return dtThunkRecord; }
  Type* visit(const types::AnyNumericType* t) { return dtNumeric; }
  Type* visit(const types::AnyOwnedType* t) { return dtOwned; }
  Type* visit(const types::AnyPodType* t) { return dtAnyPOD; }
  Type* visit(const types::AnyRealType* t) { return dtAnyReal; }
  Type* visit(const types::AnyRecordType* t) { return dtAnyRecord; }
  Type* visit(const types::AnySharedType* t) { return dtShared; }
  Type* visit(const types::AnyClassType* t) { return dtAnyManagementNonNilable; }
  Type* visit(const types::AnyUintType t) { return dtIntegral; } // a lie
  Type* visit(const types::AnyUninstantiatedType* t) { return dtUninstantiated; }
  Type* visit(const types::AnyUnionType* t) { return dtUnknown; } // a lie

  Type* visit(const types::ExternType* t) {
    // Taken from the builder function 'convertTypesToExtern'...
    //
    // Note that unfortunately (as an artifact of parsing) flags are attached
    // to the type variable (e.g., extern type 'a'...') rather than the type
    // itself (this 'types::ExternType*'). Some of those flags are important
    // such as 'FLAG_C_MEMORY_ORDER_TYPE'. They will get attached to this type
    // when the 'VarSymbol*' for the type variable is visited.
    //

    // Chapel considers extern types without info to be "primitive types",
    // and this also applies to things like default-initialization (where
    // we zero-initialize the memory), or assignment (where we bitcopy).
    auto ret = new PrimitiveType(nullptr);
    auto node = parsing::idToAst(context(), t->id())->toNamedDecl();

    INT_ASSERT(!t->linkageName().isEmpty());
    auto name = astr(node->name());

    auto ts = new TypeSymbol(name, ret);
    ts->cname = astr(t->linkageName());

    bool isFromLibrary = tc_->cur.moduleFromLibraryFile;
    attachSymbolAttributes(context(), node, ts, isFromLibrary);
    attachSymbolVisibility(node, ts);

    ts->addFlag(FLAG_EXTERN);

    auto def = insertTypeIntoModule(t->id(), ret);
    INT_ASSERT(def);

    return ret;
  }

  // declared types

  Type* visit(const types::ClassType* t) {
    if (dtObject->symbol->defPoint == nullptr) {
      // Ensure that dtObject is inserted into the module before any other
      // class types are converted, since other class types may inherit from
      // it.
      auto root = types::BasicClassType::getRootClassType(context());
      auto type = tc_->convertType(root);
      insertTypeIntoModule(root->id(), type);
    }

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
    Type* gotT = tc_->convertType(bct);
    AggregateType* at = toAggregateType(gotT);
    INT_ASSERT(at);

    Type* ret = dtUnknown;

    const types::RecordType* manager = t->managerRecordType(context());
    if (manager == nullptr) {
      auto dec = t->decorator();
      ClassTypeDecoratorEnum d = dec.isUnmanaged() ? ClassTypeDecorator::UNMANAGED
                                                   : ClassTypeDecorator::BORROWED;
      if (dec.isNonNilable()) {
        d = addNonNilToDecorator(d);
      } else {
        d = addNilableToDecorator(d);
      }

      ret = at->getDecoratedClass(d); // unamanged / borrowed is just the class type at this point
    } else {
      // owned/shared should have had a substitution for chpl_t
      INT_ASSERT(!manager->substitutions().empty());
      // convert the managed class type
      ret = tc_->convertType(manager);
    }

    return ret;
  }

  Type* visit(const types::BasicClassType* bct) {
    if (bct->isObjectType()) {
      return dtObject;
    }

    auto rc = createDummyRC(context());
    auto& rfds = fieldsForTypeDecl(&rc, bct, DefaultsPolicy::USE_DEFAULTS);

    AggregateType* at = toAggregateType(tc_->findConvertedType(bct));
    INT_ASSERT(at);

    if (at->symbol == nullptr) {
      at->symbol = new TypeSymbol(astr(bct->name()), at);
    }

    auto def = insertTypeIntoModule(bct->id(), at);
    INT_ASSERT(def);

    // convert the superclass as a field 'super'
    if (!rfds.isGeneric()) {
      if (auto parentClassType = bct->parentClassType()) {
        Type* pt = tc_->convertType(parentClassType);
        VarSymbol* v = new VarSymbol("super", pt);
        v->addFlag(FLAG_SUPER_CLASS);
        v->qual = QUAL_VAL;
        v->makeField();
        at->fields.insertAtTail(new DefExpr(v));

        at->inherits.insertAtTail(new SymExpr(pt->symbol));
      }
    }

    helpConvertCompositeType(bct, rfds, at);

    return at;
  }

  Type* visit(const types::RecordType* t) {
    auto rc = createDummyRC(context());
    // Ignore defaults because the type of 't' is already final, and should
    // not use any default values for generic fields.
    auto& rfds = fieldsForTypeDecl(&rc, t, DefaultsPolicy::IGNORE_DEFAULTS);

    AggregateType* at = toAggregateType(tc_->findConvertedType(t));
    INT_ASSERT(at);

    if (at->symbol == nullptr) {
      at->symbol = new TypeSymbol(astr(t->name()), at);
    }

    auto def = insertTypeIntoModule(t->id(), at);
    INT_ASSERT(def);

    helpConvertCompositeType(t, rfds, at);

    return at;
  }

  Type* visit(const types::EnumType* t) {
    auto node = parsing::idToAst(context(), t->id())->toEnum();
    bool isFromLibrary = tc_->cur.moduleFromLibraryFile;

    auto enumType = new EnumType();
    auto ts = new TypeSymbol(node->name().c_str(), enumType);
    enumType->symbol = ts;

    // TODO: enums with values
    for (auto elem : node->enumElements()) {
      Expr* initExpr = nullptr;
      if (elem->initExpression()) {
        initExpr = tc_->convertAstUntyped(elem->initExpression());
      }
      DefExpr* def = new DefExpr(new EnumSymbol(astr(elem->name())), initExpr);
      def->sym->type = enumType;
      enumType->constants.insertAtTail(def);

      // Because we're inserting the type at the module scope, the elements
      // need to use the globalSyms map.
      tc_->globalSyms[elem->id()] = def->sym;

      if (enumType->defaultValue == nullptr) {
        enumType->defaultValue = def->sym;
      }

      attachSymbolAttributes(context(), elem, def->sym, isFromLibrary);
    }

    attachSymbolAttributes(context(), node, ts, isFromLibrary);
    attachSymbolVisibility(node, ts);

    ts->addFlag(FLAG_RESOLVED_EARLY);

    auto def = insertTypeIntoModule(t->id(), enumType);
    INT_ASSERT(def);

    return enumType;
  }

  Type* visit(const types::UnionType* t) {
    auto rc = createDummyRC(context());
    auto& rf = fieldsForTypeDecl(&rc, t, DefaultsPolicy::USE_DEFAULTS);

    AggregateType* at = toAggregateType(tc_->findConvertedType(t));
    INT_ASSERT(at);

    if (at->symbol == nullptr) {
      at->symbol = new TypeSymbol(astr(t->name()), at);
    }

    auto def = insertTypeIntoModule(t->id(), at);
    INT_ASSERT(def);

    helpConvertCompositeType(t, rf, at);

    return at;
  }

  // Taken from 'resolution/tuples.cpp'...
  static void makeTupleName(const std::vector<TypeSymbol*>& args,
                            bool isStarTuple,
                            std::string& name,
                            std::string& cname) {
    int size = args.size();
    const char* size_str = istr(size);
    bool omitRef = !developer;

    cname = "_tuple_";
    name = "";
    if (isStarTuple) {
      TypeSymbol* nameTS = args[0];
      if (omitRef)
        nameTS = nameTS->type->getValType()->symbol;
      name += size_str;
      name += "*";
      name += toString(nameTS->type);
      cname += size_str;
      cname += "_star_";
      cname += args[0]->cname;
    } else {
      name += "(";
      cname += size_str;
      for(int i = 0; i < size; i++) {
        TypeSymbol* nameTS = args[i];
        if (omitRef)
          nameTS = nameTS->type->getValType()->symbol;
        cname += "_";
        cname += args[i]->cname;
        if (i != 0 ) name += ",";
        name += toString(nameTS->type);
      }
      name += ")";
    }
  }

  // Given an input tuple type, adjust its qualifiers so that e.g., 'in'
  // becomes 'var', etc. This is required to ensure the correct caching
  // behavior. For example, a varargs formal will be '(const in, const in)',
  // but a variable will be '(var, var)'. After normalizing, both should
  // be '(var, var)', as constness will be dropped.
  const types::TupleType* normalizeTupleType(const types::TupleType* t) {
    std::vector<types::QualifiedType> v;
    bool anyChanged = false;

    // TODO: This flag is set for every 'getQualifiedTupleType' call at the
    //       moment, which is causing spurious normalization of types here.
    //       When we get 'isVarArgTuple()' ironed out we can invert this
    //       condition and assume 'isVarArgTuple() == false' as the default.
    if (!t->isVarArgTuple()) anyChanged = true;

    for (int i = 0; i < t->numElements(); i++) {
      auto qt1 = t->elementType(i);

      // Use 'KindProperties' to normalize the 'QualifiedType::Kind'.
      auto kp = resolution::KindProperties::fromKind(qt1.kind());
      // But drop the constness.
      kp.setConst(false);

      v.push_back({ kp.toKind(), qt1.type(), qt1.param() });
      anyChanged = anyChanged || v.back() != qt1;
    }

    const types::TupleType* ret = t;
    if (anyChanged) {
      ret = types::TupleType::getQualifiedTuple(context(), std::move(v));
    }

    return ret;
  }

  Type* visit(const types::TupleType* t) {
    SET_LINENO(tc_->modChapelTuple);

    // If the normalized tuple type is different, convert that instead.
    auto normalizedType = normalizeTupleType(t);
    if (t != normalizedType) {
      return tc_->convertType(normalizedType);
    }

    auto ret = new AggregateType(AGGREGATE_RECORD);

    // Construct the 'size' field which exists in every tuple.
    auto sizeVar = new VarSymbol("size");
    sizeVar->addFlag(FLAG_PARAM);
    sizeVar->type = dtInt[INT_SIZE_DEFAULT];
    ret->fields.insertAtTail(new DefExpr(sizeVar));
    ret->substitutions.put(sizeVar, new_IntSymbol(t->numElements()));

    std::vector<TypeSymbol*> args;
    std::string name, cname;

    // Push fields.
    for (int i = 0; i < t->numElements(); i++) {
      auto qt = t->elementType(i);

      auto conv = tc_->convertType(qt.type());
      auto sym = qt.isRef() ? conv->refType->symbol : conv->symbol;
      args.push_back(sym);

      const char* fname = astr("x", istr(i));
      VarSymbol*  var  = new VarSymbol(fname);
      var->type = args[i]->type;
      ret->fields.insertAtTail(new DefExpr(var));

      // TODO: In 'getTupleInfo' we emplace substitutions against the type
      // constructor formals, do we need to bother doing that in this
      // world where the tuple type is fully concrete?
      //
      // ret->substitutions.put(typeCtorArgs[i+1], var->type->symbol);
    }

    makeTupleName(args, t->isStarTuple(), name, cname);

    ret->resolveStatus = RESOLVED;

    // TODO: We could drop this stuff setting up dispatch parents since we
    //       don't actually use it to resolve, and the tuple's not a class
    //       so it doesn't need any v-table entries.
    forv_Vec(AggregateType, t, dtTuple->dispatchParents) {
      AggregateType* at = toAggregateType(t);
      INT_ASSERT(at);
      ret->dispatchParents.add(at);
      at->dispatchChildren.add_exclusive(ret);
    }

    auto newTypeSymbol = new TypeSymbol(name.c_str(), ret);

    // TODO: Does this need to be set?
    newTypeSymbol->instantiationPoint = nullptr;

    newTypeSymbol->cname = astr(cname.c_str());
    newTypeSymbol->addFlag(FLAG_ALLOW_REF);
    newTypeSymbol->addFlag(FLAG_COMPILER_GENERATED);
    newTypeSymbol->addFlag(FLAG_TUPLE);
    newTypeSymbol->addFlag(FLAG_PARTIAL_TUPLE);
    newTypeSymbol->addFlag(FLAG_TYPE_VARIABLE);
    newTypeSymbol->addFlag(FLAG_RESOLVED_EARLY);
    newTypeSymbol->addFlag(FLAG_INSTANTIATED_GENERIC);
    if (t->isStarTuple()) newTypeSymbol->addFlag(FLAG_STAR_TUPLE);

    ret->saveGenericSubstitutions();
    ret->instantiatedFrom = dtTuple;

    // Insert all tuple instantiations in the tuple module.
    tc_->modChapelTuple->block->insertAtHead(new DefExpr(newTypeSymbol));

    return ret;
  }

  // pointer types and primitive types

  Type* visit(const types::PtrType* t) {
    // find the pointer type to instantiate
    AggregateType* base = nullptr;
    if (auto ct = t->toCPtrType()) {
      base = ct->isConst() ? dtCPointerConst : dtCPointer;
    } else {
      INT_ASSERT(t->toHeapBufferType());
      base = dtHeapBuffer;
    }

    if (base->symbol == nullptr) {
      INT_ASSERT(false && "Should not happen!");

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

    // Use untyped converter to build AST, so that we can use it for
    // instantiation.
    if (base->numFields() == 0) {
      auto id = t->id(context());
      auto node = parsing::idToAst(context(), id)->toDecl();
      INT_ASSERT(!base->symbol->defPoint);

      auto def = tc_->untypedConverter->convertAST(node, MOD_STANDARD);
      auto ts = toTypeSymbol(toDefExpr(def)->sym);

      ID inModuleId = parsing::idToParentModule(context(), id);
      auto mod = tc_->findOrSetupModule(inModuleId);
      mod->block->insertAtTail(def);

      tc_->globalSyms[id] = base->symbol;

      for_fields(field, base) {
        if (auto fn = toFnSymbol(field)) {
          flattenPrimaryMethod(ts, fn);
        }
      }

      base->processGenericFields();
    }

    // handle ptr without an element type
    if (t->eltType() == nullptr) {
      return base;
    }

    // convert the element type
    Type* convertedEltT = tc_->convertType(t->eltType());

    // instantiate it with the element type
    // note: getInstantiation should re-use existing instantiations
    int index = 1;
    Expr* insnPoint = nullptr; // TODO: does this need to be set?
    AggregateType* ret =
      base->getInstantiation(convertedEltT->symbol, index, insnPoint);
    ret->resolveStatus = RESOLVED;
    ret->symbol->addFlag(FLAG_RESOLVED_EARLY);

    ret->saveGenericSubstitutions();
    propagateNotPOD(ret);

    return ret;
  }

  Type* visit(const types::BoolType* t) {
    return dtBool;
  }

  Type* visit(const types::ComplexType* t) {
    return dtComplex[getComplexSize(t)];
  }

  Type* visit(const types::ImagType* t) {
    return dtImag[getImagSize(t)];
  }

  Type* visit(const types::IntType* t) {
    return dtInt[getIntSize(t)];
  }

  Type* visit(const types::RealType* t) {
    return dtReal[getRealSize(t)];
  }

  Type* visit(const types::UintType* t) {
    return dtUInt[getUintSize(t)];
  }
};

Type* TConverter::convertType(const types::Type* t) {
  if (t == nullptr) return dtUnknown;

  if (auto existing = findConvertedType(t)) {
    // (A) Fetch the cached converted type to reuse it if possible.
    return existing;

  }

  Type* ret = nullptr;

  // (B) Convert a type for the first time. Start by checking to see if
  // the type is an aggregate. If it is, check to see if it is a well
  // known type, otherwise make a stub first to support recursion when
  // converting the fields.
  auto selector = [&t](auto name, AggregateTag tag) {
    auto at = shouldWireWellKnownType(name.c_str());

    // Need a new AggregateType if this is an instantiation.
    bool isInstantiation = false;
    if (auto ct = t->getCompositeType();
        ct && ct->instantiatedFromCompositeType()) {
      isInstantiation = true;
    }

    auto ret = at && !isInstantiation ? at : new AggregateType(tag);
    return ret;
  };

  AggregateType* at = nullptr;
  if (auto x = t->toBasicClassType()) {
    // NOTE: 'ClassType' will convert the 'BasicClassType' to get here.
    at = selector(x->name(), AGGREGATE_CLASS);
  } else if (auto x = t->toRecordType()) {
    at = selector(x->name(), AGGREGATE_RECORD);
  } else if (auto x = t->toUnionType()) {
    at = selector(x->name(), AGGREGATE_UNION);
  }

  // It's an aggregate, so emplace a map entry before converting.
  if (at) convertedTypes[t] = {false, at};

  // Invoke the visitor to convert the type.
  ConvertTypeHelper visitor = { this };
  ret = t->dispatch<Type*>(visitor);
  INT_ASSERT(ret);

  // Set the converted type once again.
  convertedTypes[t] = {true, ret};

  if (ret->symbol->hasFlag(FLAG_INSTANTIATED_GENERIC) == false &&
      ret != dtObject) {
    ID id;
    if (auto ct = t->getCompositeType()) {
      id = ct->id();
    } else if (auto et = t->toEnumType()) {
      id = et->id();
    } else if (auto ext = t->toExternType()) {
      id = ext->id();
    }
    if (!id.isEmpty() && !isDecoratedClassType(ret)) {
      auto ast = parsing::idToAst(context, id);
      untypedConverter->noteConvertedSym(ast, ret->symbol);
    }
  }

  // If we need to generate a 'ref' wrapper for the type, do so now.
  if (!ret->refType) {
    SET_LINENO(ret->symbol);

    // TODO: This is "AGGREGATE_RECORD" in "getOrMakeRefTypeDuringCodegen"?
    // But making these early 'ref' wrappers records interferes with a lot
    // of optimization passes that are looking for records, so my guess is
    // that the codegen generated ones should probably be 'CLASS' as well.
    AggregateType* ref = new AggregateType(AGGREGATE_CLASS);
    const char* name = astr("_ref_", ret->symbol->name);
    if (isDecoratedClassType(ret)) {
      name = astr("_ref(", ret->symbol->name, ")");
    }
    TypeSymbol* refTs = new TypeSymbol(name, ref);
    refTs->cname = astr("_ref_", ret->symbol->cname);
    refTs->addFlag(FLAG_REF);
    refTs->addFlag(FLAG_NO_DEFAULT_FUNCTIONS);
    refTs->addFlag(FLAG_NO_OBJECT);
    refTs->addFlag(FLAG_RESOLVED_EARLY);
    globalInsertionPoint->insertAtTail(new DefExpr(refTs));
    ref->fields.insertAtTail(new DefExpr(new VarSymbol("_val", ret)));
    ret->refType = ref;
  }

  return ret;
}

struct ConvertDefaultValueHelper {
  TConverter* tc_ = nullptr;
  const AstNode* node_ = nullptr;
  TConverter::RV& rv_;

  Context* context() { return tc_->context; }

  Expr* visit(const types::Type* t) {
    std::string msg;
    msg += "Constructing default value for type tag '";
    msg += chpl::types::typetags::tagToString(t->tag());
    msg += "'";
    TC_UNIMPL(msg.c_str());
    return TC_PLACEHOLDER(tc_);
  }

  Expr* visit(const types::ExternType* t) {
    // Taken from 'functionResolution.cpp:14000~'. Make a temp and zero it.
    types::QualifiedType qt = { types::QualifiedType::VAR, t };
    auto temp = tc_->makeNewTemp(qt);
    tc_->insertStmt(new CallExpr(PRIM_ZERO_VARIABLE, temp));
    return new SymExpr(temp);
  }

  Expr* visit(const types::BoolType* t) {
    return new SymExpr(new_BoolSymbol(false));
  }

  Expr* visit(const types::IntType* t) {
    auto var = new_IntSymbol(0, getIntSize(t));
    return new SymExpr(var);
  }

  Expr* visit(const types::UintType* t) {
    auto var = new_UIntSymbol(0, getUintSize(t));
    return new SymExpr(var);
  }

  Expr* visit(const types::RealType* t) {
    auto var = new_RealSymbol("0.0", getRealSize(t));
    return new SymExpr(var);
  }

  Expr* visit(const types::CPtrType* t) {
    return new SymExpr(gNil);
  }

  Expr* visit(const types::ClassType* t) {
    if (t->manager()) {
      return visit(t->managerRecordType(context()));
    } else {
      // Note that we use 'nil' regardless of whether or not the class is
      // non-nilable or not, because non-nilable classes set to 'nil' or
      // default-initialized should have been caught earlier in compilation.
      return new SymExpr(gNil);
    }
  }

  Expr* visit(const types::RecordType* t) {
    types::QualifiedType qt = { types::QualifiedType::VAR, t };

    // For extern records, just zero-initialize a temporary.
    if (t->linkage() == uast::Decl::EXTERN) {
      auto temp = tc_->makeNewTemp(qt);
      tc_->insertStmt(new CallExpr(PRIM_ZERO_VARIABLE, temp));
      return new SymExpr(temp);;
    }

    // Add type/param arguments if type is generic.
    // Language requires we use named arguments here.
    std::vector<CallInfoActual> actuals;
    if (t->instantiatedFromCompositeType()) {
      auto rc = createDummyRC(context());
      auto& rfds = fieldsForTypeDecl(&rc, t, DefaultsPolicy::USE_DEFAULTS);
      for (int i = 0; i < rfds.numFields(); i++) {
        auto qt = t->substitution(rfds.fieldDeclId(i));
        if (!qt.isUnknown()) {
          actuals.push_back({qt, rfds.fieldName(i)});
        }
      }
    }

    // Otherwise, construct a 'CallInfo' representing 't.init()'...
    auto ci1 = resolution::CallInfo::createSimple(tc_->ustr("init"), actuals);
    auto ci2 = resolution::CallInfo::createWithReceiver(ci1, qt);

    // Search for 'init()' using the generated call.
    const ResolvedFunction* rf = nullptr;
    auto fn = tc_->convertFunctionForGeneratedCall(ci2, node_, &rf);

    if (!fn) return TC_PLACEHOLDER(tc_);

    // Found it, so invoke the default-initializer on a temp.
    auto temp = tc_->makeNewTemp(qt);
    auto initCall = new CallExpr(fn, temp);

    std::vector<const AstNode*> actualAsts;
    auto tfs = rf->signature();
    FormalActualMap fam(rf->signature(), ci2);
    INT_ASSERT(fam.isValid());

    if (fam.numFormalsMapped() > 1) {
      // Use an 'ActualConverter' to handle potential default-arguments, as
      // long as there's more than just the receiver formal.
      TConverter::ActualConverter ac(tc_, node_, actualAsts, tfs, fam, rv_);
      ac.convertAndInsertActuals(initCall, /*skipReceiver=*/true);
    }

    // Insert the initializer call as a statement.
    tc_->insertStmt(initCall);

    // The returned expression is a reference to the temporary.
    auto ret = new SymExpr(temp);

    return ret;
  }

  Expr* visit(const types::TupleType* t) {
    types::QualifiedType qt = { types::QualifiedType::VAR, t };
    auto temp = tc_->makeNewTemp(qt);

    auto initFn = tc_->findOrConvertTupleInit(t);
    auto initCall = new CallExpr(initFn);

    initCall->insertAtTail(temp);

    for (int i = 0; i < t->numElements(); i++) {
      // Construct the default value for each component, recursively.
      auto qtField = t->elementType(i);
      auto e1 = tc_->defaultValueForType(qtField.type(), node_, rv_);
      auto e2 = tc_->storeInTempIfNeeded(e1, qtField);
      initCall->insertAtTail(e2);
    }

    tc_->insertStmt(initCall);

    // The returned expression is a reference to the temporary.
    auto ret = new SymExpr(temp);

    return ret;
  }
};

Expr*
TConverter::defaultValueForType(const types::Type* t,
                                const AstNode* node,
                                RV& rv) {
  ConvertDefaultValueHelper visitor = { this, node, rv };
  auto ret = t->dispatch<Expr*>(visitor);
  return ret;
}

Symbol*
TConverter::makeNewTemp(const types::QualifiedType& qt, bool insertDef) {
  auto ret = newTemp();
  ret->addFlag(FLAG_EXPR_TEMP);
  ret->qual = convertQualifier(qt.kind());
  ret->type = convertType(qt.type());

  if (insertDef) insertStmt(new DefExpr(ret));

  return ret;
}

SymExpr*
TConverter::storeInTempIfNeeded(Expr* e, const types::QualifiedType& qt) {
  if (SymExpr* se = toSymExpr(e)) {
    return se;
  }

  Symbol* t = nullptr;

  // Prevent default-constructed 'QualifiedType' from slipping in.
  // Note: Can't use, e.g., ``e->qualType()`` here because ``qualType`` may
  //   attempt to invoke production's resolution in some cases.
  INT_ASSERT(qt.hasTypePtr());
  // otherwise, store the value in a temp
  t = makeNewTemp(qt);
  insertStmt(new CallExpr(PRIM_MOVE, t, e));
  return new SymExpr(t);
}

SymExpr*
TConverter::insertDerefTemp(SymExpr* se, const types::QualifiedType& qt) {
  auto deref = new CallExpr(PRIM_DEREF, se->copy());
  return storeInTempIfNeeded(deref, qt);
}

// note: new_IntSymbol etc already returns existing if already created
Symbol* TConverter::convertParam(const types::QualifiedType& qt) {
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
    auto val = escapeStringC(sp->value().c_str());
    if (t->isStringType()) {
      return new_StringSymbol(val.c_str());
    } else if (t->isCStringType()) {
      return new_CStringSymbol(val.c_str());
    } else if (t->isBytesType()) {
      return new_BytesSymbol(val.c_str());
    }
  } else if (auto up = p->toUintParam()) {
    const types::UintType* t = qt.type()->toUintType();
    return new_UIntSymbol(up->value(), getUintSize(t));
  } else if (auto ep = p->toEnumParam()) {
    // First, convert the type in case it hasn't been already
    auto t = convertType(qt.type());

    // After conversion, the enum elements should be in 'globalSyms'
    auto val = ep->value();
    auto ret = globalSyms[val.id];
    CHPL_ASSERT(t == ret->type);
    return ret;
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
  } else if (r->returnIntent() == uast::Function::PARAM ||
             r->returnIntent() == uast::Function::TYPE) {
    // Sometimes, apparently, we allow a function to have this kind of return
    // intent without actually returning a value.
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
    TC_UNIMPL("Unhandled lifetime clause");
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

Symbol* TConverter::convertVariable(const uast::Variable* node,
                                    RV& rv,
                                    bool useLinkageName,
                                    MultiDeclState* multiState,
                                    Expr* initExprOverride) {
  astlocMarker markAstLoc(node->id());

  bool isStatic = false;
  if (auto ag = node->attributeGroup()) {
    isStatic = ag->getAttributeNamed(USTR("functionStatic"));
  }

  if (node->name() == USTR("_")) {
    // no need to convert _
    return nullptr;
  }

  bool isRemote = node->destination() != nullptr ||
                  (multiState != nullptr && multiState->localeTemp != nullptr);
  bool isTypeVar = node->kind() == uast::Variable::TYPE;
  bool isExtern = node->linkage() == uast::Decl::EXTERN;
  auto inModule = cur.symbol->toModule();
  bool moduleScopeVar = false;

  if (node->kind() == uast::Variable::PARAM) {
#ifndef NDEBUG
    auto type = rv.byAst(node).type();
    CHPL_ASSERT(type.isParam() && type.hasParamPtr());
#endif
    return nullptr;
  } else if (isTypeVar && !isExtern) {
#ifndef NDEBUG
    auto type = rv.byAst(node).type();
    CHPL_ASSERT(type.isType() && type.hasTypePtr());
#endif
    return nullptr;
  }

  // Special handling for extern type variables. In this case, we will point
  // the definition of the "variable" towards the 'TypeSymbol' of the
  // converted 'types::ExternType*'. We will not generate a 'VarSymbol*' to
  // represent the variable.
  if (isTypeVar && isExtern) {
    auto re = rv.byAstOrNull(node);
    INT_ASSERT(re);

    // Only bother if we have an 'ExternType' and there wasn't an error.
    auto& qt = re->type();
    if (qt.type()) {
      if (qt.type()->isExternType()) {
        auto t = convertType(qt.type());
        INT_ASSERT(t);
        auto ret = t->symbol;
        INT_ASSERT(node->name() == ret->name ||
                   (node->linkageName() &&
                    node->linkageName()->toStringLiteral()->value() == ret->name));
        INT_ASSERT(ret->hasFlag(FLAG_EXTERN));
        INT_ASSERT(node->initExpression() == nullptr);

        attachSymbolAttributes(context, node, ret, cur.moduleFromLibraryFile);
        attachSymbolVisibility(node, ret);

        // Remove the 'DefExpr' for the converted 'ExternType' and place it
        // at the declaration point of the variable. It's OK if this might
        // happen multiple times, because if it does then it means the
        // program is ill-formed.
        //
        // Note: might not be 'inTree' if in the module initialization block
        auto def = ret->defPoint->remove();
        insertStmt(def);

        noteConvertedSym(node, ret);

        // Exit since we are not actually creating a "variable".
        return ret;
      } else {
        INT_ASSERT(node->initExpression() != nullptr);
        // No action needed; e.g. 'extern type x = int(32);'
        return nullptr;
      }
    }
  }

  auto varSym = findOrCreateVar(node);
  if (inModule != nullptr) {
    moduleScopeVar = parsing::idIsModuleScopeVar(context, node->id());
  }

  if (moduleScopeVar) {
    varSym->addFlag(FLAG_RESOLVED_EARLY);
  }

  if (fIdBasedMunging && node->linkage() == uast::Decl::DEFAULT_LINKAGE) {
    // is it a module-scope variable?
    // adjust the cname for module-scope variables
    if (moduleScopeVar && inModule) {
      varSym->cname = astr(inModule->id().symbolPath().c_str(), ".",
                           varSym->name);
    }
  }
  uast::Variable::Kind symbolKind = node->kind();
  if (isRemote) {
    TC_UNIMPL("remote variable");
  }

  // Adjust the variable according to its kind, e.g. 'const'/'type'.
  attachSymbolStorage(symbolKind, varSym, /* setQual */ true);

  attachSymbolAttributes(context, node, varSym, cur.moduleFromLibraryFile);

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
  types::QualifiedType initQt;
  if (const uast::AstNode* ie = node->initExpression()) {
    const uast::BracketLoop* bkt = ie->toBracketLoop();
    if (bkt && isTypeVar) {
      TC_UNIMPL("convert array type");
    } else {
      initExpr = convertExpr(ie, rv, &initQt);
    }

    if (isStatic) {
      TC_UNIMPL("function-static variables");
    }
  } else {
    initExpr = convertExprOrNull(node->initExpression(), rv, &initQt);
  }

  if ((!typeExpr && !initExpr) && multiState) {
    TC_UNIMPL("multi-vars");
  }

  DefExpr* def = nullptr;
  if (isRemote) {
    TC_UNIMPL("remote variable");
  } else {
    def = new DefExpr(varSym);
    if (!moduleScopeVar) {
      insertStmt(def);
    } else {
      // module variables need to be stored outside of module init fn
      INT_ASSERT(cur.moduleSymbol);
      cur.moduleSymbol->block->insertAtTail(def);
    }

    CallExpr* move = nullptr;
    if (varSym->isRef()) {
      if (initQt.isRef()) {
          move = new CallExpr(PRIM_MOVE, varSym, initExpr);
      } else if (move == nullptr) {
        auto expr = storeInTempIfNeeded(initExpr , initQt);
        move = new CallExpr(PRIM_MOVE, varSym,
                                       new CallExpr(PRIM_ADDR_OF, expr));
      }
    } else if (initExprOverride != nullptr) {
      move = new CallExpr(PRIM_MOVE, varSym, initExprOverride);
    } else if (!node->hasPragma(context, uast::PRAGMA_NO_INIT)) {
      const resolution::ResolvedExpression* re = rv.byAstOrNull(node);
      if (initExpr == nullptr) {
        // compute the default value for this type
        if (re) {
          types::QualifiedType qt = re->type();
          if (!qt.isUnknownOrErroneous()) {
            initExpr = defaultValueForType(qt.type(), node, rv);
          }
        }
      }

      // TODO: All variable initializations should have some kind of associated
      // action indicating the intended behavior.
      if (re->hasAssociatedActions()) {
        auto& actions = re->associatedActions();
        INT_ASSERT(actions.size() == 1);
        auto action = actions[0];
        if (action.action() == resolution::AssociatedAction::ASSIGN) {
          const ResolvedFunction *rf;
          auto elide = paramElideCallOrNull(actions[0].fn(), re->poiScope(), &rf);
          INT_ASSERT(!elide);
          auto calledFn = findOrConvertFunction(rf);
          // Use temp to avoid constness errors later in compiler
          VarSymbol* initTemp = newTemp("initTemp", convertType(re->type().type()));
          insertStmt(new DefExpr(initTemp));
          insertStmt(new CallExpr(calledFn, initTemp, initExpr));
          move = new CallExpr(PRIM_MOVE, varSym, initTemp);
        } else if (action.action() == resolution::AssociatedAction::MOVE_INIT ||
                   action.action() == resolution::AssociatedAction::DEFAULT_INIT) {
          move = new CallExpr(PRIM_MOVE, varSym, initExpr);
        } else {
          TC_UNIMPL("unknown associated action for variable initialization");
          move = new CallExpr(PRIM_MOVE, varSym, initExpr);
        }
      } else {
        move = new CallExpr(PRIM_MOVE, varSym, initExpr);
      }
    }

    if (move)
      insertStmt(move);
  }

  auto loopFlags = LoopAttributeInfo::fromVariableDeclaration(context, node);
  if (!loopFlags.empty()) {
    TC_UNIMPL("loop attribute info");
    // cur.aList->insertAtTail(new CallExpr(PRIM_GPU_ATTRIBUTE_BLOCK));
    //if (auto primBlock = loopFlags.createPrimitivesBlock(*this)) {
    //  cur.aList->insertAtTail(primBlock);
    //}
  }
  // If the init expression of this variable is a domain and this
  // variable is not const, propagate that information by setting
  // 'definedConst' in the domain to false.
  setDefinedConstForDefExprIfApplicable(def, &def->sym->flags);

  // Fix up the AST based on the type, if it should be known
  setVariableType(node, varSym, rv);

  noteConvertedSym(node, varSym);

  return varSym;
}

// this does a micro-optimization to replace
//   GOTO(epilogue)
//   epilogue:
// with relying on the pass-through
void TConverter::simplifyEpilogue(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_NO_FN_BODY)) return;

  if (Expr* labelDef = cur.epilogueLabel->defPoint) {
    if (GotoStmt* g = toGotoStmt(labelDef->prev)) {
      if (g->gotoTag == GOTO_RETURN) {
        // remove the GOTO and rely on fall-through
        g->remove();
      }
    }
  }

  // if the epilogue label is now unused, remove it
  if (cur.epilogueLabel->firstSymExpr() == nullptr) {
    cur.epilogueLabel->defPoint->remove();
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

IntentTag TConverter::convertFormalIntentQt(types::QualifiedType::Kind kind) {
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
  TC_DEBUGF(this, "id = %s\n", v->id().str().c_str());

  // Get the type of the variable itself
  if (const resolution::ResolvedExpression* rr = rv.byAstOrNull(v)) {
    types::QualifiedType qt = rr->type();
    if (!qt.isUnknown()) {
      if (trace) {
        TC_DEBUGF(this, "qt is \n");
        qt.dump();
      }

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

Expr* TConverter::paramElideCallOrNull(const TypedFnSignature* sig,
                                       const PoiScope* poiScope,
                                       const ResolvedFunction** outRf) {
  if (!sig->untyped()->idIsFunction()) return nullptr;

  // TODO: Do we need to handle nested functions differently here?
  chpl::resolution::ResolutionContext rcval(context);
  auto rf = resolveFunction(&rcval, sig, poiScope);

  if (rf == nullptr) return nullptr;

  Expr* ret = nullptr;

  // Consider the return type of the called function.
  // Do we need to emit this call at all?
  // TODO: If the call is producing a type, do we really need to bother?
  if (!functionExistsAtRuntime(rf)) {
    // don't emit the call, but instead emit a SymExpr for the relevant
    // type or param.
    const auto& qt = rf->returnType();
    if (qt.isType()) {
      Type* t = convertType(qt.type());
      ret = new SymExpr(t->symbol);
    } else {
      if (!qt.param()) {
        // param return-intent on function without a return value
        ret = new SymExpr(gNone);
      } else {
        Symbol* p = convertParam(qt);
        ret = new SymExpr(p);
      }
    }
  }

  if (outRf) *outRf = rf;

  return ret;
}

void TConverter::convertImplicitInit(const types::CompositeType* ct, ID id, RV& rv) {
  ResolutionContext rcval(context);
  auto& results = resolution::resolveFieldResults(&rcval, ct, id, DefaultsPolicy::USE_DEFAULTS, false);
  auto var = results.fieldAst()->toVarLikeDecl();
  if (var->storageKind() == types::QualifiedType::TYPE ||
      var->storageKind() == types::QualifiedType::PARAM) {
    return;
  } else if (results.results().byAst(results.fieldAst()).type().type()->isNothingType()) {
    return;
  }

  ConvertedSymbolState calledFnState_ = cur;
  std::swap(calledFnState_, cur);

  RV fieldVis(rv.rc(), cur.symbol, *this, results.results());
  results.fieldAst()->traverse(fieldVis);

  auto sym = cur.localSyms[id];
  types::QualifiedType outQt;
  auto field = codegenGetField(nullptr, sym->name, rv, &outQt);
  field = storeInTempIfNeeded(field, outQt);
  insertStmt(new CallExpr(PRIM_MOVE, field, new SymExpr(sym)));

  std::swap(calledFnState_, cur);
}

Expr* TConverter::convertMoveInitAssignOrNull(const Call* node, RV& rv) {
  auto op = node->toOpCall();
  if (!op || op->op() != USTR("=")) return nullptr;

  if (cur.fnSymbol->isInitializer() ||
      cur.fnSymbol->isCopyInit()) {
    auto inits = cur.resolvedFunction->implicitInits();
    if (auto it = inits.find(node->id()); it != inits.end()) {
      auto& vec = it->second;
      for (auto [ct, id] : vec) {
        convertImplicitInit(ct, id, rv);
      }
    }
  }

  auto re = rv.byAstOrNull(node);
  auto& candidate = re->mostSpecific().only();
  auto sig = candidate.fn();
  bool noCandidateForCall = !candidate || !sig;
  if (!noCandidateForCall) return nullptr;
  if (!re->hasAssociatedActions()) return nullptr;
  auto& actions = re->associatedActions();
  if (actions[0].action() != AssociatedAction::MOVE_INIT) {
    return nullptr;
  }

  types::QualifiedType lhsQt, rhsQt;
  auto lhs = convertExpr(op->actual(0), rv, &lhsQt);
  auto rhs = convertExpr(op->actual(1), rv, &rhsQt);

  lhs = storeInTempIfNeeded(lhs, lhsQt);
  rhs = storeInTempIfNeeded(rhs, rhsQt);

  return new CallExpr(PRIM_MOVE, lhs, rhs);
}

// TODO: Can we generate some proper uAST here, somehow?
Expr* TConverter::convertEnumIntegralCastOrNull(const Call* node, RV& rv) {
  auto op = node->toOpCall();
  if (!op || op->op() != USTR(":")) return nullptr;

  auto re = rv.byAstOrNull(node);
  auto& candidate = re->mostSpecific().only();
  auto sig = candidate.fn();
  if (!sig->formalType(1).type()->isEnumType()) return nullptr;
  INT_ASSERT(sig->untyped()->isCompilerGenerated());

  FnSymbol* fn = new FnSymbol(astrScolon);
  fn->addFlag(FLAG_RESOLVED);
  fn->addFlag(FLAG_RESOLVED_EARLY);
  fn->addFlag(FLAG_OPERATOR);
  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->addFlag(FLAG_LAST_RESORT);
  auto from = new ArgSymbol(INTENT_CONST_IN, "from",
                            convertType(sig->formalType(0).type()));
  fn->insertFormalAtTail(from);

  // TODO: implement for non-concrete enums
  auto enumType = convertType(sig->formalType(1).type());
  auto ret = new VarSymbol("ret", enumType);
  ret->addFlag(FLAG_RESOLVED_EARLY);
  fn->insertAtTail(new DefExpr(ret));
  fn->insertAtTail(new CallExpr(PRIM_MOVE, ret,
                                new CallExpr(PRIM_CAST, enumType->symbol, from)));
  fn->insertAtTail(new CallExpr(PRIM_RETURN, ret));
  cur.moduleSymbol->block->insertAtTail(new DefExpr(fn));

  fn->retType = enumType;

  types::QualifiedType qt;
  auto arg = convertExpr(op->actual(0), rv, &qt);
  arg = storeInTempIfNeeded(arg, qt);
  return new CallExpr(fn, arg);
}

Expr* TConverter::convertNewCallOrNull(const Call* node, RV& rv) {
  if (!node->calledExpression() ||
      !node->calledExpression()->isNew()) {
    return nullptr;
  }

  // We need the 'ResolvedExpression' to get the 'NEW_INIT' action.
  auto re = rv.byAstOrNull(node);
  if (!re) return nullptr;

  // Fetch the signature of the 'init()' procedure.
  const TypedFnSignature* init = nullptr;
  for (const auto& action : re->associatedActions()) {
    if (action.action() == AssociatedAction::NEW_INIT) {
      init = action.fn();
      break;
    }
  }

  const ResolvedFunction* rf = nullptr;
  if (paramElideCallOrNull(init, re->poiScope(), &rf)) {
    INT_ASSERT(false && "Should not have param elided initializer call!");
  } else if (!rf) {
    return nullptr;
  }

  auto type = init->formalType(0).type();
  Expr* ret = nullptr;

  // Pieces needed to construct the 'CallInfo'.
  const bool raiseErrors = false;
  std::vector<CallInfoActual> actuals;
  const AstNode* questionArg = nullptr;
  std::vector<const AstNode*> actualAsts;

  // Use 'prepareActuals' instead of 'create' here because the latter does
  // not understand how to handle a 'new' expression, and produces an
  // incorrect 'CallInfo'. A new expression is not actually a normal call,
  // though we represent it with a CI here because it does have actuals
  // (which can be named).
  resolution::CallInfo::prepareActuals(context, node, rv.byPostorder(),
                                       raiseErrors,
                                       actuals,
                                       questionArg,
                                       &actualAsts);

  bool hasQuestionArg = (questionArg != nullptr);

  if (type->isRecordType()) {
    actualAsts.insert(actualAsts.begin(), nullptr);
    // find or convert the init function
    FnSymbol* initFn = findOrConvertFunction(rf);
    AggregateType* type = toAggregateType(initFn->_this->getValType());

    VarSymbol* initTemp = newTemp("initTemp", type);
    auto initCall = new CallExpr(initFn, initTemp);
    insertStmt(new DefExpr(initTemp));

    // TODO: is it right that 'hasQuestionArg' is always false here?
    auto ci1 = resolution::CallInfo(USTR("init"), types::QualifiedType(),
                                   /*isMethodCall*/ false,
                                   /*hasQuestionArg*/ hasQuestionArg,
                                   /*isParenless*/ false,
                                   /*actuals*/ std::move(actuals));
    auto ci2 = resolution::CallInfo::createWithReceiver(ci1,
                                   init->formalType(0));
    auto fam = FormalActualMap(init->untyped(), ci2);

    TConverter::ActualConverter ac(this, node, actualAsts, init, fam, rv);
    ac.convertAndInsertActuals(initCall, /*skipReceiver=*/true);
    insertStmt(initCall);

    ret = new SymExpr(initTemp);
  } else {
    // For 'new unmanaged C(...)' it should generate a call to a '_new'
    // function. The actuals are passed along to it down below.
    FnSymbol* calledFn = findOrConvertNewWrapper(rf);
    auto initCall = new CallExpr(calledFn);

    actualAsts.insert(actualAsts.begin(), nullptr);
    // TODO: is it right that 'hasQuestionArg' is always false here?
    auto ci1 = resolution::CallInfo(USTR("init"), types::QualifiedType(),
                                   /*isMethodCall*/ false,
                                   /*hasQuestionArg*/ hasQuestionArg,
                                   /*isParenless*/ false,
                                   /*actuals*/ std::move(actuals));
    auto ci2 = resolution::CallInfo::createWithReceiver(ci1,
                                   init->formalType(0));
    auto fam = FormalActualMap(init->untyped(), ci2);

    TConverter::ActualConverter ac(this, node, actualAsts, init, fam, rv);
    ac.convertAndInsertActuals(initCall, /*skipReceiver=*/true);

    ret = initCall;

    // If this a managed class, use the result of _new to initialize the
    // manager record.
    auto ct = re->type().type()->toClassType();
    if (auto mt = ct ? ct->managerRecordType(context) : nullptr) {
      types::QualifiedType recvQt = { re->type().kind(), mt };

      auto dec = types::ClassTypeDecorator(types::ClassTypeDecorator::UNMANAGED);
      dec = dec.copyNilabilityFrom(ct->decorator());
      auto ut = ct->withDecorator(context, dec);
      types::QualifiedType uqt = { re->type().kind(), ut };

      std::vector<CallInfoActual> mtActuals;
      mtActuals.push_back(CallInfoActual(uqt));

      auto ci1 = resolution::CallInfo::createSimple(USTR("init"), mtActuals);
      auto ci2 = resolution::CallInfo::createWithReceiver(ci1, recvQt);

      // Search for 'init()' using the generated call.
      const ResolvedFunction* rf = nullptr;
      auto fn = convertFunctionForGeneratedCall(ci2, node, &rf);

      VarSymbol* initTemp = newTemp("_new_mngd_tmp", convertType(recvQt.type()));
      insertStmt(new DefExpr(initTemp));

      // 'init' on managed types expects an unmanaged class
      auto newResult = storeInTempIfNeeded(ret, init->formalType(0));
      Expr* unm = new CallExpr(PRIM_CAST, convertType(ut)->symbol, newResult);
      unm = storeInTempIfNeeded(unm, uqt);
      insertStmt(new CallExpr(fn, initTemp, unm));
      ret = new SymExpr(initTemp);
    }
  }

  return ret;
}

Expr* TConverter::convertTupleCallOrNull(const Call* node, RV& rv) {
  auto tuple = node ? node->toTuple() : nullptr;
  if (!tuple) return nullptr;

  auto re = rv.byAstOrNull(node);
  INT_ASSERT(re);

  auto& qtTuple = re->type();
  auto tt = qtTuple.type() ? qtTuple.type()->toTupleType() : nullptr;
  if (!tt) return TC_PLACEHOLDER(this);

  auto fn = findOrConvertTupleInit(tt);
  if (!fn) return TC_PLACEHOLDER(this);

  types::QualifiedType qt = { types::QualifiedType::VAR, tt };
  auto temp = makeNewTemp(qt);

  auto call = new CallExpr(fn);
  call->insertAtTail(temp);

  for (auto ast : node->actuals()) {
    // TODO: Handle copy-init calls? Or should these be referential?
    types::QualifiedType qtAst;
    auto e1 = convertExpr(ast, rv, &qtAst);
    auto e2 = storeInTempIfNeeded(e1, qtAst);
    call->insertAtTail(e2);
  }

  auto ret = storeInTempIfNeeded(call, qt);

  return ret;
}

template <typename Iter>
void TConverter::convertAndInsertPrimCallActuals(CallExpr* call,
                                                 const Iter& actuals,
                                                 RV& rv) {
  // use a simple strategy to convert prim calls
  // (assuming no named argument passing)
  // TODO: do some associated actions need to be handled here?
  enterCallActuals(call);
  for (auto ast : actuals) {
    call->insertAtTail(convertExpr(ast, rv));
  }
  exitCallActuals();
}

Expr* TConverter::convertPrimCallOrNull(const Call* node, RV& rv) {
  auto primCall = node->toPrimCall();
  if (!primCall) return nullptr;

  // there should not be associated actions for primitive calls
  auto re = rv.byAstOrNull(node);
  if (re) {
    for (auto a : re->associatedActions()) {
      // For now, allow only MOVE_INIT actions, indicating that the result of
      // this primitive is intended to be 'moved'.
      //
      // Note: Right now call-init-deinit is casting a wide net and including
      // integers in this analysis, so we might get rid of this check long
      // term.
      INT_ASSERT(a.action() == AssociatedAction::MOVE_INIT);
      INT_ASSERT(a.id() == node->id());
    }
  }

  using namespace chpl::uast::primtags;
  Expr* ret = nullptr;
  if (primCall->prim() == PRIM_RT_ERROR) {
    ret = new CallExpr(primCall->prim(), new_CStringSymbol("<cannot handle PRIM_RT_ERROR without strings>"));
  } else if (primCall->prim() == PRIM_TO_UNMANAGED_CLASS ||
             primCall->prim() == PRIM_TO_BORROWED_CLASS ||
             primCall->prim() == PRIM_TO_NILABLE_CLASS ||
             primCall->prim() == PRIM_TO_NON_NILABLE_CLASS) {
    auto qt = rv.byAst(primCall).type();
    auto sym = convertType(qt.type())->symbol;
    auto arg = convertExpr(primCall->actual(0), rv);
    arg = storeInTempIfNeeded(arg, rv.byAst(primCall->actual(0)).type());
    ret = new CallExpr(PRIM_CAST, sym, arg);
  } else if (primCall->prim() == chpl::uast::primtags::PRIM_FIELD_BY_NUM) {
    auto re = rv.byAst(primCall->actual(1));
    int64_t idx = re.type().param()->toIntParam()->value();
    types::QualifiedType qtField;
    ret = codegenGetField(primCall->actual(0), idx-1, rv, &qtField);
    ret = storeInTempIfNeeded(ret, qtField);
  } else {
    ret = new CallExpr(primCall->prim());

    convertAndInsertPrimCallActuals(toCallExpr(ret), node->actuals(), rv);

    handlePostCallActions(toCallExpr(ret), node, re, rv);
  }

  return ret;
}

static std::tuple<Expr*, Symbol*, types::QualifiedType>
locateFieldSymbolAndType(TConverter* tc,
                         Expr* receiver,
                         const types::QualifiedType& qtReceiver,
                         const char* fieldName,
                         int fieldIndex) {
  // Construct an error value we can return when convenient.
  std::tuple error { nullptr, ((Symbol*) nullptr), types::QualifiedType() };
  auto t = qtReceiver.type();
  auto cls = t->toClassType();
  auto ct = t ? t->getCompositeType() : nullptr;
  Expr* base = receiver;

  // Nothing we can do if we don't have a workable receiver type.
  if (!cls && !ct) return error;

  //
  // TODO: Do tuple and record setup here and then streamline the followup.
  //
  if (auto tt = t->toTupleType()) {
    if (fieldName != nullptr) {
      INT_ASSERT(false && "Fetching tuple index by name!");
    }

    bool inBounds = (0 <= fieldIndex && fieldIndex < tt->numElements());
    if (!inBounds) return error;

    auto fieldType = tt->elementType(fieldIndex);

    if (auto at = toAggregateType(tc->convertType(ct))) {
      // Adjust for the presence of the generated 'size' field.
      const int idx = (fieldIndex + 2);
      return { base, at->getField(idx), fieldType };
    }

    return error;
  }

  //
  // Else, it's a regular composite.
  //

  auto rc = createDummyRC(tc->context);
  auto dpo = resolution::DefaultsPolicy::IGNORE_DEFAULTS;
  auto& rfds = fieldsForTypeDecl(&rc, ct, dpo);
  auto at = toAggregateType(tc->convertType(ct));

  int dynoFieldIndex = -1;

  // TODO: Fetching superclass field (non-flat access).
  if (fieldName != nullptr) {
    bool found = false;

    for (int i = 0; i < rfds.numFields(); i++) {
      if (rfds.fieldName(i) != fieldName) continue;
      dynoFieldIndex = i;
      found = true;
      break;
    }

    if (!found && (cls || ct->isBasicClassType())) {
      auto bct = ct->toBasicClassType();
      types::QualifiedType qtSuper = {types::QualifiedType::CONST_IN,
                                      bct->parentClassType()};
      if (0==strcmp(fieldName, "super")) {
        return { base, at->getField(1), qtSuper };
      } else {
        Expr* newBase = new CallExpr(PRIM_CAST,
                                      tc->convertType(qtSuper.type())->symbol,
                                      base);
        newBase = tc->storeInTempIfNeeded(newBase, qtSuper);
        return locateFieldSymbolAndType(tc, newBase,
                                           qtSuper,
                                           fieldName, -1);
      }
    }
  } else {
    dynoFieldIndex = fieldIndex;
  }

  bool inBounds = (0 <= dynoFieldIndex && dynoFieldIndex < rfds.numFields());
  if (!inBounds) return error;

  auto fieldType = rfds.fieldType(dynoFieldIndex);

  // TODO: Is it appropriate to always use the 'BasicClassType' here?
  if (at) {
    int superOffset = 0;
    if (auto bct = ct->toBasicClassType()) {
      if (bct->parentClassType() != nullptr) {
        superOffset = 1;
      }
    }

    // TODO: Currently we do two scans (consulting 'ResolvedFields' above also
    // does a scan to find the field index). Is there a way we can get away
    // with only doing one while also retrieving the field's frontend type?
    const int idx = (dynoFieldIndex + 1) + superOffset;
    auto field = at->getField(idx);
    if (fieldName) {
      CHPL_ASSERT(0==strcmp(fieldName, field->name));
    }
    return { base, field, fieldType };
  }

  return error;
}

static bool isMethodContainedInMethod(Context* context, const ID& id) {
  using namespace parsing;
  if (id.isEmpty() || !idIsMethod(context, id)) return false;
  for (auto up = id.parentSymbolId(context); up;
            up = up.parentSymbolId(context)) {
    if (idIsMethod(context, up)) return true;
    if (idIsModule(context, up)) break;
  }
  return false;
}

static Expr* codegenGetFieldImpl(TConverter* tc,
                                 primtags::PrimitiveTag prim,
                                 Expr* inRecv,
                                 const types::QualifiedType& inQtRecv,
                                 const char* fieldName,
                                 int fieldIndex,
                                 TConverter::RV& rv,
                                 types::QualifiedType* outQt) {
  INT_ASSERT(fieldName || fieldIndex >= 0);
  auto qtRecv = inQtRecv;
  Expr* recv = inRecv;

  if (recv == nullptr) {
    const TypedFnSignature* sig = nullptr;
    if (auto rf = tc->cur.resolvedFunction) sig = rf->signature();

    if (sig && sig->isMethod()) {
      if (auto& id = sig->id()) {
        if (isMethodContainedInMethod(tc->context, id)) {
          // TODO: Have to poke around or have the frontend give more context.
          TC_UNIMPL("Disambiguating implicit 'this' for nested methods!");
        }
      }

      recv = tc->codegenImplicitThis(rv);
      qtRecv = sig->formalType(0);

    } else {
      // TODO: Have to poke around or have the frontend give more context.
      TC_UNIMPL("Fetching field from implicit receiver!");
      return TC_PLACEHOLDER(tc);
    }
  }

  // The receiver does not have a useable type, so bail out.
  if (!qtRecv.hasTypePtr() || qtRecv.isUnknownOrErroneous() ||
      !qtRecv.type()->getCompositeType()) {
    return TC_PLACEHOLDER(tc);
  }

  // Compute the other pieces we need to perform the field access.
  auto [base, sym, qtField] = locateFieldSymbolAndType(tc, recv, qtRecv,
                                                       fieldName,
                                                       fieldIndex);
  if (!base) return TC_PLACEHOLDER(tc);

  // Should not be generating fetches to compile-time values.
  // TODO: Except for RTTs...
  INT_ASSERT(!qtField.isType() && !qtField.isParam());

  if (0 == strcmp(sym->name, "super")) {
    prim = PRIM_GET_MEMBER_VALUE;
  } else if (prim == PRIM_UNKNOWN) {
    prim = qtField.isRef() ? PRIM_GET_MEMBER_VALUE : PRIM_GET_MEMBER;
  }

  INT_ASSERT(prim == PRIM_GET_MEMBER_VALUE ||
             prim == PRIM_GET_MEMBER);

  auto ret = new CallExpr(prim, base, new SymExpr(sym));

  // The type of the field was requested.
  if (outQt) {
    if (prim == PRIM_GET_MEMBER || sym->isRef()) {
      // Adjust it to have the 'ref' type since that's what a fetch produces.
      auto kp = resolution::KindProperties::fromKind(qtField.kind());
      kp.setRef(true);
      types::QualifiedType qt { kp.toKind(), qtField.type(), qtField.param() };
      INT_ASSERT(kp.valid() && !qt.isUnknownOrErroneous());
      *outQt = qt;
    } else {
      *outQt = qtField;
    }
  }

  return ret;
}

Expr*
TConverter::codegenGetField(const AstNode* recvAst,
                            const char* fieldName, RV& rv,
                            types::QualifiedType* outQt) {
  types::QualifiedType qtRecv;
  auto recv = recvAst ? convertExpr(recvAst, rv, &qtRecv) : nullptr;
  const int fieldIndex = -1;
  auto ret = codegenGetFieldImpl(this, PRIM_UNKNOWN, recv, qtRecv,
                                 fieldName,
                                 fieldIndex,
                                 rv, outQt);
  return ret;
}

Expr* TConverter::codegenGetField(const AstNode* recvAst,
                                  int fieldIndex, RV& rv,
                                  types::QualifiedType* outQt) {
  types::QualifiedType qtRecv;
  auto recv = recvAst ? convertExpr(recvAst, rv, &qtRecv) : nullptr;
  const char* fieldName = nullptr;
  auto ret = codegenGetFieldImpl(this, PRIM_UNKNOWN, recv, qtRecv,
                                 fieldName,
                                 fieldIndex,
                                 rv, outQt);
  return ret;
}

// Gets its own little helper for now in the event that finding the implicit
// this becomes more compilcated down the road, e.g., for nested functions
Expr* TConverter::codegenImplicitThis(RV& rv) {
  INT_ASSERT(cur.fnSymbol && cur.fnSymbol->isMethod());
  return new SymExpr(cur.fnSymbol->_this);
}

// TODO: The frontend should tell us this is happening so that we don't have
// to pattern-match against this. E.g., generate a method for param indexing.
Expr* TConverter::convertIntrinsicTupleIndexingOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  // Param tuple indexing e.g., 'tuple[0]'.
  if (ci.name() == USTR("this") && ci.numActuals() == 2) {
    auto qt1 = ci.actual(0).type();
    auto qt2 = ci.actual(1).type();
    auto tt = qt1.type() ? qt1.type()->toTupleType() : nullptr;

    if (tt && qt2.param() && qt2.type() && qt2.type()->isIntType()) {
      auto recv = actualAsts[0];
      auto idx = qt2.param()->toIntParam()->value();

      if (auto recvRe = rv.byAstOrNull(recv)) {
        if (auto id = recvRe->toId()) {
          auto ast = parsing::idToAst(context, id);
          if (ast->isVarArgFormal()) {

            INT_ASSERT(cur.fnSymbol);
            int count = 0;
            for_formals(arg, cur.fnSymbol) {
              if (arg->hasFlag(FLAG_EXPANDED_VARARGS)) {
                if (count == idx) {
                  return new SymExpr(arg);
                }
                count += 1;
              }
            }
          }
        }
      }

      // Generate a field access (which handles any tuple-specific details).
      types::QualifiedType qtField;
      auto fetch = codegenGetField(actualAsts[0], idx, rv, &qtField);
      auto ret = storeInTempIfNeeded(fetch, qtField);
      return ret;
    }
  }

  return nullptr;
}

Expr* TConverter::convertFieldInitOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  // If not in an initializer, then 'this.x = ...' cannot be an init point.
  if ((!cur.fnSymbol->isInitializer() &&
      !cur.fnSymbol->isCopyInit()) ||
      ci.name() != USTR("=")) {
    return nullptr;
  }

  // If the call maps directly to a function then it is not initialization.
  if (re->mostSpecific().only()) return nullptr;

  // No code to generate for initialize type/param fields
  if (auto type = ci.actual(0).type();
      type.isParam() || type.isType()) {
    return nullptr;
  }

  // Search for a relevant initialization action.
  const AssociatedAction* action = nullptr;
  for (const auto& a : re->associatedActions()) {
    if (a.action() == AssociatedAction::COPY_INIT ||
        a.action() == AssociatedAction::INIT_OTHER) {
      action = &a;
      break;
    }
  }

  CallExpr* ret = nullptr;

  // Emit a 'init=' call if an action was found, or a PRIM_MOVE otherwise.
  if (action) {

    // TODO: Encapsulate this whole process of "convert a call using a TFS"
    // into a separate helper method.
    auto init = action->fn();
    INT_ASSERT(init && init->isInitEquals());

    const ResolvedFunction* rf = nullptr;
    if (paramElideCallOrNull(init, re->poiScope(), &rf)) {
      INT_ASSERT(false && "Should not have param elided initializer call!");
    } else if (!rf) {
      return TC_PLACEHOLDER(this);
    }

    auto fn = findOrConvertFunction(rf);
    INT_ASSERT(fn);

    // Need to rename the field initialization CI from '=' to 'init=' so that
    // it does not register as an 'OpCall', which influences the behavior of
    // the FAMap
    auto initCI = resolution::CallInfo::copyAndRename(ci, USTR("init="));
    auto fam = FormalActualMap(init->untyped(), initCI);
    INT_ASSERT(fam.isValid());

    auto call = new CallExpr(fn);
    convertAndInsertActuals(call, node, actualAsts, init, fam, rv);
    ret = call;

  } else {
    // If no associated action was found then emit a bitcopy.
    // TODO: Adjusting reference levels (e.g., insert 'PRIM_DEREF').
    ret = new CallExpr(PRIM_ASSIGN);
    convertAndInsertPrimCallActuals(ret, actualAsts, rv);
  }

  INT_ASSERT(ret);

  return ret;
}

Expr* TConverter::convertIntrinsicCastOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  // The call is not a cast.
  if (ci.name() != USTR(":")) return nullptr;

  auto& qt = re->type();
  INT_ASSERT(qt.type());

  // The cast was not typed, so the AST is malformed.
  if (qt.isUnknownOrErroneous()) return TC_PLACEHOLDER(this);

  if (qt.isParam()) {
    // The param value was not set, so the code is malformed.
    if (!qt.hasParamPtr()) return TC_PLACEHOLDER(this);

    // Otherwise, convert the param value and return it.
    auto sym = convertParam(re->type());
    auto ret = new SymExpr(sym);
    return ret;
  }

  // Match casts by actual type(s).
  INT_ASSERT(node->numActuals() == 2);
  auto& qt1 = rv.byAst(node->actual(0)).type();
  auto& qt2 = rv.byAst(node->actual(1)).type();

  // The cast is malformed, the second actual should be a type.
  if (qt2.kind() != types::QualifiedType::TYPE) return TC_PLACEHOLDER(this);

  if (qt1.type() == qt2.type()) {
    // TODO: we actually have to insert a copy here
    if (qt1.type()->isCPtrType()) {
      return convertExpr(node->actual(0), rv);
    } else if (qt1.type()->isPrimitiveType() ||
               qt1.type()->isStringType()) {
      // Temporary workaround to get some things working
      auto t = makeNewTemp(qt2);
      auto orig = storeInTempIfNeeded(convertExpr(node->actual(0), rv), qt1);
      insertStmt(new CallExpr(PRIM_MOVE, t, orig));
      return new SymExpr(t);
    } else {
      TC_UNIMPL("Eliding cast to same type!");
      return TC_PLACEHOLDER(this);
    }
  }

  const ResolvedFunction* rf = nullptr;
  if (auto fn = convertFunctionForGeneratedCall(ci, cur.symbol, &rf)) {
    // Otherwise, using the provided CI, try to resolve a call to a cast
    // function. Many times intrinsic casts are still implemented using
    // a standard/internal module function.

    INT_ASSERT(rf && rf->signature());
    auto tfs = rf->signature();

    auto fam = FormalActualMap(tfs, ci);
    INT_ASSERT(fam.isValid());

    auto ret = new CallExpr(fn);
    convertAndInsertActuals(ret, node, actualAsts, tfs, fam, rv);
    return ret;
  }

  // There was no call, so we missed a case! Return a placeholder.
  TC_UNIMPL("Unhandled intrinsic cast!");
  return TC_PLACEHOLDER(this);
}

Expr* TConverter::convertIntrinsicLogicalOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  // The call is not a logical operator.
  if (ci.name() != USTR("&&") && ci.name() != USTR("||")) return nullptr;

  // If the call maps directly to a function then it is a "normal" call.
  if (re->mostSpecific().only()) return nullptr;

  // If the type is not the bool type then it is not an intrinsic.
  // TODO: And both types must be 'bool' or implicitly convertible.
  if (re->type().type() != types::BoolType::get(context)) return nullptr;

  INT_ASSERT(node->numActuals() == 2);
  auto arg1 = node->actual(0);
  auto arg2 = node->actual(1);
  auto reArg1 = rv.byAstOrNull(arg1);

  bool isLhsParamFalse = reArg1 && reArg1->type().isParamFalse();
  bool isLhsParamTrue = reArg1 && reArg1->type().isParamTrue();

  // TODO: Can we just have the parent converter routine param-fold this?
  bool canFoldAwayTree = (ci.name() == USTR("&&") && isLhsParamFalse) ||
                         (ci.name() == USTR("||") && isLhsParamTrue);
  bool canFoldAwayCond = (ci.name() == USTR("&&") && isLhsParamTrue) ||
                         (ci.name() == USTR("||") && isLhsParamFalse);

  if (canFoldAwayTree) {
    return isLhsParamTrue ? new SymExpr(gTrue) : new SymExpr(gFalse);
  }

  if (canFoldAwayCond) {
    return convertExpr(arg2, rv);
  }

  //
  // Otherwise we need to implement the lowered short-circuiting behavior.
  //
  //  var f = a() && b() && c();
  //  ...
  //            &&
  //          /    \
  //        &&      \
  //      /    \     \
  //    a()    b()   c()
  //
  //  (...Produced by recursion to left sub-tree...)
  //
  //  var tmp1
  //  move tmp1, a()
  //
  //  if tmp1 {
  //    var tmp2
  //    move tmp2, b()
  //    move tmp1, tmp2
  //  }
  //
  //  (...Produced by evaluating root '&&'...)
  //
  //  if tmp1 {
  //    var tmp3
  //    move tmp3, c()
  //    move tmp1, tmp3
  //  }
  //
  // For '||' the structure is the same, but the condition is negated.
  //
  // The temporaries in the branch may be elided automatically since
  // the expression 'b()' is wrapped in a 'storeInTempIfNeeded(...)'
  // call.
  //

  // First, evaluate the left sub-tree.
  types::QualifiedType qtArg1;
  auto exprArg1 = convertExpr(arg1, rv, &qtArg1);

  if (qtArg1.isRef()) {
    qtArg1 = KindProperties::removeRef(qtArg1);
  }
  // Make a temp...
  auto temp = makeNewTemp(qtArg1);

  // Move the left sub-tree into the temp.
  auto moveArg1 = new CallExpr(PRIM_MOVE, temp, exprArg1);
  insertStmt(moveArg1);

  // Prepare the condition.
  auto thenBlock = new BlockStmt();
  pushBlock(thenBlock);

  types::QualifiedType qtArg2;
  auto exprArg2 = convertExpr(arg2, rv, &qtArg2);
  auto tempArg2 = storeInTempIfNeeded(exprArg2, qtArg2);

  auto moveArg2 = new CallExpr(PRIM_MOVE, temp, tempArg2);
  insertStmt(moveArg2);

  popBlock();

  // Negate the condition if we are dealing with '||'.
  Expr* cond = new SymExpr(temp);
  if (ci.name() == USTR("||")) {
    const ResolvedFunction* rf = nullptr;
    auto ciNeg = resolution::CallInfo::createSimple(ustr("!"), qtArg1);
    auto fnNeg = convertFunctionForGeneratedCall(ciNeg, node, &rf);
    INT_ASSERT(fnNeg && rf);

    // TODO: Negation is not in a temporary, is that OK?
    auto bt = types::QualifiedType(types::QualifiedType::VAR, types::BoolType::get(context));
    cond = storeInTempIfNeeded(new CallExpr(fnNeg, cond), bt);
  }

  // Insert the prepared branch into the tree.
  auto branch = new CondStmt(cond, thenBlock, nullptr);
  insertStmt(branch);

  // But the result is the temporary.
  auto ret = new SymExpr(temp);

  return ret;
}

Expr* TConverter::convertExternAssignmentOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  if (ci.name() != USTR("=")) return nullptr;

  bool isExtern = false;
  if (ci.actual(0).type().type()->isExternType() &&
      ci.actual(1).type().type()->isExternType()) {
    isExtern = true;
  } else if (ci.actual(0).type() == ci.actual(1).type()) {
    auto type = ci.actual(0).type().type();
    if (auto ct = type->toCompositeType()) {
      isExtern = ct->linkage() == uast::Decl::Linkage::EXTERN;
    }
  }

  if (!isExtern) return nullptr;

  types::QualifiedType rhs;
  auto rhsExpr = convertExpr(node->actual(1), rv, &rhs);
  rhsExpr = storeInTempIfNeeded(rhsExpr, rhs);
  types::QualifiedType lhs;
  auto lhsExpr = convertExpr(node->actual(0), rv, &lhs);
  lhsExpr = storeInTempIfNeeded(lhsExpr, lhs);

  auto assign = new CallExpr(PRIM_ASSIGN, lhsExpr, rhsExpr);

  return assign;
}

Expr* TConverter::convertTupleExpand(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  if (ci.name() != USTR("...")) return nullptr;

  auto qt = rv.byAst(node->actual(0)).type();
  auto tup = storeInTempIfNeeded(convertExpr(node->actual(0), rv), qt);
  return new CallExpr(PRIM_TUPLE_EXPAND, tup);
}

Expr* TConverter::convertGroupedAssign(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  // In practice, we have something like:
  //
  // (OpCall =
  //   (Tuple a, b)
  //   (FnCall helper)
  //
  // and there are associated 'ASSIGN' actions on 'a' and 'b'
  auto op = node->toOpCall();
  if (!op || op->op() != USTR("=")) return nullptr;

  auto lhs = op->actual(0)->toTuple();
  if (!lhs) return nullptr;

  auto rhs = op->actual(1);
  INT_ASSERT(rhs->isFnCall());

  types::QualifiedType rhsQt;
  auto rhsExpr = convertExpr(rhs, rv, &rhsQt);
  INT_ASSERT(rhsQt.type()->isTupleType());
  auto rhsSym = storeInTempIfNeeded(rhsExpr, rhsQt);

  for (int i = 0; i < lhs->numActuals(); i++) {
    auto lhsAst = lhs->actual(i);
    types::QualifiedType lhsQt;
    auto lhsExpr = convertExpr(lhsAst, rv, &lhsQt);
    lhsExpr = storeInTempIfNeeded(lhsExpr, lhsQt);

    // Extract the i'th element from the RHS tuple.
    Expr* getElem = new CallExpr(PRIM_GET_MEMBER, rhsSym->copy(),
                                new_CStringSymbol(astr("x", istr(i))));
    getElem = storeInTempIfNeeded(getElem, rhsQt.type()->toTupleType()->elementType(i));

    auto re = rv.byAst(lhsAst);
    INT_ASSERT(re.hasAssociatedActions());
    INT_ASSERT(re.associatedActions().size() == 1);
    auto action = re.associatedActions()[0];
    INT_ASSERT(action.action() == AssociatedAction::ASSIGN);

    // Assign it to the LHS element.
    const ResolvedFunction* rf;
    auto elide = paramElideCallOrNull(action.fn(), re.poiScope(), &rf);
    INT_ASSERT(!elide);
    auto calledFn = findOrConvertFunction(rf);
    CallExpr* ret = new CallExpr(calledFn, lhsExpr, getElem);
    insertStmt(ret);
  }
  return new CallExpr(PRIM_NOOP);;
}

Expr* TConverter::convertNamedCallOrNull(const Call* node, RV& rv) {
  auto re = rv.byAstOrNull(node);

  // Regular calls need the info from the 'ResolvedExpression'.
  if (!re) return nullptr;

  // In mose cases the call should be specified in 're->mostSpecific()'.
  auto& candidate = re->mostSpecific().only();
  auto sig = candidate.fn();
  bool noCandidateForCall = !candidate || !sig;

  // Fetch the resolved function and elide the call entirely if possible.
  // TODO: Can we make another overload that just takes the AST/RV?
  //       Can we elide a call even if we don't know the signature if
  //       the type of the call is 'param'?
  const ResolvedFunction* rf = nullptr;
  if (sig) {
    if (Expr* elided = paramElideCallOrNull(sig, re->poiScope(), &rf)) {
      return elided;
    } else if (!rf) {
      return nullptr;
    }
  }

  // Create a 'CallInfo' to populate the actual ASTs as a side-effect.
  std::vector<const AstNode*> actualAsts;
  const bool raiseErrors = true;
  auto ci = resolution::CallInfo::create(context, node, rv.byPostorder(),
                                         raiseErrors,
                                         &actualAsts);

  if (sig && sig->isMethod() && !ci.isMethodCall() &&
      !ci.isOpCall()) {
    // If the call is a method call, but the CI does not indicate it,
    // then we're dealing with an implicit receiver.
    //
    // In that case, we need to adjust the CI to make it a method call.
    ci = resolution::CallInfo::createWithReceiver(ci, sig->formalType(0));
    auto fn = cur.symbol->toFunction();
    INT_ASSERT(fn && fn->isMethod());
    actualAsts.insert(actualAsts.begin(), fn->thisFormal());
  }

  // No need to resolve assignment betwen types
  if (ci.name() == USTR("=") &&
      (ci.actual(0).type().isType() || ci.actual(0).type().isParam())) {
    return TC_ELIDED(this, node);
  }

  if (noCandidateForCall) {
    // The call may be to an intrinsic handled entirely by the compiler.
    // Or it could be another call-like pattern e.g., field init-point.
    Expr* ret = nullptr;

    ret = convertFieldInitOrNull(node, re, actualAsts, ci, rv);
    if (ret) return ret;

    ret = convertIntrinsicTupleIndexingOrNull(node, re, actualAsts, ci, rv);
    if (ret) return ret;

    ret = convertIntrinsicCastOrNull(node, re, actualAsts, ci, rv);
    if (ret) return ret;

    ret = convertIntrinsicLogicalOrNull(node, re, actualAsts, ci, rv);
    if (ret) return ret;

    ret = convertExternAssignmentOrNull(node, re, actualAsts, ci, rv);
    if (ret) return ret;

    ret = convertTupleExpand(node, re, actualAsts, ci, rv);
    if (ret) return ret;

    ret = convertGroupedAssign(node, re, actualAsts, ci, rv);
    if (ret) return ret;

    TC_UNIMPL("Unhandled named call with no candidate!");
    return TC_PLACEHOLDER(this);
  }

  auto calledFn = findOrConvertFunction(rf);
  CallExpr* ret = new CallExpr(calledFn);

  // This mapping drives the conversion of actuals.
  auto& fam = candidate.formalActualMap();
  INT_ASSERT(fam.isValid());

  convertAndInsertActuals(ret, node, actualAsts, sig, fam, rv);

  if (fVerify && calledFn->isMethod() && calledFn->numFormals() > 0) {
    auto arg = toSymExpr(ret->get(1));
    INT_ASSERT(arg->symbol() != gMethodToken);
  }

  return ret;
}

TConverter::ActualConverter::ActualConverter(
                                TConverter* tc,
                                const AstNode* astForCall,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::TypedFnSignature* tfs,
                                const FormalActualMap& fam,
                                RV& rv)
    : tc_(tc), tfs_(tfs), astForCall_(astForCall),
      actualAsts_(actualAsts),
      fam_(fam),
      rv_(rv),
      stmtInsertionPoint_(tc->cur.stmtList()) {
  INT_ASSERT(stmtInsertionPoint_);

  // Use the arity provided by 'fam' since it has already accounted for
  // variable-arity formals such as varargs when computing alignment.
  INT_ASSERT(fam_.isValid());

  // Determine if the call requires any defaults at all.
  for (auto& fa : fam_.byFormals()) {
    callRequiresDefaults_ |= fa.hasDefault();
  }

  // Exit early if defaults are not required.
  if (!callRequiresDefaults_) return;

  if (!tfs_) {
    TC_UNIMPL("Converting default arguments without 'TypedFnSignature'!");
    return;
  }

  prepareCalledFnConversionState();
}

void TConverter::ActualConverter::prepareCalledFnConversionState() {
  //
  // Otherwise, we prepare to convert default arguments by setting up
  // a temporary "conversion state" for the called function.
  //
  Context* context = tc_->context;

  auto re = rv_.byAstOrNull(astForCall_);
  INT_ASSERT(re);

  ResolutionContext rcval(context);
  auto rf = resolveFunction(&rcval, tfs_, re->poiScope());
  INT_ASSERT(rf);

  ID parentModule = parsing::idToParentModule(context, rf->id());
  auto moduleSymbol = tc_->findOrSetupModule(parentModule);

  // Prepare the state block for the called function.
  // TODO: Helper function?
  // NOTE: Also, GCC is not happy with an initializer-list, here.
  auto& s = calledFnState_;
  s = {};
  s.symbol                 = parsing::idToAst(context, rf->id());
  s.fnSymbol               = tc_->findOrConvertFunction(rf);
  s.moduleSymbol           = moduleSymbol;
  s.retVar                 = nullptr;
  s.epilogueLabel          = nullptr;
  s.resolvedFunction       = rf;
  s.actualConverter        = this;
  s.moduleFromLibraryFile  = moduleSymbol->hasFlag(FLAG_PRECOMPILED);
  s.topLevelModTag         = moduleSymbol->modTag;
}

Expr* TConverter::ActualConverter::convertActual(const FormalActual& fa) {
  if (tc_->shouldElideFormal(tfs_, fa.formalIdx())) return nullptr;

  // One or the other must hold in order to compute the slot index.
  INT_ASSERT(fa.hasDefault() || fa.hasActual());

  if (fa.hasActual()) {
    return convertActualWithArg(fa);
  } else {
    return convertActualUsingDefault(fa);
  }
}

Expr* TConverter::ActualConverter::convertActualWithArg(const FormalActual& fa) {
  Context* context = tc_->context;
  INT_ASSERT(fa.hasActual());

  // If there is no default argument then an actual is present.
  int idxActual = fa.actualIdx();
  INT_ASSERT(0 <= idxActual && idxActual < ((int) actualAsts_.size()));

  // And there should be AST there.
  auto astActual = fa.hasActual() ? actualAsts_[idxActual] : nullptr;
  INT_ASSERT(astActual);

  // Convert the actual and leave.
  types::QualifiedType actualType;
  Expr* actualExpr = nullptr;
  if (auto formal = astActual->toFormal()) {
    // We don't have a 'this' identifier for implicit method calls, so we
    // use the 'this' formal's uAST as a signal to generate the implicit
    // 'this'.
    INT_ASSERT(formal->name() == USTR("this"));
    actualExpr = tc_->codegenImplicitThis(rv_);
  } else {
    actualExpr = tc_->convertExpr(astActual, rv_, &actualType);
  }

  Expr* temp = nullptr;
  if (CallExpr* call = toCallExpr(actualExpr);
      call && call->isPrimitive(PRIM_TUPLE_EXPAND)) {
    // expanded later
    temp = actualExpr;
  } else {
    temp = tc_->storeInTempIfNeeded(actualExpr, actualType);
  }

  // Note: Assumes that an unknown formal indicates some kind of fabricated
  // formal/actual map based on an untyped signature. This can happen when
  // we create a _new wrapper (which doesn't exist in the frontend)
  // and we want to insert actuals.
  if (!fa.formalType().isUnknown() &&
      fa.formalType().type() != fa.actualType().type()) {
    auto got = canPassScalar(context, fa.actualType(), fa.formalType());
    if (got.converts() &&
        got.conversionKind() != CanPassResult::ConversionKind::TO_REFERENTIAL_TUPLE) {
      bool isExternFn = parsing::idIsExtern(context, tfs_->untyped()->id());
      if (isExternFn &&
          fa.formalType().type()->isCStringType() &&
          astActual->isStringLiteral()) {
        // When passing a string literal to an extern 'c' function expecting
        // a c_string, just create a c_string directly instead of allocating
        // a class.
        auto sym = new_CStringSymbol(
            astActual->toStringLiteral()->value().c_str());
        temp = new SymExpr(sym);
      } else {
        auto kind = got.conversionKind();
        if (kind == CanPassResult::ConversionKind::BORROWS ||
            kind == CanPassResult::ConversionKind::BORROWS_SUBTYPE) {
          auto ct = fa.actualType().type()->toClassType();
          Expr* borrow = nullptr;

          if (ct->manager()) {
            auto mrt = types::QualifiedType(fa.actualType().kind(),
                                            ct->managerRecordType(context));
            types::QualifiedType qtField;
            borrow = codegenGetFieldImpl(tc_, PRIM_UNKNOWN, temp, mrt,
                                         "chpl_p",
                                         -1,
                                         rv_, &qtField);
            borrow = tc_->storeInTempIfNeeded(borrow, qtField);
          } else {
            borrow = temp;
          }

          auto type = tc_->convertType(fa.formalType().type());
          temp = tc_->storeInTempIfNeeded(new CallExpr(PRIM_CAST, type->symbol, borrow), fa.formalType());
        } else {
          auto type = tc_->convertType(fa.formalType().type());
          temp = tc_->storeInTempIfNeeded(new CallExpr(PRIM_CAST, type->symbol, temp), fa.formalType());
        }
      }
    }
  } else if (SymExpr* se = toSymExpr(temp)) {
    // Insert dereference temps as-needed based on the formal/actual types
    //
    // TODO: Can we better represent this in the frontend to save on these
    // kinds of heuristics here? Perhaps by re-using the 'access' information
    // from 'maybe-const'?
    if (auto re = rv_.byAstOrNull(astActual); re && re->hasAssociatedActions()) {
      INT_ASSERT(re->associatedActions().size() == 1);
      auto action = re->associatedActions()[0].action();
      if (action == AssociatedAction::ASSIGN ||
          action == AssociatedAction::MOVE_INIT) {
        if (se->isRef()) {
          types::QualifiedType type = { types::QualifiedType::VAR,
                                        re->type().type() };
          temp = tc_->insertDerefTemp(se, type);
        }
      } else if (action == AssociatedAction::NEW_INIT) {
        INT_ASSERT(astActual->isFnCall());
        INT_ASSERT(astActual->toFnCall()->calledExpression()->isNew());
      } else {
        TC_UNIMPL("unhandled associated action on an actual");
      }
    } else {
      if (fa.actualType().isRef() &&
          se->isRef() &&
          !fa.formalType().isUnknownKindOrType() &&
          !fa.formalType().isRef()) {
        types::QualifiedType type = { types::QualifiedType::VAR,
                                      fa.formalType().type() };
        temp = tc_->insertDerefTemp(se, type);
      }
    }
  }

  return temp;
}

Expr* TConverter::ActualConverter::convertActualUsingDefault(const FormalActual& fa) {
  Context* context = tc_->context;

  // One or the other must hold in order to compute the slot index.
  INT_ASSERT(fa.hasDefault());

  // Note: 'calledFnState_' is only populated if there are default args
  // Otherwise we need a default argument. Get the formal's init expression.
  const Function* fn = calledFnState_.symbol->toFunction();
  const Decl* decl = fn->formal(fa.formalIdx());
  const AstNode* initExpr = nullptr;
  if (auto var = decl->toVarLikeDecl()) {
    initExpr = var->initExpression();
  } else {
    TC_UNIMPL("Unhandled formal when unpacking init expression!");
  }

  // If there is no uAST for an init-expression and the function is not
  // compiler-generated, then there is nothing we can do.
  CHPL_ASSERT(initExpr || decl->id().isFabricatedId());

  //
  // At this point, we are resolving a default argument value. In order to
  // do so, we have to swap on the temporary "conversion state" that we
  // prepared for the called function, run the converter on the formal's
  // init-expr in order to generate AST, and then swap back to the
  // "conversion state" of the function that contains the call. To handle
  // the case where a formal's init-expr refers to another formal, the
  // converter is set up to replace references to formals with actuals
  // "on demand".
  //

  auto rf = calledFnState_.resolvedFunction;
  INT_ASSERT(rf);

  auto& rr = rf->resolutionById();

  // Swap on the called function state.
  std::swap(calledFnState_, tc_->cur);
  BlockStmt* block = new BlockStmt();
  tc_->pushBlock(block);

  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rvCalledFn(&rcval, fn, *tc_, rr);

  // Convert the init-expression for the formal of interest, or generate a
  // default-value for the type if function is compiler-generated.
  Expr* expr = nullptr;
  types::QualifiedType exprType;
  if (initExpr == nullptr && fa.hasDefault()) {
    INT_ASSERT(!fa.hasActual());
    INT_ASSERT(tfs_->untyped()->isCompilerGenerated());
    INT_ASSERT(tfs_->untyped()->name() == USTR("init"));
    // The formal is part of a compiler-generated function, so we can just
    // construct a default-value for the type as uAST will not be supplied.
    INT_ASSERT(!decl || decl->id().isFabricatedId());

    auto t = tfs_->formalType(fa.formalIdx()).type();
    expr = tc_->defaultValueForType(t, decl, rvCalledFn);
    exprType = fa.formalType();

  } else {
    // Otherwise, invoke the typed converter using the new visitor.
    expr = tc_->convertExpr(initExpr, rvCalledFn, &exprType);
  }

  expr = tc_->storeInTempIfNeeded(expr, exprType);

  // TODO: Handle conversions as needed!
  // Swap off the called function state.
  tc_->popBlock();
  std::swap(calledFnState_, tc_->cur);
  stmtInsertionPoint_->insertAtTail(block);
  block->flattenAndRemove();

  auto se = toSymExpr(expr);
  INT_ASSERT(se);
  return se;
}

int TConverter::ActualConverter::
expandTupleHelper(CallExpr* call,
                  const AstNode* actualAst,
                  Expr* tupleExpr) {
  auto astActual = actualAst->toOpCall();
  INT_ASSERT(astActual->op() == USTR("..."));
  auto ident = astActual->actual(0)->toIdentifier();
  auto id = rv_.byAst(ident).toId();
  auto tup = parsing::idToAst(tc_->context, id);

  int counter = 0;
  if (tup->isVarArgFormal()) {
    for_formals(arg, tc_->cur.fnSymbol) {
      if (!arg->hasFlag(FLAG_EXPANDED_VARARGS)) continue;
      call->insertAtTail(new SymExpr(arg));
      counter++;
    }
  } else {
    auto qt = rv_.byAst(tup).type().type()->toTupleType();
    for (int i = 0; i < qt->numElements(); i++) {
      auto getField = new CallExpr(PRIM_GET_MEMBER_VALUE, tupleExpr->copy(), new_CStringSymbol(astr("x", istr(i))));
      auto temp = tc_->storeInTempIfNeeded(getField, qt->elementType(i));
      call->insertAtTail(temp);
    }
    counter = qt->numElements();
  }

  return counter;
}

void TConverter::ActualConverter::
convertAndInsertActuals(CallExpr* call, bool skipReceiver) {
  if (fam_.numFormalsMapped() == 0) {
    return;
  }

  auto savedExprList = tc_->cur.lastList();
  auto savedStmtList = tc_->cur.stmtList();

  tc_->enterCallActuals(call);

  int i = 0;
  int skipUnpack = 0;
  for (const FormalActual& fa : fam_.byFormals()) {
    const int curIdx = i++;
    if (skipUnpack > 0) {
      // We may skip formal/actual pairs in the event of a tuple expansion
      skipUnpack--;
      continue;
    }
    if (skipReceiver && curIdx == 0) continue;

    if (auto expr = this->convertActual(fa)) {
      // Is usually a SymExpr, but can be a PRIM_TUPLE_EXPAND which needs
      // to remain in the call's actuals
      if (CallExpr* actual = toCallExpr(expr)) {
        auto actualAst = fa.hasActual() ? actualAsts_[fa.actualIdx()] : nullptr;
        INT_ASSERT(actual->isPrimitive(PRIM_TUPLE_EXPAND));
        skipUnpack += expandTupleHelper(call, actualAst, actual->get(1));
      } else if (SymExpr* se = toSymExpr(expr)) {
        call->insertAtTail(expr);

        if (auto decl = fa.formal()) {
          // A vararg formal can never be referred to by another formal.
          if (decl->isVarArgFormal()) continue;

          // Add entries so that slots can be looked up by formal ID.
          auto it = formalIdToActualSymbol_.find(decl->id());
          INT_ASSERT(it == formalIdToActualSymbol_.end());
          formalIdToActualSymbol_.emplace(decl->id(), se->symbol());
        }
      }
    }
  }

  tc_->exitCallActuals();

  // Ensure the 'current AST lists' match - converter may swap contexts.
  INT_ASSERT(tc_->cur.stmtList() == savedStmtList);
  INT_ASSERT(tc_->cur.lastList() == savedExprList);
}

// TODO: How are implicit receivers handled here? There won't be an AST for it?
void TConverter::convertAndInsertActuals(
                                CallExpr* c,
                                const AstNode* node,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::TypedFnSignature* tfs,
                                const FormalActualMap& fam,
                                RV& rv,
                                bool skipReceiver) {
  ActualConverter ac(this, node, actualAsts, tfs, fam, rv);
  ac.convertAndInsertActuals(c, skipReceiver);
}

Symbol* TConverter::ActualConverter::interceptFormalUse(const ID& id) {
  auto it = formalIdToActualSymbol_.find(id);
  if (it != formalIdToActualSymbol_.end()) {
    return it->second;
  } else {
    INT_ASSERT(false && "Expected entry for formal ID!");
  }
  return nullptr;
}

void TConverter::handlePostCallActions(CallExpr* c,
                                       const Call* node,
                                       const resolution::ResolvedExpression* re,
                                       RV& rv) {
}

ArgSymbol* TConverter::convertFormal(const Formal* fml, RV& rv) {
  auto typeExpr = convertRuntimeTypeExpression(fml->typeExpression(), rv);
  if (typeExpr) {
    TC_UNIMPL("runtime type formal");
  }

  ArgSymbol* ret = findOrCreateFormal(fml);

  attachSymbolAttributes(context, fml, ret, cur.moduleFromLibraryFile);

  setVariableType(fml, ret, rv);

  // Special handling for implicit receiver formal.
  if (fml->name() == USTR("this")) {
    ret->addFlag(FLAG_ARG_THIS);
    if (ret->intent == INTENT_TYPE) setupTypeIntentArg(ret);
  }

  if (parsing::idIsInBundledModule(context, fml->id())) {
    if (fml->initExpression()) {
      auto block = new BlockStmt();
      pushBlock(block);
      types::QualifiedType qt;
      auto expr = convertAstUntyped(fml->initExpression());
      block->insertAtTail(expr);
      popBlock();
      ret->defaultExpr = block;
    }
  }

  return ret;
}

// See: 'resolution/expandVarArgs.cpp'
//
// Some big benefits to working with the typed converter. First off, we
// don't need to create a dummy tuple to inject into the 'where' clause,
// because the where is already param-evaluated. So we can drop that code.
// Next, we can also drop worrying about the lifetime constraints code.
void TConverter::convertAndInsertVarArgs(const VarArgFormal* fml, RV& rv) {
  auto fn = cur.fnSymbol;

  fn->addFlag(FLAG_EXPANDED_VARARGS);

  auto typeExpr = convertRuntimeTypeExpression(fml->typeExpression(), rv);
  if (typeExpr) {
    TC_UNIMPL("runtime type formal");
  }

  if (auto re = rv.byAstOrNull(fml)) {
    auto& qt = re->type();
    auto tt = qt.type() ? qt.type()->toTupleType() : nullptr;

    if (tt) {
      for (int i = 0; i < tt->numElements(); i++) {
        // See: 'insertFormalsForVarArg'

        auto qtElement = tt->elementType(i);
        auto name = astr("_e", istr(i), "_", fml->name().c_str());

        // Start with 'INTENT_BLANK', but we'll overwrite it below.
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, name, dtUnknown);

        // Manually set the formal's fields.
        arg->originalIntent = convertFormalIntent(fml->intent());
        arg->intent = convertFormalIntentQt(qtElement.kind());
        arg->variableExpr = nullptr;
        arg->type = convertType(qtElement.type());
        arg->addFlag(FLAG_EXPANDED_VARARGS);

        attachSymbolAttributes(context, fml, arg, cur.moduleFromLibraryFile);

        auto def = new DefExpr(arg);
        fn->insertFormalAtTail(def);
      }
    }
  }
}

bool TConverter::enter(const Module* node, RV& rv) {
  TC_DEBUGF(this, "enter module %s %s\n", node->id().str().c_str(),
            asttags::tagToString(node->tag()));

  if (modulesToConvert.count(node->id()) == 0) {
    // this module should not be converted.
    TC_DEBUGF(this, "Not in modules to convert\n");
    return false;
  }

  if (node == cur.symbol) {
    // we are visiting it to convert the module initialization function
    // so proceed with the traversal
    TC_DEBUGF(this, "Proceeding with module converting\n");
    return true;
  } else {
    TC_DEBUGF(this, "Recording a submodule\n");
    submodulesEncountered.push_back(node);

    return false;
  }
  return true;
}

void TConverter::exit(const Module* node, RV& rv) {
  TC_DEBUGF(this, "exit module %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool
TConverter::shouldElideFormal(const TypedFnSignature* tfs, int formalIdx) {
  INT_ASSERT(tfs && 0 <= formalIdx && formalIdx < tfs->numFormals());
  bool ret = false;

  auto& qt = tfs->formalType(formalIdx);

  if (qt.isType() || qt.isParam()) {
    // In general, always elide 'type' and 'param' formals. The information
    // they provide does not need to be carried around because the frontend
    // has already resolved the program.
    ret = true;

    // But keep around formals with runtime types.
    if (qt.isType() && typeExistsAtRuntime(qt.type())) ret = false;

  } else if (tfs->formalName(formalIdx) == USTR("this") &&
             tfs->isOperator()) {
    // We can skip receivers for operator methods as they should have
    // the 'TYPE' or 'BLANK' intent. If they don't then the frontend
    // should have caught this and emitted an error.
    ret = true;
  }

  return ret;
}

// This helper function performs the process of translating useful information
// about the 'dyno' function and attaching it to the converted function in the
// form of flags (this is how the old compiler conveys the majority of its
// information about symbols).
//
// Since there are quite a few potential flags, it makes sense to separate out
// this process to avoid bloating the rest of the process of converting a
// function.
static void attachFunctionFlags(TConverter* tc, const Function* node,
                                FnSymbol* fn) {
  Context* context = tc->context;
  auto& cur = tc->cur;
  const ResolvedFunction* rf = cur.resolvedFunction;
  INT_ASSERT(rf);

  attachSymbolAttributes(context, node, fn, cur.moduleFromLibraryFile);
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

  if (node->isMethod() && node->kind() != uast::Function::OPERATOR) {
    fn->addFlag(FLAG_METHOD);
    if (node->isPrimaryMethod()) {
      fn->addFlag(FLAG_METHOD_PRIMARY);
    }
  }

  fn->addFlag(FLAG_RESOLVED);
  fn->addFlag(FLAG_RESOLVED_EARLY);
  fn->setNormalized(true);

  if (cur.resolvedFunction->signature()->instantiatedFrom() != nullptr) {
    // generic instantiations are "invisible"
    fn->addFlag(FLAG_INVISIBLE_FN);
  }

  if (auto tfs = cur.resolvedFunction->signature()) {
    auto ufs = tfs->untyped();
    if (ufs->isCompilerGenerated()) {
      // Mark the function as being compiler-generated if appropriate.
      fn->addFlag(FLAG_COMPILER_GENERATED);
    }
  }

  if (fn->name == astrDeinit) {
    fn->addFlag(FLAG_DESTRUCTOR);
  }

  if (isAssignOp(node->name())) {
    fn->addFlag(FLAG_ASSIGNOP);
  }

  if (node->kind() == uast::Function::ITER) {
    fn->addFlag(FLAG_ITERATOR_FN);

  } else if (node->kind() == uast::Function::OPERATOR) {
    fn->addFlag(FLAG_OPERATOR);

  } else if (node->isAnonymous()) {
    fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
    fn->addFlag(FLAG_ANONYMOUS_FN);
    if (node->kind() == uast::Function::LAMBDA) {
      fn->addFlag(FLAG_LEGACY_LAMBDA);
    }
  }

  if (auto t = rf->returnType().type()) {
    if (t->isVoidType()) {
      fn->addFlag(FLAG_VOID_NO_RETURN_VALUE);
    }
  }
}

bool TConverter::enter(const Function* node, RV& rv) {
  enterScope(node, rv);
  TC_DEBUGF(this, "enter function %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  if (node != cur.symbol) {
    // It's a function, but not the one we are working on.
    // Stop the traversal. This Function will be handled when
    // convertFunction is called.
    return false;
  }

  TC_DEBUGF(this, "Really converting Function %s\n", node->name().c_str());

  astlocMarker markAstLoc(node->id());

  INT_ASSERT(cur.resolvedFunction != nullptr);

  FnSymbol* fn = new FnSymbol(astr(node->name()));

  ModuleSymbol* mod = nullptr;
  if (auto id = parsing::idToParentModule(context, node->id())) {
    mod = findOrSetupModule(id);
  }

  INT_ASSERT(mod);

  // Insert the function flattened, directly into the module.
  mod->block->insertAtTail(new DefExpr(fn));

  // Free to set as caller should have saved.
  cur.moduleSymbol = mod;
  cur.fnSymbol = fn;

  // note the correspondence between the ResolvedFunction & what it converts to
  // (for calls & done here to support recursive calls)
  fns[cur.resolvedFunction] = fn;

  fn->userString = constructUserString(node);

  attachFunctionFlags(this, node, fn);

  //
  // Begin by looping over and converting the formals.
  //

  if (node->numFormals() > 0) {
    // create the formals first & put them in a map
    TC_DEBUGF(this, "Converting formals\n");
    enterFormals(fn);
    // TODO: gather the formals we want to create
    // to un-pack tuples etc.

    int formalIdx = -1;
    for (auto decl : node->formals()) {
      formalIdx += 1;
      astlocMarker markAstLoc(decl->id());

      // Some formals (e.g., `type`/`param`) can be skipped.
      auto tfs = cur.resolvedFunction->signature();
      if (shouldElideFormal(tfs, formalIdx)) continue;

      // Fill in each ArgSymbol and add the DefExpr
      if (auto fml = decl->toFormal()) {
        auto arg = convertFormal(fml, rv);

        if (arg->hasFlag(FLAG_ARG_THIS)) {
          fn->thisTag = arg->intent;
          fn->_this = arg;
        }

        fn->insertFormalAtTail(arg);

      } else if (auto va = decl->toVarArgFormal()) {
        convertAndInsertVarArgs(va, rv);

      } else if (decl->toTupleDecl()) {
        // A tuple decl, where components are formals or tuple decls.
        TC_UNIMPL("Unhandled tuple formal");
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
        TC_UNIMPL("Unhandled formal");
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
    INT_ASSERT(node->id().postOrderId() == -1);
    fn->cname = astr(node->id().symbolPath());
  }

  RetTag retTag = convertRetTag(node->returnIntent());
  Expr* retType = convertRuntimeTypeExpression(node->returnType(), rv);
  Expr* whereClause = nullptr;

  Expr* lifetimeConstraints = nullptr;
  if (node->numLifetimeClauses() > 0) {
    TC_DEBUGF(this, "Converting lifetime clause\n");
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

  fn->retType = convertType(cur.resolvedFunction->returnType().type());

  if (fn->returnsRefOrConstRef() && !fn->retType->isRef()) {
    fn->retType = fn->retType->refType;
  }

  // visit the body to convert
  if (node->body()) {
    TC_DEBUGF(this, "Converting body into [%i]\n", fn->body->id);
    pushBlock(fn->body);

    if (fn->retType != dtVoid) {
      // construct the RVV
      cur.retVar = newTemp("ret", fn->retType);
      cur.retVar->addFlag(FLAG_RVV);

      if (fn->retTag == RET_PARAM)      cur.retVar->addFlag(FLAG_PARAM);
      if (fn->retTag == RET_TYPE)       cur.retVar->addFlag(FLAG_TYPE_VARIABLE);
      if (fn->hasFlag(FLAG_MAYBE_TYPE)) cur.retVar->addFlag(FLAG_MAYBE_TYPE);

      fn->insertAtHead(new DefExpr(cur.retVar));
    }

    // construct the epilogue label
    cur.epilogueLabel = new LabelSymbol(astr("_end_", fn->name));
    cur.epilogueLabel->addFlag(FLAG_EPILOGUE_LABEL);
    // note: label is added to the AST later

    for (auto stmt : node->body()->stmts()) {
      astlocMarker markAstLoc(stmt->id());
      stmt->traverse(rv);
    }

    if (cur.fnSymbol->isInitializer() ||
        cur.fnSymbol->isCopyInit()) {
      auto inits = cur.resolvedFunction->implicitInits();
      if (auto it = inits.find(ID()); it != inits.end()) {
        auto& vec = it->second;
        for (auto [ct, id] : vec) {
          convertImplicitInit(ct, id, rv);
        }
      }
    }


    // add the epilogue label to the AST
    fn->insertAtTail(new DefExpr(cur.epilogueLabel));

    // TODO: add deinit calls for the end-of-block actions for the function here

    // add the single return
    if (fn->retType == dtVoid) {
      fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    } else {
      fn->insertAtTail(new CallExpr(PRIM_RETURN, cur.retVar));
    }

    popBlock();
  }

  simplifyEpilogue(fn);

  if (trace) {
    TC_DEBUGF(this, "converted to: ");
    nprint_view(fn);
    TC_DEBUGF(this, "\n");
  }

  // clear return variables so they can't be confused
  cur.retVar = nullptr;
  cur.epilogueLabel = nullptr;

  if (auto attr = node->attributeGroup()) {
    UniqueString name = UniqueString::get(context, "dyno.printConvertedAst");
    if (attr->getAttributeNamed(name)) {
      // If the dyno attribute is set, print the converted AST.
      TC_DEBUGF(this, "Converted AST for %s:\n", node->id().str().c_str());
      print_view(fn);
    }
  }

  if (fVerify && fn->isMethod() && fn->numFormals() > 0) {
    auto arg = fn->getFormal(1);
    INT_ASSERT(arg->type != dtMethodToken);
  }

  if (parsing::idIsNestedFunction(context, node->id())) {
    // Allow nested functions to be hoisted to the top-level of the module,
    // but do not allow them to be candidates for production's resolution.
    fn->addFlag(FLAG_INVISIBLE_FN);
  }

  return false;
}

void TConverter::exit(const Function* node, RV& rv) {
  exitScope(node, rv);
  TC_DEBUGF(this, "exit function %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Variable* node, RV& rv) {
  TC_DEBUGF(this, "enter variable %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  auto sym = convertVariable(node, rv, true);

  // OK for type variables to do nothing
  // TODO: assert that sym==null if it's a type alias only
  INT_ASSERT(sym ||
             node->kind() == uast::Variable::TYPE ||
             node->kind() == uast::Variable::PARAM);

  return false;
}

void TConverter::exit(const Variable* node, RV& rv) {
  TC_DEBUGF(this, "exit variable %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const TupleDecl* node, RV& rv) {
  if (node->isTupleDeclFormal()) {
    enterScope(node, rv);
    return true;
  }

  types::QualifiedType initQt;
  auto initExpr = convertExpr(node->initExpression(), rv, &initQt);
  auto tup = makeNewTemp(initQt);
  insertStmt(new CallExpr(PRIM_MOVE, tup, initExpr));
  for (int i = 0; i < node->numDecls(); i++) {
    auto decl = node->decl(i);
    Expr* declInit = new CallExpr(PRIM_GET_MEMBER_VALUE,
                                 tup,
                                 new_CStringSymbol(astr("x", istr(i))));
    declInit = storeInTempIfNeeded(declInit, initQt.type()->toTupleType()->elementType(i));
    auto sym = convertVariable(decl->toVariable(), rv, true, nullptr, declInit);
    INT_ASSERT(sym);
  }
  return false;
}
void TConverter::exit(const TupleDecl* node, RV& rv) {
  if (node->isTupleDeclFormal()) {
    exitScope(node, rv);
  }
}

bool TConverter::enter(const Literal* node, RV& rv) {
  TC_DEBUGF(this, "enter literal %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));


  Expr* se = convertAstUntyped(node);
  insertExpr(se);

  return false;
}
void TConverter::exit(const Literal* node, RV& rv) {
  TC_DEBUGF(this, "exit literal %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

static std::tuple<const AstNode*, UniqueString>
accessExpressionDetails(const AstNode* node) {
  if (auto x = node->toIdentifier()) {
    return { nullptr, x->name() };
  } else if (auto x = node->toDot()) {
    return { x->receiver(), x->field() };
  }
  return {};
}

static bool
accessAppearsToBeFieldAccess(TConverter* tc, const AstNode* node,
                             TConverter::RV& rv) {
  Context* context = tc->context;

  if (auto re = rv.byAstOrNull(node)) {
    if (auto& id = re->toId()) {
      // The frontend has determined this to refer to a field, so trust it.
      if (parsing::idIsField(context, id)) return true;

    } else if (auto candidate = re->mostSpecific().only()) {
      if (auto sig = candidate.fn()) {
        if (sig->isCompilerGenerated()) {
          // It's a field accessor we generated.
          if (parsing::idIsField(context, sig->id())) return true;
        }
        return false;
      }
    }
  }

  auto [recvAst, fieldName] = accessExpressionDetails(node);
  std::ignore = recvAst;

  // Special cases for fields the compiler handles directly.
  if (fieldName == USTR("super")) return true;

  return false;
}

Expr* TConverter::convertFieldAccessOrNull(const AstNode* node, RV& rv) {
  INT_ASSERT(node && (node->isIdentifier() || node->isDot()));

  if (!accessAppearsToBeFieldAccess(this, node, rv)) return nullptr;

  auto re = rv.byAstOrNull(node);
  auto candidate = re->mostSpecific().only();
  auto sig = candidate ? candidate.fn() : nullptr;

  // The latter should hold because a non-compiler-generated signature is
  // not considered to be field access (and should always be called).
  INT_ASSERT(!sig || sig->isCompilerGenerated());

  if (sig && genDefaultFieldAccessorWrapperFns) {
    // The frontend invokes a compiler-generated field accessor, and the
    // typed converter was configured to respect that, so treat this as
    // as a regular parenless call.
    return convertParenlessCallOrNull(node, rv);
  }

  // Otherwise we'll do our best to generate a "plain-old" field access.
  types::QualifiedType qtField;
  auto [recvAst, fieldName] = accessExpressionDetails(node);

  types::QualifiedType qtRecv;
  Expr* recv = recvAst ? convertExpr(recvAst, rv, &qtRecv) : nullptr;
  recv = recv ? storeInTempIfNeeded(recv, qtRecv) : nullptr;

  // Handle 'chpl_p' field access for owned/shared classes, and also simulate
  // forwarding of class fields on owned/shared classes.
  //
  // TODO: May need to handle actual _owned records as well
  if (auto ct = recvAst ? qtRecv.type()->toClassType() : nullptr;
      ct && ct->manager()) {
    auto mrt = types::QualifiedType(qtRecv.kind(),
                                    ct->managerRecordType(context));
    // Can always generate an access for 'chpl_p', since we're either accessing
    // that field directly, or forwarding to it.
    auto fwd = codegenGetFieldImpl(this, PRIM_UNKNOWN, recv, mrt,
                                   "chpl_p",
                                   -1,
                                   rv, &qtField);
    if (fieldName == "chpl_p") {
      return storeInTempIfNeeded(fwd, qtField);
    } else {
      recv = storeInTempIfNeeded(fwd, qtField);
      qtRecv = qtField;
    }
  }

  const int fieldIndex = -1;
  auto get = codegenGetFieldImpl(this, PRIM_UNKNOWN, recv, qtRecv,
                                 fieldName.c_str(),
                                 fieldIndex,
                                 rv, &qtField);

  // NOTE: Field accesses should be stored in temps to make the rest of
  // the compiler passes after callDestructors happy (part of the normal
  // form).
  auto ret = storeInTempIfNeeded(get, qtField);

  return ret;
}

Expr* TConverter::convertParenlessCallOrNull(const AstNode* node, RV& rv) {
  INT_ASSERT(node && (node->isIdentifier() || node->isDot()));

  auto re = rv.byAstOrNull(node);
  auto candidate = re->mostSpecific().only();
  auto sig = candidate ? candidate.fn() : nullptr;
  if (!sig) return nullptr;

  // Fetch the resolved function and elide the call entirely if possible.
  const ResolvedFunction* rf = nullptr;
  if (Expr* elided = paramElideCallOrNull(sig, re->poiScope(), &rf)) {
    return elided;
  } else if (!rf) {
    return TC_PLACEHOLDER(this);
  }

  auto calledFn = findOrConvertFunction(rf);
  Expr* ret = nullptr;

  if (sig->isMethod()) {
    if (node->isIdentifier()) {
      ret = new CallExpr(calledFn, codegenImplicitThis(rv));
    } else {
      auto [recvAst, fieldName] = accessExpressionDetails(node);
      std::ignore = fieldName;
      auto thisType = rf->signature()->formalType(0);
      if (!thisType.isType()) {
        types::QualifiedType qtRecv;
        auto recv = recvAst ? convertExpr(recvAst, rv, &qtRecv) : nullptr;
        ret = new CallExpr(calledFn, storeInTempIfNeeded(recv, qtRecv));
      } else {
        ret = new CallExpr(calledFn);
      }
    }
  } else {
    ret = new CallExpr(calledFn);
  }

  ret = storeInTempIfNeeded(ret, re->type());
  return ret;
}

bool TConverter::enter(const Dot* node, RV& rv) {
  TC_DEBUGF(this, "enter dot %s %s\n", node->id().str().c_str(),
                  asttags::tagToString(node->tag()));

  Expr* expr = nullptr;
  if (Expr* x = convertFieldAccessOrNull(node, rv)) {
    expr = x;
  } else if (Expr* x = convertParenlessCallOrNull(node, rv)) {
    expr = x;
  }

  if (!expr) {
    TC_UNIMPL("Unhandled kind of dot expression!");
    expr = TC_PLACEHOLDER(this);
  }

  insertExpr(expr);

  return false;
}

void TConverter::exit(const Dot* node, RV& rv) {}

bool TConverter::enter(const Identifier* node, RV& rv) {
  TC_DEBUGF(this, "enter identifier %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
  Expr* expr = nullptr;

  if (node->name() == USTR("nil")) {
    expr = new SymExpr(gNil);

  } else if (auto re = rv.byAstOrNull(node)) {
    if (Expr* x = convertFieldAccessOrNull(node, rv)) {
      expr = x;

    } else if (Expr* x = convertParenlessCallOrNull(node, rv)) {
      expr = x;

    } else if (auto& id = re->toId()) {
      // If advanced disambiguation failed, try to do a simple ID lookup.
      Symbol* sym = findOrCreateSym(id, rv);
      expr = new SymExpr(sym);
    }
  }

  if (!expr) {
    TC_UNIMPL("Identifier case not handled!");
    expr = TC_PLACEHOLDER(this);
  }

  insertExpr(expr);

  return false;
}

void TConverter::exit(const Identifier* node, RV& rv) {
  TC_DEBUGF(this, "exit identifier %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Return* node, RV& rv) {
  TC_DEBUGF(this, "enter return %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
  CallExpr* ret = new CallExpr(PRIM_RETURN);

  insertStmt(ret);
  enterCallActuals(ret);

  return true;
}

SymExpr* TConverter::insertClassConversion(types::QualifiedType to,
                                           types::QualifiedType from,
                                           const AstNode* astForErr,
                                           Expr* fromExpr,
                                           RV& rv) {
  // TODO: This would be better served via associated actions.
  auto ct = to.type()->toClassType();
  if (auto mt = ct->managerRecordType(context)) {
    types::QualifiedType recvQt = { to.kind(), mt };

    std::vector<CallInfoActual> mtActuals;
    mtActuals.push_back(CallInfoActual(from));

    auto ci1 = resolution::CallInfo::createSimple(USTR("init="), mtActuals);
    auto ci2 = resolution::CallInfo::createWithReceiver(ci1, recvQt);

    // Search for 'init()' using the generated call.
    const ResolvedFunction* rf = nullptr;
    auto fn = convertFunctionForGeneratedCall(ci2, astForErr, &rf);

    VarSymbol* initTemp = newTemp("initTemp", convertType(recvQt.type()));
    insertStmt(new DefExpr(initTemp));

    insertStmt(new CallExpr(fn, initTemp, fromExpr));
    return new SymExpr(initTemp);
  } else {
    auto cast = new CallExpr(PRIM_CAST, convertType(to.type())->symbol, fromExpr);
    return storeInTempIfNeeded(cast, to);
  }
}

void TConverter::exit(const Return* node, RV& rv) {
  TC_DEBUGF(this, "exit return %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  CallExpr* ret = exitCallActuals();
  INT_ASSERT(ret && ret->isPrimitive(PRIM_RETURN) && ret->numActuals() <= 1);
  ret->remove();

  // normalize returns to use the Return Value Variable (RVV)
  if (node->value() != nullptr) {
    Expr* retExpr = ret->get(1)->remove();
    CallExpr* move = nullptr;
    auto retQt = rv.byAst(node->value()).type();
    auto temp = storeInTempIfNeeded(retExpr, retQt);
    if (cur.fnSymbol->returnsRefOrConstRef() && !retQt.isRef()) {
      move = new CallExpr(PRIM_MOVE,
                          cur.retVar, new CallExpr(PRIM_ADDR_OF, temp));
    } else {
      auto re = rv.byAstOrNull(node);
      auto commonType = cur.resolvedFunction->returnType();
      if (retQt.type() != commonType.type() &&
          commonType.type()->isClassType()) {
        // TODO: this should be an associated action
        temp = insertClassConversion(commonType, retQt, node, temp, rv);
        move = new CallExpr(PRIM_MOVE, cur.retVar, temp);
      } else if (re && re->hasAssociatedActions()) {
        auto action = re->associatedActions()[0];
        if (action.action() == resolution::AssociatedAction::ASSIGN) {
          const ResolvedFunction* rf;
          INT_ASSERT(!paramElideCallOrNull(action.fn(), re->poiScope(), &rf));
          auto fn = findOrConvertFunction(rf);
          move = new CallExpr(fn, cur.retVar, temp);
        } else if (action.action() == resolution::AssociatedAction::MOVE_INIT) {
          move = new CallExpr(PRIM_MOVE, cur.retVar, temp);
        } else {
          TC_UNIMPL("Unhandled associated action on return value");
          move = new CallExpr(PRIM_MOVE, cur.retVar, temp);
        }
      } else {
        move = new CallExpr(PRIM_MOVE, cur.retVar, temp);
      }
    }
    insertStmt(move);
  }

  // replace with GOTO(epilogue)
  auto jump = new GotoStmt(GOTO_RETURN, cur.epilogueLabel);
  insertStmt(jump);
  markReturn();
}

bool TConverter::enter(const Call* node, RV& rv) {
  TC_DEBUGF(this, "enter call %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  // Branch to handle the different 'call-like' constructs.
  Expr* expr = nullptr;
  if (auto x = convertPrimCallOrNull(node, rv)) {
    expr = x;
  } else if (auto x = convertMoveInitAssignOrNull(node, rv)) {
    expr = x;
  } else if (auto x = convertNewCallOrNull(node, rv)) {
    expr = x;
  } else if (auto x = convertTupleCallOrNull(node, rv)) {
    expr = x;
  } else if (auto x = convertNamedCallOrNull(node, rv)) {
    expr = x;
  } else if (auto x = convertEnumIntegralCastOrNull(node, rv)) {
    expr = x;
  }

  if (!expr) {
    // This may not always be the case, but for now we're missing cases.
    TC_UNIMPL("Unhandled type of call!");
    expr = TC_PLACEHOLDER(this);
  }

  insertExpr(expr);

  return false;
}

void TConverter::exit(const Call* node, RV& rv) {
  TC_DEBUGF(this, "exit call %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Conditional* node, RV& rv) {
  enterScope(node, rv);
  TC_DEBUGF(this, "enter conditional %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  // Not param-known condition; visit both branches as normal.

  auto insertCondTest = [&](types::QualifiedType condType,
                            const AstNode* node,
                            Expr* condExpr) {
    // emit a call to '_cond_test' and store the result in a temp
    auto ci = resolution::CallInfo(
                         ustr("_cond_test"),
                         /* calledType */ types::QualifiedType(),
                         /* isMethodCall */ false,
                         /* hasQuestionArg */ false,
                         /* isParenless */ false,
                         {CallInfoActual(condType)});
    FnSymbol* condFn = convertFunctionForGeneratedCall(ci, node);
    CallExpr* condCall = new CallExpr(condFn, condExpr);
    types::QualifiedType type = {types::QualifiedType::CONST_VAR,
                                 types::BoolType::get(context)};
    return storeInTempIfNeeded(condCall, type);
  };

  if (node->isExpressionLevel()) {
    INT_ASSERT(node->elseBlock());
    INT_ASSERT(node->thenBlock()->numStmts() == 1);
    INT_ASSERT(node->elseBlock()->numStmts() == 1);

    // Temp stores the result of the if expression.
    auto temp = makeNewTemp(rv.byAst(node).type());

    auto makeMove = [this, &rv, &temp](const uast::AstNode* node) {
      auto expr = convertExpr(node, rv);
      return new CallExpr(PRIM_MOVE, temp, expr);
    };

    if (auto cond = rv.byAst(node->condition()).type();
        cond.isParam()) {
      auto block = cond.isParamTrue() ? node->thenBlock() : node->elseBlock();
      insertStmt(makeMove(block->stmt(0)));
    } else {
      types::QualifiedType qtCond;
      auto condExpr = convertExpr(node->condition(), rv, &qtCond);
      auto condTempUse = storeInTempIfNeeded(condExpr, qtCond);

      if (!qtCond.type()->isBoolType()) {
        condTempUse = insertCondTest(qtCond, node->condition(), condTempUse);
      } else if (qtCond.isRef()) {
        condTempUse = insertDerefTemp(condTempUse,
                                      KindProperties::removeRef(qtCond));
      }

      // TODO: Insert conversion if necessary?
      auto thenBlock = new BlockStmt(makeMove(node->thenBlock()->stmt(0)));
      auto elseBlock = new BlockStmt(makeMove(node->elseBlock()->stmt(0)));

      // After normalize the 'IfExpr' is lowered to a 'CondStmt'.
      auto branch = new CondStmt(condTempUse, thenBlock, elseBlock);
      insertStmt(branch);
    }

    // The result of the 'if-expr' will be stored in 'temp'.
    insertExpr(new SymExpr(temp));
  } else {
    // Branch-sensitive traversal doesn't really work for the expression-level
    // case because we need to build the 'move'. So just rely on this for
    // statement-level conditionals.
    if (!branchSensitivelyTraverse(node, rv)) return false;

    auto condRE = rv.byAst(node->condition());
    astlocMarker markAstLoc(node->id());

    types::QualifiedType qtCond;
    Expr* cond = nullptr;
    VarSymbol* ifVarSym = nullptr;
    BlockStmt* thenBlock = nullptr;
    BlockStmt* elseBlock = nullptr;

    if (auto ifVar = node->condition()->toVariable()) {
      // If the condition is an 'if var', we convert it and stick the
      // variable into the 'then' block.
      INT_ASSERT(ifVar->kind() == uast::Variable::CONST ||
                 ifVar->kind() == uast::Variable::VAR);
      INT_ASSERT(ifVar->initExpression());

      uast::Variable::Kind symbolKind = ifVar->kind();

      types::QualifiedType qtInitExpr;
      auto initExpr = convertExpr(ifVar->initExpression(), rv, &qtInitExpr);

      // Generate a call to 'chpl_checkBorrowIfVar'.
      auto qtFalse = types::QualifiedType::createParamBool(context, false);
      auto ci = resolution::CallInfo(ustr("chpl_checkBorrowIfVar"),
                                     /*calledType*/ types::QualifiedType(),
                                     /*isMethodCall*/ false,
                                     /*hasQuestionArg*/ false,
                                     /*isParenless*/ false,
                                     { CallInfoActual(qtInitExpr),
                                       CallInfoActual(qtFalse) });

      // TODO: Need to make sure this temporary is destroyed, and not the
      // inner variable which just contains a '.borrow()' / the results of
      // the 'check...' call.
      auto initSymExpr = storeInTempIfNeeded(initExpr, qtInitExpr);

      const ResolvedFunction* rfCheckFn = nullptr;
      auto checkFn = convertFunctionForGeneratedCall(std::move(ci), ifVar,
                                                     &rfCheckFn);

      // Set the condition type for use later.
      qtCond = rfCheckFn->returnType();

      // Generate the 'chpl_checkBorrowIfVar' call.
      auto checkCall = new CallExpr(checkFn, initSymExpr, new SymExpr(gFalse));

      // A temp will be created for the borrow. It is the condition.
      cond = storeInTempIfNeeded(checkCall, qtCond);

      // Now generate the 'if var' itself. Use 'findOrCreateVar' to ensure
      // that code in the 'if' will find it. The variable will be
      // initialized in the 'then' block.
      ifVarSym = findOrCreateVar(ifVar);

      // Set the variable's type.
      ifVarSym->type = cond->typeInfo();

      // Attach storage and attributes. This sets 'const' if needed.
      const bool setQual = true;
      attachSymbolStorage(symbolKind, ifVarSym, setQual);
      attachSymbolAttributes(context, ifVar, ifVarSym,
                             cur.moduleFromLibraryFile);
      attachSymbolVisibility(ifVar, ifVarSym);
    } else {
      cond = convertExpr(node->condition(), rv, &qtCond);
      if (qtCond.isRef()) {
        cond = storeInTempIfNeeded(new CallExpr(PRIM_DEREF, cond),
                                   KindProperties::removeRef(qtCond));
      }
    }
    INT_ASSERT(cond);

    cond = storeInTempIfNeeded(cond, qtCond);

    // if it's not a 'bool', emit a call to '_cond_test'.
    if (auto rr = rv.byAstOrNull(node->condition())) {
      types::QualifiedType qt = rr->type();
      if (!qt.isUnknown()) {
        if (!qt.type()->isBoolType()) {
          cond = insertCondTest(qt, node->condition(), cond);
        }
      }
    }

    // convert the 'then' block
    {
      astlocMarker markAstLoc(node->thenBlock()->id());
      thenBlock = new BlockStmt();

      if (ifVarSym) {
        // Insert any 'if-var' declaration into the 'then' block.
        auto se = toSymExpr(cond);
        INT_ASSERT(se);

        thenBlock->insertAtTail(new DefExpr(ifVarSym));
        auto ifVarInitExpr = new CallExpr(PRIM_TO_NON_NILABLE_CLASS,
                                          new SymExpr(se->symbol()));
        auto ifVarInitMove = new CallExpr(PRIM_MOVE, new SymExpr(ifVarSym),
                                          ifVarInitExpr);
        thenBlock->insertAtTail(ifVarInitMove);
      }

      pushBlock(thenBlock);
      node->thenBlock()->traverse(rv);
      popBlock();
    }

    if (node->hasElseBlock()) {
      astlocMarker markAstLoc(node->elseBlock()->id());
      elseBlock = new BlockStmt();
      pushBlock(elseBlock);
      node->elseBlock()->traverse(rv);
      popBlock();
    }

    auto ret = new CondStmt(cond, thenBlock, elseBlock);
    insertStmt(ret);
  }

  return false;
}

void TConverter::exit(const Conditional* node, RV& rv) {
  exitScope(node, rv);
  TC_DEBUGF(this, "exit conditional %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const ExternBlock* node, RV& rv) {
  // TODO: Remove dependency on this + 'build.cpp'.
  auto stmt = convertAstUntyped(node);
  INT_ASSERT(isBlockStmt(stmt));
  insertStmt(stmt);
  return false;
}

void TConverter::exit(const ExternBlock* node, RV& rv) {}

static SymExpr* makeCaseCond(TConverter* tc, TConverter::RV& rv,
                             const uast::When* when,
                             SymExpr* selectExpr,
                             const uast::AstNode* cs) {
  auto re = rv.byAst(cs);

  // Grab the '==' ResolvedFunction
  auto action = re.getAction(AssociatedAction::COMPARE);
  INT_ASSERT(action);
  auto cmp = action->fn();

  // TODO: create a wrapper for this kind of thing
  const ResolvedFunction* rf = nullptr;
  if (tc->paramElideCallOrNull(cmp, re.poiScope(), &rf)) {
    INT_ASSERT(false && "Should not have param elided initializer call!");
  } else if (!rf) {
    return toSymExpr(TC_PLACEHOLDER(tc));
  }

  auto fn = tc->findOrConvertFunction(rf);
  INT_ASSERT(fn);

  types::QualifiedType caseType;
  Expr* caseExpr = tc->convertExpr(cs, rv, &caseType);
  caseExpr = tc->storeInTempIfNeeded(caseExpr, caseType);

  // TODO: handle case where passing to '==' has an associated action
  auto call = new CallExpr(fn, selectExpr->copy(), caseExpr);

  auto cond = tc->storeInTempIfNeeded(call, action->type());

  return cond;
}

static SymExpr* getWhenCond(TConverter* tc, TConverter::RV& rv,
                            const uast::When* when,
                            SymExpr* selectExpr) {
  if (when->numCaseExprs() == 1) {
    auto cs = when->caseExpr(0);
    return makeCaseCond(tc, rv, when, selectExpr, cs);
  } else {
    // Multiple cases should follow '||'-like short-circuiting, such that
    // equality comparisons are not evaluated unless the preceding cases do
    // not match.

    VarSymbol* agg = new VarSymbol("_case_cond_agg", dtBool);
    tc->insertStmt(new CallExpr(PRIM_MOVE, agg, gFalse));
    tc->insertStmt(new DefExpr(agg));

    int count = 0;
    for (auto cs : when->caseExprs()) {
      if (auto action = rv.byAst(cs).getAction(AssociatedAction::COMPARE)) {
        // If it's param-true, 'getWhenCond' would not have been called
        // If it's param-false, there's no need to generate any comparisons
        if (action->type().isParam()) continue;
      }

      auto cond = makeCaseCond(tc, rv, when, selectExpr, cs);

      auto thenBlock = new BlockStmt();
      thenBlock->insertAtTail(new CallExpr(PRIM_MOVE, agg, gTrue));

      auto elseBlock = new BlockStmt();
      auto branch = new CondStmt(cond, thenBlock, elseBlock);
      tc->insertStmt(branch);

      // Push the else branch so that the next case check is inserted there
      tc->pushBlock(elseBlock);
      count += 1;
    }

    // pop the blocks we pushed, to get back to the when-stmt level
    for (int i = 0; i < count; i++) {
      tc->popBlock();
    }

    return new SymExpr(agg);
  }
}

bool TConverter::enter(const Select* node, RV& rv) {
  enterScope(node, rv);
  // TODO:
  // - test case-exprs where the '==' operators have associated actions
  //   (e.g. in-intent on record argument)

  //Note: out-of-order otherwise is an error addressed by post-parse-checks

  // TODO: Trying to convert this with the branch sensitive traversal is a bit
  // awkward because we want to have only one generated select expression, and
  // would need to store that somehow between when-stmt traversals. It also
  // feels easier to manage the stacking of CondStmts here.

  types::QualifiedType selectQT;
  auto selectExpr = convertExpr(node->expr(), rv, &selectQT);
  auto selectSym = storeInTempIfNeeded(selectExpr, selectQT);

  auto traverse = [this,&rv](const When* when) {
    enterScope(when, rv);
    when->body()->traverse(rv);
    exitScope(when, rv);
  };

  int count = 0;
  for (auto when : node->whenStmts()) {
    if (when->isOtherwise()) {
      traverse(when);
    } else {
      bool anyParamTrue = false;
      bool allParamFalse = true;
      for(auto cs : when->caseExprs()) {
        if (auto action = rv.byAst(cs).getAction(AssociatedAction::COMPARE)) {
          if (!action->type().isParamFalse()) {
            allParamFalse = false;
          }

          if (action->type().isParamTrue()) {
            anyParamTrue = true;
          }
        }
      }

      // Note: this differs from traditional behavior in production, where each
      // comparison was made.
      if (anyParamTrue) {
        traverse(when);

        // Nothing else can match after this, so we're done
        break;
      } else if (!allParamFalse) {
        auto cond = getWhenCond(this, rv, when, selectSym);

        // Traversing the body creates a BlockStmt we can remove
        traverse(when);
        auto thenBlock = cur.lastList()->last()->remove();

        auto elseBlock = new BlockStmt();
        auto branch = new CondStmt(cond, thenBlock, elseBlock);
        insertStmt(branch);

        pushBlock(elseBlock);
        count += 1;
      }
    }
  }

  // For every when-stmt, pop the else-conditions off
  for (int i = 0; i < count; i++) {
    popBlock();
  }

  return false;
}

void TConverter::exit(const Select* node, RV& rv) {
  exitScope(node, rv);
  TC_DEBUGF(this, "exit select %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Block* node, RV& rv) {
  enterScope(node, rv);
  // Necessary for explicit standalone block-statements
  auto block = new BlockStmt();
  pushBlock(block);
  return true;
}

void TConverter::exit(const Block* node, RV& rv) {
  exitScope(node, rv);
  auto cur = popBlock();
  insertStmt(cur);
}

bool TConverter::enter(const Range* node, RV& rv) {
  auto re = rv.byAstOrNull(node);

  auto& candidate = re->mostSpecific().only();
  auto sig = candidate.fn();

  // TODO: Do we need to handle nested functions differently here?
  chpl::resolution::ResolutionContext rcval(context);
  auto rf = resolveFunction(&rcval, sig, re->poiScope());

  const std::vector<const AstNode*> actualAsts =
    {node->lowerBound(), node->upperBound()};

  std::vector<CallInfoActual> actuals;
  if (node->lowerBound()) {
    actuals.push_back(CallInfoActual(rv.byAst(node->lowerBound()).type()));
  }
  if (node->upperBound()) {
    actuals.push_back(CallInfoActual(rv.byAst(node->upperBound()).type()));
  }
  auto ci = resolution::CallInfo::createSimple(sig->untyped()->name(), actuals);

  auto calledFn = findOrConvertFunction(rf);
  CallExpr* ret = new CallExpr(calledFn);

  // This mapping drives the conversion of actuals.
  auto& fam = candidate.formalActualMap();
  INT_ASSERT(fam.isValid());

  convertAndInsertActuals(ret, node, actualAsts, sig, fam, rv);

  insertExpr(ret);

  return false;
}

void TConverter::exit(const Range* node, RV& rv) {
}

bool TConverter::enter(const While* node, RV& rv) {
  auto primCall = node->condition()->toPrimCall();

  if (primCall && primCall->prim() == PRIM_BLOCK_C_FOR_LOOP) {
    node->condition()->traverse(rv);
    auto cond = cur.lastList()->last()->remove();

    enterScope(node, rv);
    // Necessary for explicit standalone block-statements
    auto block = new BlockStmt();
    pushBlock(block);

    node->body()->traverse(rv);

    exitScope(node, rv);
    auto cur = popBlock();

    insertStmt(CForLoop::buildCForLoop(toCallExpr(cond), cur, {}));

    return false;
  } else {
    return true;
  }
}

void TConverter::exit(const While* node, RV& rv) {
}

bool TConverter::enter(const For* node, RV& rv) {
  enterScope(node, rv);
  if (!node->isParam()) return true;

  const ResolvedExpression& rr = rv.byAst(node);
  const ResolvedParamLoop* resolvedLoop = rr.paramLoop();

  // no param resolution results, act like a normal loop
  if (resolvedLoop == nullptr) return true;


  for (const auto& loopBody : resolvedLoop->loopBodies()) {
    BlockStmt* block = new BlockStmt();
    pushBlock(block);

    ConvertedSymbolState calledFnState_ = cur;
    std::swap(calledFnState_, cur);
    RV loopVis(rv.rc(), node, *this, loopBody);

    for (const AstNode* child : node->children()) {
      // Written to visit "all but the iterand" in case we add more
      // fields/children to the For class later.
      if (child != node->iterand()) {
        child->traverse(loopVis);
      }
    }

    std::swap(calledFnState_, cur);

    insertStmt(popBlock());
  }

  return false;
}
void TConverter::exit(const For* node, RV& rv) {
  exitScope(node, rv);
}

bool TConverter::enter(const Init* node, RV& rv) {
  auto inits = cur.resolvedFunction->implicitInits();
  if (auto it = inits.find(node->id()); it != inits.end()) {
    auto& vec = it->second;
    for (auto [ct, id] : vec) {
      convertImplicitInit(ct, id, rv);
    }
  }
  return false;
}
void TConverter::exit(const Init* node, RV& rv) {
}

bool TConverter::enter(const AstNode* node, RV& rv) {
  enterScope(node, rv);
  TC_DEBUGF(this, "enter ast %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
  return true;
}
void TConverter::exit(const AstNode* node, RV& rv) {
  exitScope(node, rv);
  TC_DEBUGF(this, "exit ast %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

chpl::owned<UastConverter> createTypedConverter(chpl::Context* context) {
  return toOwned(new TConverter(context));
}

namespace chpl::uast {
template <>
struct AstVisitorPrecondition<TConverter> {
  static bool skipSubtree(const AstNode* node, TConverter& rv) {
    return rv.isDoneExecuting() || rv.markedThrow();
  }
};

};
