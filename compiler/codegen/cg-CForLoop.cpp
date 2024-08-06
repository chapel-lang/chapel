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

#include "CForLoop.h"

#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "codegen.h"
#include "driver.h"
#include "llvmTracker.h"
#include "llvmVer.h"
#include "ForLoop.h"
#include "LayeredValueTable.h"

#ifdef HAVE_LLVM
#include "llvm/IR/Module.h"
#endif

#include <algorithm>

#ifdef HAVE_LLVM


// constructs !0 = !name
static llvm::Metadata* constructLLVMMetadata(llvm::StringRef name) {
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();
  return llvm::MDString::get(ctx, name);
}

// constructs !0 = !{!name, !value}
static llvm::Metadata* constructLLVMMetadata(llvm::StringRef name, llvm::Metadata* value) {
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  llvm::Metadata *metaArray[] = {
    llvm::MDString::get(ctx, name),
    value,
  };
  return llvm::MDNode::get(ctx, metaArray);
}
// constructs !0 = !{!name, !1}
//            !1 = value
static llvm::Metadata* constructLLVMMetadata(llvm::StringRef name, llvm::ArrayRef<llvm::Metadata*> array) {
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  auto value = llvm::MDNode::get(ctx, array);
  llvm::Metadata *metaArray[] = {
    llvm::MDString::get(ctx, name),
    value,
  };
  return llvm::MDNode::get(ctx, metaArray);
}

// constructs !0 = !{!name, i1 value}
static llvm::Metadata* constructLLVMMetadata(llvm::StringRef name, bool value) {
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  auto constant = llvm::ConstantAsMetadata::get(
    llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), value)
  );
  return constructLLVMMetadata(name, constant);
}
// constructs !0 = !{!name, i64 value}
static llvm::Metadata* constructLLVMMetadata(llvm::StringRef name, int64_t value) {
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  auto constant = llvm::ConstantAsMetadata::get(
    llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), value)
  );
  return constructLLVMMetadata(name, constant);
}
// constructs !0 = !{!name, !1}
//            !1 = !{value}
static llvm::Metadata* constructLLVMMetadata(llvm::StringRef name, llvm::StringRef value) {
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  llvm::Metadata* strArr[] = {llvm::MDString::get(ctx, value)};
  auto arrRef = llvm::ArrayRef<llvm::Metadata*>(strArr, 1);
  return constructLLVMMetadata(name, arrRef);
}

static bool loopHasAnyParallelAccess(bool thisLoopParallelAccess,
                                     llvm::MDNode*& accessGroup) {
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();
    // Does the current loop, or any outer loop in the loop stack,
  // require llvm.loop.parallel_accesses metadata?
  bool anyParallelAccesses = false;
  if (thisLoopParallelAccess) {
    anyParallelAccesses = true;
    accessGroup = llvm::MDNode::getDistinct(ctx, {});
  } else {
    accessGroup = NULL;
    for (auto & loopData : info->loopStack) {
      if (loopData.markMemoryOps)
        anyParallelAccesses = true;
    }
  }
  return anyParallelAccesses;
}

static void addLoopParallelAccess(std::vector<llvm::Metadata*>& args,
                                  bool thisLoopParallelAccess,
                                  llvm::MDNode*& accessGroup) {
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  std::vector<llvm::Metadata*> v;
  v.push_back(constructLLVMMetadata("llvm.loop.parallel_accesses"));

  // Generate {"llvm.loop.parallel_accesses", group1, group2, ...}
  // where the groups are any parallel loops we are currently in
  // (including outer loops to this loop).
  if (thisLoopParallelAccess) {
    v.push_back(accessGroup);
  }
  for (auto & loopData : info->loopStack) {
    if (loopData.markMemoryOps) {
      v.push_back(loopData.accessGroup);
    }
  }

  llvm::MDNode* parAccesses = llvm::MDNode::get(ctx, v);
  args.push_back(parAccesses);
}

