/*
 * Copyright 2004-2020 Cray Inc.
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

// Forward declare some LLVM things
namespace llvm {
  class Type;
  class Function;
  class ModulePass;
}

#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueHandle.h"

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
 * It would be desirable for this pass to support wide pointers larger
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

#define GLOBAL_TYPE ".gt."
#define GLOBAL_FN ".gf."
#define GLOBAL_FN_GLOBAL_ADDR ".gf.addr."
#define GLOBAL_FN_GLOBAL_LOCID ".gf.loc."
#define GLOBAL_FN_GLOBAL_NODEID ".gf.node."
#define GLOBAL_FN_GLOBAL_MAKE ".gf.make."
#define GLOBAL_FN_GLOBAL_TO_WIDE ".gf.g2w."
#define GLOBAL_FN_WIDE_TO_GLOBAL ".gf.w2g."

typedef llvm::DenseMap<llvm::Type*, GlobalPointerInfo> globalTypes_t;
typedef std::vector<llvm::WeakVH> specialFunctions_t;
typedef llvm::TrackingVH<llvm::Value> runtime_fn_t;

struct GlobalToWideInfo {
  unsigned globalSpace;
  unsigned wideSpace;

  unsigned globalPtrBits;
  // this optimization currently assumes wide pointers are
  // stored in a 128-bit struct representation that contains
  //  locale-id
  //      node
  //  addr

  llvm::Type* localeIdType;
  llvm::Type* nodeIdType;

  globalTypes_t gTypes;
  specialFunctions_t specialFunctions;

  // args:  dst local address, src nodeid, src address, num bytes, atomicness
  runtime_fn_t getFn;
  // args:  dst nodeid, dst address, local address, num bytes, atomicness
  runtime_fn_t putFn;

  // args:  dst nodeid, dst address
  //        src nodeid, src address
  //        num bytes
  runtime_fn_t getPutFn;

  // args:  dst nodeid, dst addr, c (byte), num bytes
  runtime_fn_t memsetFn;

  // Dummy function storing the runtime dependencies
  // so that they are not removed by the inliner.
  // This function should be removed from the module
  // once GlobalToWide completes.
  bool hasPreservingFn;
  runtime_fn_t preservingFn;

  GlobalToWideInfo()
    : globalSpace(0), wideSpace(0), globalPtrBits(0),
      localeIdType(NULL), nodeIdType(NULL), gTypes(), specialFunctions(),
      hasPreservingFn(false) { }
};

llvm::ModulePass *createGlobalToWide(GlobalToWideInfo* info,
                                     std::string setLayout);

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
