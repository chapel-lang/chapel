#ifndef _LLVMGLOBALTOWIDE_H_
#define _LLVMGLOBALTOWIDE_H_

#ifdef HAVE_LLVM

//#include "llvmUtil.h"
#include "llvm/Config/llvm-config.h"

#if LLVM_VERSION_MAJOR > 3 || (LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR > 2 )
#include "llvm/IR/Module.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Attributes.h"
#else
#include "llvm/Module.h"
#include "llvm/Intrinsics.h"
#include "llvm/Attributes.h"
#endif


#include "llvm/PassManager.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallPtrSet.h"


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
 * pointers themselves, this pass needs  addrFnName, getFnName, etc
 * to be passed in. These functions should be available externally (ie later
 * linked with the program). It would be nice to inline them but so far
 * that has proven difficult since they appear unused when the inliner
 * runs (since it runs before this pass) and so are removed by the inliner.
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

struct GlobalToWideInfo {
  unsigned globalSpace;
  unsigned wideSpace;
  llvm::Type* localeIdType;
  llvm::Type* nodeIdType;
  globalTypes_t gTypes;
  specialFunctions_t specialFunctions;

  // args:  packed wide ptr. Returns the address portion.
  const char* addrFnName;
  // args:  packed wide ptr, &locale. sets locale = wide.locale.
  // It has the complicated signature in order to keep
  //  arguments passed by pointer (vs structure) to avoid
  //  some issues with passing/returning structures.
  const char* locFnName;
  // args:  packed wide ptr. Returns the node number portion.
  const char* nodeFnName;
  // args:  const locale*, address. Returns a packed wide pointer (void*).
  const char* makeFnName;

  // args:  dst local address, wide address {locale,i8*}, num bytes, atomicness
  const char* getFnName;
  // args:  dst wide address {locale,i8*}, local address, num bytes, atomicness
  const char* putFnName;

  // args:  dst wide address {locale,i8*};
  //        src wide address {locale,i8*},
  //        num bytes
  const char* getPutFnName;

  // args:  dst wide address {locale,i8*}, c (byte), num bytes
  const char* memsetFnName;

  GlobalToWideInfo()
    : globalSpace(0), wideSpace(0), localeIdType(NULL), nodeIdType(NULL), gTypes(), specialFunctions(), addrFnName(NULL), locFnName(NULL), nodeFnName(NULL), makeFnName(NULL), getFnName(NULL), putFnName(NULL), getPutFnName(NULL), memsetFnName(NULL) {
  }
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
