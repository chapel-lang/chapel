/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#include "chpl/util/break.h"
#include "llvmTracker.h"
#include "llvmUtil.h"
#include "llvmDebug.h"
#include "symbol.h"
#include "llvm/Support/Format.h"

#include <cstdio>
#include <cassert>

#ifdef HAVE_LLVM

#include "llvm/Support/Debug.h"
#include "llvm/IR/Module.h"

#if TRACK_LLVM_VALUES
#include "llvm/IR/AssemblyAnnotationWriter.h"
#include <unordered_map>
#endif

bool isArrayVecOrStruct(llvm::Type* t)
{
  return t->isArrayTy() || t->isVectorTy() || t->isStructTy();
}

llvm::AllocaInst* makeAlloca(llvm::Type* type,
                             const char* name,
                             llvm::Instruction* insertBefore,
                             unsigned n, unsigned align)
{
  // It's important to alloca at the front of the function in order
  // to avoid having an alloca in a loop which is a good way to achieve
  // stack overflow.
  llvm::Function *func = insertBefore->getParent()->getParent();
  llvm::BasicBlock* entryBlock = & func->getEntryBlock();
  const llvm::DataLayout &DL = func->getParent()->getDataLayout();

  if( insertBefore->getParent() == entryBlock ) {
    // Add before specific instruction in entry block.
  } else if(llvm::Instruction *i = func->getEntryBlock().getTerminator()) {
    // Add before terminator in entry block.
    insertBefore = i;
  } else {
    // Add at the end of entry block.
    insertBefore = NULL;
  }

  llvm::AllocaInst *tempVar;

  llvm::Value* size =
    llvm::ConstantInt::get(llvm::Type::getInt32Ty(type->getContext()), n);

  if( insertBefore ) {
    if (align != 0) {
      tempVar = new llvm::AllocaInst(type,
                                     DL.getAllocaAddrSpace(),
                                     size, llvm::Align(align),
                                     name, getInsertPosition(insertBefore));
      trackLLVMValue(tempVar);
    } else {
      tempVar = new llvm::AllocaInst(type,
                                     DL.getAllocaAddrSpace(),
                                     size, name, getInsertPosition(insertBefore));
      trackLLVMValue(tempVar);
    }
  } else {
    if (align != 0) {
      tempVar = new llvm::AllocaInst(type,
                                     DL.getAllocaAddrSpace(),
                                     size, llvm::Align(align),
                                     name, entryBlock);
      trackLLVMValue(tempVar);
    } else {
      tempVar = new llvm::AllocaInst(type,
                                     DL.getAllocaAddrSpace(),
                                     size, name, entryBlock);
      trackLLVMValue(tempVar);
    }
  }

  return tempVar;
}

llvm::AllocaInst* createAllocaInFunctionEntry(llvm::IRBuilder<>* irBuilder,
                                              llvm::Type* type,
                                              const char* name)
{
  // It's important to alloca at the front of the function in order
  // to avoid having an alloca in a loop which is a good way to achieve
  // stack overflow.
  llvm::Function *func = irBuilder->GetInsertBlock()->getParent();
  if(llvm::Instruction *i = func->getEntryBlock().getTerminator()) {
    irBuilder->SetInsertPoint(i);
  } else {
    irBuilder->SetInsertPoint(&func->getEntryBlock());
  }

  // reset the current debug location to avoid accidentally attaching it to the alloca
  llvm::DebugLoc currentDebugLocation;
  if (debugInfo) {
    currentDebugLocation = irBuilder->getCurrentDebugLocation();
    irBuilder->SetCurrentDebugLocation(llvm::DebugLoc());
  }

  llvm::AllocaInst *tempVar = irBuilder->CreateAlloca(type, nullptr, name);
  trackLLVMValue(tempVar);

  irBuilder->SetInsertPoint(&func->back());
  if (debugInfo)
    irBuilder->SetCurrentDebugLocation(currentDebugLocation);
  return tempVar;
}

