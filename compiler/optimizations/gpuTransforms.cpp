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

#include <cstdio>
#include <algorithm>
#include <set>
#include <queue>
#include "stmt.h"
#include "stlUtil.h"
#include "passes.h"
#include "ModuleSymbol.h"
#include "LoopStmt.h"
#include "ForLoop.h"
#include "expr.h"
#include "driver.h"
#include "CForLoop.h"
#include "bb.h"
#include "astutil.h"
#include "optimizations.h"
#include "timer.h"

#include "global-ast-vecs.h"

static Timer gpuTransformTimer;

static bool debugPrintGPUChecks = false;
static bool allowFnCallsFromGPU = true;
static int indentGPUChecksLevel = 0;

extern int classifyPrimitive(CallExpr *call, bool inLocal);
extern bool inLocalBlock(CallExpr *call);

// ----------------------------------------------------------------------------
// Utilities
// ----------------------------------------------------------------------------

static bool isFieldAccessPrimitive(CallExpr *call) {
  return call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_GET_MEMBER) ||
      call->isPrimitive(PRIM_SET_MEMBER) ||
      call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
      call->isPrimitive(PRIM_SET_SVEC_MEMBER);
}

// If any SymExpr is referring to a variable defined outside the
// function return the SymExpr. Otherwise return nullptr
static SymExpr* hasOuterVarAccesses(FnSymbol* fn) {
  std::vector<SymExpr*> ses;
  collectSymExprs(fn, ses);
  for_vector(SymExpr, se, ses) {
    if (VarSymbol* var = toVarSymbol(se->symbol())) {
      if (var->defPoint->parentSymbol != fn) {
        if (!var->isParameter() && var != gVoid) {
          if (CallExpr* parent = toCallExpr(se->parentExpr)) {
            if (isFieldAccessPrimitive(parent)) {
              continue;
            }
          }
          return se;
        }
      }
    }
  }
  return nullptr;
}

static void errorForOuterVarAccesses(FnSymbol* fn) {
  if (SymExpr* se = hasOuterVarAccesses(fn)) {
    VarSymbol* var = toVarSymbol(se->symbol());
    INT_ASSERT(var);
    USR_FATAL(se, "variable '%s' must be defined in the function it"
                  " is used in for GPU usage", var->name);
  }
}

static VarSymbol* insertNewVarAndDef(BlockStmt* insertionPoint,
   const char* name, Type* type) {
  VarSymbol *var = new VarSymbol(name, type);
  var->defPoint = new DefExpr(var);
  insertionPoint->insertAtTail(var->defPoint);
  return var;
}

static VarSymbol* generateAssignmentToPrimitive(
    FnSymbol* fn, const char *varName, PrimitiveTag prim, Type *primReturnType) {

  VarSymbol *var = insertNewVarAndDef(fn->body, varName, primReturnType);
  CallExpr *c1 = new CallExpr(PRIM_MOVE, var, new CallExpr(prim));
  fn->insertAtTail(c1);

  return var;
}
static bool isDefinedInTheLoop(Symbol* sym, CForLoop* loop) {
  LoopStmt* curLoop = LoopStmt::findEnclosingLoop(sym->defPoint);

  while (curLoop != nullptr) {
    if (curLoop == loop) {
      return true;
    }
    curLoop = LoopStmt::findEnclosingLoop(curLoop->parentExpr);
  }

  return false;
}

// This is primarily to handle the indexOfInterest generated for promoted
// expressions. That symbol is a ref that's defined outside the for loop, but
// it is def'd and use'd only inside the block. Moreover, one of its defs is
// actually redundant and should be removed. However at this stage in the
// compilation it is not. The bottom line is, that ref could actually just be a
// local variable in the loop body. So, we handle that specially to avoid
// passing that as an argument to the GPU kernel.  TODO: investigate whether
// that def is removed later in the compilation.  Ideally move GPU transforms
// after that pass
static bool isDegenerateOuterRef(Symbol* sym, CForLoop* loop) {
  if (isDefinedInTheLoop(sym, loop) ||
      !sym->hasFlag(FLAG_TEMP)      ||
      !sym->isRef()                 ||
      !isVarSymbol(sym)) {
    return false;
  }

  for_SymbolUses(use, sym) {
      if (LoopStmt::findEnclosingLoop(use) != loop) {
        return false;
      }
    }

  for_SymbolDefs(def, sym) {
      if (LoopStmt::findEnclosingLoop(def) != loop) {
        return false;
      }
    }

  return true;
}

