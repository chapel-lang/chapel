//===-- X86WinEHState - Insert EH state updates for win32 exceptions ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// All functions using an MSVC EH personality use an explicitly updated state
// number stored in an exception registration stack object. The registration
// object is linked into a thread-local chain of registrations stored at fs:00.
// This pass adds the registration object and EH state updates.
//
//===----------------------------------------------------------------------===//

#include "X86.h"
#include "llvm/Analysis/LibCallSemantics.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/WinEHFuncInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/Local.h"

using namespace llvm;
using namespace llvm::PatternMatch;

#define DEBUG_TYPE "winehstate"

namespace {
class WinEHStatePass : public FunctionPass {
public:
  static char ID; // Pass identification, replacement for typeid.

  WinEHStatePass() : FunctionPass(ID) {}

  bool runOnFunction(Function &Fn) override;

  bool doInitialization(Module &M) override;

  bool doFinalization(Module &M) override;

  void getAnalysisUsage(AnalysisUsage &AU) const override;

  const char *getPassName() const override {
    return "Windows 32-bit x86 EH state insertion";
  }

private:
  void emitExceptionRegistrationRecord(Function *F);

  void linkExceptionRegistration(IRBuilder<> &Builder, Value *Handler);
  void unlinkExceptionRegistration(IRBuilder<> &Builder);
  void addCXXStateStores(Function &F, MachineModuleInfo &MMI);
  void addCXXStateStoresToFunclet(Value *ParentRegNode, WinEHFuncInfo &FuncInfo,
                                  Function &F, int BaseState);
  void insertStateNumberStore(Value *ParentRegNode, Instruction *IP, int State);

  Value *emitEHLSDA(IRBuilder<> &Builder, Function *F);

  Function *generateLSDAInEAXThunk(Function *ParentFunc);

  int escapeRegNode(Function &F);

  // Module-level type getters.
  Type *getEHLinkRegistrationType();
  Type *getSEHRegistrationType();
  Type *getCXXEHRegistrationType();

  // Per-module data.
  Module *TheModule = nullptr;
  StructType *EHLinkRegistrationTy = nullptr;
  StructType *CXXEHRegistrationTy = nullptr;
  StructType *SEHRegistrationTy = nullptr;

  // Per-function state
  EHPersonality Personality = EHPersonality::Unknown;
  Function *PersonalityFn = nullptr;

  /// The stack allocation containing all EH data, including the link in the
  /// fs:00 chain and the current state.
  AllocaInst *RegNode = nullptr;

  /// Struct type of RegNode. Used for GEPing.
  Type *RegNodeTy = nullptr;

  /// The index of the state field of RegNode.
  int StateFieldIndex = ~0U;

  /// The linked list node subobject inside of RegNode.
  Value *Link = nullptr;
};
}

FunctionPass *llvm::createX86WinEHStatePass() { return new WinEHStatePass(); }

char WinEHStatePass::ID = 0;

bool WinEHStatePass::doInitialization(Module &M) {
  TheModule = &M;
  return false;
}

bool WinEHStatePass::doFinalization(Module &M) {
  assert(TheModule == &M);
  TheModule = nullptr;
  EHLinkRegistrationTy = nullptr;
  CXXEHRegistrationTy = nullptr;
  SEHRegistrationTy = nullptr;
  return false;
}

void WinEHStatePass::getAnalysisUsage(AnalysisUsage &AU) const {
  // This pass should only insert a stack allocation, memory accesses, and
  // framerecovers.
  AU.setPreservesCFG();
}

