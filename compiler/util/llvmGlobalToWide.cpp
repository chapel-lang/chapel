//===----------------------------------------------------------------------===//
// Chapel LLVM Wide Opt 
//===----------------------------------------------------------------------===//
// When --llvm-wide-opt is invoked, the code generator generates global
// pointers - ie, those with a special address space - instead of wide
// pointer structures. 
//
// Then, LLVM optimizations are run on this bitcode if you supply --fast.
// This pass then is run to lower the operations on global pointers to
// appropriate calls to the runtime (e.g. load -> get, store -> put).
//
// To invoke, use the flag --llvm-wide-opt along with --llvm and some
//  reason to generate wide pointers ( --no-local or a comms layer configured )
//
//===----------------------------------------------------------------------===//

#include "llvmGlobalToWide.h"

#ifdef HAVE_LLVM

#define DEBUG_TYPE "global-to-wide"

#include "llvmUtil.h"

#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Transforms/Utils/ValueMapper.h"

#include <cstdio>

using namespace llvm;

namespace {

  static const bool debugAllPassOne = false;
  static const bool debugAllPassTwo = false;
  static const bool extraChecks = false;
  // Set a function name here to get lots of debugging output.
  static const char* debugThisFn = "";

  AllocaInst* makeAlloca(llvm::Type* type,
                         const char* name,
                         Instruction* insertBefore)
  {
    // It's important to alloca at the front of the function in order
    // to avoid having an alloca in a loop which is a good way to achieve
    // stack overflow.
    Function *func = insertBefore->getParent()->getParent();
    BasicBlock* entryBlock = & func->getEntryBlock();

    if( insertBefore->getParent() == entryBlock ) {
      // Add before specific instruction in entry block.
    } else if(llvm::Instruction *i = func->getEntryBlock().getTerminator()) {
      // Add before terminator in entry block.
      insertBefore = i;
    } else {
      // Add at the end of entry block.
      insertBefore = NULL;
    }

    AllocaInst *tempVar;

    if( insertBefore ) {
      tempVar = new AllocaInst(type, name, insertBefore);
    } else {
      tempVar = new AllocaInst(type, name, entryBlock);
    }

    return tempVar;
  }

#if 0
  void typeChangeReplaceInstWithInst(Instruction* Old, Instruction* New)
  {
    assert(Old->getParent());
    assert(New->getParent());
    assert(New && "Value::replaceAllUsesWith(<null>) is invalid!");
    assert(New != Old && "this->replaceAllUsesWith(this) is NOT valid!");
  
    // Notify all ValueHandles (if present) that this value is going away.
    if (Old->hasValueHandle())
      ValueHandleBase::ValueIsRAUWd(Old, New);
  
    while (!Old->use_empty()) {
      User* U = Old->use_back();

      U->replaceUsesOfWith(Old, New);
    }
 
    Old->eraseFromParent();
  }
#endif

  // Like the version in BasicBlockUtils but assumes New is already
  // in the block.
  void myReplaceInstWithInst(Instruction* Old, Instruction* New)
  {
    assert(Old->getParent());
    assert(New->getParent());
    Old->replaceAllUsesWith(New);
    if(Old->hasName() && !New->hasName()) New->takeName(Old);
    Old->eraseFromParent();
  }

  bool isaGlobalPointer(GlobalToWideInfo* info, Type* type)
  {
    PointerType* pt = dyn_cast<PointerType>(type);
    if( pt && pt->getAddressSpace() == info->globalSpace ) return true;
    return false;
  }

  Constant* createSizeof(GlobalToWideInfo* info, Type* type)
  {
    assert( !containsGlobalPointers(info, type) );
    return ConstantExpr::getSizeOf(type);
  }

  Constant* createLoadStoreControl(Module &M, GlobalToWideInfo* info, AtomicOrdering ordering, SynchronizationScope scope) {
    int atomic_part = 0;
    int sync_part = 0;
    int val = 0;
    atomic_part = ordering;
    sync_part = scope;
    sync_part *= 256;
    val = atomic_part + sync_part;
    Type* ty = Type::getInt64Ty(M.getContext());
    return ConstantInt::get(ty, val);
  }

  Value* createWideBitCast(GlobalToWideInfo* info, Value* widePtr, Type* widePtrType, Instruction* insertBefore) {
    /* a sketch of how it would work with structs:
     *
    // The destination type should be a wide pointer.
    assert(widePtrType->isStructTy());

    Value* loc = ExtractValueInst::Create(widePtr,
                                          info->wideLocGEP,
                                          "", insertBefore);

    Value* ptr = ExtractValueInst::Create(widePtr,
                                          info->wideAddrGEP,
                                          "", insertBefore);

    Constant* undef = UndefValue::get(widePtrType);
    Constant* undefLocPtr = ConstantExpr::getExtractValue(undef,
                                                       info->wideAddrGEP);
    // get the local address space pointer.
    Value* cast = CastInst::CreatePointerCast(ptr, undefLocPtr->getType(), "", insertBefore);

    Instruction* locSet = InsertValueInst::Create(undef, loc,
                                                  info->wideLocGEP,
                                                  "", insertBefore);

    Instruction* ptrSet = InsertValueInst::Create(locSet, cast,
                                                  info->wideAddrGEP,
                                                  "", insertBefore);
    return ptrSet;
    */
    if( widePtr->getType() == widePtrType ) return widePtr;
    else return CastInst::CreatePointerCast(widePtr, widePtrType,
                                            "", insertBefore);
  }

  void checkFunctionExistAndHasArgs(Constant* f, unsigned nArgs)
  {
    assert(f);
    if( Function* ff = dyn_cast<Function>(f) ) {
      assert(ff->getParent());
    }
    Type* t = f->getType();
    FunctionType* ft = NULL;
    if( PointerType* pt = dyn_cast<PointerType>(t) ) {
      t = pt->getElementType();
    }
    ft = cast<FunctionType>(t);
    assert(ft);
    assert(ft->getNumParams() == nArgs);
  }


  struct TypeFixer : public ValueMapTypeRemapper {
  public:
    /// remapType - The client should implement this method if they want to
    /// remap types while mapping values.
    virtual Type *remapType(Type *SrcTy) = 0;

    // When remapping things with remapped types, these functions
    // provide an opportunity to do the remapping. They should return
    // NULL if that specific thing does not need to be remapped. They
    // do not need to handle remapping e.g. structure or arrays
    //  if only elements need remapping.
    virtual Constant* remapConstant(const Constant* C,
                                    ValueToValueMapTy &VM,
                                    RemapFlags Flags) = 0;
  };

  struct GlobalTypeFixer : TypeFixer {
    Module & M;
    GlobalToWideInfo * info;
    bool debugPassTwo;
    // these are here to save some typing.
    llvm::Type* voidTy;
    llvm::Type* voidPtrTy;
    llvm::Type* glVoidPtrTy;
    llvm::Type* wideVoidPtrTy;
    llvm::Type* ptrLocTy;
    llvm::Type* i64Ty;
    llvm::Type* i8Ty;

    // See llvmGlobalToWide.h for descriptions of these functions
    llvm::Constant* addrFn;
    llvm::Constant* locFn;
    llvm::Constant* nodeFn;
    llvm::Constant* makeFn;
    llvm::Constant* getFn;
    llvm::Constant* putFn;
    llvm::Constant* getPutFn;
    llvm::Constant* memsetFn;