// ----------------------------------------------------------------------------
// GpuizableLoop
// ----------------------------------------------------------------------------

// Used to evaluate if a loop is eligible to be outlined into a GPU kernel and
// extracts information about the loop's bounds and indices.
class GpuizableLoop {
  CForLoop* loop_ = nullptr;
  FnSymbol* parentFn_ = nullptr;
  bool isEligible_ = false;
  Symbol* upperBound_ = nullptr;
  std::vector<Symbol*> loopIndices_;
  std::vector<Symbol*> lowerBounds_;
  bool shouldErrorIfNotGpuizable_;

public:
  GpuizableLoop(BlockStmt* blk);

  CForLoop* loop() const { return loop_; }
  bool isEligible() const { return isEligible_; }
  Symbol* upperBound() const { return upperBound_; }
  const std::vector<Symbol*>& loopIndices() const { return loopIndices_; }
  const std::vector<Symbol*>& lowerBounds() const { return lowerBounds_; }
  bool isIndexVariable(Symbol* sym) const {
    return std::find(loopIndices_.begin(), loopIndices_.end(), sym) !=
      loopIndices_.end();
  }

private:
  bool determineIfShouldErrorIfNotGpuizable();
  bool evaluateLoop();
  bool isAlreadyInGpuKernel();
  bool parentFnAllowsGpuization();
  bool callsInBodyAreGpuizable();
  bool attemptToExtractLoopInformation();
  bool extractIndicesAndLowerBounds();
  bool extractUpperBound();
  void reportNotGpuizable(const BaseAST* ast, const char *msg);

  bool callsInBodyAreGpuizableHelp(BlockStmt* blk,
                                   std::set<FnSymbol*>& okFns,
                                   std::set<FnSymbol*> visitedFns);
};

GpuizableLoop::GpuizableLoop(BlockStmt *blk) {
  INT_ASSERT(blk->getFunction());

  this->loop_ = toCForLoop(blk);
  this->parentFn_ = toFnSymbol(blk->getFunction());
  this->shouldErrorIfNotGpuizable_ = determineIfShouldErrorIfNotGpuizable();
  this->isEligible_ = evaluateLoop();

  // Ideally we should error out earlier than this with a more specific
  // error message but here's a final fallback error.
  // There's one use case we want to exempt, which is failure to
  // gpuize a nested loop. In this case if there was a failure to gpuize
  // the outer loop we already would have errored.
  if(this->shouldErrorIfNotGpuizable_ &&
    !this->isEligible_ &&
    !isAlreadyInGpuKernel())
  {
    USR_FATAL(blk, "Loop containing assertOnGpu() is not gpuizable");
  }
}

bool GpuizableLoop::determineIfShouldErrorIfNotGpuizable() {
  CForLoop *cfl = this->loop_;
  INT_ASSERT(cfl);

  // The gpuizable check will kick in if `assertOnGpu()` appears in the
  // body of the loop absent of any control flow.
  // It's necessary to do this instead of just checking the first
  // statement as by the time we get to gpuTransforms code may have
  // been added to the start of the loop (for example to
  // assign to the loop iteration variable if we're iterating
  // over values rather than indices)
  for_alist(expr, cfl->body) {
    CallExpr *call = toCallExpr(expr);
    if (call && call->isPrimitive(PRIM_ASSERT_ON_GPU)) {
      return true;
    }
  }

  return false;
}

bool GpuizableLoop::isAlreadyInGpuKernel() {
  return this->parentFn_->hasFlag(FLAG_GPU_CODEGEN);
}

bool GpuizableLoop::evaluateLoop() {
  CForLoop *cfl = this->loop_;
  INT_ASSERT(cfl);

  if (!cfl->inTree())
    return false;

  if (!cfl->isOrderIndependent())
    return false;

  // We currently don't support launching kernels from kernels. So if
  // the loop is within a function already marked for use on the GPU,
  // don't GPUize.
  if(isAlreadyInGpuKernel()) {
    return false;
  }

  return parentFnAllowsGpuization() &&
         callsInBodyAreGpuizable() &&
         attemptToExtractLoopInformation();
}

