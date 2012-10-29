#ifndef LLVMUTIL_H
#define LLVMUTIL_H

#ifdef HAVE_LLVM

#include <utility>

#include "llvm/Config/llvm-config.h"
#include "llvm/Module.h"
#if LLVM_VERSION_MAJOR > 3 || (LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR > 1 )
#include "llvm/IRBuilder.h"
#else
#include "llvm/Support/IRBuilder.h"
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

llvm::Value* codegenSizeofLLVM(llvm::Type* type);

llvm::Value *convertValueToType(llvm::Value *value, llvm::Type *newType, bool isSigned = false);
PromotedPair convertValuesToLarger(llvm::Value *value1, llvm::Value *value2, bool isSigned1 = false, bool isSigned2 = false);

#endif //HAVE_LLVM

#endif //LLVMUTIL_H