    GlobalTypeFixer(Module & M, GlobalToWideInfo * info, bool debugPassTwo)
      : M(M), info(info), debugPassTwo(debugPassTwo)
    {
      assert(info->globalSpace > 0);
      assert(info->localeIdType);
      assert(info->nodeIdType);
      // Wide pointer address space must differ from the local one...
      assert(info->globalSpace != 0);
      assert(info->localeIdType != 0);
      assert(info->nodeIdType != 0);

      voidTy = llvm::Type::getVoidTy(M.getContext());
      voidPtrTy = llvm::Type::getInt8PtrTy(M.getContext(), 0);
      glVoidPtrTy = llvm::Type::getInt8PtrTy(M.getContext(),
                                             info->globalSpace);
      wideVoidPtrTy = convertTypeGlobalToWide(&M, info, glVoidPtrTy);
      ptrLocTy = info->localeIdType->getPointerTo(0);
      i64Ty = llvm::Type::getInt64Ty(M.getContext());
      i8Ty = llvm::Type::getInt8Ty(M.getContext());

      assert(voidPtrTy);
      assert(wideVoidPtrTy);

      addrFn = info->addrFn;
      if( ! addrFn ) {
        addrFn = M.getOrInsertFunction("chpl_wide_ptr_get_address_sym",
                                       voidPtrTy, wideVoidPtrTy, NULL);
      }
      checkFunctionExistAndHasArgs(addrFn, 1);

      locFn = info->locFn;
      if( ! locFn ) {
        locFn = M.getOrInsertFunction("chpl_wide_ptr_read_localeID_sym",
                                      voidTy, wideVoidPtrTy, ptrLocTy, NULL);
      }
      checkFunctionExistAndHasArgs(locFn, 2);

      nodeFn = info->nodeFn;
      if( ! nodeFn ) {
        nodeFn = M.getOrInsertFunction("chpl_wide_ptr_get_node_sym",
                                       info->nodeIdType, wideVoidPtrTy, NULL);
      }
      checkFunctionExistAndHasArgs(nodeFn, 1);

      makeFn = info->makeFn;
      if( ! makeFn ) {
        makeFn = M.getOrInsertFunction("chpl_return_wide_ptr_loc_sym",
                                     wideVoidPtrTy, ptrLocTy, voidPtrTy, NULL);
      }
      checkFunctionExistAndHasArgs(makeFn, 2);

      getFn = info->getFn;
      if( ! getFn ) {
        getFn = M.getOrInsertFunction("chpl_gen_comm_get_ctl_sym", voidTy,
                                      voidPtrTy, wideVoidPtrTy,
                                      i64Ty, i64Ty, NULL);
      }
      checkFunctionExistAndHasArgs(getFn, 4);

      putFn = info->putFn;
      if( ! putFn ) {
        putFn = M.getOrInsertFunction("chpl_gen_comm_put_ctl_sym", voidTy,
                                      wideVoidPtrTy, voidPtrTy,
                                      i64Ty, i64Ty, NULL);
      }
      checkFunctionExistAndHasArgs(putFn, 4);

      getPutFn = info->getPutFn;
      if( ! getPutFn ) {
        getPutFn = M.getOrInsertFunction("chpl_gen_comm_getput_sym", voidTy,
                                         wideVoidPtrTy, wideVoidPtrTy,
                                         i64Ty, NULL);
      }
      checkFunctionExistAndHasArgs(getPutFn, 3);

      memsetFn = info->memsetFn;
      if( ! memsetFn ) {
        memsetFn = M.getOrInsertFunction("chpl_gen_comm_memset_sym", voidTy,
                                         wideVoidPtrTy, i8Ty, i64Ty, NULL);
      }
      checkFunctionExistAndHasArgs(memsetFn, 3);
    }

    Function* getGlobalToWideFn(Type* globalPtrTy) {
      populateFunctionsForGlobalToWideType(&M, info, globalPtrTy);
      return info->gTypes[globalPtrTy].globalToWideFn;
    }
    Function* getWideToGlobalFn(Type* globalPtrTy) {
      populateFunctionsForGlobalToWideType(&M, info, globalPtrTy);
      return info->gTypes[globalPtrTy].wideToGlobalFn;
    }

    CallInst* callGlobalToWideFn(Value* globalPtr, Instruction* insertBefore) {
      Type* globalTy = globalPtr->getType();
      Function* fn = getGlobalToWideFn(globalTy);
      Value* local_args[1];
      local_args[0] = globalPtr;
      CallInst* call = CallInst::Create( fn, local_args, "", insertBefore);
      return call;
    }
    CallInst* callWideToGlobalFn(Value* widePtr, Type* globalTy, Instruction* insertBefore) {
      Function* fn = getWideToGlobalFn(globalTy);
      Value* local_args[1];
      local_args[0] = widePtr;
      CallInst* call = CallInst::Create( fn, local_args, "", insertBefore);
      return call;
    }

    // When we encounter a call to wideToGlobal or globalToWide,
    // we store in the map that we will replace it with its argument.
    // This function looks for that argument being a call to wideToGlobal
    // or globalToWide in order to find the ultimate source.
    // If it's anything else (e.g. a PHINode), we won't go further.
    Value* getWideArg(Value* arg) {
      while(arg) {

        bool stop_this_time = true;
        CallInst *call = dyn_cast<CallInst>(arg);

        if( call ) {
          // Verify that the instruction is in a basic block.
          assert(call->getParent());

          Function* calledFn = call->getCalledFunction(); // null if indirect
          // handle wide2global, global2wide
          if ( calledFn && info->specialFunctions.count(calledFn) ) {
            // Distinguish among the various special functions by the
            // function signature.
            if( calledFn->getName().startswith(GLOBAL_FN_WIDE_TO_GLOBAL) ||
                calledFn->getName().startswith(GLOBAL_FN_GLOBAL_TO_WIDE) )
            {
              // GLOBAL_FN_WIDE_TO_GLOBAL wide2global(wide)->*100 --- remove
              // GLOBAL_FN_GLOBAL_TO_WIDE global2wide(*100)->wide --- remove
              arg = call->getArgOperand(0);
              stop_this_time = false;
            } else {
              assert(0 && "Unknown special function");
            }
          }
        }

        // Stop searching if we didn't find another call to w2g or g2w
        if( stop_this_time ) break;
      }

      return arg;
    }

