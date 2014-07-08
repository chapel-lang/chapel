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
bool isTypeEquivalent(LLVM_TARGET_DATA * targetData, llvm::Type* a, llvm::Type* b)
{
  int64_t aN = arrayVecN(a);
  int64_t bN = arrayVecN(a);
  if( a == b ) {
    return true;
  } else if( a->isStructTy() && b->isStructTy() ) {
    llvm::StructType *aTy = llvm::dyn_cast<llvm::StructType>(a);
    llvm::StructType *bTy = llvm::dyn_cast<llvm::StructType>(b);
    if( aTy->isLayoutIdentical(bTy) ) return true;
  } else if( aN >= 0 && aN == bN &&
             arrayVecEltType(a) && arrayVecEltType(a) == arrayVecEltType(b) ) {
    return true;
  } else {
    // Are they the same size?
    if( targetData->getTypeSizeInBits(a) ==
        targetData->getTypeSizeInBits(b) ) {
      return true;
    }
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
    bool isSigned) {

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
    if( isTypeEquivalent(targetData, curType, newType) ) {
      // We turn it into a store/load to convert the type
      // since LLVM does not allow bit casts on structure types.
      llvm::Value* tmp = createTempVarLLVM(builder, newType, "");
      llvm::Type* fromType = curType->getPointerTo();
      llvm::Value* tmp2 = builder->CreatePointerCast(tmp, fromType);
      builder->CreateStore(value, tmp2);
      return builder->CreateLoad(tmp);
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

bool isTypeSizeSmallerThan(LLVM_TARGET_DATA * layout, llvm::Type* ty, uint64_t max_size_bytes)
{
  if( ! ty->isSized() ) return false; // who knows how big it is!

  uint64_t sz = layout->getTypeSizeInBits(ty);
  sz = (sz + 7)/8; // now in bytes.

  if( sz < max_size_bytes ) return true;
  return false;
}

#endif

