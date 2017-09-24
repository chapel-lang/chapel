/*
 * Copyright 2004-2017 Cray Inc.
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


// Merges sequences of loads or sequences of stores
// on address space(globalSpace) into memcpy operations so
// that we can do fewer puts or gets. For example
// %i1 = getelementptr ... %p, ..., 1
// %i2 = getelementptr ... %p, ..., 2
// %v1 = load %i1
// %v2 = load %i2
//
// will be replaced by
// %tmp = alloca
// memcpy(%tmp, %p, ...)
// %i1 = getelementptr ... %tmp, ..., 1
// %i2 = getelementptr ... %tmp ..., 2
// %v1 = load %i1
// %v2 = load %i2
//
// This optimization doesn't worry about combining such loads
// or stores into memcpys or memsets since MemCpyOptimizer
// should do that. It's just small cases where there are
// a few elements - MemCpyOptimizer might decide it's better
// to load/store to inline the memcpy for example, or the
// code generator might have started with loads and stores.

#include "llvmAggregateGlobalOps.h"

#ifdef HAVE_LLVM

#include "llvmUtil.h"

#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/Statistic.h"

#if HAVE_LLVM_VER >= 35
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#else
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/CallSite.h"
#endif


#if HAVE_LLVM_VER >= 35
#include "llvm/IR/Verifier.h"
#else
#include "llvm/Analysis/Verifier.h"
#endif

#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/MemoryDependenceAnalysis.h"
#include "llvm/Transforms/Utils/ValueMapper.h"

#if HAVE_LLVM_VER >= 37
#include "llvm/IR/GetElementPtrTypeIterator.h"
#endif

#include <cstdio>
#include <list>
#include <string>
#include <unordered_map>

using namespace llvm;

namespace {


static const bool DEBUG = false;
static const bool extraChecks = true;
// Set a function name here to get lots of debugging output.
static const char* debugThisFn = "";//"deinit6";


// If there is a gap between memory that we are loading,
// for example due to padding, or just because we didn't
// need some data, and the gap is < this amount, we
// will do one get and then just fish out the parts we
// used.
#define GET_EXTRA 64

static inline
bool isMergeableGlobalLoadOrStore(Instruction* I,
                                  unsigned globalSpace,
                                  bool findLoad, bool findStore)
{
  if( findLoad && isa<LoadInst>(I) ) {
    LoadInst *load = cast<LoadInst>(I);
    if( load->getPointerAddressSpace() == globalSpace &&
        load->isSimple() ) {
      return true;
    }
  }
  if( findStore && isa<StoreInst>(I)) {
    StoreInst *store = cast<StoreInst>(I);
    if( store->getPointerAddressSpace() == globalSpace &&
        store->isSimple() ) {
      return true;
    }
  }
  return false;
}
static inline
Value* getLoadStorePointer(Instruction* I)
{
  if( isa<LoadInst>(I) ) {
    LoadInst *load = cast<LoadInst>(I);
    return load->getPointerOperand();
  }
  if( isa<StoreInst>(I)) {
    StoreInst *store = cast<StoreInst>(I);
    return store->getPointerOperand();
  }
  return NULL;
}

// Given a start and end load/store instruction (in the same basic block),
// reorder the instructions so that the addressing instructions are
// first, the load/store instructions are next, and then the
// uses of loaded values are last. This reordering is valid when
// the other instructions do not read or write memory.
// Returns the last instruction in the reordering.
static
Instruction* reorderAddressingMemopsUses(Instruction *FirstLoadOrStore,
                                         Instruction *LastLoadOrStore,
                                         bool DebugThis)
{
  SmallPtrSet<Instruction*, 8> memopsUses;
  Instruction *LastMemopUse = NULL;

#if HAVE_LLVM_VER >= 38
  for (BasicBlock::iterator BI = FirstLoadOrStore->getIterator();
       !isa<TerminatorInst>(BI);
       ++BI)
#else
  for (BasicBlock::iterator BI = FirstLoadOrStore;
       !isa<TerminatorInst>(BI);
       ++BI)
#endif
  {
    Instruction& insnRef = *BI;
    Instruction* insn = &insnRef;
    bool isUseOfMemop = false;

    if( isa<StoreInst>(insn) || isa<LoadInst>(insn) ) {
      memopsUses.insert(insn);
      continue;
    }
    // Check -- are any operands to this instruction memopsUses?
    for (User::op_iterator i = insn->op_begin(), e = insn->op_end(); i != e; ++i) {
      Value *v = *i;
      if(Instruction *uses_insn = dyn_cast<Instruction>(v)) {
        if( memopsUses.count(uses_insn) ){
          isUseOfMemop = true;
          break;
        }
      }
    }

    if( isUseOfMemop ) memopsUses.insert(insn);

    if( insn == LastLoadOrStore ) break;
  }

  LastMemopUse = LastLoadOrStore;

  // Reorder the instructions here.
  // Move all addressing instructions before StartInst.
  // Move all uses of loaded values before LastLoadOrStore (which will be removed).
#if HAVE_LLVM_VER >= 38
  for (BasicBlock::iterator BI = FirstLoadOrStore->getIterator();
       !isa<TerminatorInst>(BI);)
#else
  for (BasicBlock::iterator BI = FirstLoadOrStore;
       !isa<TerminatorInst>(BI);)
#endif
  {
    Instruction& insnRef = *BI;
    Instruction* insn = &insnRef;
    ++BI; // don't invalidate iterator.
    // Leave loads/stores where they are (they will be removed)
    if( isa<StoreInst>(insn) || isa<LoadInst>(insn) ) {
      if( DebugThis ) {
        errs() << "found load/store: "; insn->dump();
      }
    } else if( memopsUses.count(insn) ) {
      if( DebugThis ) {
        errs() << "found memop use: "; insn->dump();
      }
      // Move uses of memops to after the final memop.
      insn->removeFromParent();
      insn->insertAfter(LastMemopUse);
      LastMemopUse = insn;
    } else {
      if( DebugThis ) {
        errs() << "found other: "; insn->dump();
      }
      // Move addressing instructions to before the first memop.
      insn->removeFromParent();
      insn->insertBefore(FirstLoadOrStore);
    }
    if( insn == LastLoadOrStore ) break;
  }

  return LastMemopUse;
}

// The next several fns are stolen almost totally unmodified from MemCpyOptimizer.
// modified code areas say CUSTOM.

static int64_t GetOffsetFromIndex(const GEPOperator *GEP,
                                  unsigned Idx,
                                  bool &VariableIdxFound,
                                  const DataLayout &DL){
  // Skip over the first indices.
  gep_type_iterator GTI = gep_type_begin(GEP);
  for (unsigned i = 1; i != Idx; ++i, ++GTI)
    /*skip along*/;

  // Compute the offset implied by the rest of the indices.
  int64_t Offset = 0;
  for (unsigned i = Idx, e = GEP->getNumOperands(); i != e; ++i, ++GTI) {
    ConstantInt *OpC = dyn_cast<ConstantInt>(GEP->getOperand(i));
    if (!OpC)
      return VariableIdxFound = true;
    if (OpC->isZero()) continue;  // No offset.

    // Handle struct indices, which add their field offset to the pointer.
#if HAVE_LLVM_VER >= 40
    if (StructType *STy = GTI.getStructTypeOrNull())
#else
    if (StructType *STy = dyn_cast<StructType>(*GTI))
#endif
    {
      Offset += DL.getStructLayout(STy)->getElementOffset(OpC->getZExtValue());
      continue;
    }

    // Otherwise, we have a sequential type like an array or vector.  Multiply
    // the index by the ElementSize.
    uint64_t Size = DL.getTypeAllocSize(GTI.getIndexedType());
    Offset += Size*OpC->getSExtValue();
  }

  return Offset;
}
/// IsPointerOffset - Return true if Ptr1 is provably equal to Ptr2 plus a
/// constant offset, and return that constant offset.  For example, Ptr1 might
/// be &A[42], and Ptr2 might be &A[40].  In this case offset would be -8.
static bool IsPointerOffset(Value *Ptr1, Value *Ptr2, int64_t &Offset,
                            const DataLayout &DL) {
  Ptr1 = Ptr1->stripPointerCasts();
  Ptr2 = Ptr2->stripPointerCasts();

  // Handle the trivial case first.
  if (Ptr1 == Ptr2) {
    Offset = 0;
    return true;
  }

  GEPOperator *GEP1 = dyn_cast<GEPOperator>(Ptr1);
  GEPOperator *GEP2 = dyn_cast<GEPOperator>(Ptr2);

  bool VariableIdxFound = false;

  // If one pointer is a GEP and the other isn't, then see if the GEP is a
  // constant offset from the base, as in "P" and "gep P, 1".
  if (GEP1 && !GEP2 && GEP1->getOperand(0)->stripPointerCasts() == Ptr2) {
    Offset = -GetOffsetFromIndex(GEP1, 1, VariableIdxFound, DL);
    return !VariableIdxFound;
  }

  if (GEP2 && !GEP1 && GEP2->getOperand(0)->stripPointerCasts() == Ptr1) {
    Offset = GetOffsetFromIndex(GEP2, 1, VariableIdxFound, DL);
    return !VariableIdxFound;
  }

  // Right now we handle the case when Ptr1/Ptr2 are both GEPs with an identical
  // base.  After that base, they may have some number of common (and
  // potentially variable) indices.  After that they handle some constant
  // offset, which determines their offset from each other.  At this point, we
  // handle no other case.
  if (!GEP1 || !GEP2 || GEP1->getOperand(0) != GEP2->getOperand(0))
    return false;

  // Skip any common indices and track the GEP types.
  unsigned Idx = 1;
  for (; Idx != GEP1->getNumOperands() && Idx != GEP2->getNumOperands(); ++Idx)
    if (GEP1->getOperand(Idx) != GEP2->getOperand(Idx))
      break;

  int64_t Offset1 = GetOffsetFromIndex(GEP1, Idx, VariableIdxFound, DL);
  int64_t Offset2 = GetOffsetFromIndex(GEP2, Idx, VariableIdxFound, DL);
  if (VariableIdxFound) return false;

  Offset = Offset2-Offset1;
  return true;
}