    void fixInstruction(Instruction* insn)
    {
      if( debugPassTwo ) {
        errs() << " atf|" << *insn << "|" << "\n";
      }

      // First, check to see if the instruction operates on
      // global pointer values. If not, there is no reason
      // to modify it.
      // We only handle instructions that
      // operate *directly* on global pointer values. We leave
      // the more general type-switching and re-threading to
      // a second pass through the instructions in the function
      // with RemapInstruction.
      bool needsWork = false;
      for(unsigned int i=0; i < insn->getNumOperands(); i++) {
        Value *old = insn->getOperand(i);
        if( isaGlobalPointer(info, old->getType()) ) needsWork = true;
      }
      if( isaGlobalPointer(info, insn->getType()) ) needsWork = true;

      if( ! needsWork ) {
        if( debugPassTwo ) errs() << " okf|" << *insn << "|" << "\n";
        return;
      }

      switch(insn->getOpcode()) {
        case Instruction::PHI: {
          PHINode* oldPHI = cast<PHINode>(insn);

          Type* globalTy = oldPHI->getType();
          Type* wideTy = convertTypeGlobalToWide(&M, info, globalTy);
          unsigned nIncoming = oldPHI->getNumIncomingValues();
          SmallVector<Value*,16> newIncoming(nIncoming);

          // fix phi nodes that have global pointer types.
          for(unsigned j = 0; j < nIncoming; j++) {
            BasicBlock* incomingBlock = oldPHI->getIncomingBlock(j);
            Value* incomingValue = oldPHI->getIncomingValue(j);
            // Add a call to global2wide before the terminator in
            // incomingBlock and save the result to store in the
            // new phi node.
            TerminatorInst* incomingEnd = incomingBlock->getTerminator();
            newIncoming[j] = callGlobalToWideFn(incomingValue, incomingEnd);
          }

          // create a new phi node with the replacement incoming values.
          PHINode* newPHI = PHINode::Create(wideTy, nIncoming,"",oldPHI);
          for(unsigned j = 0; j < nIncoming; j++) {
            newPHI->addIncoming(newIncoming[j],
                                oldPHI->getIncomingBlock(j));
          }
          newPHI->takeName(oldPHI);
          // and replace the old phi node with the result of a call
          // to wide to global.
          Instruction* firstNonPHI = oldPHI->getParent()->getFirstNonPHI();
          assert(firstNonPHI);
          CallInst* call = callWideToGlobalFn(newPHI, globalTy, firstNonPHI);
          myReplaceInstWithInst(oldPHI, call);
          break;
        }
        case Instruction::BitCast: {
          CastInst *oldCast = cast<CastInst>(insn);
          // bitcast could be casting *100 types
          // with wide pointer structure (vs packed wide pointers)
          // this has to do more.

          // First,  workaround a problem introduced by LLVM
          // optimization.
          
          Type* srcTy = oldCast->getSrcTy();
          Type* dstTy = oldCast->getDestTy();

          bool fromOk = srcTy->isPointerTy() || 
                        srcTy->isFunctionTy();
          bool toOK = dstTy->isPointerTy() || 
                      dstTy->isFunctionTy();

          bool fromGlobal = srcTy->isPointerTy() &&
                 srcTy->getPointerAddressSpace() == info->globalSpace;
          bool toGlobal   = dstTy->isPointerTy() &&
                 dstTy->getPointerAddressSpace() == info->globalSpace;
          bool fromWide = srcTy->isPointerTy() &&
                 srcTy->getPointerAddressSpace() == info->wideSpace;
          bool toWide   = dstTy->isPointerTy() &&
                 dstTy->getPointerAddressSpace() == info->wideSpace;


          if( ! (fromOk && toOK) ) {
            // What do we do for casts that aren't pointer-to-pointer?
            assert(0 && "casting between pointer and non-pointer");
          } else if( fromGlobal && toGlobal ) {
            // OK.
          } else if( fromGlobal || toGlobal ) {
            // Only one of them is global.
            if( oldCast->getSrcTy()->isPointerTy() &&
                oldCast->getDestTy()->isPointerTy() ) {
              if( fromGlobal && !toGlobal ) {
                // If this gets triggered, it might be from SROA.
                // See LLVM bug 15907.
                if( !toWide)
                  assert( 0 && "casting from global to local pointer");
              } else {
                if( !fromWide)
                  assert( 0 && "casting from local to global pointer");
              }
            } else {
              assert( 0 && "casting between global and non-global pointer");
            }
          }

          // Now, replace the cast with a wide pointer cast.
          Type* globalToTy = oldCast->getDestTy();
          Value* op = oldCast->getOperand(0);
          Type* wideToTy = convertTypeGlobalToWide(&M, info, globalToTy);

          // w2g(wide-cast(g2w(thing)))

          CallInst* g2w = callGlobalToWideFn(op, oldCast);
          Value* cst = createWideBitCast(info, g2w, wideToTy, oldCast);
          CallInst* w2g = callWideToGlobalFn(cst, globalToTy, oldCast);

          myReplaceInstWithInst(oldCast, w2g);
          break; }
        case Instruction::Load: {
          LoadInst *oldLoad = cast<LoadInst>(insn);
          if( oldLoad->getPointerAddressSpace() == info->globalSpace ) {
            // w2g(get(g2w(address)))

            Value* glAddrOp = oldLoad->getPointerOperand();
            CallInst* wAddr = callGlobalToWideFn(glAddrOp, oldLoad);

            // call to 'get'
            Type* glLoadedTy = oldLoad->getType();
            Type* wLoadedTy = convertTypeGlobalToWide(&M, info, glLoadedTy);
            // Create a call to 'get'
            // first, alloca a temporary to 'get' into
            Value* alloc = makeAlloca(wLoadedTy, "", oldLoad); 
            Value* castAlloc = new BitCastInst(alloc, voidPtrTy, "", oldLoad);
            Value* args[4];
            args[0] = castAlloc;
            args[1] = createWideBitCast(info, wAddr,
                                        wideVoidPtrTy,
                                        oldLoad);
            args[2] = createSizeof(info, wLoadedTy);
            args[3] = createLoadStoreControl(M, info, oldLoad->getOrdering(), oldLoad->getSynchScope());

            Value* call = CallInst::Create(getFn, args, "", oldLoad);
            assert(call);

            // Now load from the alloc'd area.
            Instruction* loadedWide = new LoadInst(alloc, "",
                                       oldLoad->isVolatile(),
                                       oldLoad->getAlignment(),
                                       oldLoad->getOrdering(),
                                       oldLoad->getSynchScope(),
                                       oldLoad);

            // now convert loadedWide back into a global type,
            // if necessary.
            Instruction* loadedGl = loadedWide;
            if( loadedWide->getType() != glLoadedTy ) {
              loadedGl = callWideToGlobalFn(loadedWide, glLoadedTy, oldLoad);
            }
            myReplaceInstWithInst(oldLoad, loadedGl);
          }
          break; }
        case Instruction::Store: {
          StoreInst *oldStore = cast<StoreInst>(insn);
          if( oldStore->getPointerAddressSpace() == info->globalSpace ) {
            // put(g2w(address),g2w(value)))

            Value* glAddrOp = oldStore->getPointerOperand();
            Value* glValueOp = oldStore->getValueOperand();
            Type* glValueTy = glValueOp->getType();

            Type* wStoredTy = convertTypeGlobalToWide(&M, info, glValueTy);

            Value* wValueOp = callGlobalToWideFn(glValueOp, oldStore);
            Value* wPtrOp = callGlobalToWideFn(glAddrOp, oldStore);

            // Create a call to 'put'
            // first, alloca a temporary to 'put' from
            Value* alloc = makeAlloca(wStoredTy, "", oldStore);
            Value* castAlloc = new BitCastInst(alloc, voidPtrTy, "", oldStore);

            // Now store to the alloc'd area
            Instruction* st = new StoreInst(wValueOp, alloc,
                                            oldStore->isVolatile(),
                                            oldStore->getAlignment(),
                                            oldStore->getOrdering(),
                                            oldStore->getSynchScope(),
                                            oldStore);
            assert(st);

            // Now put from the alloc'd area
            Value* args[4];
            args[0] = createWideBitCast(info, wPtrOp,
                                        wideVoidPtrTy,
                                        oldStore);
            args[1] = castAlloc;
            args[2] = createSizeof(info, wStoredTy);
            args[3] = createLoadStoreControl(M, info, oldStore->getOrdering(), oldStore->getSynchScope());

            Instruction* put = CallInst::Create(putFn, args, "", oldStore);
            myReplaceInstWithInst(oldStore, put);
          }
          break; }
        case Instruction::Call: {
          // handle e.g. wide2global, global2wide, memcpy
          CallInst *call = cast<CallInst>(insn);
          Function* calledFn = call->getCalledFunction(); // null if indirect

          if ( calledFn && info->specialFunctions.count(calledFn) ) {
            // Distinguish among the various special functions by name.
            if( calledFn->getName().startswith(GLOBAL_FN_WIDE_TO_GLOBAL))
            {
              // GLOBAL_FN_WIDE_TO_GLOBAL wide2global(wide)->*100 --- remove
              // Do nothing - replace these later.
            } else if( calledFn->getName().startswith(GLOBAL_FN_GLOBAL_TO_WIDE) )
            {
              // GLOBAL_FN_GLOBAL_TO_WIDE global2wide(*100)->wide --- remove
              // Do nothing - replace these later
            } else if( calledFn->getName().startswith(GLOBAL_FN_GLOBAL_ADDR) )
            {
              // GLOBAL_FN_GLOBAL_ADDR .gf.addr(*100)->*   --- extract addr
              // w2g( extract-addr( g2w(addr) ) )

              Value* glAddr = call->getArgOperand(0);

              Type* glLocAddrTy = calledFn->getReturnType();
              Type* wLocAddrTy = convertTypeGlobalToWide(&M, info, glLocAddrTy);
              Value* wAddr = callGlobalToWideFn(glAddr, call);

              Value* args[1];
              args[0] = createWideBitCast(info, wAddr,
                                          wideVoidPtrTy,
                                          call);
              Instruction* extr = CallInst::Create(addrFn, args, "", call);
              if( extr->getType() != calledFn->getReturnType() ) {
                extr = CastInst::CreatePointerCast(extr, wLocAddrTy, "", call);
              }
              Instruction *glbl = extr;
              // Now convert it back to global if necessary.
              if( extr->getType() != glLocAddrTy ) {
                glbl = callWideToGlobalFn(extr, glLocAddrTy, call);
              }
              assert(glbl->getType()->isPointerTy());
              myReplaceInstWithInst(call, glbl);
            } else if( calledFn->getName().startswith(GLOBAL_FN_GLOBAL_LOCID)){
              // GLOBAL_FN_GLOBAL_LOCID .gf.loc(*100),locale - extract .locale
              // extract-loc(g2w(addr))

              Value* glAddr = call->getArgOperand(0);

              Value* wAddr = callGlobalToWideFn(glAddr, call);

              Value* localePtr = makeAlloca(info->localeIdType, "", call);
              Value* args[2];
              args[0] = createWideBitCast(info, wAddr,
                                          wideVoidPtrTy,
                                          call);
              args[1] = localePtr;
              CallInst::Create(locFn, args, "", call);
              Instruction* loc = new LoadInst(localePtr, "", call);
              assert(!loc->getType()->isPointerTy());
              myReplaceInstWithInst(call, loc);
            } else if( calledFn->getName().startswith(GLOBAL_FN_GLOBAL_NODEID)){
              // GLOBAL_FN_GLOBAL_NODEID .gf.node(*100)->node - extract .node
              // extract-node(g2w(addr))

              Value* glAddr = call->getArgOperand(0);

              Value* wAddr = callGlobalToWideFn(glAddr, call);

              Value* args[1];
              args[0] = createWideBitCast(info, wAddr,
                                          wideVoidPtrTy,
                                          call);
              Instruction* node = CallInst::Create(nodeFn, args, "", call);
              assert(!node->getType()->isPointerTy());
              myReplaceInstWithInst(call, node);
            } else if( calledFn->getName().startswith(GLOBAL_FN_GLOBAL_MAKE) ){
              // GLOBAL_FN_GLOBAL_MAKE .gf.make(loc,*)->*100 --- make wide
              // w2g( make(g2w(addr), loc) )

              Value* locale = call->getArgOperand(0);
              Value* glLocAddr = call->getArgOperand(1);

              Value* wLocAddr = callGlobalToWideFn(glLocAddr, call);

              Type* gResType = calledFn->getReturnType();
              Type* wResType = convertTypeGlobalToWide(&M, info, gResType);
              
              Value* addr = wLocAddr;
              if( wLocAddr->getType() != voidPtrTy ) {
                addr = CastInst::CreatePointerCast(addr,
                                                   voidPtrTy,
                                                   "", call);
              }
              // Create a temporary with the locale value.
              Value* localePtr = makeAlloca(info->localeIdType, "", call);
              new StoreInst(locale, localePtr, call);
              Value* args[2];
              args[0] = localePtr;
              args[1] = addr;
              Instruction* make = CallInst::Create(makeFn, args, "", call);
              if( make->getType() != wResType ) {
                make = CastInst::CreatePointerCast(make,
                                                wResType,
                                                "", call);
              }
              assert(make->getType()->isPointerTy());

              make = callWideToGlobalFn(make, gResType, call);

              myReplaceInstWithInst(call, make);
            } else {
              assert(0 && "Unknown special function");
            }
          } else if( calledFn && isa<MemIntrinsic>(call) ) {
            if( isa<MemCpyInst>(call) || isa<MemMoveInst>(call) ) {
              // handle memcpy
              // is the source a global pointer?
              // memcpy(dst, src, n)
              // memcpy(g2w(dst), g2w(src), n)
              unsigned dstSpace,srcSpace;

              Value* gDst = call->getArgOperand(0);
              Value* gSrc = call->getArgOperand(1);
              Value* n = call->getArgOperand(2);

              dstSpace = gDst->getType()->getPointerAddressSpace();
              srcSpace = gSrc->getType()->getPointerAddressSpace();

              Value* wDst = callGlobalToWideFn(gDst, call);
              Value* wSrc = callGlobalToWideFn(gSrc, call);
              Value* ctl = createLoadStoreControl(M, info, NotAtomic, SingleThread);

              Instruction* putget = NULL;

              if( dstSpace == info->globalSpace &&
                  srcSpace != info->globalSpace ) {
                // It's a PUT
                Value* args[4];
                args[0] = createWideBitCast(info, wDst,
                                            wideVoidPtrTy,
                                            call);
                args[1] = wSrc;
                args[2] = n;
                args[3] = ctl;

                putget = CallInst::Create(putFn, args, "", call);
              } else if( srcSpace == info->globalSpace &&
                         dstSpace != info->globalSpace ) {
                // It's a GET
                Value* args[4];
                args[0] = wDst;
                args[1] = createWideBitCast(info, wSrc,
                                            wideVoidPtrTy,
                                            call);
                args[2] = n;
                args[3] = ctl;

                putget = CallInst::Create(getFn, args, "", call);
              } else {
                Value* args[3];
                args[0] = createWideBitCast(info, wDst,
                                            wideVoidPtrTy,
                                            call);
                args[1] = createWideBitCast(info, wSrc,
                                            wideVoidPtrTy,
                                            call);
                args[2] = n;

                assert(getPutFn && "Missing get-put-function for global-to-global memcpy");
                putget = CallInst::Create(getPutFn, args, "", call);
              }

              myReplaceInstWithInst(call, putget);
            } else if( isa<MemSetInst>(call) ) {
              // handle memset.
              Value* gDst = call->getArgOperand(0);
              Value* c = call->getArgOperand(1);
              Value* n = call->getArgOperand(2);

              Value* wDst = callGlobalToWideFn(gDst, call);

              Instruction* mset = NULL;

              Value* args[3];
              args[0] = createWideBitCast(info, wDst,
                                          wideVoidPtrTy,
                                          call);
              args[1] = c;
              args[2] = n;
              assert(memsetFn && "Missing memset-function for global memset");
              mset = CallInst::Create(memsetFn, args, "", call);
              myReplaceInstWithInst(call, mset);
            } else {
              assert(false && "Unknown intrinsic call with global pointer");
            }
          }
          break; }
        default:
          break;
      }
    }