bool WinEHStatePass::runOnFunction(Function &F) {
  // If this is an outlined handler, don't do anything. We'll do state insertion
  // for it in the parent.
  StringRef WinEHParentName =
      F.getFnAttribute("wineh-parent").getValueAsString();
  if (WinEHParentName != F.getName() && !WinEHParentName.empty())
    return false;

  // Check the personality. Do nothing if this is not an MSVC personality.
  LandingPadInst *LP = nullptr;
  for (BasicBlock &BB : F) {
    LP = BB.getLandingPadInst();
    if (LP)
      break;
  }
  if (!LP)
    return false;
  PersonalityFn =
      dyn_cast<Function>(LP->getPersonalityFn()->stripPointerCasts());
  if (!PersonalityFn)
    return false;
  Personality = classifyEHPersonality(PersonalityFn);
  if (!isMSVCEHPersonality(Personality))
    return false;

  // Disable frame pointer elimination in this function.
  // FIXME: Do the nested handlers need to keep the parent ebp in ebp, or can we
  // use an arbitrary register?
  F.addFnAttr("no-frame-pointer-elim", "true");

  emitExceptionRegistrationRecord(&F);

  auto *MMIPtr = getAnalysisIfAvailable<MachineModuleInfo>();
  assert(MMIPtr && "MachineModuleInfo should always be available");
  MachineModuleInfo &MMI = *MMIPtr;
  if (Personality == EHPersonality::MSVC_CXX) {
    addCXXStateStores(F, MMI);
  }

  // Reset per-function state.
  PersonalityFn = nullptr;
  Personality = EHPersonality::Unknown;
  return true;
}

/// Get the common EH registration subobject:
///   typedef _EXCEPTION_DISPOSITION (*PEXCEPTION_ROUTINE)(
///       _EXCEPTION_RECORD *, void *, _CONTEXT *, void *);
///   struct EHRegistrationNode {
///     EHRegistrationNode *Next;
///     PEXCEPTION_ROUTINE Handler;
///   };
Type *WinEHStatePass::getEHLinkRegistrationType() {
  if (EHLinkRegistrationTy)
    return EHLinkRegistrationTy;
  LLVMContext &Context = TheModule->getContext();
  EHLinkRegistrationTy = StructType::create(Context, "EHRegistrationNode");
  Type *FieldTys[] = {
      EHLinkRegistrationTy->getPointerTo(0), // EHRegistrationNode *Next
      Type::getInt8PtrTy(Context) // EXCEPTION_DISPOSITION (*Handler)(...)
  };
  EHLinkRegistrationTy->setBody(FieldTys, false);
  return EHLinkRegistrationTy;
}

/// The __CxxFrameHandler3 registration node:
///   struct CXXExceptionRegistration {
///     void *SavedESP;
///     EHRegistrationNode SubRecord;
///     int32_t TryLevel;
///   };
Type *WinEHStatePass::getCXXEHRegistrationType() {
  if (CXXEHRegistrationTy)
    return CXXEHRegistrationTy;
  LLVMContext &Context = TheModule->getContext();
  Type *FieldTys[] = {
      Type::getInt8PtrTy(Context), // void *SavedESP
      getEHLinkRegistrationType(), // EHRegistrationNode SubRecord
      Type::getInt32Ty(Context)    // int32_t TryLevel
  };
  CXXEHRegistrationTy =
      StructType::create(FieldTys, "CXXExceptionRegistration");
  return CXXEHRegistrationTy;
}

/// The _except_handler3/4 registration node:
///   struct EH4ExceptionRegistration {
///     void *SavedESP;
///     _EXCEPTION_POINTERS *ExceptionPointers;
///     EHRegistrationNode SubRecord;
///     int32_t EncodedScopeTable;
///     int32_t TryLevel;
///   };
Type *WinEHStatePass::getSEHRegistrationType() {
  if (SEHRegistrationTy)
    return SEHRegistrationTy;
  LLVMContext &Context = TheModule->getContext();
  Type *FieldTys[] = {
      Type::getInt8PtrTy(Context), // void *SavedESP
      Type::getInt8PtrTy(Context), // void *ExceptionPointers
      getEHLinkRegistrationType(), // EHRegistrationNode SubRecord
      Type::getInt32Ty(Context),   // int32_t EncodedScopeTable
      Type::getInt32Ty(Context)    // int32_t TryLevel
  };
  SEHRegistrationTy = StructType::create(FieldTys, "SEHExceptionRegistration");
  return SEHRegistrationTy;
}