struct MemOpRange { // from MemsetRange in MemCpyOptimizer
  // Start/End - A semi range that describes the span that this range covers.
  // The range is closed at the start and open at the end: [Start, End).
  int64_t Start, End;
  // CUSTOM: End including slack space, to allow for gaps
  int64_t SlackEnd;
  /// StartPtr - The getelementptr instruction that points to the start of the
  /// range.
  Value *StartPtr;
  /// Alignment - The known alignment of the first store.
  unsigned Alignment;
  // The load or store instructions. Called TheStores because
  // we stole this code from MemCpyOptimizer, but it might also store load instructions.
  SmallVector<Instruction*, 16> TheStores;
};
struct MemOpRanges { // from MemsetRanges in MemCpyOptimizer
  /// Ranges - A sorted list of the memset ranges.  We use std::list here
  /// because each element is relatively large and expensive to copy.
  std::list<MemOpRange> Ranges;
  typedef std::list<MemOpRange>::iterator range_iterator;
  const DataLayout &DL;
  MemOpRanges(const DataLayout &td) : DL(td) { }
  typedef std::list<MemOpRange>::const_iterator const_iterator;
  const_iterator begin() const { return Ranges.begin(); }
  const_iterator end() const { return Ranges.end(); }
  bool empty() const { return Ranges.empty(); }
  bool moreThanOneOp() const {
    if( Ranges.size() > 1 ) return true;
    MemOpRanges::const_iterator I = begin();
    MemOpRanges::const_iterator E = end();
    if( I != E ) {
      const MemOpRange &Range = *I;
      if( Range.TheStores.size() > 1 ) return true;
    }
    return false;
  }
  void addInst(int64_t offsetFromFirst, Instruction *Inst) {
    if( StoreInst *SI = dyn_cast<StoreInst>(Inst) ) {
      addStore(offsetFromFirst, SI);
    }
    if( LoadInst *LI = dyn_cast<LoadInst>(Inst) ) {
      addLoad(offsetFromFirst, LI);
    }
  }
  void addStore(int64_t OffsetFromFirst, StoreInst *SI) {
    int64_t StoreSize = DL.getTypeStoreSize(SI->getOperand(0)->getType());
    int64_t Slack = 0; // TODO - compute slack based on structure padding.
                       // Make slack include padding if it is after this
                       // element in a structure.

    addRange(OffsetFromFirst, StoreSize, Slack,
             SI->getPointerOperand(), SI->getAlignment(), SI);
  }
  // CUSTOM because MemsetRanges doesn't work with LoadInsts.
  void addLoad(int64_t OffsetFromFirst, LoadInst *LI) {
    Type* ptrType = LI->getOperand(0)->getType();
    int64_t LoadSize = DL.getTypeStoreSize(ptrType->getPointerElementType());
    int64_t Slack =  GET_EXTRA; // Pretend loads use more space...

    addRange(OffsetFromFirst, LoadSize, Slack,
             LI->getPointerOperand(), LI->getAlignment(), LI);
  }
  // CUSTOM adds Slack
  void addRange(int64_t Start, int64_t Size, int64_t Slack, Value *Ptr,
                unsigned Alignment, Instruction *Inst);
};

