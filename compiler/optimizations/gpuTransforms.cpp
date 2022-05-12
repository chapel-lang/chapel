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

#include "global-ast-vecs.h"

bool debugPrintGPUChecks = false;
bool allowFnCallsFromGPU = true;
int indentGPUChecksLevel = 0;

extern int classifyPrimitive(CallExpr *call, bool inLocal);
extern bool inLocalBlock(CallExpr *call);

struct OutlineInfo {
  // Given a CForLoop that we want to outline into a function for a GPU kernel
  // extract the loopIndices, lowerBounds, and upperBound and populate it with
  // boilerplate computation to calculate what index the kernel should operate
  // on given its thread ID. After buildStubOutlinedFunction is finished its up
  // to the caller to populate its body with the body of the loop and to call
  // the outlined function. It's possible that we fail to build the function if
  // the loop boundaries don't represent a range if this is the case we return
  // false and the caller should stop trying to outline.
  bool buildStubOutlinedFunction(CForLoop* loop);

  CForLoop* loop = NULL;

  std::vector<Symbol*> loopIndices;
  std::vector<Symbol*> lowerBounds;
  Symbol* upperBound = NULL;

  FnSymbol* fn;
  std::vector<Symbol*> kernelIndices;
  std::vector<Symbol*> kernelActuals;

  SymbolMap copyMap;

  private:
  bool extractIndicesAndLowerBounds(CForLoop* loop);
  bool extractUpperBound(CForLoop* loop);
  void generateIndexComputation();
};

static VarSymbol* generateAssignmentToPrimitive(FnSymbol* fn,
                                                const char *varName,
                                                PrimitiveTag prim,
                                                Type *primReturnType);

static void generateEarlyReturn(OutlineInfo& info);

static bool shouldOutlineLoop(BlockStmt* blk, bool allowFnCalls);

static bool shouldOutlineLoopHelp(BlockStmt* blk,
                                  std::set<FnSymbol*>& okFns,
                                  std::set<FnSymbol*> visitedFns,
                                  bool allowFnCalls);

static SymExpr* hasOuterVarAccesses(FnSymbol* fn);
static void markGPUSuitableLoops();

static Symbol* addKernelArgument(OutlineInfo& info, Symbol* symInLoop);

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
// expressions. That symbol is a ref that's defined outside the for loop, but it
// is def'd and use'd only inside the block. Moreover, one of its defs is
// actually redundant and should be removed. However at this stage in the
// compilation it is not. The bottom line is, that ref could actually just be a
// local variable in the loop body. So, we handle that specially to avoid
// passing that as an argument to the GPU kernel.
// TODO: investigate whether that def is removed later in the compilation.
// Ideally move GPU transforms after that pass
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

static bool isIndexVariable(OutlineInfo& info, Symbol* sym) {
  std::vector<Symbol*>& indices = info.loopIndices;

  return std::find(indices.begin(), indices.end(), sym) != indices.end();
}

bool OutlineInfo::extractUpperBound(CForLoop* loop) {
  if(BlockStmt* bs = toBlockStmt(loop->testBlockGet())) {
    for_exprs_postorder (expr, bs) {
      if(CallExpr *call = toCallExpr(expr)) {
        if(call->isPrimitive(PRIM_LESSOREQUAL)) {
          if(SymExpr *symExpr = toSymExpr(call->get(2))) {

            SymExpr* lhsSymExpr = toSymExpr(call->get(1));
            INT_ASSERT(lhsSymExpr && lhsSymExpr->symbol() == this->loopIndices[0]);

            this->upperBound = symExpr->symbol();

            break;
          }
        }
      }
    }
  }

  return this->upperBound != NULL;
}

bool OutlineInfo::extractIndicesAndLowerBounds(CForLoop* loop) {
  if(BlockStmt* bs = toBlockStmt(loop->initBlockGet())) {
    for_alist (expr, bs->body) {
      if(CallExpr *call = toCallExpr(expr)) {
        if(call->isPrimitive(PRIM_ASSIGN) ||
           call->isPrimitive(PRIM_MOVE)) {

          SymExpr *idxSymExpr = toSymExpr(call->get(1));
          SymExpr *boundSymExpr = toSymExpr(call->get(2));

          INT_ASSERT(idxSymExpr);
          INT_ASSERT(boundSymExpr);

          this->loopIndices.push_back(idxSymExpr->symbol());
          this->lowerBounds.push_back(boundSymExpr->symbol());
        }
      }
    }

    INT_ASSERT(bs->body.length == (int)this->loopIndices.size());
    INT_ASSERT(bs->body.length == (int)this->lowerBounds.size());
  } else {
    return false;
  }

  return true;
}