    Type *remapType(Type *SrcTy) {
      return convertTypeGlobalToWide(&M, info, SrcTy);
    }

    Constant* remapConstant(const Constant* C,
                          ValueToValueMapTy &VM,
                          RemapFlags Flags) {
      Type* CT = C->getType();
      if( isa<PointerType>(CT) &&
          CT->getPointerAddressSpace() == info->globalSpace) {

        Type* newType = convertTypeGlobalToWide(&M, info, C->getType());

        if (isa<BlockAddress>(C)) {
          assert(0 && "BlockAddress shouldn't involve a global pointer");
        }
        if (isa<ConstantPointerNull>(C)) {
          if( newType->isStructTy() ) {
            // Null global pointer -> null wide struct.
            return Constant::getNullValue(newType);
          } else {
            // Null global pointer -> null wide pointer.
            return ConstantPointerNull::get(cast<PointerType>(newType));
          }
        }
        if (isa<GlobalValue>(C)) {
          // Should have already handled global variables/aliases/fns.

          // A use of a global variable?
          // TODO -- remove the bitcasts we added earlier.
          assert(0);
        }
      } 
      // Otherwise, return NULL to indicate we opted out
      //  of modifying the constant directly.
      return NULL;
    }
  };

  // Does nothing for global variables, functions, arguments.
  Constant* typeMapConstant(Constant* C,
                            ValueToValueMapTy &VM,
                            RemapFlags Flags,
                            TypeFixer *TypeMapper) {
    ValueToValueMapTy::iterator I = VM.find(C);
    
    // If the value already exists in the map, use it.
    if (I != VM.end() && I->second) return cast<Constant>(I->second);

    // Is it a global/fn? We were supposed to already handle these.
    if( isa<GlobalValue>(C) ) return C;

    Type* ty = TypeMapper->remapType(C->getType());

    SmallVector<Constant*, 16> elements(C->getNumOperands());
    bool newElement = false;
    for(unsigned int i=0; i < C->getNumOperands(); i++) {
      Constant* element = cast<Constant>(C->getOperand(i));
      elements[i] = typeMapConstant(element, VM, Flags, TypeMapper);
      if( elements[i] != element ) {
        newElement = true;
      }
    }

    // Check: are all of the elements the same?
    //        is the type the same?
    if( ty == C->getType() && !newElement ) {
      // Nothing else to do. Just return the constant.
      VM[C] = C;
      return C;
    }

    // If the type needs to change, offer it up to fixConstant,
    // which returns NULL if MapValue will handle it (ie it's
    // just the arguments that change).
    if( ty != C->getType() ) {
      Constant* newC = TypeMapper->remapConstant(C, VM, Flags);
      if( newC ) {
        VM[C] = newC;
        return newC;
      }
    }

    // Otherwise, just use MapValue to create a new version with the
    //  new arguments.
    Constant* ret = MapValue(C, VM, Flags, TypeMapper);
    if( ! ret ) ret = C;

    assert(ret->getType() == ty);

    return ret;
  }

  // Does nothing for global variables, functions, arguments.
  Value* typeMapValue(const Value* V,
                      ValueToValueMapTy &VM,
                      RemapFlags Flags,
                      TypeFixer *TypeMapper) {
    ValueToValueMapTy::iterator I = VM.find(V);
    
    // If the value already exists in the map, use it.
    if (I != VM.end() && I->second) return I->second;

    // If the value is a constant, handle it specially
    // (since we have to go into the constant structure
    //  to look for things like sizeof(int *100)
    //  that show no outward signs of needing remap).
    Constant *C = const_cast<Constant*>(dyn_cast<Constant>(V));
    if (C) return typeMapConstant(C, VM, Flags, TypeMapper);

    Value* ret = MapValue(V, VM, Flags, TypeMapper);
    if( ! ret ) ret = const_cast<Value*>(V);
    return ret;
  }