bool GpuizableLoop::parentFnAllowsGpuization() {
  FnSymbol *cur = this->parentFn_;
  while (cur) {
    if (cur->hasFlag(FLAG_NO_GPU_CODEGEN)) {
      reportNotGpuizable(cur, "parent function disallows execution on a GPU");
      return false;
    }

    // this is obviously a weak implementation. But the purpose is to track the
    // call chain from things like `coforall_fn`, `wrapcoforall_fn` etc, which
    // are always single invocation
    if (CallExpr *singleCall = cur->singleInvocation()) {
      cur = singleCall->getFunction();
    }
    else {
      break;
    }
  }
  return true;
}

bool GpuizableLoop::callsInBodyAreGpuizable() {
  std::set<FnSymbol*> okFns;
  std::set<FnSymbol*> visitedFns;
  return callsInBodyAreGpuizableHelp(this->loop_, okFns, visitedFns);
}

bool GpuizableLoop::callsInBodyAreGpuizableHelp(BlockStmt* blk,
                                                std::set<FnSymbol*>& okFns,
                                                std::set<FnSymbol*> visitedFns) {
  FnSymbol* fn = blk->getFunction();
  if (debugPrintGPUChecks) {
    printf("%*s%s:%d: %s[%d]\n", indentGPUChecksLevel, "",
           fn->fname(), fn->linenum(), fn->name, fn->id);
  }

  if (visitedFns.count(blk->getFunction()) != 0) {
    if (blk->getFunction()->hasFlag(FLAG_FUNCTION_TERMINATES_PROGRAM)) {
      return true; // allow `halt` to be potentially called recursively
    } else {
      reportNotGpuizable(fn, "function is recursive");
      return false;
    }
  }

  visitedFns.insert(blk->getFunction());

  std::vector<CallExpr*> calls;
  collectCallExprs(blk, calls);

  for_vector(CallExpr, call, calls) {
    if (call->primitive) {
      // only primitives that are fast and local are allowed for now
      bool inLocal = inLocalBlock(call);
      int is = classifyPrimitive(call, inLocal);
      if ((is != FAST_AND_LOCAL)) {
        reportNotGpuizable(call, "primitive is not fast and local");
        return false;
      }
    } else if (call->isResolved()) {
      if (!allowFnCallsFromGPU)
        return false;

      FnSymbol *fn = call->resolvedFunction();

      if (fn->hasFlag(FLAG_NO_GPU_CODEGEN)) {
        reportNotGpuizable(fn, "function is marked as not eligible for GPU execution");
        return false;
      }

      if(fn->hasFlag(FLAG_EXTERN) &&
        !fn->hasFlag(FLAG_GPU_CODEGEN) &&
        !fn->hasFlag(FLAG_GPU_AND_CPU_CODEGEN))
      {
        std::string msg = "function calls out to extern function (";
        msg += fn->name;
        msg += "), which is not marked as GPU eligible";
        reportNotGpuizable(fn, msg.c_str());
        return false;
      }

      if (hasOuterVarAccesses(fn)) {
        reportNotGpuizable(call, "call has outer var access");
        return false;
      }

      indentGPUChecksLevel += 2;
      if (okFns.count(fn) != 0 ||
          callsInBodyAreGpuizableHelp(fn->body, okFns, visitedFns)) {
        indentGPUChecksLevel -= 2;
        okFns.insert(fn);
      } else {
        indentGPUChecksLevel -= 2;
        return false;
      }
    }
  }
  return true;
}



bool GpuizableLoop::attemptToExtractLoopInformation() {
  // Pattern match loop boundaries to determine lower
  // and upper bounds. If we fail to match exit early.
  return extractIndicesAndLowerBounds() && extractUpperBound();
}