GenRet LLVMMetadata::codegen() {

  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  llvm::Metadata* metadata = nullptr;
  if (kind == LAT_NO_VALUE) {
    // do some extra work here so the end result is
    // !0 = distinct {!0,  !1}
    // !1 = !str;
    auto md = constructLLVMMetadata(key);
    metadata = llvm::MDNode::get(ctx, md);
  } else if (kind == LAT_INT) {
    metadata = constructLLVMMetadata(key, int_val);
  } else if (kind == LAT_BOOL) {
    metadata = constructLLVMMetadata(key, bool_val);
  } else if (kind == LAT_STRING) {
    // char* can normally be passed to StringRef using implicit conversion
    // in this case, C++ "decays" the pointer to a bool value and calls the bool version
    // even changing the argument to char* would still causes this to occur
    metadata = constructLLVMMetadata(key, llvm::StringRef(string_val));
  } else if (kind == LAT_ATTRIBUTE) {
    auto ret = attribute_val->codegen();
    if (auto* MD = llvm::dyn_cast<llvm::MetadataAsValue>(ret.val)) {
      metadata = constructLLVMMetadata(key, MD->getMetadata());
    }
  }

  GenRet ret;
  if (metadata != nullptr) {
    ret.val = llvm::MetadataAsValue::get(ctx, metadata);
  }
  return ret;
}



static void addLoopUserMetadata(LoopStmt* loop, std::vector<llvm::Metadata*>& args, LLVMMetadataList attrs) {
  for (const auto& attr: attrs) {
    auto ret = attr->codegen();
    if (auto* MD = llvm::dyn_cast<llvm::MetadataAsValue>(ret.val))
      args.push_back(MD->getMetadata());
    else
      USR_WARN(loop, "failed to add llvm metadata");
  }
}

// Returns the loop metadata node to associate with the branch.
// If thisLoopParallelAccess is set, accessGroup will be set to the
// metadata node to use in llvm.access.group metadata for this loop.
static llvm::MDNode* generateLoopMetadata(LoopStmt* loop,
                                          bool thisLoopParallelAccess,
                                          llvm::MDNode*& accessGroup)
{
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  std::vector<llvm::Metadata*> args;
  // Resolve operand 0 for the loop id self reference
  auto tmpNode        = llvm::MDNode::getTemporary(ctx, chpl::empty);
  args.push_back(tmpNode.get());

  if(fNoVectorize == false && loop->isVectorizable()) {

    // llvm.loop.vectorize.enable metadata is only used by LoopVectorizer to:
    // 1) Explicitly disable vectorization of particular loop
    // 2) Print warning when vectorization is enabled (using metadata) and
    //    vectorization didn't occur
    // Here we do not emit that metadata; instead emitting parallel
    // llvm.loop.parallel_accesses.

    bool anyParallelAccesses = loopHasAnyParallelAccess(thisLoopParallelAccess, accessGroup);

    if (anyParallelAccesses) {
      addLoopParallelAccess(args, thisLoopParallelAccess, accessGroup);
    }

    // When using the Region Vectorizer, emit rv.loop.vectorize.enable metadata
    if(fRegionVectorizer) {
      args.push_back(constructLLVMMetadata("rv.loop.vectorize.enable", true));

      // Note that the Region Vectorizer once required
      // llvm.loop.vectorize.width but no longer does.
    }

  }

  addLoopUserMetadata(loop, args, loop->getAdditionalLLVMMetadata());

  // only construct metadata if there is metadata to be had
  if(args.size() > 1) {
    llvm::MDNode *loopMetadata = llvm::MDNode::get(ctx, args);
    loopMetadata->replaceOperandWith(0, loopMetadata);
    return loopMetadata;
  }
  return nullptr;
}
// loopMetadata is the metadata to associate with the branch.
// It will be extended to add llvm.loop.parallel_accesses
//   for the current loop (represented by parallel_accesses)
//   as well as any enclosing loops (from loopStack).
static void addLoopMetadata(llvm::Instruction* instruction,
                            llvm::MDNode* loopMetadata,
                            llvm::MDNode* accessGroup)
{
  instruction->setMetadata("llvm.loop", loopMetadata);
}

#endif

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