// Emit an exception registration record. These are stack allocations with the
// common subobject of two pointers: the previous registration record (the old
// fs:00) and the personality function for the current frame. The data before
// and after that is personality function specific.
void WinEHStatePass::emitExceptionRegistrationRecord(Function *F) {
  assert(Personality == EHPersonality::MSVC_CXX ||
         Personality == EHPersonality::MSVC_X86SEH);

  StringRef PersonalityName = PersonalityFn->getName();
  IRBuilder<> Builder(&F->getEntryBlock(), F->getEntryBlock().begin());
  Type *Int8PtrType = Builder.getInt8PtrTy();
  if (Personality == EHPersonality::MSVC_CXX) {
    RegNodeTy = getCXXEHRegistrationType();
    RegNode = Builder.CreateAlloca(RegNodeTy);
    // FIXME: We can skip this in -GS- mode, when we figure that out.
    // SavedESP = llvm.stacksave()
    Value *SP = Builder.CreateCall(
        Intrinsic::getDeclaration(TheModule, Intrinsic::stacksave), {});
    Builder.CreateStore(SP, Builder.CreateStructGEP(RegNodeTy, RegNode, 0));
    // TryLevel = -1
    StateFieldIndex = 2;
    insertStateNumberStore(RegNode, Builder.GetInsertPoint(), -1);
    // Handler = __ehhandler$F
    Function *Trampoline = generateLSDAInEAXThunk(F);
    Link = Builder.CreateStructGEP(RegNodeTy, RegNode, 1);
    linkExceptionRegistration(Builder, Trampoline);
  } else if (Personality == EHPersonality::MSVC_X86SEH) {
    // If _except_handler4 is in use, some additional guard checks and prologue
    // stuff is required.
    bool UseStackGuard = (PersonalityName == "_except_handler4");
    RegNodeTy = getSEHRegistrationType();
    RegNode = Builder.CreateAlloca(RegNodeTy);
    // SavedESP = llvm.stacksave()
    Value *SP = Builder.CreateCall(
        Intrinsic::getDeclaration(TheModule, Intrinsic::stacksave), {});
    Builder.CreateStore(SP, Builder.CreateStructGEP(RegNodeTy, RegNode, 0));
    // TryLevel = -2 / -1
    StateFieldIndex = 4;
    insertStateNumberStore(RegNode, Builder.GetInsertPoint(),
                           UseStackGuard ? -2 : -1);
    // ScopeTable = llvm.x86.seh.lsda(F)
    Value *FI8 = Builder.CreateBitCast(F, Int8PtrType);
    Value *LSDA = Builder.CreateCall(
        Intrinsic::getDeclaration(TheModule, Intrinsic::x86_seh_lsda), FI8);
    Type *Int32Ty = Type::getInt32Ty(TheModule->getContext());
    LSDA = Builder.CreatePtrToInt(LSDA, Int32Ty);
    // If using _except_handler4, xor the address of the table with
    // __security_cookie.
    if (UseStackGuard) {
      Value *Cookie =
          TheModule->getOrInsertGlobal("__security_cookie", Int32Ty);
      Value *Val = Builder.CreateLoad(Int32Ty, Cookie);
      LSDA = Builder.CreateXor(LSDA, Val);
    }
    Builder.CreateStore(LSDA, Builder.CreateStructGEP(RegNodeTy, RegNode, 3));
    Link = Builder.CreateStructGEP(RegNodeTy, RegNode, 2);
    linkExceptionRegistration(Builder, PersonalityFn);
  } else {
    llvm_unreachable("unexpected personality function");
  }

  // Insert an unlink before all returns.
  for (BasicBlock &BB : *F) {
    TerminatorInst *T = BB.getTerminator();
    if (!isa<ReturnInst>(T))
      continue;
    Builder.SetInsertPoint(T);
    unlinkExceptionRegistration(Builder);
  }
}

Value *WinEHStatePass::emitEHLSDA(IRBuilder<> &Builder, Function *F) {
  Value *FI8 = Builder.CreateBitCast(F, Type::getInt8PtrTy(F->getContext()));
  return Builder.CreateCall(
      Intrinsic::getDeclaration(TheModule, Intrinsic::x86_seh_lsda), FI8);
}

