/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _LLVMUTIL_H_
#define _LLVMUTIL_H_

#ifdef HAVE_LLVM

#include <utility>

#include "llvm/Config/llvm-config.h"
#include "llvm/Module.h"

#if LLVM_VERSION_MAJOR > 3 || (LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR > 1 )
#define HAVE_LLVM_VER 32
#include "llvm/IRBuilder.h"
#include "llvm/DataLayout.h"
#define LLVM_TARGET_DATA llvm::DataLayout
#define LLVM_ATTRIBUTE llvm::Attributes
static inline bool llvm_fn_param_has_attr(llvm::Function* f, unsigned idx, llvm::Attributes::AttrVal v)
{
  //return f->getAttributes().getParamAttributes(idx).hasAttribute(v);
  return f->getParamAttributes(idx).hasAttribute(v);
}
#else
#define HAVE_LLVM_VER 31
#include "llvm/Support/IRBuilder.h"
#include "llvm/Target/TargetData.h"
#define LLVM_TARGET_DATA llvm::TargetData
#define LLVM_ATTRIBUTE llvm::Attribute
static inline bool llvm_fn_param_has_attr(llvm::Function* f, unsigned idx, llvm::AttrConst v)
{
  return f->paramHasAttr(idx, v);
}

#endif
#include "llvm/Intrinsics.h"
#include "llvm/Attributes.h"

struct PromotedPair {
  llvm::Value* a;
  llvm::Value* b;
  bool isSigned;
  PromotedPair(llvm::Value* a, llvm::Value* b, bool isSigned)
    : a(a), b(b), isSigned(isSigned)
  {
  }
};

llvm::Constant* codegenSizeofLLVM(llvm::Type* type);

llvm::Value *convertValueToType(llvm::Value *value, llvm::Type *newType, bool isSigned = false);
PromotedPair convertValuesToLarger(llvm::Value *value1, llvm::Value *value2, bool isSigned1 = false, bool isSigned2 = false);

#endif //HAVE_LLVM

#endif //LLVMUTIL_H