  // Change types of an instruction in-place. Note that some
  // LLVM routines will be unhappy if the types no longer check.
  void typeRemapInstruction(Instruction *I,
                            ValueToValueMapTy &VM,
                            RemapFlags Flags,
                            TypeFixer *TypeMapper) {
    // Put any operands that need to change into the map.
    unsigned i = 0;
    for (User::op_iterator op = I->op_begin(), E = I->op_end();
         op != E; ++op,i++) {
      Value *V = *op;
      Value *oldV = V;
      Value *newV = V;

      // Does the type change?
      Type *old_type = V->getType();
      Type *new_type = TypeMapper->remapType(old_type);

      // Is it a global/fn? We were supposed to already handle these.
      if( isa<GlobalValue>(V) ) continue;

      // Check for it in the map.
      ValueToValueMapTy::iterator I = VM.find(V);
    
      // If the value already exists in the map, use it.
      if (I != VM.end() && I->second) newV = I->second;
      else {
        Instruction *opI = dyn_cast<Instruction>(V);
        if( opI ) {
          // Do nothing for an instruction.
        } else {
          newV = typeMapValue(V, VM, Flags, TypeMapper);
          // Check that the new argument is the mapped type.
          // We don't do this for instructions since in
          // general we might not have remapped them yet.
          assert(newV != NULL);
          assert( newV->getType() == new_type );
        }
      }

      if( oldV != newV ) {
        *op = newV;
      }
    }


    Type *old_type = I->getType();
    Type *new_type = NULL;
    if( old_type ) TypeMapper->remapType(old_type);

    // Now, remap operands that we know about needing remap
    // If an operand is not in the map, just leave it alone.
    //  That's especially important for already-replaced
    //  operands (since they won't be in the map).
    // This will update PHI nodes incoming blocks that have
    // been remapped.
    //
    RemapInstruction(I, VM, RF_IgnoreMissingEntries, TypeMapper);

    if( extraChecks ) {
      if( VM.count(I) ) {
        Value* V = VM[I];
        if( new_type ) assert(V->getType() == new_type);
      } else {
        if( new_type ) assert(I->getType() == new_type);
      }
    }
  }




  // GlobalToWide - The first implementation, without getAnalysisUsage.
  struct GlobalToWide : public ModulePass {
    static char ID; // Pass identification, replacement for typeid

    GlobalToWideInfo * info;
    std::string layoutAfterwards;

    bool debugPassOne;
    bool debugPassTwo;

    /* info->globalSpace is the address space storing global pointers that
     *   need to be converted to wide pointers
     * layout is the target layout we should set the module to
     *   (could remove p record for address space 'space')
     */
    GlobalToWide(GlobalToWideInfo* _info, std::string layout)
      : ModulePass(ID), info(_info), layoutAfterwards(layout),
        //wideAddrGEPer(), wideLocGEPer(),
        debugPassOne(false),
        debugPassTwo(false)
    {
    }

    // Constructor for running within opt, for testing and
    // bugpoint.
    GlobalToWide()
      : ModulePass(ID), info(NULL), layoutAfterwards(""),
        debugPassOne(false),
        debugPassTwo(false)
    {
    }