/// addRange - Add a new store to the MemOpRanges data structure.  This adds a
/// new range for the specified store at the specified offset, merging into
/// existing ranges as appropriate.
///
/// Do a linear search of the ranges to see if this can be joined and/or to
/// find the insertion point in the list.  We keep the ranges sorted for
/// simplicity here.  This is a linear search of a linked list, which is ugly,
/// however the number of ranges is limited, so this won't get crazy slow.
// CUSTOM -- uses SlackEnd instead of End
void MemOpRanges::addRange(int64_t Start, int64_t Size, int64_t Slack, Value *Ptr,
                            unsigned Alignment, Instruction *Inst) {
  int64_t End = Start+Size;
  int64_t SlackEnd = Start+Size+Slack;
  range_iterator I = Ranges.begin(), E = Ranges.end();

  while (I != E && Start > I->SlackEnd)
    ++I;

  // We now know that I == E, in which case we didn't find anything to merge
  // with, or that Start <= I->End.  If End < I->Start or I == E, then we need
  // to insert a new range.  Handle this now.
  if (I == E || SlackEnd < I->Start) {
    MemOpRange &R = *Ranges.insert(I, MemOpRange());
    R.Start        = Start;
    R.End          = End;
    R.SlackEnd     = SlackEnd;
    R.StartPtr     = Ptr;
    R.Alignment    = Alignment;
    R.TheStores.push_back(Inst);
    return;
  }

  // This store overlaps with I, add it.
  I->TheStores.push_back(Inst);

  // CUSTOM: Update End too.
  if (End > I->End) I->End = End;

  // At this point, we may have an interval that completely contains our store.
  // If so, just add it to the interval and return.
  if (I->Start <= Start && I->SlackEnd >= SlackEnd)
    return;

  // Now we know that Start <= I->End and End >= I->Start so the range overlaps
  // but is not entirely contained within the range.

  // See if the range extends the start of the range.  In this case, it couldn't
  // possibly cause it to join the prior range, because otherwise we would have
  // stopped on *it*.
  if (Start < I->Start) {
    I->Start = Start;
    I->StartPtr = Ptr;
    I->Alignment = Alignment;
  }

  // Now we know that Start <= I->End and Start >= I->Start (so the startpoint
  // is in or right at the end of I), and that End >= I->Start.  Extend I out to
  // End.
  if (SlackEnd > I->SlackEnd) {
    I->SlackEnd = SlackEnd;
    range_iterator NextI = I;
    while (++NextI != E && SlackEnd >= NextI->Start) {
      // Merge the range in.
      I->TheStores.append(NextI->TheStores.begin(), NextI->TheStores.end());
      if (NextI->SlackEnd > I->SlackEnd)
        I->SlackEnd = NextI->SlackEnd;
      if (NextI->End > I->End)
        I->End = NextI->End;
      Ranges.erase(NextI);
      NextI = I;
    }
  }
}