// Following the C "usual arithmetic conversions" rules
// see http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf p 52
/*
http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf

First, if the corresponding real type of either operand is long double, the
other operand is converted, without change of type domain, to a type whose
corresponding real type is long double.

Otherwise, if the corresponding real type of either operand is double, the other
operand is converted, without change of type domain, to a type whose
corresponding real type is double.

Otherwise, if the corresponding real type of either operand is float, the other
operand is converted, without change of type domain, to a type whose
corresponding real type is float.

Otherwise, the integer promotions are performed on both operands. Then the
following rules are applied to the promoted operands:
If both operands have the same type, then no further conversion is needed.
Otherwise, if both operands have signed integer types or both have unsigned
integer types, the operand with the type of lesser integer conversion rank is
converted to the type of the operand with greater rank.
Otherwise, if the operand that has unsigned integer type has rank greater or
equal to the rank of the type of the other operand, then the operand with
signed integer type is converted to the type of the operand with unsigned
integer type.

Otherwise, if the type of the operand with signed integer type can represent
all of the values of the type of the operand with unsigned integer type, then
the operand with unsigned integer type is converted to the type of the
operand with signed integer type.
Otherwise, both operands are converted to the unsigned integer type
corresponding to the type of the operand with signed integer type.
*/
PromotedPair convertValuesToLarger(
    llvm::IRBuilder<> *irBuilder,
    llvm::Value *value1,
    llvm::Value *value2,
    bool isSigned1,
    bool isSigned2) {

  llvm::Type *type1 = value1->getType();
  llvm::Type *type2 = value2->getType();

  if(type1 == type2 && isSigned1 == isSigned2) {
    return PromotedPair(value1, value2, isSigned1);
  }

  //Floating point values
  if(type1->isFloatingPointTy() && type2->isFloatingPointTy()) {
    if(type1->getPrimitiveSizeInBits() > type2->getPrimitiveSizeInBits()) {
      llvm::Value* fpe2 = irBuilder->CreateFPExt(value2, type1);
      trackLLVMValue(fpe2);
      return PromotedPair(value1, fpe2, true);
    } else {
      llvm::Value* fpe1 = irBuilder->CreateFPExt(value1, type2);
      trackLLVMValue(fpe1);
      return PromotedPair(fpe1, value2, true);
    }
  }

  //Floating point / Integer values
  if(type1->isFloatingPointTy() && type2->isIntegerTy()) {
    if(isSigned2) {
      llvm::Value* stf = irBuilder->CreateSIToFP(value2, type1);
      trackLLVMValue(stf);
      return PromotedPair(value1, stf, true);
    } else {
      llvm::Value* utf = irBuilder->CreateUIToFP(value2, type1);
      trackLLVMValue(utf);
      return PromotedPair(value1, utf, true);
    }
  }

  //Integer / Floating point values
  if(type2->isFloatingPointTy() && type1->isIntegerTy()) {
    if(isSigned1) {
      llvm::Value* stf = irBuilder->CreateSIToFP(value1, type2);
      trackLLVMValue(stf);
      return PromotedPair(stf, value2, true);
    } else {
      llvm::Value* utf = irBuilder->CreateUIToFP(value1, type2);
      trackLLVMValue(utf);
      return PromotedPair(utf, value2, true);
    }
  }

  //Integer values
  if(type1->isIntegerTy() && type2->isIntegerTy()) {
    if(isSigned1 == isSigned2) {
      // both are signed or both are unsigned.
      if(type1->getPrimitiveSizeInBits() > type2->getPrimitiveSizeInBits()) {
        if(isSigned2) {
          llvm::Value* cast2 = irBuilder->CreateSExtOrBitCast(value2, type1);
          trackLLVMValue(cast2);
          return PromotedPair(value1, cast2, true);
        } else {
          llvm::Value* cast2 = irBuilder->CreateZExtOrBitCast(value2, type1);
          trackLLVMValue(cast2);
          return PromotedPair(value1, cast2, false);
        }
      } else {
        if(isSigned1) {
          llvm::Value* cast1 = irBuilder->CreateSExtOrBitCast(value1, type2);
          trackLLVMValue(cast1);
          return PromotedPair(cast1, value2, true);
        } else {
          llvm::Value* cast1 = irBuilder->CreateZExtOrBitCast(value1, type2);
          trackLLVMValue(cast1);
          return PromotedPair(cast1, value2, false);
        }
      }
    } else {
      // signed/unsigned. Does unsigned integer type have > rank?
      // if so, convert to unsigned.
      if( !isSigned1 &&
              type1->getPrimitiveSizeInBits() >=
              type2->getPrimitiveSizeInBits()) {
        // value1 is unsigned and >= bits; value2 is signed
        llvm::Value* cast2 = irBuilder->CreateSExtOrBitCast(value2, type1);
        trackLLVMValue(cast2);
        return PromotedPair(value1, cast2, false);
      } else if( !isSigned2 &&
                     type1->getPrimitiveSizeInBits() <=
                     type2->getPrimitiveSizeInBits() ) {
        // value2 is unsigned and >= bits; value1 is signed
        llvm::Value* cast1 = irBuilder->CreateSExtOrBitCast(value1, type2);
        trackLLVMValue(cast1);
        return PromotedPair(cast1, value2, false);
      } else {
        // Otherwise, if the type of the operand with signed integer
        // type can represent all of the values of the type of the operand
        // with unsigned integer type, then the operand with unsigned
        // integer type is converted to the type of the operand with
        // signed integer type.
        if( isSigned1 &&
               type1->getPrimitiveSizeInBits()-1 >=
               type2->getPrimitiveSizeInBits()) {
          // value1 is signed, value2 is not
          llvm::Value* cast2 = irBuilder->CreateZExtOrBitCast(value2, type1);
          trackLLVMValue(cast2);
          return PromotedPair(value1, cast2, true);
        } else if( isSigned2 &&
                      type1->getPrimitiveSizeInBits() <=
                      type2->getPrimitiveSizeInBits() - 1) {
          llvm::Value* cast1 = irBuilder->CreateZExtOrBitCast(value1, type2);
          trackLLVMValue(cast1);
          return PromotedPair(cast1, value2, true);
        } else {
          // otherwise, both operands are converted to the unsigned
          // integer type corresponding to the type of the operand
          // with signed integer type.
          if( isSigned1 ) {
            // convert both to unsigned type1
            llvm::Value* cast2 = irBuilder->CreateZExtOrBitCast(value2, type1);
            trackLLVMValue(cast2);
            return PromotedPair(value1, cast2, false);
          } else {
            // convert both to unsigned type2
            llvm::Value* cast1 = irBuilder->CreateZExtOrBitCast(value1, type2);
            trackLLVMValue(cast1);
            return PromotedPair(cast1, value2, false);
          }
        }
      }
    }
  }

  //Pointers
  if(type1->isPointerTy() && type2->isPointerTy()) {
    llvm::Type *castTy;

#if HAVE_LLVM_VER >= 150
    // pointers are opaque, so equivalent to always being a void pointer;
    // the below logic is moot
    castTy = type1;
#else
    llvm::Type* int8_type = llvm::Type::getInt8Ty(value1->getContext());
    bool t1isVoidStar = (type1->getPointerElementType() == int8_type);
    bool t2isVoidStar = (type2->getPointerElementType() == int8_type);

    assert(type1->getPointerAddressSpace() == type2->getPointerAddressSpace());

    // if type2 a non-void pointer type, then set castTy to type2
    // otherwise just use type1
    if ((t1isVoidStar) && (!t2isVoidStar)) {
      castTy = type2;
    } else {
      castTy = type1;
    }
#endif

    llvm::Value* cast1 = irBuilder->CreatePointerCast(value1, castTy);
    llvm::Value* cast2 = irBuilder->CreatePointerCast(value2, castTy);
    trackLLVMValue(cast1);
    trackLLVMValue(cast2);
    return PromotedPair(cast1, cast2, false);
  }

  return PromotedPair(NULL, NULL, false);
}

