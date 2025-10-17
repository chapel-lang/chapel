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

#ifndef _PASS_MANAGER_PASSES_H_
#define _PASS_MANAGER_PASSES_H_

#include "PassManager.h"
#include "baseAST.h"
#include "ValueMappedTable.h"

/***
  A header containing passes runnable by the new pass manager. The order
  they are listed here is a rough execution order. It is possible
  that some passes may be run out of order (or multiple times) in the
  future.
*/

/**
  Generate wrapper functions for extern functions that take arrays.
*/
class ExpandExternArrayCalls : public PassT<FnSymbol*> {
 public:
  bool shouldProcess(FnSymbol* fn) override;
  void process(FnSymbol* fn) override;

  static bool
  isFormalArray(ArgSymbol* formal, UnresolvedSymExpr** outType=nullptr);

  static bool isRetExprVoid(BlockStmt* retExprType);
};

/**
  Flatten all classes so they are defined at module scope.
*/
class FlattenClasses : public PassT<TypeSymbol*> {
 public:
  void process(TypeSymbol* ts) override;
};

/**
  Inserts blocks into iterator bodies after each yield which are
  responsible for handling cleanup actions at that yield point.
*/
class CreateIteratorBreakBlocks : public PassT<CallExpr*> {
 public:
  bool shouldProcess(CallExpr* call) override;
  void process(CallExpr* call) override;
};

/**
  Builds up destructor bodies by inserting destructor calls for fields.
*/
class FixupDestructors : public PassT<FnSymbol*> {
 public:
  bool shouldProcess(FnSymbol* fn) override;
  void process(FnSymbol* fn) override;
};

/**
  This pass over calls is a workaround which plugs leaks coming from forall
  exprs that are array types.
*/
class AutoDestroyLoopExprTemps: public PassT<CallExpr*> {
 public:
  bool shouldProcess(CallExpr* call) override;
  void process(CallExpr* call) override;
};

class InsertDestructorCalls: public PassT<CallExpr*> {
  public:
    bool shouldProcess(CallExpr* fn) override;
    void process(CallExpr* fn) override;
};

class LowerAutoDestroyRuntimeType: public PassT<CallExpr*> {
  public:
    bool shouldProcess(CallExpr* fn) override;
    void process(CallExpr* fn) override;
};

class InsertCopiesForYields: public PassT<CallExpr*> {
  public:
    bool shouldProcess(CallExpr* fn) override;
    void process(CallExpr* fn) override;
};

class CallDestructorsCallCleanup: public PassT<CallExpr*> {
  public:
    bool shouldProcess(CallExpr* fn) override;
    void process(CallExpr* fn) override;
};

class RemoveElidedOnBlocks: public PassT<BlockStmt*> {
  public:
    bool shouldProcess(BlockStmt* fn) override;
    void process(BlockStmt* fn) override;
};

/**
  Adjust assignment of POD types to perform bitcopies.
*/
class BulkCopyRecords : public PassT<FnSymbol*> {
 public:
  bool shouldProcess(FnSymbol* fn) override;
  void process(FnSymbol* fn) override;

  // TODO: this is a "pure" function on types, but uses
  // a map for caching. Could break out
  bool typeContainsRef(Type* t, bool isRoot = true);

  bool isTrivialAssignment(FnSymbol* fn);

  static void replaceSimpleAssignment(FnSymbol* fn);

  static bool isAssignment(FnSymbol* fn);

 private:
  std::map<Type*, bool> containsRef;
};

/**
  Remove init= and auto-copy calls for POD types.
*/
class RemoveUnnecessaryAutoCopyCalls : public PassT<FnSymbol*> {
 public:
  bool shouldProcess(FnSymbol* fn) override;
  void process(FnSymbol* fn) override;

 private:
  std::vector<CallExpr*> calls;
};

/**
  Compute all live calls to a function and store them in 'fn->calledBy'.
  This pass is a general/helper pass and will be called multiple times.
*/
class ComputeCallSitesPass : public PassT<FnSymbol*> {
  bool shouldProcess(FnSymbol* fn) override;
  void process(FnSymbol* fn) override;
};

/**
  Pull out global constants from loop runs in order to reduce loads.
*/
class LocalizeGlobals : public PassT<FnSymbol*> {
 public:
  void process(FnSymbol* fn) override;

 private:
  Map<Symbol*, VarSymbol*> globals;
  std::vector<SymExpr*> symExprs;
};

/**
  Before this pass is run the 'ComputeCallSites' pass must be run.

  TODO: How to communicate the dependency programmatically?

  Adjust all functions that return star tuples so that they take the
  star tuple as a formal, and write to that formal instead.

  The first pass iterates over all calls to functions that return
  star tuples. It inserts a new ref formal and adjusts the function
  to return void. Then it adjusts all callsites accordingly.
*/
class ReturnStarTuplesByRefArgs1 : public PassT<FnSymbol*> {
  bool shouldProcess(FnSymbol* fn) override;
  void process(FnSymbol* fn) override;
};