    virtual bool runOnModule(Module &M) {
      bool madeInfo = false;

      if( debugThisFn[0] || debugAllPassOne || debugAllPassTwo ) {
        errs() << "GlobalToWide: ";
        errs().write_escaped(M.getModuleIdentifier()) << '\n';
        errs().write_escaped(M.getTargetTriple()) << '\n';
      }

      // Normally we expect a user of this optimization to have
      // already produced an info object with the important
      // information, but if not we set some defaults here so
      // that tests can be created and bugpoint can be run.
      if( !info ) {
        errs() << "Warning: GlobalToWide using default configuration\n";
        info = new GlobalToWideInfo();
        madeInfo = true;
        info->globalSpace = 100;
        info->wideSpace = 101;
        info->localeIdType = M.getTypeByName("struct.c_localeid_t");
        if( ! info->localeIdType ) {
          StructType* t = StructType::create(M.getContext(), "struct.c_localeid_t");
          t->setBody(Type::getInt32Ty(M.getContext()), Type::getInt32Ty(M.getContext()), NULL);
          info->localeIdType = t;
        }
        info->nodeIdType = Type::getInt32Ty(M.getContext());

        // Now go identify special functions in the module by name.
        for (Module::iterator next_func = M.begin(); next_func!= M.end(); )
        {
          Function *F = &*next_func;
          ++next_func;

          FunctionType* FT = F->getFunctionType();

          // This may look like a crazy amount of checking, but we
          // need to do it in order to have bugpoint work with this
          // optimization, since it will basically try different ways
          // of corrupting the input.
          if( F->getName().startswith(GLOBAL_FN_GLOBAL_ADDR) &&
              FT->getNumParams() == 1 &&
              FT->getReturnType()->isPointerTy() &&
              FT->getReturnType()->getPointerAddressSpace() == 0 &&
              containsGlobalPointers(info, FT->getParamType(0)) ) {
            Type* gType = FT->getParamType(0);
            GlobalPointerInfo & r = info->gTypes[gType];
            r.addrFn = F;
            info->specialFunctions.insert(F);
          } else if( F->getName().startswith(GLOBAL_FN_GLOBAL_LOCID) &&
                     FT->getNumParams() == 1 &&
                     FT->getReturnType() == info->localeIdType &&
                     containsGlobalPointers(info, FT->getParamType(0)) ) {
            Type* gType = FT->getParamType(0);
            GlobalPointerInfo & r = info->gTypes[gType];
            r.locFn = F;
            info->specialFunctions.insert(F);
          } else if( F->getName().startswith(GLOBAL_FN_GLOBAL_NODEID) &&
                     FT->getNumParams() == 1 &&
                     FT->getReturnType() == info->nodeIdType &&
                     containsGlobalPointers(info, FT->getParamType(0)) ) {
            Type* gType = FT->getParamType(0);
            GlobalPointerInfo & r = info->gTypes[gType];
            r.nodeFn = F;
            info->specialFunctions.insert(F);
          } else if( F->getName().startswith(GLOBAL_FN_GLOBAL_MAKE) &&
                     FT->getNumParams() == 2 &&
                     FT->getParamType(0) == info->localeIdType &&
                     FT->getParamType(1)->isPointerTy() &&
                     FT->getParamType(1)->getPointerAddressSpace() == 0 &&
                     containsGlobalPointers(info, FT->getReturnType()) ) {
            Type* gType = FT->getReturnType();
            GlobalPointerInfo & r = info->gTypes[gType];
            r.makeFn = F;
            info->specialFunctions.insert(F);
          } else if( F->getName().startswith(GLOBAL_FN_GLOBAL_TO_WIDE) &&
                     FT->getNumParams() == 1 &&
                     containsGlobalPointers(info, FT->getParamType(0)) ) {
            Type* gType = FT->getParamType(0);
            GlobalPointerInfo & r = info->gTypes[gType];
            r.globalToWideFn = F;
            info->specialFunctions.insert(F);
          } else if( F->getName().startswith(GLOBAL_FN_WIDE_TO_GLOBAL) &&
                     FT->getNumParams() == 1 &&
                     containsGlobalPointers(info, FT->getReturnType()) ) {
            Type* gType = FT->getReturnType();
            GlobalPointerInfo & r = info->gTypes[gType];
            r.wideToGlobalFn = F;
            info->specialFunctions.insert(F);
          }
        }
      }

      assert(info->globalSpace > 0);
      assert(info->localeIdType);
      assert(info->nodeIdType);

      // Wide pointer address space must differ from the local one...
      assert(info->globalSpace != 0);
      assert(info->wideSpace != 0);
      assert(info->localeIdType != 0);
      assert(info->nodeIdType != 0);

      GlobalTypeFixer fixer(M, info, debugPassTwo);
      GlobalTypeFixer* TypeMapper = &fixer; 

      /* This transformation operates in two major parts parts:
       *  - (as a prerequisite, source that has global address space pointers,
       *     and uses dummy functions returned by getAddrFn and friends)
       *  - first, update all function signatures and call sites and globals.
       *     This is an interprocedural pass.  Generally it operates by
       *     replacing uses of a oldThing with wideToGlobal(newThing);
       *     where newThing operates only on wide types and oldThing
       *     operated only on global types.
       *  - second, update all function bodies. Here we lower load/store/memcpy
       *    to call put/get functions and lower all instructions to operate
       *    exclusively on wide types, so that the global types are no
       *    longer used (even if they still exist in the LLVM context).
       */ 

      for (Module::iterator next_func = M.begin(); next_func!= M.end(); )
      {
        Function *F = &*next_func;
        ++next_func;

        debugPassOne = debugAllPassOne;
        if( debugThisFn[0] && F->getName() == debugThisFn ) {
          debugPassOne = true;
        }

        if( debugPassOne ) {
          errs() << "==================================== start pass one\n";
          errs() << "starting pass one with function ";
          errs().write_escaped(F->getName()) << '\n';
        }

        // skip the special functions like wideToGlobal
        if (info->specialFunctions.count(F)) {
          continue;
        }

        // skip intrinsic functions (ie don't make a wide version of memcpy)
        if (F->isIntrinsic()) {
          continue;
        }

        bool update_return = false;
        bool update_parameters = false;

        // check return type
        FunctionType *FTy = F->getFunctionType();
        if (containsGlobalPointers(info, FTy->getReturnType())) {
          update_return = true;
        }

        // check parameters
        std::vector<Type*> Params;

        unsigned i = 0;
        for (Function::arg_iterator I = F->arg_begin(), 
                E = F->arg_end(); I!=E; ++I, ++i) {
          if (containsGlobalPointers(info, I->getType())) {
            Type *new_type = convertTypeGlobalToWide(&M, info, I->getType());
            Params.push_back(new_type);
            update_parameters = true;
          } else {
            Params.push_back(I->getType());
          }
        }

        
        if( debugPassOne ) {
          // Wait until we have converted the argument types since
          // we might rename them... before duming the fn.
          F->dump();
          errs() << "-----------------------------\n";
        }

        // if we don't need to update the function's return value or at least
        // one parameter, then move on to the next one
        if (!(update_return || update_parameters)) {
          continue;
        }

        // Create the new function type based on the recomputed
        // parameters.
        Type *RetTy;
        if (update_return) {
          RetTy = convertTypeGlobalToWide(&M, info, FTy->getReturnType());
        } else {
          RetTy = FTy->getReturnType();
        }

        FunctionType *NFTy = FunctionType::get(
            RetTy,
            Params,
            FTy->isVarArg());

        if (NFTy == FTy) {
          continue;
        }

        Function *NF = Function::Create(NFTy, F->getLinkage());
        NF->copyAttributesFrom(F);

        F->getParent()->getFunctionList().insert(F, NF);
        NF->takeName(F);

        // Loop over all callers of the function, transforming the call
        // sites to change address space 100 pointers to wide pointers
        //
        SmallVector<Value*, 16> Args;

        for(Function::use_iterator UI = F->use_begin(), UE = F->use_end();
                UI!=UE; ) {
          User *Old = *UI;
          ++UI;
          CallSite CS(Old);
          if (CS.getInstruction()) {
            assert(CS.getCalledFunction() == F); 
            Instruction *Call = CS.getInstruction(); 
            const AttributeSet &CallPAL = CS.getAttributes();

            // Loop over the operands, inserting globalToWide function calls in
            // the caller as appropriate.
            unsigned ArgIndex = 1;

            for(CallSite::arg_iterator AE = CS.arg_end(), AI = CS.arg_begin();
                AI != AE; ++AI, ++ArgIndex) {

              if(!containsGlobalPointers(info, AI->get()->getType())) {
                // unmodified argument
                Args.push_back(*AI);
              } else {
                Instruction *New;
                New = fixer.callGlobalToWideFn(AI->get(), Call);
                Args.push_back(New);
              }
            }

            
            // replace_with = add a new call
            Instruction *New;

            if (InvokeInst *II = dyn_cast<InvokeInst>(Call)) {
              New = InvokeInst::Create(NF, II->getNormalDest(), II->getUnwindDest(),
                                       Args, "", Call);
              cast<InvokeInst>(New)->setCallingConv(CS.getCallingConv());
              cast<InvokeInst>(New)->setAttributes(CallPAL);
            } else {
              New = CallInst::Create(NF, Args, "", Call);
              cast<CallInst>(New)->setCallingConv(CS.getCallingConv());
              cast<CallInst>(New)->setAttributes(CallPAL);
              if (cast<CallInst>(Call)->isTailCall())
                cast<CallInst>(New)->setTailCall();
            }

            // replace_with wideToGlobal if needed on result
            if (containsGlobalPointers(info, F->getReturnType())) {
              New = fixer.callWideToGlobalFn(New, F->getReturnType(), Call);
            }

            // replace uses of the function return value with New

            // Update the alias analysis implementation to know that we are replacing
            // the old call with a new one.
            //AA.replaceWithNewValue(Call, New);

            // Update the callgraph to know that the callsite has been transformed.
            //CallGraphNode *CalleeNode = CG[Call->getParent()->getParent()];
            //CalleeNode->replaceCallEdge(Call, New, NF_CGN);

            if (!Call->use_empty()) {
              Call->replaceAllUsesWith(New);
              New->takeName(Call);
            }

            // Finally, remove the old call from the program, reducing the
            // use-count of F
            Call->eraseFromParent();

            Args.clear();
          }
        }

        // use replaceAllUsesWith() to change the constant function pointers
        // now that we have fixed up the calls
        Constant *NC = ConstantExpr::getPointerCast(NF, F->getType());
        F->replaceAllUsesWith(NC);


        // Since we have now created the new function, splice the body of the
        // old function right into the new function, leaving the old rotting
        // hulk of the function empty.

        if (F->begin() != F->end()) {
          // fix up functions that have bodies
          // also skip "special" functions like wideToGlobal
          // (since they have no body)

          NF->getBasicBlockList().splice(NF->begin(), F->getBasicBlockList());

          // Loop over the argument list, transferring uses of the old arguments
          // over to the new arguments, also transferring over the names as
          // well.
          //
          for (Function::arg_iterator I = F->arg_begin(), E = F->arg_end(),
                     I2 = NF->arg_begin(); I != E; ++I, ++I2) {

            // if this is an unmodified argument
            if (!containsGlobalPointers(info, I->getType())) {
              I->replaceAllUsesWith(I2);
              I2->takeName(I);
              // AA.replaceWithNewValue(I, I2);
              continue;
            }

            Instruction *New;
            Instruction* firstNonPHI = NF->getEntryBlock().getFirstNonPHI();
            assert(firstNonPHI);
            New = fixer.callWideToGlobalFn(I2, I->getType(), firstNonPHI);

            I->replaceAllUsesWith(New);
            I2->setName(I->getName()); // + "_wide");
            New->takeName(I);
            // AA.replaceWithNewValue(I, New);
          }

          if (containsGlobalPointers(info, F->getReturnType())) {
            for (Function::iterator BB = NF->begin(), E = NF->end(); 
                    BB != E; ++BB) {
              if (ReturnInst *RI = dyn_cast<ReturnInst>(BB->getTerminator())) {
                Instruction *New;
                New = fixer.callGlobalToWideFn(RI->getReturnValue(), RI);
                New = ReturnInst::Create(M.getContext(), New, RI);
                BB->getInstList().erase(RI);
              }
            }
          } 
    
          // DEBUG: verify function
          if( debugPassOne ) {
            errs() << "verifying new function after pass one: ";
            errs().write_escaped(NF->getName()) << '\n';
            NF->dump();
          }
          if( extraChecks ) {
            verifyFunction(*NF);
          }
        }

        F->eraseFromParent();

        if( debugPassOne ) {
          errs() << "==================================== end pass one fn\n";
        }
      }

      {
        ValueToValueMapTy VM;
        RemapFlags Flags = RF_IgnoreMissingEntries;
        // iterate through all global variables
        for (Module::global_iterator GI = M.global_begin(), GE = M.global_end();
                GI != GE; ++GI) {
          GlobalVariable *gv = &*GI;

          Type *old_type = gv->getType()->getPointerElementType();
          Type *new_type = convertTypeGlobalToWide(&M, info, old_type);
          Constant *old_init = NULL;
          Constant *new_init = NULL;

          if( gv->hasInitializer() ) {
            old_init = gv->getInitializer();
            new_init = typeMapConstant(old_init, VM, Flags, TypeMapper);
          }

          if (new_type == old_type && new_init == old_init) {
            continue; // don't do anything.
          }

          // Otherwise, we need to create a new global variable.
          GlobalVariable *new_var = new GlobalVariable(M, new_type,
              gv->isConstant(), gv->getLinkage(), new_init, "", gv,
              gv->getThreadLocalMode(), 0);  //, gv->isExternallyInitialized());

          Constant *cast_ptr = ConstantExpr::getPointerCast(new_var, gv->getType());
          gv->replaceAllUsesWith(cast_ptr);
          new_var->takeName(gv);
        }


        // iterate through all global aliases
        for (Module::alias_iterator AI = M.alias_begin(), AE = M.alias_end();
                AI != AE; ++AI) {
          GlobalAlias *ga = &*AI;

          GlobalValue *gv = const_cast<GlobalValue*>(ga->getAliasedGlobal());
          Type *old_type = ga->getType();
          Type *new_type = convertTypeGlobalToWide(&M, info, ga->getType());
          if (new_type == old_type) {
            continue; // don't do anything.
          }

          Constant *init = ConstantExpr::getPointerCast(gv, new_type);
          GlobalAlias *new_alias = new GlobalAlias(new_type, ga->getLinkage(),
              "", init, &M);

          Constant *cast_ptr = ConstantExpr::getPointerCast(new_alias, ga->getType());

          ga->replaceAllUsesWith(cast_ptr);
          new_alias->takeName(ga);
        }
      }



      // Pass #2
      ValueToValueMapTy VM;
      RemapFlags Flags = RF_IgnoreMissingEntries;
      SmallVector<Instruction*,16> Junk;

      for(Module::iterator func = M.begin(); func!= M.end(); func++)
      {
        Function *F = &*func;

        debugPassTwo = debugAllPassTwo;
        if( debugThisFn[0] && F->getName() == debugThisFn ) {
          debugPassTwo = true;
        }
        fixer.debugPassTwo = debugPassTwo;

        // Don't do anything if there is no body.
        // Does nothing for special functions since they have no body.
        if( F->begin() == F->end() ) continue;

        if( debugPassTwo ) {
          errs() << "Pass 2.1 to function ---------- ";
          errs().write_escaped(F->getName()) << '\n';
          if( debugPassTwo ) F->dump();
          errs() << "-----------------------------\n";
        }
 
        /*
         for all functions
            for all basic blocks
              for all insns
                lower everything except calls to g2w and w2g
                    (by adding calls to g2w and w2g).
                including for all phi nodes
                replace any phi node with types needing adjustment with
                t = wide-to-global(phi(global-to-wide(1),global-to-wide(2),..))
            for all basic blocks
              for all insns
                for any call %z = w2g %b or %x = g2w %a, store
                %z -> %b or %x -> %a in the mapping for RemapInstruction,
                and put that call into the junk list.
            for all basic blocks
              for all insns
                RemapInstruction, moving all global types to wide types
            Throw out the junk.
         */
        for (Function::iterator BI = F->begin(), BE = F->end(); BI != BE; ) {
          BasicBlock* BB = BI;
          ++BI;

          for (BasicBlock::iterator I = BB->begin(), E = BB->end(); I != E; ) {
            Instruction *insn = &*I;
            Instruction *prev = NULL;
            if( I != BB->begin() ) {
              --I;
              prev = &*I;
              ++I;
            }

            ++I;
      
            fixer.fixInstruction(insn);

            if( debugPassTwo ) {
              // Print out insns that we added, from prev to I.
              BasicBlock::iterator J;
              if( prev != NULL ) {
                J = BasicBlock::iterator(prev);
              } else {
                J = BB->begin();
              }
              for( ; J != I; ++J ) {
                Instruction *new_insn = &*J;
                if( new_insn != prev && new_insn != insn )
                  errs() << " new|" << *new_insn << "|" << "\n";
              }
            }
          }
        }

        if( extraChecks ) {
          verifyFunction(*F);
        }

        if( debugPassTwo ) {
          errs() << "After rewriting global ops, function is: ";
          errs().write_escaped(F->getName()) << '\n';
          F->dump();
          errs() << "-----------------------------\n";
          errs() << "Now pass 2.2 mapping w2g and g2w: \n";
        }

        for (Function::iterator BI = F->begin(), BE = F->end(); BI != BE; ) {
          BasicBlock* BB = BI;
          ++BI;
  
          //if( debugPassTwo ) {
          //  errs() << BB->getName() << ":\n";
          //}
 
          for (BasicBlock::iterator I = BB->begin(), E = BB->end(); I != E; ) {
            Instruction *insn = &*I;
            ++I;

            // Update the map to change calls to wide2global and global2wide
            // to just be their ultimate arguments.
            // Don't look for the start if it's already in the map...
            if( isa<CallInst>(insn) && ! VM.count(insn) ) {
              Value* r = fixer.getWideArg(insn);
              if( r != insn ) {
                // Since we're adding it to the map, we have
                // to already do the type mapping on it.
                r = typeMapValue(r, VM, Flags, TypeMapper);
                VM[insn] = r;
                Junk.push_back(insn);
              }
            }
          }
        }

        if( debugPassTwo ) {
          errs() << "Now pass 2.3 remapping instructions\n";
        }

        for (Function::iterator BI = F->begin(), BE = F->end(); BI != BE; ) {
          BasicBlock* BB = BI;
          ++BI;
  
          for (BasicBlock::iterator I = BB->begin(), E = BB->end(); I != E; ) {
            Instruction *insn = &*I;
            ++I;

            typeRemapInstruction(insn, VM, Flags, TypeMapper);
          }
        }

        VM.clear();

        for( unsigned i = 0; i < Junk.size(); i++ ) {
          Instruction *insn = Junk[i];
          insn->removeFromParent();
          insn->setName("");
          insn->dropAllReferences();
        }
 
        // Delete any junk we have accumulated...
        // we should have removed all references to global fn's.
        // references.
        // Now do the actual delete'ing.
        Junk.clear();

        if( debugPassTwo ) {
          errs() << "AFTER PASS 2 the function is:\n";

          for (Function::iterator BI = F->begin(), BE = F->end(); BI != BE; ) {
            BasicBlock* BB = BI;
            ++BI;
  
            if( debugPassTwo ) {
              errs() << BB->getName() << ":\n";
            }
   
            for (BasicBlock::iterator I = BB->begin(), E = BB->end(); I != E; ) {
              Instruction *insn = &*I;
              ++I;
              errs() << "    |" << *insn << "|" << "\n";
            }
          }
        }

        if( extraChecks ) {
          verifyFunction(*F);
        }
      }

      // Delete special functions, verifying that there are
      // no uses left.

      for(specialFunctions_t::iterator I = info->specialFunctions.begin(),
                                       E = info->specialFunctions.end();
          I != E; ++I ) {
        Function* F = *I;
        assert( F->use_empty() && "Special functions should've been replaced"); 
        F->eraseFromParent();
      }

      // Delete the dummy depedencies preserving function
      Constant* cf = info->preservingFn;
      if( cf ) {
        Function* f = dyn_cast<Function>(cf);
        if( f ) {
          info->preservingFn = NULL;
          f->eraseFromParent();
        }
      }

      // After it all, put the target info back.
      if( !madeInfo ) M.setDataLayout(layoutAfterwards);
      if( madeInfo ) delete info;

      if( extraChecks ) {
        verifyModule(M);
      }

      return true;
    }
  };
}

