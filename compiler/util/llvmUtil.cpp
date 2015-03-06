/*
 * Copyright 2004-2015 Cray Inc.
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

#include "llvmUtil.h"

#include <cstdio>
#include <cassert>

#ifdef HAVE_LLVM

static
bool isArrayVecOrStruct(llvm::Type* t)
{
  return t->isArrayTy() || t->isVectorTy() || t->isStructTy();
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
    llvm::VectorType *vt = llvm::dyn_cast<llvm::VectorType>(t);
    unsigned n = vt->getNumElements();
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

llvm::Constant* codegenSizeofLLVM(llvm::Type* type)
{
  return llvm::ConstantExpr::getSizeOf(type);
}

static
bool isTypeEquivalent(LLVM_TARGET_DATA * targetData, llvm::Type* a, llvm::Type* b, bool force)
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


  alignA = targetData->getPrefTypeAlignment(a);
  alignB = targetData->getPrefTypeAlignment(b);
  sizeA = targetData->getTypeStoreSize(a);
  sizeB = targetData->getTypeStoreSize(b);

  // Are they the same size?
  if( sizeA == sizeB ) return true;

  if( !force ) return false;

  // Are they the same size, within alignment?
  if( sizeA < sizeB ) {
    // Try making size A bigger...
    if( sizeA + alignA > sizeB ) return true;
  } else {
    // A >= B
    // Try making size B bigger...
    if( sizeB + alignB > sizeA ) return true;
  }

  return false;
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
    tempVar = new llvm::AllocaInst(type, size, align, name, insertBefore);
  } else {
    tempVar = new llvm::AllocaInst(type, size, align, name, entryBlock);
  }

  return tempVar;
}

llvm::Value* createTempVarLLVM(llvm::IRBuilder<>* builder, llvm::Type* type, const char* name)
{
  // It's important to alloca at the front of the function in order
  // to avoid having an alloca in a loop which is a good way to achieve
  // stack overflow.
  llvm::Function *func = builder->GetInsertBlock()->getParent();
  if(llvm::Instruction *i = func->getEntryBlock().getTerminator()) {
    builder->SetInsertPoint(i);
  } else {
    builder->SetInsertPoint(&func->getEntryBlock());
  }

  llvm::AllocaInst *tempVar = builder->CreateAlloca(type, 0, name);
  builder->SetInsertPoint(&func->back());
  return tempVar;
}

llvm::Value *convertValueToType(
    llvm::IRBuilder<> *builder,
    LLVM_TARGET_DATA * targetData,
    llvm::Value *value,
    llvm::Type *newType,
    bool isSigned,
    bool force) {

  llvm::Type *curType = value->getType();
  
  if(curType == newType) {
    return value;
  }
  
  //Integer values
  if(newType->isIntegerTy() && curType->isIntegerTy()) {
    if(newType->getPrimitiveSizeInBits() > curType->getPrimitiveSizeInBits()) {
      // Sign extend if isSigned, but never sign extend single bits.
      if(isSigned && ! curType->isIntegerTy(1)) {
        return builder->CreateSExtOrBitCast(value, newType);
      }
      else {
        return builder->CreateZExtOrBitCast(value, newType);
      }
    }
    else {
      return builder->CreateTruncOrBitCast(value, newType);
    }
  }
  
  //Floating point values
  if(newType->isFloatingPointTy() && curType->isFloatingPointTy()) {
    if(newType->getPrimitiveSizeInBits() > curType->getPrimitiveSizeInBits()) {
      return builder->CreateFPExt(value, newType);
    }
    else {
      return builder->CreateFPTrunc(value, newType);
    }
  }

  //Integer value to floating point value
  if(newType->isFloatingPointTy() && curType->isIntegerTy()) {
    if(isSigned) {
      return builder->CreateSIToFP(value, newType);
    }
    else {
      return builder->CreateUIToFP(value, newType);
    }
  }

  //Floating point value to integer value
  if(newType->isIntegerTy() && curType->isFloatingPointTy()) {
    return builder->CreateFPToSI(value, newType);
  }
  
  //Integer to pointer
  if(newType->isPointerTy() && curType->isIntegerTy()) {
    return builder->CreateIntToPtr(value, newType);
  }

  //Pointers
  if(newType->isPointerTy() && curType->isPointerTy()) {
    if( newType->getPointerAddressSpace() !=
        curType->getPointerAddressSpace() ) {
      assert( 0 && "Can't convert pointer to different address space");
    }
    return builder->CreatePointerCast(value, newType);
  }

  // Structure types. 
  // This is important in order to handle clang structure expansion
  // (e.g. calling a function that returns {int64,int64})
  if( isArrayVecOrStruct(curType) || isArrayVecOrStruct(newType) ) {
    if( isTypeEquivalent(targetData, curType, newType, force) ) {
      // We turn it into a store/load to convert the type
      // since LLVM does not allow bit casts on structure types.
      llvm::Value* tmp_alloc;
      if( targetData->getTypeStoreSize(newType) >=
          targetData->getTypeStoreSize(curType) )
        tmp_alloc = createTempVarLLVM(builder, newType, "");
      else {
        tmp_alloc = createTempVarLLVM(builder, curType, "");
      }
      // Now cast the allocation to both fromType and toType.
      llvm::Type* curPtrType = curType->getPointerTo();
      llvm::Type* newPtrType = newType->getPointerTo();
      // Now get cast pointers 
      llvm::Value* tmp_cur = builder->CreatePointerCast(tmp_alloc, curPtrType);
      llvm::Value* tmp_new = builder->CreatePointerCast(tmp_alloc, newPtrType);
      builder->CreateStore(value, tmp_cur);
      return builder->CreateLoad(tmp_new);
    }
  }

  return NULL;
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
    llvm::IRBuilder<> *builder,
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
      return PromotedPair(value1,
                          builder->CreateFPExt(value2, type1), true);
    } else {
      return PromotedPair(builder->CreateFPTrunc(value1, type2),
                          value2, true);
    }
  }

  //Floating point / Integer values
  if(type1->isFloatingPointTy() && type2->isIntegerTy()) {
    if(isSigned2) {
      return PromotedPair(value1,
                          builder->CreateSIToFP(value2, type1), true);
    } else {
      return PromotedPair(value1,
                          builder->CreateUIToFP(value2, type1), true);
    }
  }

  //Integer / Floating point values
  if(type2->isFloatingPointTy() && type1->isIntegerTy()) {
    if(isSigned1) {
      return PromotedPair(builder->CreateSIToFP(value1, type2),
                          value2, true);
    } else {
      return PromotedPair(builder->CreateUIToFP(value1, type2),
                          value2, true);
    }
  }

  //Integer values
  if(type1->isIntegerTy() && type2->isIntegerTy()) {
    if(isSigned1 == isSigned2) {
      // both are signed or both are unsigned.
      if(type1->getPrimitiveSizeInBits() > type2->getPrimitiveSizeInBits()) {
        if(isSigned2) {
          return PromotedPair(value1,
                        builder->CreateSExtOrBitCast(value2, type1),
                        true);
        } else {
          return PromotedPair(
                        value1,
                        builder->CreateZExtOrBitCast(value2, type1),
                        false);
        }
      } else {
        if(isSigned1) {
          return PromotedPair(
                        builder->CreateSExtOrBitCast(value1, type2),
                        value2, true);
        } else {
          return PromotedPair(
                        builder->CreateZExtOrBitCast(value1, type2),
                        value2, false);
        }
      }
    } else {
      // signed/unsigned. Does unsigned integer type have > rank?
      // if so, convert to unsigned.
      if( !isSigned1 &&
              type1->getPrimitiveSizeInBits() >=
              type2->getPrimitiveSizeInBits()) {
        // value1 is unsigned and >= bits; value2 is signed
        return PromotedPair(value1,
                            builder->CreateSExtOrBitCast(value2, type1),
                            false);
      } else if( !isSigned2 &&
                     type1->getPrimitiveSizeInBits() <=
                     type2->getPrimitiveSizeInBits() ) {
        // value2 is unsigned and >= bits; value1 is signed
        return PromotedPair(builder->CreateSExtOrBitCast(value1, type2),
                            value2, false);
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
          return PromotedPair(value1,
                        builder->CreateZExtOrBitCast(value2, type1),
                        true);
        } else if( isSigned2 &&
                      type1->getPrimitiveSizeInBits() <=
                      type2->getPrimitiveSizeInBits() - 1) {
          return PromotedPair(
                        builder->CreateZExtOrBitCast(value1, type2),
                        value2, true);
        } else {
          // otherwise, both operands are converted to the unsigned
          // integer type corresponding to the type of the operand
          // with signed integer type.
          if( isSigned1 ) {
            // convert both to unsigned type1
            return PromotedPair(value1,
                          builder->CreateZExtOrBitCast(value2, type1),
                          false);
          } else {
            // convert both to unsigned type2
            return PromotedPair(
                          builder->CreateZExtOrBitCast(value1, type2),
                          value2, false);
          }
        }
      }
    }
  }
  
  //Pointers
  if(type1->isPointerTy() && type2->isPointerTy()) {
    llvm::Type *castTy;
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

    return PromotedPair(builder->CreatePointerCast(value1, castTy),
                        builder->CreatePointerCast(value2, castTy),
                        false);
  }
  
  return PromotedPair(NULL, NULL, false);
}

int64_t getTypeSizeInBytes(LLVM_TARGET_DATA * layout, llvm::Type* ty)
{
  if( ! ty->isSized() ) return -1; // who knows how big it is!

  int64_t sz = layout->getTypeSizeInBits(ty);
  sz = (sz + 7)/8; // now in bytes.
  return sz;
}

bool isTypeSizeSmallerThan(LLVM_TARGET_DATA * layout, llvm::Type* ty, uint64_t max_size_bytes)
{
  if( ! ty->isSized() ) return false; // who knows how big it is!

  uint64_t sz = getTypeSizeInBytes(layout, ty);

  if( sz < max_size_bytes ) return true;
  return false;
}


/* This function gets the next byte offset to a structure in memory with
   type ty that represents a different value field. Padding is skipped.
   offset is the offset we are starting with
   parent_this_offset is the start offset of ty according to the parent type
   parent_next is the end offset of ty according to the parent type
     (so that it includes padding)
     */