GenRet CForLoop::codegen()
{
  if (id == breakOnCodegenID)
    gdbShouldBreakHere();

  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  reportVectorizable();

  if (outfile)
  {
    BlockStmt*  initBlock = initBlockGet();

    // These copy calls are needed or else values get code generated twice.
    std::string init      = codegenCForLoopHeader(initBlock->copy());

    BlockStmt*  testBlock = testBlockGet();
    std::string test      = codegenCForLoopHeader(testBlock->copy());

    // wrap the test with paren. Could probably check if it already has
    // outer paren to make the code a little cleaner.
    if (test != "")
      test = "(" + test + ")";

    BlockStmt*  incrBlock = incrBlockGet();
    std::string incr      = codegenCForLoopHeader(incrBlock->copy());
    std::string hdr       = "for (" + init + "; " + test + "; " + incr + ") ";

    info->cStatements.push_back(hdr);

    if (this != getFunction()->body)
      info->cStatements.push_back("{\n");

    body.codegen("");

    if (this != getFunction()->body)
    {
      std::string end  = "}";
      CondStmt*   cond = toCondStmt(parentExpr);

      if (!cond || !(cond->thenStmt == this && cond->elseStmt))
        end += "\n";

      info->cStatements.push_back(end);
    }
  }

  else
  {
#ifdef HAVE_LLVM
    llvm::Function*   func          = info->irBuilder->GetInsertBlock()->getParent();

    llvm::BasicBlock* blockStmtInit = NULL;
    llvm::BasicBlock* blockStmtBody = NULL;
    llvm::BasicBlock* blockStmtEnd  = NULL;

    BlockStmt*        initBlock     = initBlockGet();
    BlockStmt*        testBlock     = testBlockGet();
    BlockStmt*        incrBlock     = incrBlockGet();

    assert(initBlock && testBlock && incrBlock);

    getFunction()->codegenUniqueNum++;

    blockStmtBody = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_body"));
    blockStmtEnd  = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_end"));
    trackLLVMValue(blockStmtBody);
    trackLLVMValue(blockStmtEnd);

    // In order to track more easily with the C backend and because mem2reg should optimize
    // all of these cases, we generate a for loop as the same as
    // if(cond) do { body; step; } while(cond).

    // However it is appealing to generate these low-level loops directly
    // in LLVM IR:
    //   * could avoid repeated loads
    //   * could simplify generated IR
    //   * could avoid problems identifying induction variables

    // Create the init basic block
    blockStmtInit = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_c_for_init"));
    trackLLVMValue(blockStmtInit);

#if HAVE_LLVM_VER >= 160
    func->insert(func->end(), blockStmtInit);
#else
    func->getBasicBlockList().push_back(blockStmtInit);
#endif

    // Insert an explicit branch from the current block to the init block
    llvm::BranchInst* toInit = info->irBuilder->CreateBr(blockStmtInit);
    trackLLVMValue(toInit);

    // Now switch to the init block for code generation
    info->irBuilder->SetInsertPoint(blockStmtInit);

    // Code generate the init block.
    initBlock->body.codegen("");

    // Add the loop condition to figure out if we run the loop at all.
    GenRet       test0      = codegenCForLoopCondition(testBlock);
    llvm::Value* condValue0 = test0.val;

    // Normalize it to boolean
    if (condValue0->getType() != llvm::Type::getInt1Ty(info->module->getContext())) {
      condValue0 = info->irBuilder->CreateICmpNE(condValue0,
                                               llvm::ConstantInt::get(condValue0->getType(), 0),
                                               FNAME("condition"));
      trackLLVMValue(condValue0);
    }

    // Create the conditional branch
    llvm::BranchInst* condBr = info->irBuilder->CreateCondBr(condValue0, blockStmtBody, blockStmtEnd);
    trackLLVMValue(condBr);

    // Now add the body.
#if HAVE_LLVM_VER >= 160
    func->insert(func->end(), blockStmtBody);
#else
    func->getBasicBlockList().push_back(blockStmtBody);
#endif

    info->irBuilder->SetInsertPoint(blockStmtBody);
    info->lvt->addLayer();

    llvm::MDNode* accessGroup = nullptr;
    llvm::MDNode* loopMetadata = nullptr;

    loopMetadata = generateLoopMetadata(this,
                                        isParallelAccessVectorizable(),
                                        accessGroup);
    if(loopMetadata) {
      LoopData data(accessGroup, isParallelAccessVectorizable());
      info->loopStack.push_back(data);
    }

    body.codegen("");

    if(loopMetadata)
      info->loopStack.pop_back();

    info->lvt->removeLayer();

    incrBlock->body.codegen("");

    GenRet       test1      = codegenCForLoopCondition(testBlock);
    llvm::Value* condValue1 = test1.val;

    // Normalize it to boolean
    if (condValue1->getType() != llvm::Type::getInt1Ty(info->module->getContext())) {
      condValue1 = info->irBuilder->CreateICmpNE(condValue1,
                                                 llvm::ConstantInt::get(condValue1->getType(), 0),
                                                 FNAME("condition"));
      trackLLVMValue(condValue1);
    }

    // Create the conditional branch
    llvm::Instruction* endLoopBranch = info->irBuilder->CreateCondBr(condValue1, blockStmtBody, blockStmtEnd);
    trackLLVMValue(endLoopBranch);

    if(loopMetadata)
      addLoopMetadata(endLoopBranch, loopMetadata, accessGroup);

#if HAVE_LLVM_VER >= 160
    func->insert(func->end(), blockStmtEnd);
#else
    func->getBasicBlockList().push_back(blockStmtEnd);
#endif

    info->irBuilder->SetInsertPoint(blockStmtEnd);

    if (blockStmtBody) INT_ASSERT(blockStmtBody->getParent() == func);
    if (blockStmtEnd ) INT_ASSERT(blockStmtEnd->getParent()  == func);
#endif
  }

  return ret;
}