// END stolen from MemCpyOptimizer.

  struct AggregateGlobalOpsOpt : public FunctionPass {
    const DataLayout *DL;
    unsigned globalSpace;

  public:
    static char ID; // Pass identification, replacement for typeid
    AggregateGlobalOpsOpt() : FunctionPass(ID) {
      DL = 0;
      errs() << "Warning: aggregate-global-opts using default configuration\n";
      globalSpace = 100;
    }
    AggregateGlobalOpsOpt(unsigned _globalSpace) : FunctionPass(ID) {
      DL = 0;
      globalSpace = _globalSpace;
    }


    bool runOnFunction(Function &F);

  private:
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      // TODO -- update these better
      AU.setPreservesCFG();
      /*AU.addRequired<DominatorTree>();
      AU.addRequired<MemoryDependenceAnalysis>();
      AU.addRequired<AliasAnalysis>();
      AU.addRequired<TargetLibraryInfo>();*/
      //AU.addPreserved<AliasAnalysis>();
      //AU.addPreserved<MemoryDependenceAnalysis>();
    }

    Instruction *tryAggregating(Instruction *I, Value *StartPtr, bool DebugThis);
  };

  char AggregateGlobalOpsOpt::ID = 0;
  static RegisterPass<AggregateGlobalOpsOpt> X("aggregate-global-ops", "Aggregate Global Pointer Operations", false /* only looks at CFG */, false /* Analysis pass */ );

} // end anon namespace.