// Returns n elements in a vector/array or -1
static
int64_t arrayVecN(llvm::Type *t)
{
  if( t->isArrayTy() ) {
    llvm::ArrayType *at = llvm::dyn_cast<llvm::ArrayType>(t);
    unsigned n = at->getNumElements();
    return n;
  } else if( t->isVectorTy() ) {
    unsigned n;
    if (llvm::FixedVectorType *vt = llvm::dyn_cast<llvm::FixedVectorType>(t)) {
      n = vt->getNumElements();
    } else {
      // Scalable vector type not handled here
      return -1;
    }
    return n;
  } else {
    return -1;
  }
}


static
llvm::Type* arrayVecEltType(llvm::Type *t)
{
  if( t->isArrayTy() ) {
    llvm::ArrayType *at = llvm::dyn_cast<llvm::ArrayType>(t);
    return at->getElementType();
  } else if( t->isVectorTy() ) {
    llvm::VectorType *vt = llvm::dyn_cast<llvm::VectorType>(t);
    return vt->getElementType();
  } else {
    return NULL;
  }
}

static
bool isTypeEquivalent(const llvm::DataLayout& layout, llvm::Type* a, llvm::Type* b, bool force)
{
  int64_t aN = arrayVecN(a);
  int64_t bN = arrayVecN(a);
  int alignA, alignB;
  int64_t sizeA, sizeB;

  if( a == b ) {
    return true;
  } else if( a->isStructTy() && b->isStructTy() ) {
    llvm::StructType *aTy = llvm::dyn_cast<llvm::StructType>(a);
    llvm::StructType *bTy = llvm::dyn_cast<llvm::StructType>(b);
    if( aTy->isLayoutIdentical(bTy) ) return true;
    // handle case like
    // {float, float, float, float} <=> { <2xfloat>, <2xfloat> }
    // fall through...
  } else if( aN >= 0 && aN == bN &&
             arrayVecEltType(a) && arrayVecEltType(a) == arrayVecEltType(b) ) {
    return true;
  }


  alignA = layout.getPrefTypeAlign(a).value();
  alignB = layout.getPrefTypeAlign(b).value();
  sizeA = layout.getTypeStoreSize(a);
  sizeB = layout.getTypeStoreSize(b);

  // Are they the same size?
  if( sizeA == sizeB ) return true;

  if( !force ) return false;

  // Are they the same size, within alignment?
  if( sizeA < sizeB ) {
    // Try making size A bigger...
    if( sizeA + alignA >= sizeB ) return true;
  } else {
    // A >= B
    // Try making size B bigger...
    if( sizeB + alignB >= sizeA ) return true;
  }

  return false;
}