// This function is used to codegen the init, test, and incr segments of c for
// loops. In c for loops instead of using statements comma operators must be
// used. So for the init instead of generating something like:
//   i = 4;
//   j = 4;
//
// We need to generate:
// i = 4, j = 4
std::string CForLoop::codegenCForLoopHeader(BlockStmt* block)
{
  GenInfo*    info = gGenInfo;
  std::string seg  = "";

  for_alist(expr, block->body)
  {
    CallExpr* call = toCallExpr(expr);

    // Generate defExpr normally (they always get codegenned at the top of a
    // function currently, if that changes this code will probably be wrong.)
    if (DefExpr* defExpr = toDefExpr(expr))
    {
      defExpr->codegen();
    }

    // If inlining is off, the init, test, and incr are just functions and we
    // need to generate them inline so we use codegenValue. The semicolon is
    // added so it can be replaced with the comma later. If inlining is on
    // the test will be a <= and it also needs to be codegenned with
    // codegenValue.
    //
    // TODO when the test operator is user specifiable and not just <= this
    // will need to be updated to include all possible conditionals. (I'm
    // imagining we'll want a separate function that can check if a primitive
    // is a conditional as I think we'll need that info elsewhere.)
    else if (call && (call->isResolved() || isRelationalOperator(call) ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE)))
    {
      std::string callStr = codegenValue(call).c;

      if (callStr != "")
      {
        seg += callStr + ';';
      }
    }

    // Similar to above, generate symExprs
    else if (SymExpr* symExpr = toSymExpr(expr))
    {
      std::string symStr = codegenValue(symExpr).c;

      if (symStr != "")
      {
        seg += symStr + ';';
      }
    }

    // Everything else is just a bunch of statements. We do normal codegen() on
    // them which ends up putting whatever got codegenned into CStatements. We
    // pop all of those back off (note that the order we pop and attach to our
    // segment is important.)
    else
    {
      int prevStatements = (int) info->cStatements.size();

      expr->codegen();

      int newStatements  = (int) info->cStatements.size() - prevStatements;

      for (std::vector<std::string>::iterator it = info->cStatements.end() - newStatements;
           it != info->cStatements.end();
           ++it)
      {
        seg += *it;
      }

      info->cStatements.erase(info->cStatements.end() - newStatements,
                              info->cStatements.end());
    }
  }

  // replace all the semicolons (from "statements") with commas
  std::replace(seg.begin(), seg.end(), ';', ',');

  // remove all the newlines
  seg.erase(std::remove(seg.begin(), seg.end(), '\n'), seg.end());

  // remove the last character if any were generated (it's a trailing comma
  // since we previously had an appropriate "trailing" semicolon)
  if (seg.size () > 0)
    seg.resize (seg.size () - 1);

  return seg;
}

GenRet CForLoop::codegenCForLoopCondition(BlockStmt* block)
{
  GenRet ret;

#ifdef HAVE_LLVM
  for_alist(expr, block->body)
  {
    ret = expr->codegen();
  }

  return codegenValue(ret);

#else

  return ret;

#endif
}
