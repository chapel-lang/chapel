#ifndef _LLVMUTIL_H_
#define _LLVMUTIL_H_

#ifdef HAVE_LLVM

#include <utility>

#include "llvm/Config/llvm-config.h"

#if LLVM_VERSION_MAJOR > 3 || (LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR > 1 )
#if LLVM_VERSION_MAJOR > 3 || (LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR > 2 )
#define HAVE_LLVM_VER 33
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DataLayout.h"
#define LLVM_TARGET_DATA llvm::DataLayout
#define LLVM_ATTRIBUTE llvm::Attribute
static inline bool llvm_fn_param_has_attr(llvm::Function* f, unsigned idx, llvm::Attribute::AttrKind v)
{
  return f->getAttributes().hasAttribute(idx, v);
}
#else
#define HAVE_LLVM_VER 32
#include "llvm/Module.h"
#include "llvm/Value.h"
#include "llvm/IRBuilder.h"
#include "llvm/DataLayout.h"
#include "llvm/Intrinsics.h"
#include "llvm/Attributes.h"
#define LLVM_TARGET_DATA llvm::DataLayout
#define LLVM_ATTRIBUTE llvm::Attributes
static inline bool llvm_fn_param_has_attr(llvm::Function* f, unsigned idx, llvm::Attributes::AttrVal v)
{
  //return f->getAttributes().getParamAttributes(idx).hasAttribute(v);
  return f->getParamAttributes(idx).hasAttribute(v);
}
#endif

#else
#define HAVE_LLVM_VER 31
#include "llvm/Module.h"
#include "llvm/Value.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Intrinsics.h"
#include "llvm/Attributes.h"
#define LLVM_TARGET_DATA llvm::TargetData
#define LLVM_ATTRIBUTE llvm::Attribute
static inline bool llvm_fn_param_has_attr(llvm::Function* f, unsigned idx, llvm::AttrConst v)
{
  return f->paramHasAttr(idx, v);
}

#endif

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