static
uint64_t doGetTypeFieldNext(LLVM_TARGET_DATA * layout, llvm::Type* ty, uint64_t offset, uint64_t parent_this_offset, uint64_t parent_next_offset)
{
  llvm::SequentialType* stype = NULL;
  llvm::StructType* struct_type = NULL;
  const llvm::StructLayout* struct_layout = NULL;
  llvm::Type* eltType = NULL;
  unsigned i, n;
  uint64_t local_offset, next_offset_here, offset_here;

  //ty->dump();
  //printf("offset %i parent %i,%i\n", (int) offset, (int) parent_this_offset, (int) parent_next_offset);

  assert(parent_this_offset <= offset && offset <= parent_next_offset);

  if( ! isArrayVecOrStruct(ty) ) {
    return parent_next_offset;
  }

  local_offset = offset - parent_this_offset;

  if( ty->isArrayTy() || ty->isVectorTy() ) {
    stype = llvm::cast<llvm::SequentialType>(ty);
    eltType = stype->getElementType();
    // Not using getTypeSizeInBytes so that:
    // 1) we get an assertion error if the type is not sized
    // 2) we use uint64s for the type instead of int64s
    uint64_t sz = layout->getTypeSizeInBits(eltType);
    sz = (sz + 7)/8; // now in bytes.
    uint64_t this_offset = local_offset / sz;
    this_offset = parent_this_offset + this_offset*sz;
 
    // All types the same.. so we can pretend we're working with the first one.
    return doGetTypeFieldNext(layout, eltType,
                              offset, this_offset, this_offset+sz);
  }

  // structure type.
  struct_type = llvm::cast<llvm::StructType>(ty);
  n = struct_type->getNumElements();
  struct_layout = layout->getStructLayout(struct_type);

  // Scroll forward in the structure until we find the last element
  // starting at offset.
  for( i=0; i<n; i++ ) {
    next_offset_here = struct_layout->getElementOffset(i);
    //printf("struct offset(%i) is %i\n", (int) i, (int) next_offset_here);
    if( next_offset_here > local_offset ) break;
  }

  if( i != n ) {
    // it's before the last element, so we can use the next element
    // as the next start position.
    parent_next_offset = parent_this_offset + next_offset_here;
  }

  i--;
  // now i is the element n-1 or
  // the last element with element offset <= offset

  offset_here = struct_layout->getElementOffset(i);

  parent_this_offset = parent_this_offset + offset_here;
  //printf("offset_here is %i\n", (int) offset_here);

  eltType = struct_type->getElementType(i);

  return doGetTypeFieldNext(layout, eltType, offset, parent_this_offset, parent_next_offset);
}


uint64_t getTypeFieldNext(LLVM_TARGET_DATA * layout, llvm::Type* ty, uint64_t offset)
{
  uint64_t sz;

  // Not using getTypeSizeInBytes so that:
  // 1) we get an assertion error if the type is not sized
  // 2) we use uint64s for the type instead of int64s
  assert(ty->isSized());
  sz = layout->getTypeSizeInBits(ty);
  sz = (sz + 7)/8; // now in bytes.


  return doGetTypeFieldNext(layout, ty, offset, 0, sz);
}

#endif