// in the event a temporary is made,
// *alloca will store the alloca instruction for it
// and this function will emit a lifetime start intrinsic.
llvm::Value *convertValueToType(llvm::IRBuilder<>* irBuilder,
                                const llvm::DataLayout& layout,
                                llvm::LLVMContext &ctx,
                                llvm::Value *value, llvm::Type *newType,
                                llvm::AllocaInst **alloca,
                                bool isSigned, bool force) {

  llvm::Type *curType = value->getType();

  if(curType == newType) {
    return value;
  }

  //Integer values
  if(newType->isIntegerTy() && curType->isIntegerTy()) {
    if(newType->getPrimitiveSizeInBits() > curType->getPrimitiveSizeInBits()) {
      // Sign extend if isSigned, but never sign extend single bits.
      if(isSigned && ! curType->isIntegerTy(1)) {
        return trackLLVMValue(irBuilder->CreateSExtOrBitCast(value, newType));
      }
      else {
        return trackLLVMValue(irBuilder->CreateZExtOrBitCast(value, newType));
      }
    }
    else {
      return trackLLVMValue(irBuilder->CreateTruncOrBitCast(value, newType));
    }
  }

  //Floating point values
  if(newType->isFloatingPointTy() && curType->isFloatingPointTy()) {
    if(newType->getPrimitiveSizeInBits() > curType->getPrimitiveSizeInBits()) {
      return trackLLVMValue(irBuilder->CreateFPExt(value, newType));
    }
    else {
      return trackLLVMValue(irBuilder->CreateFPTrunc(value, newType));
    }
  }

  //Integer value to floating point value
  if(newType->isFloatingPointTy() && curType->isIntegerTy()) {
    if(isSigned) {
      return trackLLVMValue(irBuilder->CreateSIToFP(value, newType));
    }
    else {
      return trackLLVMValue(irBuilder->CreateUIToFP(value, newType));
    }
  }

  //Floating point value to integer value
  if(newType->isIntegerTy() && curType->isFloatingPointTy()) {
    return trackLLVMValue(irBuilder->CreateFPToSI(value, newType));
  }

  //Integer to pointer
  if(newType->isPointerTy() && curType->isIntegerTy()) {
    return trackLLVMValue(irBuilder->CreateIntToPtr(value, newType));
  }

  //Pointer to integer
  if(newType->isIntegerTy() && curType->isPointerTy()) {
    return trackLLVMValue(irBuilder->CreatePtrToInt(value, newType));
  }

  //Pointers
  if(newType->isPointerTy() && curType->isPointerTy()) {

    // It's safe to convert pointers between generic (0) and non generic (non
    // 0) address spaces
    auto newTypeAddrSpace = newType->getPointerAddressSpace();
    auto curTypeAddrSpace = curType->getPointerAddressSpace();
    if(newTypeAddrSpace != 0 && curTypeAddrSpace != 0 &&
       newTypeAddrSpace != curTypeAddrSpace)
    {
      assert( 0 && "Can't convert pointer to different address space");
    }
    return trackLLVMValue(irBuilder->CreatePointerCast(value, newType));
  }

  // Structure types.
  // This is important in order to handle clang structure expansion
  // (e.g. calling a function that returns {int64,int64})
  if( isArrayVecOrStruct(curType) || isArrayVecOrStruct(newType) ) {
    if( isTypeEquivalent(layout, curType, newType, force) ) {
      // We turn it into a store/load to convert the type
      // since LLVM does not allow bit casts on structure types.
      llvm::AllocaInst* tmp_alloc;
      llvm::Type* useTy = NULL;

      if( layout.getTypeStoreSize(newType) >=
          layout.getTypeStoreSize(curType) )
        useTy = newType;
      else
        useTy = curType;

      tmp_alloc = createAllocaInFunctionEntry(irBuilder, useTy, "");
      // todo: setValueAlignment(tmp_alloc, ???, ???);
      *alloca = tmp_alloc;
      // Now cast the allocation to both fromType and toType.
      auto curPtrType = getPointerType(curType);
      auto newPtrType = getPointerType(newType);
      // Now get cast pointers
      llvm::Value* tmp_cur = irBuilder->CreatePointerCast(tmp_alloc, curPtrType);
      trackLLVMValue(tmp_cur);
      llvm::Value* tmp_new = irBuilder->CreatePointerCast(tmp_alloc, newPtrType);
      trackLLVMValue(tmp_new);
      llvm::StoreInst* store_cur = irBuilder->CreateStore(value, tmp_cur);
      trackLLVMValue(store_cur);
#if HAVE_LLVM_VER >= 150
      return trackLLVMValue(irBuilder->CreateLoad(newType, tmp_new));
#else
      return trackLLVMValue(irBuilder->CreateLoad(
                        tmp_new->getType()->getPointerElementType(), tmp_new));
#endif
    }
  }

  return NULL;
}