/**
  The second pass may be run immediately after the first pass, and
  adjusts all member accesses for star tuple fields (to handle the
  fact that they are now ref formals).
*/
class ReturnStarTuplesByRefArgs2 : public PassT<CallExpr*> {
  bool shouldProcess(CallExpr* fn) override;
  void process(CallExpr* fn) override;
};

/**
  Build module initializer functions which initialize module variables
  and run module-level statements.
*/
class AddModuleInitBlocks : public PassT<ModuleSymbol*> {
 public:
  bool shouldProcess(ModuleSymbol* mod) override;
  void process(ModuleSymbol* mod) override;
};

/**
  Add idempotent guards to ensure each module is initialized only once.
*/
class AddInitGuards : public PassT<ModuleSymbol*> {
 public:
  AddInitGuards();
  bool shouldProcess(ModuleSymbol* mod) override;
  void process(ModuleSymbol* mod) override;

  static FnSymbol* getOrCreatePreInitFn();
  static void addInitGuard(FnSymbol* fn, FnSymbol* preInitFn);
  static void addPrintModInitOrder(FnSymbol* fn);

 private:
  // This is the global preInitFn
  FnSymbol* preInitFn;
};

/**
  Insert nil checks primitives in front of most member accesses.
*/
class InsertNilChecks : public PassT<CallExpr*> {
 public:
  bool shouldProcess(CallExpr* call) override;
  void process(CallExpr* call) override;
};

/**
  This pass adjusts any symbol with a procedure-pointer type so that it is
  a new, similiar type that has line/file formals appended.

  After this pass the AST will be in an inconsistent state, and the pass
  'InsertLineNumbers' must be run in order to fix it up.

  TODO: This pass could be considered a "sub-pass" or a "hidden dependency"
        of 'InsertLineNumbers', but we don't have the machinery to express
        that at the moment.

  This pass is similiar to e.g., 'adjustAllSymbolTypes' in spirit, however
  it operates over passed in containers instead of assuming the existence
  of global vectors.
*/
class AddLineFileInfoToProcPtrTypes : public PassT<Symbol*> {
 public:
  bool shouldProcess(Symbol* sym) override;
  void process(Symbol* sym) override;
};

/**
  This pass currently depends on:

    - AddLineFileInfoToProcPtrTypes
    - ComputeCallSites

  TODO: We don't need to recompute all callsites - only the subset
        that we'll be working on. Need to figure out a way to only
        compute a small number of callsites.

  Thread line numbers and filenames through functions and calls so
  that runtime errors show line/file information. This is a heavily
  interprocedural pass.

  The first 'process' works on a function. It may potentially modify
  the signature of the function to take a line/file if necessary.
  It then scans the callsites of the function and enqueues those which
  need to have line/file numbers propagated through them.

  The second pass updates callsites to pass line/file info.
*/
class InsertLineNumbers : public PassTU<FnSymbol*, CallExpr*> {
 public:
  struct LineAndFile {
    Symbol* line = nullptr;
    Symbol* file = nullptr;
  };

  void process(FnSymbol* fn) override;
  void process(FnSymbol* fn, CallExpr* call) override;

  static int addFilenameTableEntry(const std::string& name);
  static int getFilenameTableIndex(const std::string& name);
  static const std::vector<std::string>& getFilenameTable();

 private:
  static bool shouldPreferASTLine(CallExpr* call);
  static bool mustAddLineInfoFormalsToFn(FnSymbol* fn);
  static bool mustAddLineInfoActualsToCall(CallExpr* call);
  static LineAndFile makeASTLine(CallExpr* call);
  static void insertLineNumber(CallExpr* call, LineAndFile lineAndFile);
  static void assertInvariants(FnSymbol *fn);
  LineAndFile getLineAndFileForFn(FnSymbol *fn);

  std::unordered_map<FnSymbol*, LineAndFile> lineAndFilenameMap;
  std::unordered_set<CallExpr*> fixedCalls;

  static ValueMappedTable<std::string> gFilenameTable;
};

/**
  This pass adjusts any symbol with a procedure-pointer type so that it is
  a new, similiar type where all of its widenable components are widened.
  A widenable component is e.g., a 'ref' formal or a 'class' formal.

  After this pass the AST will be in an inconsistent state, and the pass
  'InsertWideReferences' pass must be run in order to fix it up.

  TODO: Transform 'IWR' from a on old-style to new-style compiler pass.
*/
class WidenComponentsOfProcPtrTypes : public PassT<Symbol*> {
 public:
  bool shouldProcess(Symbol* sym) override;
  void process(Symbol* sym) override;
};

#endif