bool GpuizableLoop::extractIndicesAndLowerBounds() {
  if(BlockStmt* bs = toBlockStmt(loop_->initBlockGet())) {
    for_alist (expr, bs->body) {
      if(CallExpr *call = toCallExpr(expr)) {
        if(call->isPrimitive(PRIM_ASSIGN) ||
           call->isPrimitive(PRIM_MOVE)) {

          SymExpr *idxSymExpr = toSymExpr(call->get(1));
          SymExpr *boundSymExpr = toSymExpr(call->get(2));

          INT_ASSERT(idxSymExpr);
          INT_ASSERT(boundSymExpr);

          this->loopIndices_.push_back(idxSymExpr->symbol());
          this->lowerBounds_.push_back(boundSymExpr->symbol());
        }
      }
    }

    INT_ASSERT(bs->body.length == (int)this->loopIndices_.size());
    INT_ASSERT(bs->body.length == (int)this->lowerBounds_.size());
  } else {
    reportNotGpuizable(loop_, "loop indices do not match expected pattern for GPU execution");
    return false;
  }

  return true;
}

bool GpuizableLoop::extractUpperBound() {
  if(BlockStmt* bs = toBlockStmt(loop_->testBlockGet())) {
    for_exprs_postorder (expr, bs) {
      if(CallExpr *call = toCallExpr(expr)) {
        if(call->isPrimitive(PRIM_LESSOREQUAL)) {
          if(SymExpr *symExpr = toSymExpr(call->get(2))) {

            SymExpr* lhsSymExpr = toSymExpr(call->get(1));
            INT_ASSERT(lhsSymExpr && lhsSymExpr->symbol() == loopIndices_[0]);

            upperBound_ = symExpr->symbol();

            break;
          }
        }
      }
    }
  }

  if(upperBound_ == nullptr) {
    reportNotGpuizable(loop_, "upper bound does not match expected pattern for GPU execution");
    return false;
  }
  return true;
}

void GpuizableLoop::reportNotGpuizable(const BaseAST* ast, const char *msg) {
  if(this->shouldErrorIfNotGpuizable_) {
    USR_FATAL_CONT(loop_, "Loop containing assertOnGpu() is not eligible for execution on a GPU");
    USR_PRINT(ast, "%s", msg);
    USR_STOP();
  }
}

// ----------------------------------------------------------------------------
// GpuKernel
// ----------------------------------------------------------------------------

// Given a GpuizableLoop that was determined to be "eligible" we generate an
// outlined function
// for GPU code generation that:
//    - Contains computation to determine what index of the loop is being
//      processed based on GPU block and thread ID
//    - Contains a copy of loop's body
//    - Passes in any variables that are declared outside of the loop as
//      parameters to this new function.
class GpuKernel {
  const GpuizableLoop &gpuLoop;
  FnSymbol* fn_;
  std::vector<Symbol*> kernelIndices_;
  std::vector<Symbol*> kernelActuals_;
  SymbolMap copyMap_;
  bool lateGpuizationFailure_;

  public:
  SymExpr* blockSize_;

  GpuKernel(const GpuizableLoop &gpuLoop, DefExpr* insertionPoint);
  FnSymbol* fn() const { return fn_; }
  const std::vector<Symbol*>& kernelActuals() { return kernelActuals_; }
  bool lateGpuizationFailure() const { return lateGpuizationFailure_; }

  private:
  void buildStubOutlinedFunction(DefExpr* insertionPoint);
  void populateBody(CForLoop *loop, FnSymbol *outlinedFunction);
  void normalizeOutlinedFunction();
  void finalize();

  void generateIndexComputation();
  void generateEarlyReturn();
  void markGPUSubCalls(FnSymbol* fn);
  Symbol* addKernelArgument(Symbol* symInLoop);
  Symbol* addLocalVariable(Symbol* symInLoop);
};

GpuKernel::GpuKernel(const GpuizableLoop &gpuLoop, DefExpr* insertionPoint)
  : gpuLoop(gpuLoop)
  , lateGpuizationFailure_(false)
  , blockSize_(nullptr)
{
  buildStubOutlinedFunction(insertionPoint);
  populateBody(gpuLoop.loop(), fn_);
  normalizeOutlinedFunction();
  if(!lateGpuizationFailure_) {
    finalize();
  }
}