static VarSymbol* insertNewVarAndDef(BlockStmt* insertionPoint, const char* name,
                                     Type* type) {
  VarSymbol *var = new VarSymbol(name, type);
  var->defPoint = new DefExpr(var);
  insertionPoint->insertAtTail(var->defPoint);
  return var;
}

bool OutlineInfo::buildStubOutlinedFunction(CForLoop* loop) {
  this->loop = loop;

  // Pattern match loop boundaries to determine lower
  // and upper bounds. If we fail to match exit early.
  if(!extractIndicesAndLowerBounds(loop) || 
     !extractUpperBound(loop))
  {
    return false;
  }

  fn = new FnSymbol("chpl_gpu_kernel");

  fn->body->blockInfoSet(new CallExpr(PRIM_BLOCK_LOCAL));

  fn->addFlag(FLAG_RESOLVED);
  fn->addFlag(FLAG_ALWAYS_RESOLVE);
  fn->addFlag(FLAG_GPU_CODEGEN);

  generateIndexComputation();
  generateEarlyReturn(*this);

  return true;
}

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
                                     OutlineInfo& info) {

  VarSymbol *varBoundDelta = insertNewVarAndDef(gpuLaunchBlock,
                                                "chpl_block_delta",
                                                dtInt[INT_SIZE_DEFAULT]);
  VarSymbol *numThreads = insertNewVarAndDef(gpuLaunchBlock,
                                             "chpl_num_gpu_threads",
                                             dtInt[INT_SIZE_DEFAULT]);

  CallExpr *c1 = new CallExpr(PRIM_ASSIGN, varBoundDelta,
                              new CallExpr(PRIM_SUBTRACT,
                                           info.upperBound,
                                           info.lowerBounds[0]));
  gpuLaunchBlock->insertAtTail(c1);

  CallExpr *c2 = new CallExpr(PRIM_ASSIGN, numThreads,
                              new CallExpr(PRIM_ADD, varBoundDelta,
                                           new_IntSymbol(1)));
  gpuLaunchBlock->insertAtTail(c2);

  return numThreads;
}

static VarSymbol* generateAssignmentToPrimitive(
  FnSymbol* fn, const char *varName, PrimitiveTag prim, Type *primReturnType) {

  VarSymbol *var = insertNewVarAndDef(fn->body, varName, primReturnType);
  CallExpr *c1 = new CallExpr(PRIM_MOVE, var, new CallExpr(prim));
  fn->insertAtTail(c1);
  return var;
}

static Symbol* addKernelArgument(OutlineInfo& info, Symbol* symInLoop) {
  Type* symType = symInLoop->typeInfo();
  ArgSymbol* newFormal = new ArgSymbol(INTENT_IN, symInLoop->name, symType);
  info.fn->insertFormalAtTail(newFormal);

  info.kernelActuals.push_back(symInLoop);
  info.copyMap.put(symInLoop, newFormal);

  return newFormal;
}