int64_t getTypeSizeInBytes(const llvm::DataLayout& layout, llvm::Type* ty)
{
  if( ! ty->isSized() ) return -1; // who knows how big it is!

  int64_t sz = layout.getTypeSizeInBits(ty);
  sz = (sz + 7)/8; // now in bytes.
  return sz;
}

bool isTypeSizeSmallerThan(const llvm::DataLayout& layout, llvm::Type* ty, uint64_t max_size_bytes)
{
  if( ! ty->isSized() ) return false; // who knows how big it is!

  uint64_t sz = getTypeSizeInBytes(layout, ty);

  if( sz < max_size_bytes ) return true;
  return false;
}


void print_llvm(llvm::Type* t)
{
  if (t == NULL)
    fprintf(stderr, "NULL");
  else
    t->print(llvm::dbgs(), true);

  fprintf(stderr, "\n");
}

void print_llvm(llvm::Value* v)
{
  if (v == NULL)
    fprintf(stderr, "NULL");
  else
    v->print(llvm::dbgs(), true);

  fprintf(stderr, "\n");
}

void print_llvm(llvm::Module* m)
{
  if (m == NULL)
    fprintf(stderr, "NULL");
  else
    m->print(llvm::dbgs(), NULL);

  fprintf(stderr, "\n");
}

void print_llvm(llvm::Metadata* m)
{
  if (m == NULL)
    fprintf(stderr, "NULL");
  else
    m->print(llvm::dbgs(), nullptr, true);

  fprintf(stderr, "\n");
}

static void printfLLVMHelper(const char* fmt) {
  auto fd = getLlvmPrintIrFile();
  *fd << fmt;
}

#if TRACK_LLVM_VALUES
// these are only used for TRACK_LLVM_VALUES
static void flushLLVMHelper() {
  auto fd = getLlvmPrintIrFile();
  fd->flush();
}

static void printIDLLVMHelper(int id, const char* suffix) {
  auto fd = getLlvmPrintIrFile();
  *fd << "[" << llvm::format_decimal(id, 4) << "]" << suffix;
}
#endif

