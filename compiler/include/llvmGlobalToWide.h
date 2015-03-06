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

#ifndef _LLVMGLOBALTOWIDE_H_
#define _LLVMGLOBALTOWIDE_H_

#ifdef HAVE_LLVM

#include "llvmUtil.h"

#include "llvm/PassManager.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallPtrSet.h"

#if HAVE_LLVM_VER >= 35
#include "llvm/IR/ValueHandle.h"
#else
#include "llvm/Support/ValueHandle.h"
#endif

/* The LLVM Global to Wide transformation allows the Chapel code generator
 * to emit multi-locale global pointer code that can be optimized by existing
 * LLVM passes. However, this pass is written in a way that would allow it
 * to be used in another context, provided that similar runtime support
 * functions are available.
 *
 * A number of 'dummy' functions are declared which will be replaced by
 * something else when the pass runs. They can be used to go between a
 * wide/packed pointer and a global (usually address space 100) pointer. They
 * also allow for the extraction of address, locale, and node from a wide
 * pointer, or for the creation of a wide pointer. These 'dummy' functions
 * are emitted by the Chapel code generator instead of direct calls to
 * runtime functions that operate on wide pointers. These dummy functions
 * are stored in the GlobalPointerInfo structure and can be retrieved or
 * created during code generation with getAddrFn and friends.
 *
 * In order to lower global pointer operations into get/put communication
 * calls, and to lower the dummy functions operating on the global/wide
 * pointers themselves, this pass needs  addrFn, getFn, etc
 * to be passed in. These functions should be available when the pass is
 * run (ie, not removed by an inliner if they are inline). The pass keeps
 * track of them with a TrackingVH so we will have them if they are replaced.
 * But we are out of luck if they are deleted. One way to prevent them from
 * being deleted, if they are marked inline and internal, is to create
 * a external-linkage function that uses the address of them. If you do
 * that, and set preservingFn to it, GlobalToWide will delete that function
 * when it finishes its work.
 *
 * It would be desireable for this pass to support wide pointers larger
 * than 64 bits. However, in order to prevent data type layouts from
 * changing (in case e.g. offsets have been lowered to constants), it is
 * necessary to keep the global pointer size the same as the ultimate wide
 * pointer size. As of LLVM 3.3, LLVM optimizations do not work correctly
 * with pointers > 64 bits. Once that is fixed, this optimization should be
 * able to work with 128-bit pointers (probably requiring minor modification
 * since the current version uses bitcasts on pointers since it currently
 * only works with packed pointers).
 */
struct GlobalPointerInfo {
  // What LLVM type do we have for the wide pointer
  // (ie struct containing locale, address -- or packed pointer)
  llvm::Type* wideTy;

  // The LLVM function (never actually implemented)
  // that returns the address portion of a global pointer
  // (ie the .addr field if it were a wide pointer).
  //   global ptr  -> global ptr.addr
  llvm::Function* addrFn;

  // The LLVM function (never actually implemented)
  // that reads a locale into a variable
  // (ie the .locale field if it were a wide pointer).
  //   global ptr -> locale
  llvm::Function* locFn;

  // The LLVM function (never actually implemented)
  // that returns node storing a global pointer
  // (ie the .locale.node field if it were a wide pointer).
  //   global ptr  -> global ptr.locale.node
  llvm::Function* nodeFn;

  // The LLVM function (never actually implemented)
  // that constructs a wide pointer from a locale
  // and an address.
  //   locale, address -> global ptr
  llvm::Function* makeFn;

  // Used by the optimization temporarily
  llvm::Function* globalToWideFn;
  llvm::Function* wideToGlobalFn;

  GlobalPointerInfo() : wideTy(NULL),
                        addrFn(NULL), locFn(NULL), nodeFn(NULL), makeFn(NULL),
                        globalToWideFn(NULL), wideToGlobalFn(NULL) { }
};

#define GLOBAL_PTR_BITS 64
#define GLOBAL_TYPE ".gt."
#define GLOBAL_FN ".gf."
#define GLOBAL_FN_GLOBAL_ADDR ".gf.addr."
#define GLOBAL_FN_GLOBAL_LOCID ".gf.loc."
#define GLOBAL_FN_GLOBAL_NODEID ".gf.node."
#define GLOBAL_FN_GLOBAL_MAKE ".gf.make."
#define GLOBAL_FN_GLOBAL_TO_WIDE ".gf.g2w."
#define GLOBAL_FN_WIDE_TO_GLOBAL ".gf.w2g."

typedef llvm::DenseMap<llvm::Type*, GlobalPointerInfo> globalTypes_t;
typedef llvm::SmallPtrSet<llvm::Function*, 32> specialFunctions_t;
typedef llvm::TrackingVH<llvm::Constant> runtime_fn_t;

struct GlobalToWideInfo {
  unsigned globalSpace;
  unsigned wideSpace;
  llvm::Type* localeIdType;
  llvm::Type* nodeIdType;
  globalTypes_t gTypes;
  specialFunctions_t specialFunctions;

  // args:  packed wide ptr. Returns the address portion.
  runtime_fn_t addrFn;
  // args:  packed wide ptr, &locale. sets locale = wide.locale.
  // It has the complicated signature in order to keep
  //  arguments passed by pointer (vs structure) to avoid
  //  some issues with passing/returning structures.
  runtime_fn_t locFn;
  // args:  packed wide ptr. Returns the node number portion.
  runtime_fn_t nodeFn;
  // args:  const locale*, address. Returns a packed wide pointer (void*).
  runtime_fn_t makeFn;

  // args:  dst local address, wide address {locale,i8*}, num bytes, atomicness
  runtime_fn_t getFn;
  // args:  dst wide address {locale,i8*}, local address, num bytes, atomicness
  runtime_fn_t putFn;

  // args:  dst wide address {locale,i8*};
  //        src wide address {locale,i8*},
  //        num bytes
  runtime_fn_t getPutFn;

  // args:  dst wide address {locale,i8*}, c (byte), num bytes
  runtime_fn_t memsetFn;

  // Dummy function storing the runtime dependencies
  // so that they are not removed by the inliner.
  // This function should be removed from the module
  // once GlobalToWide completes.
  runtime_fn_t preservingFn;

  GlobalToWideInfo()
    : globalSpace(0), wideSpace(0), localeIdType(NULL), nodeIdType(NULL), gTypes(), specialFunctions() { }
};

llvm::ModulePass *createGlobalToWide(GlobalToWideInfo* info, std::string setLayout);

llvm::Type* convertTypeGlobalToWide(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* t);

bool containsGlobalPointers(GlobalToWideInfo* info, llvm::Type* t);

void populateFunctionsForGlobalType(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy);
void populateFunctionsForGlobalToWideType(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy);

llvm::Function* getAddrFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy);
llvm::Function* getLocFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy);
llvm::Function* getNodeFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy);
llvm::Function* getMakeFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy);
llvm::Function* getGlobalToWideFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy);
llvm::Function* getWideToGlobalFn(llvm::Module *module, GlobalToWideInfo* info, llvm::Type* globalPtrTy);

#endif

#endif