void GpuKernel::buildStubOutlinedFunction(DefExpr* insertionPoint) {
  fn_ = new FnSymbol("chpl_gpu_kernel");

  fn_->body->blockInfoSet(new CallExpr(PRIM_BLOCK_LOCAL));

  fn_->addFlag(FLAG_RESOLVED);
  fn_->addFlag(FLAG_ALWAYS_RESOLVE);
  fn_->addFlag(FLAG_GPU_CODEGEN);

  generateIndexComputation();
  generateEarlyReturn();

  insertionPoint->insertBefore(new DefExpr(fn_));
}

Symbol* GpuKernel::addKernelArgument(Symbol* symInLoop) {
  Type* symType = symInLoop->typeInfo();
  ArgSymbol* newFormal = new ArgSymbol(INTENT_IN, symInLoop->name, symType);
  fn_->insertFormalAtTail(newFormal);

  kernelActuals_.push_back(symInLoop);
  copyMap_.put(symInLoop, newFormal);

  return newFormal;
}

Symbol* GpuKernel::addLocalVariable(Symbol* symInLoop) {
  VarSymbol* newSym = toVarSymbol(symInLoop->copy());

  INT_ASSERT(newSym);

  fn()->insertAtHead(new DefExpr(newSym));
  copyMap_.put(symInLoop, newSym);

  return newSym;
}

/**
 *  For each loopIndex, generates and inserts the following AST into fn:
 *
 *  blockIdxX  = __primitive('gpu blockIdx x')
 *  blockDimX  = __primitive('gpu blockDim x')
 *  threadIdxX = __primitive('gpu threadIdx x')
 *  t0 = varBlockIdxX * varBlockDimX
 *  t1 = t0 + threadIdxX
 *  index = t1 + lowerBound
 *
 *  Also adds the loopIndex->index to the copyMap_
 **/
void GpuKernel::generateIndexComputation() {
  std::vector<Symbol*>::size_type numIndices = gpuLoop.loopIndices().size();
  INT_ASSERT(gpuLoop.lowerBounds().size() == numIndices);

  for (std::vector<Symbol*>::size_type i=0 ; i<numIndices ; i++) {
    Symbol* loopIndex  = gpuLoop.loopIndices()[i];
    Symbol* lowerBound = gpuLoop.lowerBounds()[i];

    VarSymbol *varBlockIdxX = generateAssignmentToPrimitive(fn_, "blockIdxX",
      PRIM_GPU_BLOCKIDX_X, dtInt[INT_SIZE_32]);
    VarSymbol *varBlockDimX = generateAssignmentToPrimitive(fn_, "blockDimX",
      PRIM_GPU_BLOCKDIM_X, dtInt[INT_SIZE_32]);
    VarSymbol *varThreadIdxX = generateAssignmentToPrimitive(fn_, "threadIdxX",
      PRIM_GPU_THREADIDX_X, dtInt[INT_SIZE_32]);

    VarSymbol *tempVar = insertNewVarAndDef(fn_->body, "t0",
      dtInt[INT_SIZE_32]);
    CallExpr *c1 = new CallExpr(PRIM_MOVE, tempVar, new CallExpr(
      PRIM_MULT, varBlockIdxX, varBlockDimX));
    fn_->insertAtTail(c1);

    VarSymbol *tempVar1 = insertNewVarAndDef(fn_->body, "t1",
      dtInt[INT_SIZE_32]);
    CallExpr *c2 = new CallExpr(PRIM_MOVE, tempVar1, new CallExpr(
      PRIM_ADD, tempVar, varThreadIdxX));
    fn_->insertAtTail(c2);

    Symbol* startOffset = addKernelArgument(lowerBound);
    VarSymbol* index = insertNewVarAndDef(fn_->body, "chpl_simt_index",
                                          dtInt[INT_SIZE_32]);
    fn_->insertAtTail(new CallExpr(PRIM_MOVE, index, new CallExpr(
      PRIM_ADD, tempVar1, startOffset)));

    kernelIndices_.push_back(index);
    copyMap_.put(loopIndex, index);
  }

  INT_ASSERT(kernelIndices_.size() == gpuLoop.loopIndices().size());
}

/*
 * Adds the following AST to a GPU kernel
 *
 * def chpl_is_oob;
 * chpl_is_oob = `calculated thread idx` > upperBound
 * if (chpl_is_oob) {
 *   return;
 * }
 *
 */