/// Generate a thunk that puts the LSDA of ParentFunc in EAX and then calls
/// PersonalityFn, forwarding the parameters passed to PEXCEPTION_ROUTINE:
///   typedef _EXCEPTION_DISPOSITION (*PEXCEPTION_ROUTINE)(
///       _EXCEPTION_RECORD *, void *, _CONTEXT *, void *);
/// We essentially want this code:
///   movl $lsda, %eax
///   jmpl ___CxxFrameHandler3
Function *WinEHStatePass::generateLSDAInEAXThunk(Function *ParentFunc) {
  LLVMContext &Context = ParentFunc->getContext();
  Type *Int32Ty = Type::getInt32Ty(Context);
  Type *Int8PtrType = Type::getInt8PtrTy(Context);
  Type *ArgTys[5] = {Int8PtrType, Int8PtrType, Int8PtrType, Int8PtrType,
                     Int8PtrType};
  FunctionType *TrampolineTy =
      FunctionType::get(Int32Ty, makeArrayRef(&ArgTys[0], 4),
                        /*isVarArg=*/false);
  FunctionType *TargetFuncTy =
      FunctionType::get(Int32Ty, makeArrayRef(&ArgTys[0], 5),
                        /*isVarArg=*/false);
  Function *Trampoline = Function::Create(
      TrampolineTy, GlobalValue::InternalLinkage,
      Twine("__ehhandler$") + ParentFunc->getName(), TheModule);
  BasicBlock *EntryBB = BasicBlock::Create(Context, "entry", Trampoline);
  IRBuilder<> Builder(EntryBB);
  Value *LSDA = emitEHLSDA(Builder, ParentFunc);
  Value *CastPersonality =
      Builder.CreateBitCast(PersonalityFn, TargetFuncTy->getPointerTo());
  auto AI = Trampoline->arg_begin();
  Value *Args[5] = {LSDA, AI++, AI++, AI++, AI++};
  CallInst *Call = Builder.CreateCall(CastPersonality, Args);
  // Can't use musttail due to prototype mismatch, but we can use tail.
  Call->setTailCall(true);
  // Set inreg so we pass it in EAX.
  Call->addAttribute(1, Attribute::InReg);
  Builder.CreateRet(Call);
  return Trampoline;
}

void WinEHStatePass::linkExceptionRegistration(IRBuilder<> &Builder,
                                               Value *Handler) {
  Type *LinkTy = getEHLinkRegistrationType();
  // Handler = Handler
  Handler = Builder.CreateBitCast(Handler, Builder.getInt8PtrTy());
  Builder.CreateStore(Handler, Builder.CreateStructGEP(LinkTy, Link, 1));
  // Next = [fs:00]
  Constant *FSZero =
      Constant::getNullValue(LinkTy->getPointerTo()->getPointerTo(257));
  Value *Next = Builder.CreateLoad(FSZero);
  Builder.CreateStore(Next, Builder.CreateStructGEP(LinkTy, Link, 0));
  // [fs:00] = Link
  Builder.CreateStore(Link, FSZero);
}

void WinEHStatePass::unlinkExceptionRegistration(IRBuilder<> &Builder) {
  // Clone Link into the current BB for better address mode folding.
  if (auto *GEP = dyn_cast<GetElementPtrInst>(Link)) {
    GEP = cast<GetElementPtrInst>(GEP->clone());
    Builder.Insert(GEP);
    Link = GEP;
  }
  Type *LinkTy = getEHLinkRegistrationType();
  // [fs:00] = Link->Next
  Value *Next =
      Builder.CreateLoad(Builder.CreateStructGEP(LinkTy, Link, 0));
  Constant *FSZero =
      Constant::getNullValue(LinkTy->getPointerTo()->getPointerTo(257));
  Builder.CreateStore(Next, FSZero);
}