// createAggregateGlobalOpsOptPass - The public interface to this file...
FunctionPass *createAggregateGlobalOpsOptPass(unsigned globalSpace)
{
  return new AggregateGlobalOpsOpt(globalSpace);
}

/// tryAggregating - When scanning forward over instructions, we look for
/// other loads or stores that could be aggregated with this one.
/// Returns the last instruction added (if one was added) since we might have
/// removed some loads or stores and that might invalidate an iterator.
Instruction *AggregateGlobalOpsOpt::tryAggregating(Instruction *StartInst, Value *StartPtr,
    bool DebugThis) {
  if (DL == 0) return 0;

  Module* M = StartInst->getParent()->getParent()->getParent();
  LLVMContext& Context = StartInst->getContext();

  Type* int8Ty = Type::getInt8Ty(Context);
  // TODO -- sizeTy should be the size of a pointer in addr space 0
  Type* sizeTy = Type::getInt64Ty(Context);
  Type* globalInt8PtrTy = int8Ty->getPointerTo(globalSpace);
  bool isLoad = isa<LoadInst>(StartInst);
  bool isStore = isa<StoreInst>(StartInst);
  Instruction *lastAddedInsn = NULL;
  Instruction *LastLoadOrStore = NULL;

  SmallVector<Instruction*, 8> toRemove;

  // Okay, so we now have a single global load/store. Scan to find
  // all subsequent stores of the same value to offset from the same pointer.
  // Join these together into ranges, so we can decide whether contiguous blocks
  // are stored.
  MemOpRanges Ranges(*DL);

  // Put the first store in since we want to preserve the order.
  Ranges.addInst(0, StartInst);

#if HAVE_LLVM_VER >= 38
  BasicBlock::iterator BI = StartInst->getIterator();
#else
  BasicBlock::iterator BI = StartInst;
#endif
  for (++BI; !isa<TerminatorInst>(BI); ++BI) {

    Instruction& insnRef = *BI;
    Instruction* insn = &insnRef;
    if( isMergeableGlobalLoadOrStore(insn, globalSpace, isLoad, isStore) ) {
      // OK!
    } else {
      // If the instruction is readnone, ignore it, otherwise bail out.  We
      // don't even allow readonly here because we don't want something like:
      // A[1] = 2; strlen(A); A[2] = 2; -> memcpy(A, ...); strlen(A).
      if (BI->mayWriteToMemory())
        break;
      if (isStore && BI->mayReadFromMemory())
        break;
      continue;
    }

    if ( isStore && isa<StoreInst>(BI) ) {
      StoreInst *NextStore = cast<StoreInst>(BI);
      // If this is a store, see if we can merge it in.
      if (!NextStore->isSimple()) break;

      // Check to see if this store is to a constant offset from the start ptr.
      int64_t Offset;
      if (!IsPointerOffset(StartPtr, NextStore->getPointerOperand(), Offset, *DL))
        break;

      Ranges.addStore(Offset, NextStore);
      LastLoadOrStore = NextStore;
    } else {
      LoadInst *NextLoad = cast<LoadInst>(BI);
      if (!NextLoad->isSimple()) break;

      // Check to see if this load is to a constant offset from the start ptr.
      int64_t Offset;
      if (!IsPointerOffset(StartPtr, NextLoad->getPointerOperand(), Offset, *DL))
        break;

      Ranges.addLoad(Offset, NextLoad);
      LastLoadOrStore = NextLoad;
    }
  }

  // If we have no ranges, then we just had a single store with nothing that
  // could be merged in.  This is a very common case of course.
  if (!Ranges.moreThanOneOp())
    return 0;

  // Divide the instructions between StartInst and LastLoadOrStore into
  // addressing, memops, and uses of memops (uses of loads)
  reorderAddressingMemopsUses(StartInst, LastLoadOrStore, DebugThis);

  Instruction* insertBefore = StartInst;
  IRBuilder<> builder(insertBefore);

  // Now that we have full information about ranges, loop over the ranges and
  // emit memcpy's for anything big enough to be worthwhile.
  for (MemOpRanges::const_iterator I = Ranges.begin(), E = Ranges.end();
       I != E; ++I) {
    const MemOpRange &Range = *I;

    if (Range.TheStores.size() == 1) continue; // Don't bother if there's only one thing...

    builder.SetInsertPoint(insertBefore);

    // Otherwise, we do want to transform this!  Create a new memcpy.
    // Get the starting pointer of the block.
    StartPtr = Range.StartPtr;

    if( DebugThis ) {
      errs() << "base is:";
      StartPtr->dump();
    }

    // Determine alignment
    unsigned Alignment = Range.Alignment;
    if (Alignment == 0) {
      Type *EltType =
        cast<PointerType>(StartPtr->getType())->getElementType();
      Alignment = DL->getABITypeAlignment(EltType);
    }

    Instruction *alloc = NULL;

    // create temporary alloca space to communicate to/from.
    alloc = makeAlloca(int8Ty, "agg.tmp", insertBefore,
                       Range.End-Range.Start, Alignment);

    // Cast the old base pointer to i8, but with the same address space.
    //Value* StartPtrI8 = builder.CreatePointerCast(StartPtr, globalInt8PtrTy);

    // If storing, do the stores we had into our alloca'd region.
    if( isStore ) {
      for (SmallVector<Instruction*, 16>::const_iterator
           SI = Range.TheStores.begin(),
           SE = Range.TheStores.end(); SI != SE; ++SI) {
        StoreInst* oldStore = cast<StoreInst>(*SI);

        if( DebugThis ) {
          errs() << "have store in range:";
          oldStore->dump();
        }

        int64_t offset = 0;
        bool ok = IsPointerOffset(StartPtr, oldStore->getPointerOperand(),
                                  offset, *DL);
        assert(ok && offset >= 0); // we used this before, didn't we?
        assert(!(oldStore->isVolatile() || oldStore->isAtomic()));

        Constant* offsetC = ConstantInt::get(sizeTy, offset, true);
        Value* offsets[] = {offsetC};
        Value* i8Dst = builder.CreateInBoundsGEP(int8Ty,
                                                 alloc,
                                                 offsets);

        Type* origDstTy = oldStore->getPointerOperand()->getType();
        Type* DstTy = origDstTy->getPointerElementType()->getPointerTo(0);
        Value* Dst = builder.CreatePointerCast(i8Dst, DstTy);

        StoreInst* newStore =
          builder.CreateStore(oldStore->getValueOperand(), Dst);
        newStore->setAlignment(oldStore->getAlignment());
        newStore->takeName(oldStore);
      }
    }

    // cast the pointer that was load/stored to i8 if necessary.
    Value *globalPtr = builder.CreatePointerCast(StartPtr, globalInt8PtrTy);

    // Get a Constant* for the length.
    Constant* len = ConstantInt::get(sizeTy, Range.End-Range.Start, false);

    // Now add the memcpy instruction
    unsigned addrSpaceDst,addrSpaceSrc;
    addrSpaceDst = addrSpaceSrc = 0;
    if( isStore ) addrSpaceDst = globalSpace;
    if( isLoad ) addrSpaceSrc = globalSpace;

    Type *types[3];
    types[0] = PointerType::get(int8Ty, addrSpaceDst);
    types[1] = PointerType::get(int8Ty, addrSpaceSrc);
    types[2] = sizeTy;

    Function *func = Intrinsic::getDeclaration(M, Intrinsic::memcpy, types);

    Value* args[5]; // dst src len alignment isvolatile
    if( isStore ) {
      // it's a store (ie put)
      args[0] = globalPtr;
      args[1] = alloc;
    } else {
      // it's a load (ie get)
      args[0] = alloc;
      args[1] = globalPtr;
    }
    args[2] = len;
    // alignment
    args[3] = ConstantInt::get(Type::getInt32Ty(Context), 0, false);
    // isvolatile
    args[4] = ConstantInt::get(Type::getInt1Ty(Context), 0, false);

    Instruction* aMemCpy = builder.CreateCall(func, args);

    /*
    DEBUG(dbgs() << "Replace ops:\n";
      for (unsigned i = 0, e = Range.TheStores.size(); i != e; ++i)
        dbgs() << *Range.TheStores[i] << '\n';
      dbgs() << "With: " << *AMemSet << '\n');
      */

    if (!Range.TheStores.empty())
      aMemCpy->setDebugLoc(Range.TheStores[0]->getDebugLoc());

    lastAddedInsn = aMemCpy;

    // If loading, load from the memcpy'd region
    if( isLoad ) {
      for (SmallVector<Instruction*, 16>::const_iterator
           SI = Range.TheStores.begin(),
           SE = Range.TheStores.end(); SI != SE; ++SI) {
        LoadInst* oldLoad = cast<LoadInst>(*SI);
        if( DebugThis ) {
          errs() << "have load in range:";
          oldLoad->dump();
        }

        int64_t offset = 0;
        bool ok = IsPointerOffset(StartPtr, oldLoad->getPointerOperand(),
                                  offset, *DL);
        assert(ok && offset >= 0); // we used this before, didn't we?
        assert(!(oldLoad->isVolatile() || oldLoad->isAtomic()));

        Constant* offsetC = ConstantInt::get(sizeTy, offset, true);
        Value* offsets[] = {offsetC};
        Value* i8Src = builder.CreateInBoundsGEP(int8Ty,
                                                 alloc,
                                                 offsets);
        Type* origSrcTy = oldLoad->getPointerOperand()->getType();
        Type* SrcTy = origSrcTy->getPointerElementType()->getPointerTo(0);
        Value* Src = builder.CreatePointerCast(i8Src, SrcTy);

        LoadInst* newLoad = builder.CreateLoad(Src);
        newLoad->setAlignment(oldLoad->getAlignment());
        oldLoad->replaceAllUsesWith(newLoad);
        newLoad->takeName(oldLoad);
        lastAddedInsn = newLoad;
      }
    }

    // Save old loads/stores for removal
    for (SmallVector<Instruction*, 16>::const_iterator
         SI = Range.TheStores.begin(),
         SE = Range.TheStores.end(); SI != SE; ++SI) {
      Instruction* insn = *SI;
      toRemove.push_back(insn);
    }
  }

  // Zap all the old loads/stores
  for (SmallVector<Instruction*, 16>::const_iterator
       SI = toRemove.begin(),
       SE = toRemove.end(); SI != SE; ++SI) {
    (*SI)->eraseFromParent();
  }

  return lastAddedInsn;
}