void GpuKernel::generateEarlyReturn() {
  Symbol* localUpperBound = addKernelArgument(gpuLoop.upperBound());

  VarSymbol* isOOB = new VarSymbol("chpl_is_oob", dtBool);
  fn_->insertAtTail(new DefExpr(isOOB));

  CallExpr* comparison = new CallExpr(PRIM_GREATER,
                                      kernelIndices_[0],
                                      localUpperBound);
  fn_->insertAtTail(new CallExpr(PRIM_MOVE, isOOB, comparison));

  BlockStmt* thenBlock = new BlockStmt();
  thenBlock->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  fn_->insertAtTail(new CondStmt(new SymExpr(isOOB), thenBlock));
}

void GpuKernel::populateBody(CForLoop *loop, FnSymbol *outlinedFunction) {
  std::set<Symbol*> handledSymbols;
  for_alist(node, loop->body) {
    bool copyNode = true;
    std::vector<SymExpr*> symExprsInBody;
    collectSymExprs(node, symExprsInBody);

    std::vector<DefExpr*> defExprsInBody;
    collectDefExprs(node, defExprsInBody);

    CallExpr* call = toCallExpr(node);
    if(call && call->isPrimitive(PRIM_GPU_SET_BLOCKSIZE)) {
        blockSize_ = toSymExpr(call->get(1));
        call->remove();
        copyNode = false;
    } else if(call && call->isPrimitive(PRIM_ASSERT_ON_GPU)) {
      // The outlined kernel can only be executed on the GPU so there's no need
      // to copy it over. Note: not all device functions are kernels so this does
      // not ensure that this assertion won't work its way into functions
      // called from the kernel, but we remove it here anyway cause why not, it's
      // a slight optimization.
      copyNode = false;
    }
    else if (DefExpr* def = toDefExpr(node)) {
      copyNode = false; // we'll do it here to adjust our symbol map

      DefExpr* newDef = def->copy();
      this->copyMap_.put(def->sym, newDef->sym);

      outlinedFunction->insertAtTail(newDef);
    }
    else {
      // We also need to copy any defs that appear in blocks.
      // This pattern appears in Arkouda, so we do the following
      // as a workaround:
      for_vector(DefExpr, def, defExprsInBody) {
        DefExpr* newDef = def->copy();
        this->copyMap_.put(def->sym, newDef->sym);
        outlinedFunction->insertAtTail(newDef);
      }

      for_vector(SymExpr, symExpr, symExprsInBody) {
        Symbol* sym = symExpr->symbol();

        if (handledSymbols.count(sym) == 1) {
          continue;
        }
        handledSymbols.insert(sym);

        if (isDefinedInTheLoop(sym, loop)) {
          // looks like this symbol was declared within the loop body,
          // so do nothing. TODO: I am hoping that we don't need to
          // check the type of the variable here, and we'll know that it
          // is a valid variable to declare on the gpu via the loop body
          // analysis
        }
        else if (isDegenerateOuterRef(sym, loop)) {
          addLocalVariable(sym);
        }
        else if (sym->isImmediate()) {
          // nothing to do
        }
        else if (isTypeSymbol(sym)) {
          // nothing to do
        }
        else if (gpuLoop.isIndexVariable(sym)) {
          // These are handled already, nothing to do
        }
        else {
          if (CallExpr* parent = toCallExpr(symExpr->parentExpr)) {
            if (isFieldAccessPrimitive(parent)) {
              if (symExpr == parent->get(2)) {  // this is a field
                // do nothing
              }
              else if (symExpr == parent->get(1) ||
                (parent->numActuals() >= 3 && symExpr == parent->get(3)))
              {
                addKernelArgument(sym);
              }
              else {
                INT_FATAL("Malformed PRIM_GET_MEMBER_*");
              }
            }
            else if (parent->isPrimitive()) {
              addKernelArgument(sym);
            }
            else if (FnSymbol* calledFn = parent->resolvedFunction()) {
              if (!toFnSymbol(sym)) {
                addKernelArgument(sym);
              }

              if (!calledFn->hasFlag(FLAG_GPU_AND_CPU_CODEGEN)) {
                 markGPUSubCalls(calledFn);
              }
            }
            else {
              INT_FATAL("Unexpected call expression");
            }
          } else if (CondStmt* cond = toCondStmt(symExpr->parentExpr)) {
            // Parent is a conditional statement.
            if (symExpr == cond->condExpr) {
              addKernelArgument(sym);
            }
          } else {
            INT_FATAL("Unexpected symbol expression");
          }
        }
      }
    }

    if (copyNode) {
      outlinedFunction->insertAtTail(node->copy());
    }
  }

  update_symbols(outlinedFunction->body, &copyMap_);
}