// list_view() below do the same as print_llvm() above,
// except list_view() are consistent with other list_views in view.cpp:
//  - naming, for use with our debugger aliases nview, lview
//  - output to stdout
//
// nprint_view() below do the same as list_view(),
// except when TRACK_LLVM_VALUES=1 they also display LLVM IDs when available

void list_view(const llvm::Type* arg) {
  if (arg == NULL) printfLLVMHelper("<NULL>");
  else {
    auto fd = getLlvmPrintIrFile();
    arg->print(*fd, true);
  }
  printfLLVMHelper("\n");
}

void list_view(const llvm::Value* arg) {
  if (arg == NULL) printfLLVMHelper("<NULL>");
  else {
    auto fd = getLlvmPrintIrFile();
    arg->print(*fd, true);
  }
  printfLLVMHelper("\n");
}

void list_view(const llvm::Module* arg) {
  if (arg == NULL) printfLLVMHelper("<NULL>");
  else {
    auto fd = getLlvmPrintIrFile();
    arg->print(*fd, nullptr, true, true);
  }
  printfLLVMHelper("\n");
}

void list_view(const llvm::Metadata* arg) {
  if (arg == NULL) printfLLVMHelper("<NULL>");
  else {
    auto fd = getLlvmPrintIrFile();
    arg->print(*fd, nullptr, true);
  }
  printfLLVMHelper("\n");
}

////////////////////
// trackLLVMValue //
////////////////////

int breakOnLLVMID = 0;

#if TRACK_LLVM_VALUES

static std::unordered_map<const llvm::Value*, int> trackValIds;
static int nextTrackId = 1;

static void printLlvmId(const llvm::Value* val) {
  auto search = trackValIds.find(val);
  if (search != trackValIds.end())  printIDLLVMHelper(search->second, " ");
  else                              printfLLVMHelper("[    ] ");
  flushLLVMHelper();
}

// do not print anything if 'val' does not have an associated ID
static void printLlvmIdIfFound(const llvm::Value* val, const char* msg) {
  auto search = trackValIds.find(val);
  if (search == trackValIds.end()) return;
  printIDLLVMHelper(search->second, msg);
  flushLLVMHelper();
}

static int addLlvmValue(const llvm::Value* val) {
  int& insertedId = trackValIds[val];
  // Some XXX::Create() and irBuilder->CreateYYY() may return
  // a previously-created Value. Keep the existing ID for those.
  if (insertedId == 0)
    insertedId = nextTrackId++;
  return insertedId;
}

const llvm::Value* trackLLVMValue(const llvm::Value* val) {
  int newId = addLlvmValue(val);
  if (newId == breakOnLLVMID) debuggerBreakHere();
  return val;
}

llvm::Value* trackLLVMValue(llvm::Value* val) {
  trackLLVMValue((const llvm::Value*) val);
  return val;
}

void trackClonedLLVMValues(llvm::ValueToValueMapTy& VMap) {
  for (auto Entry : VMap) {
    auto it = trackValIds.find(Entry.first); // look up the clone's original
    if (it != trackValIds.end())
      trackValIds[Entry.second] = it->second; // map clone to original's ID
  }
}

class LLVMValueTracker : public llvm::AssemblyAnnotationWriter {
public:
   /// emit a string right before the start of a function
   void emitFunctionAnnot(const llvm::Function *F,
                          llvm::formatted_raw_ostream &s) {
     printLlvmId(F);
   }

   /// emit a string right after the basic block label
   void emitBasicBlockStartAnnot(const llvm::BasicBlock *BB,
                                 llvm::formatted_raw_ostream &s) {
     printLlvmIdIfFound(BB, "\n");
   }

   /// emit a string right after the basic block
   void emitBasicBlockEndAnnot(const llvm::BasicBlock *BB,
                               llvm::formatted_raw_ostream &s) {
     // nothing
   }

   /// emit a string right before an instruction is emitted
   void emitInstructionAnnot(const llvm::Instruction *I,
                             llvm::formatted_raw_ostream &s) {
     printLlvmId(I);
   }

   /// emit a comment to the right of an instruction or global value
   void printInfoComment(const llvm::Value &val,
                         llvm::formatted_raw_ostream &s) {
     // nothing
   }
};

static LLVMValueTracker llvmValueTracker;