char GlobalToWide::ID = 0;
static RegisterPass<GlobalToWide> X("global-to-wide", "GlobalToWide Pass");

ModulePass *createGlobalToWide(GlobalToWideInfo* info, std::string setLayout)
{
  assert(info->addrFn);
  assert(info->locFn);
  assert(info->nodeFn);
  assert(info->makeFn);
  assert(info->getFn);
  assert(info->putFn);
  assert(info->getPutFn);
  assert(info->memsetFn);
  return new GlobalToWide(info, setLayout);
}

static
bool containsGlobalPointers(unsigned gSpace, SmallSet<Type*, 10> & set, Type* t)
{
  // All primitive types do not need to change.
  if(t->isPrimitiveType() || t->isIntegerTy()) return false;
  assert(!t->isVoidTy());

  // Pointer types return true if they are in our address space.
  if(t->isPointerTy()){
    if( t->getPointerAddressSpace() == gSpace ) return true;
  }

  // If it's something we've already enumerated,
  // it doesn't get to say yes.
  if( set.insert(t) ) {
    // We added it to the set, we can continue.
  } else {
    // It was already in the set, don't bother
    // going deeper.
    return false;
  }

  // For any type, if they have a subtype containing a global
  // pointer, return true.
  bool ret = false;
  for (Type::subtype_iterator I = t->subtype_begin(),
       E = t->subtype_end(); I != E; ++I ) {
    ret |= containsGlobalPointers(gSpace, set, *I);
  }

  return ret;
}

bool containsGlobalPointers(GlobalToWideInfo* info, Type* t)
{
  SmallSet<Type*, 10> set;
  return containsGlobalPointers(info->globalSpace, set, t);
}