void GpuKernel::normalizeOutlinedFunction() {
  normalize(fn_);

  // When compiling Arkouda normalization introduces untyped IR.
  // To avoid that becoming a problem we check for the presence of
  // this IR and fail gpuization in that case.
  std::vector<DefExpr*> defExprsInBody;
  collectDefExprs(fn_, defExprsInBody);
  for_vector (DefExpr, def, defExprsInBody) {
    if(def->sym->type == dtUnknown) {
      this->lateGpuizationFailure_ = true;
    }
  }

  // normalization above adds PRIM_END_OF_STATEMENTs. It is probably too
  // wide of a brush. We can:
  //  (a) generate the AST we are generating from scratch inside some
  //      block, normalize that block, weed out these primitives inside
  //      that block only, flatten and remove
  //  (b) generate the new AST in the normalized form and never call
  //      normalize
  //  (c) keep things as is until this becomes an issue
  std::vector<CallExpr*> callsInBody;
  collectCallExprs(fn_, callsInBody);
  for_vector (CallExpr, call, callsInBody) {
    if (call->isPrimitive(PRIM_END_OF_STATEMENT)) {
      call->remove();
    }
  }
}

void GpuKernel::finalize() {
  // just repeat the dead code elimination steps for the new function
  cleanupLoopBlocks(this->fn_);
  deadVariableElimination(this->fn_);
  cleanupLoopBlocks(this->fn_);
  deadExpressionElimination(this->fn_);
}

void GpuKernel::markGPUSubCalls(FnSymbol* fn) {
  if (!fn->hasFlag(FLAG_GPU_AND_CPU_CODEGEN)) {
    fn->addFlag(FLAG_GPU_AND_CPU_CODEGEN);
    fn->addFlag(FLAG_GPU_CODEGEN);
  }

  errorForOuterVarAccesses(fn);

  std::vector<CallExpr*> calls;
  collectCallExprs(fn, calls);
  for_vector(CallExpr, call, calls) {
    if (FnSymbol* fn = call->resolvedFunction()) {
      markGPUSubCalls(fn);
    }
  }
}

// ----------------------------------------------------------------------------
// GPU Transforms
// ----------------------------------------------------------------------------
/**
 * Given a CForLoop with lower bound lb and upper bound ub
 * (See extractUpperBound\extractIndicesAndLowerBound to
 * see what we pattern match and extract), generate the
 * following AST and insert it into gpuLaunchBlock:
 *
 *   chpl_block_delta = ub - lb
 *   chpl_gpu_num_threads = chpl_block_delta + 1
 */
static VarSymbol* generateNumThreads(BlockStmt* gpuLaunchBlock,
                                     const GpuizableLoop& gpuLoop) {

  VarSymbol *varBoundDelta = insertNewVarAndDef(gpuLaunchBlock,
                                                "chpl_block_delta",
                                                dtInt[INT_SIZE_DEFAULT]);
  VarSymbol *numThreads = insertNewVarAndDef(gpuLaunchBlock,
                                             "chpl_num_gpu_threads",
                                             dtInt[INT_SIZE_DEFAULT]);

  CallExpr *c1 = new CallExpr(PRIM_ASSIGN, varBoundDelta,
                              new CallExpr(PRIM_SUBTRACT,
                                           gpuLoop.upperBound(),
                                           gpuLoop.lowerBounds()[0]));
  gpuLaunchBlock->insertAtTail(c1);

  CallExpr *c2 = new CallExpr(PRIM_ASSIGN, numThreads,
                              new CallExpr(PRIM_ADD, varBoundDelta,
                                           new_IntSymbol(1)));
  gpuLaunchBlock->insertAtTail(c2);

  return numThreads;
}