void WinEHStatePass::addCXXStateStores(Function &F, MachineModuleInfo &MMI) {
  WinEHFuncInfo &FuncInfo = MMI.getWinEHFuncInfo(&F);
  calculateWinCXXEHStateNumbers(&F, FuncInfo);

  // The base state for the parent is -1.
  addCXXStateStoresToFunclet(RegNode, FuncInfo, F, -1);

  // Set up RegNodeEscapeIndex
  int RegNodeEscapeIndex = escapeRegNode(F);

  // Only insert stores in catch handlers.
  Function *FrameRecover =
      Intrinsic::getDeclaration(TheModule, Intrinsic::framerecover);
  Function *FrameAddress =
      Intrinsic::getDeclaration(TheModule, Intrinsic::frameaddress);
  Constant *FI8 =
      ConstantExpr::getBitCast(&F, Type::getInt8PtrTy(TheModule->getContext()));
  for (auto P : FuncInfo.HandlerBaseState) {
    Function *Handler = const_cast<Function *>(P.first);
    int BaseState = P.second;
    IRBuilder<> Builder(&Handler->getEntryBlock(),
                        Handler->getEntryBlock().begin());
    // FIXME: Find and reuse such a call if present.
    Value *ParentFP = Builder.CreateCall(FrameAddress, {Builder.getInt32(1)});
    Value *RecoveredRegNode = Builder.CreateCall(
        FrameRecover, {FI8, ParentFP, Builder.getInt32(RegNodeEscapeIndex)});
    RecoveredRegNode =
        Builder.CreateBitCast(RecoveredRegNode, RegNodeTy->getPointerTo(0));
    addCXXStateStoresToFunclet(RecoveredRegNode, FuncInfo, *Handler, BaseState);
  }
}

/// Escape RegNode so that we can access it from child handlers. Find the call
/// to frameescape, if any, in the entry block and append RegNode to the list
/// of arguments.
int WinEHStatePass::escapeRegNode(Function &F) {
  // Find the call to frameescape and extract its arguments.
  IntrinsicInst *EscapeCall = nullptr;
  for (Instruction &I : F.getEntryBlock()) {
    IntrinsicInst *II = dyn_cast<IntrinsicInst>(&I);
    if (II && II->getIntrinsicID() == Intrinsic::frameescape) {
      EscapeCall = II;
      break;
    }
  }
  SmallVector<Value *, 8> Args;
  if (EscapeCall) {
    auto Ops = EscapeCall->arg_operands();
    Args.append(Ops.begin(), Ops.end());
  }
  Args.push_back(RegNode);

  // Replace the call (if it exists) with new one. Otherwise, insert at the end
  // of the entry block.
  IRBuilder<> Builder(&F.getEntryBlock(),
                      EscapeCall ? EscapeCall : F.getEntryBlock().end());
  Builder.CreateCall(
      Intrinsic::getDeclaration(TheModule, Intrinsic::frameescape), Args);
  if (EscapeCall)
    EscapeCall->eraseFromParent();
  return Args.size() - 1;
}

void WinEHStatePass::addCXXStateStoresToFunclet(Value *ParentRegNode,
                                                WinEHFuncInfo &FuncInfo,
                                                Function &F, int BaseState) {
  // Iterate all the instructions and emit state number stores.
  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      if (auto *CI = dyn_cast<CallInst>(&I)) {
        // Possibly throwing call instructions have no actions to take after
        // an unwind. Ensure they are in the -1 state.
        if (CI->doesNotThrow())
          continue;
        insertStateNumberStore(ParentRegNode, CI, BaseState);
      } else if (auto *II = dyn_cast<InvokeInst>(&I)) {
        // Look up the state number of the landingpad this unwinds to.
        LandingPadInst *LPI = II->getUnwindDest()->getLandingPadInst();
        // FIXME: Why does this assertion fail?
        //assert(FuncInfo.LandingPadStateMap.count(LPI) && "LP has no state!");
        int State = FuncInfo.LandingPadStateMap[LPI];
        insertStateNumberStore(ParentRegNode, II, State);
      }
    }
  }
}

void WinEHStatePass::insertStateNumberStore(Value *ParentRegNode,
                                            Instruction *IP, int State) {
  IRBuilder<> Builder(IP);
  Value *StateField =
      Builder.CreateStructGEP(RegNodeTy, ParentRegNode, StateFieldIndex);
  Builder.CreateStore(Builder.getInt32(State), StateField);
}