void nprint_view(const llvm::Value* arg) {
  auto fd = getLlvmPrintIrFile();
  if (arg == NULL) printfLLVMHelper("<NULL>");
  else if (const llvm::Function* f = llvm::dyn_cast<llvm::Function>(arg)) {
    f->print(*fd, &llvmValueTracker, true, true);
  }
  else if (const llvm::BasicBlock* bb = llvm::dyn_cast<llvm::BasicBlock>(arg)) {
    bb->print(*fd, &llvmValueTracker, true, true);
  }
  else {
    printLlvmId(arg);
    // LLVM currently does not print Value* w/ AssemblyAnnotationWriter
    arg->print(*fd, true);
  }
  printfLLVMHelper("\n");
}

void nprint_view(const llvm::Type* arg) {
  if (arg == NULL) printfLLVMHelper("<NULL>");
  else {
    auto fd = getLlvmPrintIrFile();
    arg->print(*fd, true); // no tracking currently
  }
  printfLLVMHelper("\n");
}

void nprint_view(const llvm::Module* arg) {
  if (arg == NULL) printfLLVMHelper("<NULL>");
  else {
    auto fd = getLlvmPrintIrFile();
    arg->print(*fd, &llvmValueTracker, true, true);
  }
  printfLLVMHelper("\n");
}


void nprint_view(const llvm::Metadata* arg) {
  if (arg == NULL) printfLLVMHelper("<NULL>");
  else {
    auto fd = getLlvmPrintIrFile();
    arg->print(*fd, &llvmValueTracker, nullptr, true);
  }
  printfLLVMHelper("\n");
}

#else // if TRACK_LLVM_VALUES

// LLVM IDs are not tracked; nprint_view() is the same as list_view()

void nprint_view(const llvm::Type* arg)     { list_view(arg); }
void nprint_view(const llvm::Value* arg)    { list_view(arg); }
void nprint_view(const llvm::Module* arg)   { list_view(arg); }
void nprint_view(const llvm::Metadata* arg) { list_view(arg); }

#endif // if TRACK_LLVM_VALUES

llvm::AttrBuilder llvmPrepareAttrBuilder(llvm::LLVMContext& ctx) {
  llvm::AttrBuilder ret(ctx);
  return ret;
}

void llvmAddAttr(llvm::LLVMContext& ctx, llvm::AttributeList& attrs,
            size_t idx,
            llvm::AttrBuilder& b) {
  attrs = attrs.addAttributesAtIndex(ctx, idx, b);
}

void llvmAttachStructRetAttr(llvm::AttrBuilder& b, llvm::Type* returnTy) {
  b.addStructRetAttr(returnTy);

  #if HAVE_LLVM_VER >= 180
  // matches attributes added by clang with sret
  b.addAttribute(llvm::Attribute::Writable);
  b.addAttribute(llvm::Attribute::DeadOnUnwind);
  #endif
}

bool isOpaquePointer(llvm::Type* ty) {
#if HAVE_LLVM_VER >= 170
  return ty->isPointerTy();
#else
  return ty->isOpaquePointerTy();
#endif
}

llvm::Type* tryComputingPointerElementType(llvm::Value* ptr) {
  llvm::Type* eltType = nullptr;
  if (llvm::AllocaInst* locVar = llvm::dyn_cast<llvm::AllocaInst>(ptr)) {
    eltType = locVar->getAllocatedType();
  }
  if (llvm::GlobalValue* globVar = llvm::dyn_cast<llvm::GlobalValue>(ptr)) {
    eltType = globVar->getValueType();
  }

  return eltType;
}

llvm::Type* getPointerType(llvm::LLVMContext& ctx, unsigned AS) {
#if HAVE_LLVM_VER >= 180
  return llvm::PointerType::get(ctx, AS);
#else
  return llvm::Type::getInt8PtrTy(ctx, AS);
#endif
}
llvm::Type* getPointerType(llvm::IRBuilder<>* irBuilder, unsigned AS) {
#if HAVE_LLVM_VER >= 180
  return irBuilder->getPtrTy(AS);
#else
  return irBuilder->getInt8PtrTy(AS);
#endif
}
llvm::Type* getPointerType(llvm::Type* eltType, unsigned AS) {
#if LLVM_VERSION_MAJOR < 21
  return llvm::PointerType::get(eltType, AS);
#else
  return llvm::PointerType::get(eltType->getContext(), AS);
#endif
}

#endif