// Given a global type, create the functions we will need for
// getting a node number or address. Does not create the wide type.
void populateFunctionsForGlobalType(Module *module, GlobalToWideInfo* info, Type* globalPtrTy)
{
  llvm::Type *ptrTy = NULL;
  llvm::Type* locTy = NULL;
  llvm::Type* nodeTy = NULL;

  assert(info->localeIdType);
  assert(info->nodeIdType);

  GlobalPointerInfo & r = info->gTypes[globalPtrTy];

  ptrTy = llvm::PointerType::getUnqual(globalPtrTy->getPointerElementType());
  locTy = info->localeIdType;
  nodeTy = info->nodeIdType;

  assert(ptrTy);
  assert(locTy);
  assert(nodeTy);

  // temporary functions for codegenRaddr/codegenRlocale/codegenWideAddr
  // we never actually define them, but sometimes we need to
  // generate code that uses them. So our global address space ->
  // wide pointers optimization will remove these functions.
  if( ! r.addrFn ) {
    llvm::Type* argTy[1];
    argTy[0] = globalPtrTy;
    r.addrFn = llvm::Function::Create(
                           llvm::FunctionType::get(ptrTy, argTy, false),
                           llvm::GlobalValue::ExternalLinkage,
                           GLOBAL_FN_GLOBAL_ADDR,
                           module);
    r.addrFn->setDoesNotAccessMemory();
    info->specialFunctions.insert(r.addrFn);
  }

  if( ! r.locFn ) {
    llvm::Type* argTy[1];
    argTy[0] = globalPtrTy;
    r.locFn = llvm::Function::Create(
                         llvm::FunctionType::get(locTy, argTy, false),
                         llvm::GlobalValue::ExternalLinkage,
                         GLOBAL_FN_GLOBAL_LOCID,
                         module);
    r.locFn->setDoesNotAccessMemory();
    info->specialFunctions.insert(r.locFn);
  }

  if( ! r.nodeFn ) {
    llvm::Type* argTy[1];
    argTy[0] = globalPtrTy;
    r.nodeFn = llvm::Function::Create(
                         llvm::FunctionType::get(nodeTy, argTy, false),
                         llvm::GlobalValue::ExternalLinkage,
                         GLOBAL_FN_GLOBAL_NODEID,
                         module);
    r.nodeFn->setDoesNotAccessMemory();
    info->specialFunctions.insert(r.nodeFn);
  }


  if( ! r.makeFn ) {
    llvm::Type* argTys[2];
    argTys[0] = locTy;
    argTys[1] = ptrTy;
    r.makeFn = llvm::Function::Create(
                           llvm::FunctionType::get(globalPtrTy, argTys, false),
                           llvm::GlobalValue::ExternalLinkage,
                           GLOBAL_FN_GLOBAL_MAKE,
                           module);
    r.makeFn->setDoesNotAccessMemory();
    info->specialFunctions.insert(r.makeFn);
  }
}

llvm::Function* getAddrFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy)
{
  populateFunctionsForGlobalType(module, info, globalPtrTy);
  GlobalPointerInfo & r = info->gTypes[globalPtrTy];
  return r.addrFn;
}
llvm::Function* getLocFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy)
{
  populateFunctionsForGlobalType(module, info, globalPtrTy);
  GlobalPointerInfo & r = info->gTypes[globalPtrTy];
  return r.locFn;
}
llvm::Function* getNodeFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy)
{
  populateFunctionsForGlobalType(module, info, globalPtrTy);
  GlobalPointerInfo & r = info->gTypes[globalPtrTy];
  return r.nodeFn;
}
llvm::Function* getMakeFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy)
{
  populateFunctionsForGlobalType(module, info, globalPtrTy);
  GlobalPointerInfo & r = info->gTypes[globalPtrTy];
  return r.makeFn;
}

void populateFunctionsForGlobalToWideType(Module *module, GlobalToWideInfo* info, Type* globalPtrTy)
{
  llvm::Type *widePtrTy = NULL;

  widePtrTy = convertTypeGlobalToWide(module, info, globalPtrTy);

  GlobalPointerInfo & r = info->gTypes[globalPtrTy];

  if( ! r.globalToWideFn ) {
    llvm::Type* argTy[1];
    argTy[0] = globalPtrTy;
    r.globalToWideFn = llvm::Function::Create(
                           llvm::FunctionType::get(widePtrTy, argTy, false),
                           llvm::GlobalValue::ExternalLinkage,
                           GLOBAL_FN_GLOBAL_TO_WIDE,
                           module);
    r.globalToWideFn->setDoesNotAccessMemory();
    info->specialFunctions.insert(r.globalToWideFn);
  }

  if( ! r.wideToGlobalFn ) {
    llvm::Type* argTy[1];
    argTy[0] = widePtrTy;
    r.wideToGlobalFn = llvm::Function::Create(
                           llvm::FunctionType::get(globalPtrTy, argTy, false),
                           llvm::GlobalValue::ExternalLinkage,
                           GLOBAL_FN_WIDE_TO_GLOBAL,
                           module);
    r.wideToGlobalFn->setDoesNotAccessMemory();
    info->specialFunctions.insert(r.wideToGlobalFn);
  }
}

llvm::Function* getGlobalToWideFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy)
{
  populateFunctionsForGlobalToWideType(module, info, globalPtrTy);
  GlobalPointerInfo & r = info->gTypes[globalPtrTy];
  return r.globalToWideFn;
}
llvm::Function* getWideToGlobalFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy)
{
  populateFunctionsForGlobalToWideType(module, info, globalPtrTy);
  GlobalPointerInfo & r = info->gTypes[globalPtrTy];
  return r.wideToGlobalFn;
}


Type* convertTypeGlobalToWide(Module* module, GlobalToWideInfo* info, Type* t)
{
  LLVMContext& context = module->getContext();

  assert(info->localeIdType);

  // If it doesn't contain any global pointers, it doesn't need to change.
  if( ! containsGlobalPointers(info, t) ) return t;

  // Is it already in the globalTypes map?
  {
    globalTypes_t::iterator it = info->gTypes.find(t);
    if(it != info->gTypes.end()) {
      GlobalPointerInfo g = (*it).second;
      if( g.wideTy ) return g.wideTy;
    }
  }

  // Is it a struct type?
  if (t->isStructTy()){
    StructType *st = cast<StructType>(t);
    StructType *newStruct = NULL;
    // Create a new struct...
    if (!st->isLiteral()) {
      // Rename the old structure
      std::string name;
      std::string glob_name;
      name = st->getName();
      glob_name += GLOBAL_TYPE;
      glob_name += name;
      st->setName(glob_name);
      // Create the new opaque struct to put the name in.
      newStruct = StructType::create(context, name);
      // Set the hashtable to have the opaque struct
      // (so we don't recurse forever)
      info->gTypes[st].wideTy = newStruct;
    }

    unsigned n = st->getNumElements();
    SmallVector<Type*, 16> fields(n);

    // look through fields
    for (unsigned i = 0; i < n; i++ ) {
      assert(t != st->getElementType(i)); // detect simple recursion
      fields[i] = convertTypeGlobalToWide(module, info, st->getElementType(i));
    }

    if (st->isLiteral()) {
      // literal struct types are equivalent by strucutre (vs name)
      newStruct = StructType::get(context, fields, st->isPacked());
    } else {
      // Named structure types -- set the body.
      newStruct->setBody(fields, st->isPacked());
    }

    // Great! Now we have the structure type.
    return newStruct;
  }


  if(t->isFunctionTy()){
    // look through return type and argument types
    FunctionType *fnType = cast<FunctionType>(t);
    Type* retType = fnType->getReturnType();
    Type* wideRetType = convertTypeGlobalToWide(module, info, retType);
    std::vector<Type*> wideArgTypes;
    wideArgTypes.resize(fnType->getNumParams());

    int i = 0;
    for (FunctionType::param_iterator A = fnType->param_begin(), 
          A_end = fnType->param_end(); A != A_end; ++A, i++){
      Type* param_type = *A;
      wideArgTypes[i] = convertTypeGlobalToWide(module, info, param_type);
    }

    return FunctionType::get(wideRetType, wideArgTypes, fnType->isVarArg());
  }

  if(t->isPointerTy()){
    Type* eltType = t->getPointerElementType();
    assert(t != t->getPointerElementType()); // detect simple recursion
    Type* wideEltType = convertTypeGlobalToWide(module, info, eltType);

    if( t->getPointerAddressSpace() == info->globalSpace ||
        t->getPointerAddressSpace() == info->wideSpace ) {
        // (old) Replace the pointer with a struct containing {locale, address}
        //Type* fields[2];
        //fields[0] = info->localeIdType;
        //fields[1] = PointerType::get(wideEltType, 0);
        //return StructType::get(context, fields, false);
      // Replace the pointer with a normal (packed) pointer.
      return PointerType::get(wideEltType, 0);
    } else {
      return PointerType::get(wideEltType, t->getPointerAddressSpace());
    }
  }

  if(t->isArrayTy()){
    ArrayType *arrTy = cast<ArrayType>(t);
    Type *eltType = arrTy->getElementType();
    assert(t != eltType); // detect simple recursion
    Type* wideEltType = convertTypeGlobalToWide(module, info, eltType);

    return ArrayType::get(wideEltType, arrTy->getNumElements());
  }
 
  if (t->isVectorTy()){
    VectorType *vecTy = cast<VectorType>(t);
    Type *eltType = vecTy->getElementType();
    assert(t != eltType); // detect simple recursion
    Type* wideEltType = convertTypeGlobalToWide(module, info, eltType);

    return VectorType::get(wideEltType, vecTy->getNumElements());
  }
 
  assert(0);
}


#endif