static Symbol* addLocalVariable(OutlineInfo& info, Symbol* symInLoop) {
  VarSymbol* newSym = toVarSymbol(symInLoop->copy());

  INT_ASSERT(newSym);

  info.fn->insertAtHead(new DefExpr(newSym));
  info.copyMap.put(symInLoop, newSym);

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
 *  Also adds the loopIndex->index to the copyMap
 **/
void OutlineInfo::generateIndexComputation() {
  std::vector<Symbol*>::size_type numIndices = loopIndices.size();
  INT_ASSERT(lowerBounds.size() == numIndices);

  for (std::vector<Symbol*>::size_type i=0 ; i<numIndices ; i++) {
    Symbol* loopIndex  = loopIndices[i];
    Symbol* lowerBound = lowerBounds[i];

    VarSymbol *varBlockIdxX = generateAssignmentToPrimitive(fn, "blockIdxX",
      PRIM_GPU_BLOCKIDX_X, dtInt[INT_SIZE_32]);
    VarSymbol *varBlockDimX = generateAssignmentToPrimitive(fn, "blockDimX",
      PRIM_GPU_BLOCKDIM_X, dtInt[INT_SIZE_32]);
    VarSymbol *varThreadIdxX = generateAssignmentToPrimitive(fn, "threadIdxX",
      PRIM_GPU_THREADIDX_X, dtInt[INT_SIZE_32]);

    VarSymbol *tempVar = insertNewVarAndDef(fn->body, "t0", dtInt[INT_SIZE_32]);
    CallExpr *c1 = new CallExpr(PRIM_MOVE, tempVar, new CallExpr(PRIM_MULT,
                                                                 varBlockIdxX,
                                                                 varBlockDimX));
    fn->insertAtTail(c1);

    VarSymbol *tempVar1 = insertNewVarAndDef(fn->body, "t1", dtInt[INT_SIZE_32]);
    CallExpr *c2 = new CallExpr(PRIM_MOVE, tempVar1, new CallExpr(PRIM_ADD,
                                                                  tempVar,
                                                                  varThreadIdxX));
    fn->insertAtTail(c2);

    Symbol* startOffset = addKernelArgument(*this, lowerBound);
    VarSymbol* index = insertNewVarAndDef(fn->body, "chpl_simt_index",
                                          dtInt[INT_SIZE_32]);
    fn->insertAtTail(new CallExpr(PRIM_MOVE, index, new CallExpr(PRIM_ADD,
                                                                 tempVar1,
                                                                 startOffset)));

    kernelIndices.push_back(index);
    copyMap.put(loopIndex, index);
  }

  INT_ASSERT(kernelIndices.size() == loopIndices.size());
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
static void generateEarlyReturn(OutlineInfo& info) {
  Symbol* localUpperBound = addKernelArgument(info, info.upperBound);

  VarSymbol* isOOB = new VarSymbol("chpl_is_oob", dtBool);
  info.fn->insertAtTail(new DefExpr(isOOB));

  CallExpr* comparison = new CallExpr(PRIM_GREATER,
                                      info.kernelIndices[0],
                                      localUpperBound);
  info.fn->insertAtTail(new CallExpr(PRIM_MOVE, isOOB, comparison));

  BlockStmt* thenBlock = new BlockStmt();
  thenBlock->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  info.fn->insertAtTail(new CondStmt(new SymExpr(isOOB), thenBlock));
}

static  CallExpr* generateGPUCall(OutlineInfo& info, VarSymbol* numThreads) {
  CallExpr* call = new CallExpr(PRIM_GPU_KERNEL_LAUNCH_FLAT);

  call->insertAtTail(info.fn);

  call->insertAtTail(numThreads);  // total number of GPU threads

  int blockSize = fGPUBlockSize != 0 ? fGPUBlockSize:512;
  call->insertAtTail(new_IntSymbol(blockSize));


  for_vector (Symbol, actual, info.kernelActuals) {
    call->insertAtTail(new SymExpr(actual));
  }

  return call;
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

static void markGPUSubCalls(FnSymbol* fn) {
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

static void outlineGPUKernels() {
  forv_Vec(FnSymbol*, fn, gFnSymbols) {
    std::vector<BaseAST*> asts;
    collect_asts(fn, asts);

    for_vector(BaseAST, ast, asts) {
      if (CForLoop* loop = toCForLoop(ast)) {
        if (shouldOutlineLoop(loop, allowFnCallsFromGPU)) {
          SET_LINENO(loop);

          // We may fail to build stub function if loop boundaries don't
          // pattern match the way we expect, if that happens give up on
          // outlining and continue to the next loop.
          OutlineInfo info;
          if(!info.buildStubOutlinedFunction(loop)) { continue; }

          FnSymbol* outlinedFunction = info.fn;
          fn->defPoint->insertBefore(new DefExpr(outlinedFunction));

          // if (chpl_task_getRequestedSubloc() >= 0) {
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

          loop->insertBefore(loopCloneCond);
          thenBlock->insertAtHead(gpuLaunchBlock);
          elseBlock->insertAtHead(loop->remove());

          std::set<Symbol*> handledSymbols;
          for_alist(node, loop->body) {
            bool copyNode = true;
            std::vector<SymExpr*> symExprsInBody;
            collectSymExprs(node, symExprsInBody);

            if (DefExpr* def = toDefExpr(node)) {
              copyNode = false; // we'll do it here to adjust our symbol map

              DefExpr* newDef = def->copy();
              info.copyMap.put(def->sym, newDef->sym);

              outlinedFunction->insertAtTail(newDef);
            }
            else {
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
                  addLocalVariable(info, sym);
                }
                else if (sym->isImmediate()) {
                  // nothing to do
                }
                else if (isTypeSymbol(sym)) {
                  // nothing to do
                }
                else if (isIndexVariable(info, sym)) {
                  // These are handled already, nothing to do
                }
                else {
                  if (CallExpr* parent = toCallExpr(symExpr->parentExpr)) {
                    if (parent->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                        parent->isPrimitive(PRIM_GET_MEMBER) ||
                        parent->isPrimitive(PRIM_SET_MEMBER) ||
                        parent->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
                        parent->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                        parent->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
                      if (symExpr == parent->get(2)) {  // this is a field
                        // do nothing
                      }
                      else if (symExpr == parent->get(1)) {
                        addKernelArgument(info, sym);
                      }
                      else {
                        INT_FATAL("Malformed PRIM_GET_MEMBER_*");
                      }
                    }
                    else if (parent->isPrimitive()) {
                      addKernelArgument(info, sym);
                    }
                    else if (FnSymbol* calledFn = parent->resolvedFunction()) {
                      if (!toFnSymbol(sym)) {
                        addKernelArgument(info, sym);
                      }

                      if (!calledFn->hasFlag(FLAG_GPU_AND_CPU_CODEGEN)) {
                         markGPUSubCalls(calledFn);
                      }
                    }
                    else {
                      INT_FATAL("Unexpected call expression");
                    }
                  }
                }
              }
            }

            if (copyNode) {
              outlinedFunction->insertAtTail(node->copy());
            }
          }

          update_symbols(outlinedFunction->body, &info.copyMap);
          normalize(outlinedFunction);

          // normalization above adds PRIM_END_OF_STATEMENTs. It is probably too
          // wide of a brush. We can:
          //  (a) generate the AST we are generating from scratch inside some
          //      block, normalize that block, weed out these primitives inside
          //      that block only, flatten and remove
          //  (b) generate the new AST in the normalized form and never call
          //      normalize
          //  (c) keep things as is until this becomes an issue
          std::vector<CallExpr*> callsInBody;
          collectCallExprs(outlinedFunction, callsInBody);
          for_vector (CallExpr, call, callsInBody) {
            if (call->isPrimitive(PRIM_END_OF_STATEMENT)) {
              call->remove();
            }
          }

          VarSymbol *numThreads = generateNumThreads(gpuLaunchBlock, info);
          CallExpr* gpuCall = generateGPUCall(info, numThreads);
          gpuLaunchBlock->insertAtTail(gpuCall);
          gpuLaunchBlock->flattenAndRemove();

          // just repeat the dead code elimination steps for the new function
          cleanupLoopBlocks(outlinedFunction);
          deadVariableElimination(outlinedFunction);
          cleanupLoopBlocks(outlinedFunction);
          deadExpressionElimination(outlinedFunction);
        }
      }
    }
  }
}

static bool shouldOutlineLoop(BlockStmt* blk, bool allowFnCalls) {
  if (!blk->inTree())
    return false;

  if (CForLoop* cfl = toCForLoop(blk))
    if (!cfl->isOrderIndependent())
      return false;

  std::set<FnSymbol*> okFns;
  std::set<FnSymbol*> visitedFns;

  return shouldOutlineLoopHelp(blk, okFns, visitedFns, allowFnCalls);
}

static bool shouldOutlineLoopHelp(BlockStmt* blk,
                                  std::set<FnSymbol*>& okFns,
                                  std::set<FnSymbol*> visitedFns,
                                  bool allowFnCalls) {

  if (debugPrintGPUChecks) {
    FnSymbol* fn = blk->getFunction();
    printf("%*s%s:%d: %s[%d]\n", indentGPUChecksLevel, "",
           fn->fname(), fn->linenum(), fn->name, fn->id);
  }

  if (visitedFns.count(blk->getFunction()) != 0) {
    if (blk->getFunction()->hasFlag(FLAG_FUNCTION_TERMINATES_PROGRAM)) {
      return true; // allow `halt` to be potentially called recursively
    } else {
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
        return false;
      }
    } else if (call->isResolved()) {
      if (!allowFnCalls)
        return false;

      FnSymbol* fn = call->resolvedFunction();

      if (hasOuterVarAccesses(fn))
        return false;

      indentGPUChecksLevel += 2;
      if (okFns.count(fn) != 0 ||
          shouldOutlineLoopHelp(fn->body, okFns,
                                visitedFns, allowFnCalls)) {
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

static void markGPUSuitableLoops() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (ForLoop* forLoop = toForLoop(block)) {
      if (forLoop->isOrderIndependent()) {
        if (shouldOutlineLoop(forLoop, allowFnCallsFromGPU)) {
          if (debugPrintGPUChecks)
            printf("Found viable forLoop %s:%d[%d]\n",
                   forLoop->fname(), forLoop->linenum(), forLoop->id);
        }
      }
    } else if (CForLoop* forLoop = toCForLoop(block)) {
      if (shouldOutlineLoop(forLoop, allowFnCallsFromGPU)) {
        if (debugPrintGPUChecks)
          printf("Found viable CForLoop %s:%d[%d]\n",
                 forLoop->fname(), forLoop->linenum(), forLoop->id);
      }
    }
  }
}

void gpuTransforms() {
  if (debugPrintGPUChecks) {
    markGPUSuitableLoops();
  }

  // For now, we are doing GPU outlining here. In the future, it should probably
  // be its own pass.
  if (localeUsesGPU()) {
    outlineGPUKernels();
  }
}