// AggregateGlobalOpsOpt::runOnFunction - This is the main transformation
// entry point for a function.
//
bool AggregateGlobalOpsOpt::runOnFunction(Function &F) {
  bool ChangedFn = false;
  bool DebugThis = DEBUG;

/*  std::string fname = F.getName();
  std::hash<std::string> hasher;
  int h = (int) hasher(fname);
  int mask = AGOMASK;
  int id = AGOID;
  if( (h & mask) != id) return false;

  if( fname.size() != 7 ) return false;

  if( F.getName().startswith("on_fn") ) return false;

  if (fname == "string2" || fname == "message") return false;

  if (fname == "deinit9") return false; // OK
  if (fname == "deinit5") return false; // OK
  //if (fname == "deinit6") return false;
  //if( F.getName().startswith("deinit") ) return false;
*/
  if( debugThisFn[0] && F.getName() == debugThisFn ) {
    DebugThis = true;
  }

  //MD = &getAnalysis<MemoryDependenceAnalysis>();
#if HAVE_LLVM_VER >= 37
  DL = & F.getParent()->getDataLayout();
#elif HAVE_LLVM_VER >= 35
  DL = & getAnalysisIfAvailable<DataLayoutPass>()->getDataLayout();
#else
  DL = getAnalysisIfAvailable<DataLayout>();
#endif
  //TLI = &getAnalysis<TargetLibraryInfo>();

  // Walk all instruction in the function.
  for (Function::iterator BB = F.begin(), BBE = F.end(); BB != BBE; ++BB) {

    bool ChangedBB = false;

    if( DebugThis ) {
      errs() << "Working on BB ";
      BB->dump();
    }

    for (BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE;) {
      // Avoid invalidating the iterator.
      Instruction& insnRef = *BI;
      Instruction *I = &insnRef;
      ++BI;

      if( isMergeableGlobalLoadOrStore(I, globalSpace, true, true) ) {
        Instruction* lastAdded = tryAggregating(I, getLoadStorePointer(I), DebugThis);
        if( lastAdded ) {
          ChangedBB = true;
          ChangedFn = true;
#if HAVE_LLVM_VER >= 38
          BI = lastAdded->getIterator();
#else
          BI = lastAdded;
#endif
        }
      }
    }

    if( DebugThis && ChangedBB ) {
      errs() << "in function " << F.getName() << "\n";
      errs() << "After transform BB is ";
      BB->dump();
    }

  }

  if( extraChecks ) {
#if HAVE_LLVM_VER >= 35
    assert(!verifyFunction(F, &errs()));
#else
    verifyFunction(F);
#endif
  }

  if (DebugThis && ChangedFn)
    printf("AggregateGlobalOpsOpt changed %s\n", F.getName().str().c_str());

  //MD = 0;
  return ChangedFn;
}


#endif