static CallExpr* generateGPUCall(GpuKernel& info, VarSymbol* numThreads) {
  CallExpr *call = new CallExpr(PRIM_GPU_KERNEL_LAUNCH_FLAT);

  call->insertAtTail(info.fn());

  call->insertAtTail(numThreads);  // total number of GPU threads

  if (info.blockSize_) {
    // sets blockSize if specified with by "gpu set BlockSize" primitive
    call->insertAtTail(info.blockSize_->copy());
  } else {
    int blockSize = fGPUBlockSize != 0 ? fGPUBlockSize : 512;
    call->insertAtTail(new_IntSymbol(blockSize));
  }

  for_vector (Symbol, actual, info.kernelActuals()) {
    call->insertAtTail(new SymExpr(actual));
  }

  return call;
}

static void generateGpuAndNonGpuPaths(const GpuizableLoop &gpuLoop,
                                      GpuKernel &kernel) {
  // if (chpl_task_getRequestedSubloc() >= 0) {
  //   code to determine number of threads to launch kernel with
  //   call the generated GPU kernel
  // } else {
  //   run the existing loop on the CPU
  // }
  Expr* condExpr =
      new CallExpr(PRIM_GREATEROREQUAL,
                   new CallExpr(PRIM_GET_REQUESTED_SUBLOC),
                   new_IntSymbol(0));
  BlockStmt* thenBlock = new BlockStmt();
  BlockStmt* elseBlock = new BlockStmt();
  CondStmt* loopCloneCond = new CondStmt(condExpr, thenBlock, elseBlock);
  BlockStmt* gpuLaunchBlock = new BlockStmt();

  gpuLoop.loop()->insertBefore(loopCloneCond);
  thenBlock->insertAtHead(gpuLaunchBlock);
  elseBlock->insertAtHead(gpuLoop.loop()->remove());

  VarSymbol *numThreads = generateNumThreads(gpuLaunchBlock, gpuLoop);
  CallExpr* gpuCall = generateGPUCall(kernel, numThreads);
  gpuLaunchBlock->insertAtTail(gpuCall);
  gpuLaunchBlock->flattenAndRemove();
}

static void outlineEligibleLoop(FnSymbol *fn, const GpuizableLoop &gpuLoop) {
  SET_LINENO(gpuLoop.loop());

  // Construction of the GpuKernel will create the outlined function
  GpuKernel kernel(gpuLoop, fn->defPoint);
  if(!kernel.lateGpuizationFailure()) {
    generateGpuAndNonGpuPaths(gpuLoop, kernel);
  } else {
    kernel.fn()->defPoint->remove();
  }
}

static void outlineGPUKernels() {
  forv_Vec(FnSymbol*, fn, gFnSymbols) {
    std::vector<BaseAST*> asts;
    collect_asts(fn, asts);

    for_vector(BaseAST, ast, asts) {
      if (CForLoop* loop = toCForLoop(ast)) {
        GpuizableLoop gpuLoop(loop);
        if (gpuLoop.isEligible()) {
          outlineEligibleLoop(fn, gpuLoop);
        }
      }
    }
  }
}

static void logGpuizableLoops() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (ForLoop* forLoop = toForLoop(block)) {
      if (forLoop->isOrderIndependent())
        if (GpuizableLoop(forLoop).isEligible())
          if (debugPrintGPUChecks)
            printf("Found viable forLoop %s:%d[%d]\n",
                   forLoop->fname(), forLoop->linenum(), forLoop->id);
    } else if (CForLoop* forLoop = toCForLoop(block)) {
      if (GpuizableLoop(forLoop).isEligible())
        if (debugPrintGPUChecks)
          printf("Found viable CForLoop %s:%d[%d]\n",
                 forLoop->fname(), forLoop->linenum(), forLoop->id);
    }
  }
}

void gpuTransforms() {
  if (debugPrintGPUChecks) {
    logGpuizableLoops();
  }

  // For now, we are doing GPU outlining here. In the future, it should
  // probably be its own pass.
  if (usingGpuLocaleModel()) {
    if (fReportGpuTransformTime) gpuTransformTimer.start();

    outlineGPUKernels();

    if (fReportGpuTransformTime) {
      gpuTransformTimer.stop();
      std::cout << "GPU transformation time (s): " <<
                   gpuTransformTimer.elapsedSecs() << std::endl;
    }
  }
}
