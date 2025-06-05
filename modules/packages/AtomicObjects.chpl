/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

/*  Support for atomic operations on pointers to 'unmanaged' classes.

  .. warning::

    This module has several platform restrictions in its current state:

      - It relies on Chapel ``extern`` code blocks and so requires that
        the Chapel compiler is built with LLVM enabled.
      - The implementation relies on using either ``GCC`` style inline assembly
        (for x86-64) or a GCC/clang builtin, and so is restricted to a
        ``CHPL_TARGET_COMPILER`` value of ``gnu``, ``clang``, or ``llvm``.
      - The implementation does not work with ``CHPL_ATOMICS=locks``.

  This module provides support for performing atomic operations on pointers
  to  ``unmanaged`` classes, which can be thought of as building blocks for
  creating non-blocking algorithms and data structures.

  .. note::

    Only ``unmanaged`` classes are supported as they are represented, internally,
    as raw 64-bit or 128-bit pointers. We do not support ``shared`` or ``owned``
    objects as they are essentially wrappers around an ``unmanaged`` object, and
    ``borrowed`` objects require a static lifetime.

  Global Atomics
  --------------

  By default, the ``AtomicObject`` can support atomic operations on potentially remote
  objects. This does add some additional overhead and can be turned off during initialization.

  .. code-block:: chpl

    var atomicVar : AtomicObject(unmanaged Obj, hasGlobalSupport=false);


  .. warning::

    Currently, ``AtomicObject`` only supports up to 65535 locales, and also works on
    the assumption that only the lowest 48 bits of the virtual address space will ever
    be used. An exascale solution that allows for an arbitrary number of compute nodes
    and for the entire 64-bit address space to be utilized is future work that is in-progress.

  .. note::

    When ``hasGlobalSupport=true`` and ``hasABASupport=false``, it will enable RDMA atomics,
    I.E when ``CHPL_NETWORK_ATOMICS!="none"``, which is provides a significant improvement
    in performance on systems where they are support, notable on a Cray-XC.

  .. warning::

    Currently, ``hasGlobalSupport=true`` is necessary when using it from multiple locales, even
    if it is intended to be used locally. This is due to there being no compiler primitive to create
    a 'wide' class, nor a way to cast a wide-pointer to create a wide class.

  ABA Wrapper
  -----------

  The 'ABA' problem occurs when a task *T1* reads the value *A* from location *L*,
  another task *T2* writes *B* to *L*, and another task *T3* writes the value *A* to *L*;
  once *T1* checks to see if *L* has changed, it will incorrectly assume that it has not.
  To make this more concrete, think of *A* and *B* both as a node in a linked list;
  *T1* reads *A*, *T2* allocates a new node *B* and writes it to *L* and deletes *A*,
  and *T3* allocates a new node which just so happens to be the same piece of memory that
  *A* had before and writes it to *L*. Atomic operations such the ``compareAndSwap``
  will succeed despite the fact that the nodes are not the same as it will perform
  the operation based on the virtual address.

  The ``ABA`` wrapper is one solution to this problem by coupling a 64-bit count alongside
  the normal 64-bit virtual address or the 48-bits of virtual address and 16-bit locality
  information. ``AtomicObject`` has its own ABA variants of its API, which can both take and return
  ``ABA`` wrappers. Examples of how they can be used can be observed below. It is safe to
  mix-and-match both ABA and non-ABA variants of the API, but only the ABA variants will advance
  the ABA counter.

  .. code-block:: chpl

    var atomicVar : AtomicObject(unmanaged Obj, hasABASupport=true);
    var obj1 = new unmanaged Obj();
    var obj2 = new unmanaged Obj();
    atomicVar.write(obj1);
    var a = atomicVar.readABA();
    var b = atomicVar.writeABA(obj2);
    atomicVar.writeABA(obj1);
    assert(atomicVar.compareAndSwap(obj1, obj2) == false, "This should always fail!");

  .. note::

    We ``forward`` all accesses to the ``ABA`` wrapper to the object it is wrapping
    so that whether or not the ABA versions of the ``AtomicObject`` API is used, it
    becomes as transparent as possible. This applies to all method and field accesses.


*/
prototype module AtomicObjects {
  use ChplConfig;
  private use IO;
  private use OS.POSIX;

  // Declaration of ``CMPXCHG16B`` primitives.
  extern {
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>

    __attribute__ ((aligned (16)))
    typedef struct uint128 {
      uint64_t lo;
      uint64_t hi;
    } uint128_t;

    // src: Address of a 16-byte aligned address or else General Protection Fault (GPF)
    // cmp: Expected value
    // with: New value to replace old
    // Returns: If successful or not
    static inline int _cas128bit(volatile uint128_t *src, uint128_t *cmp, uint128_t *with) {
      #if defined(__x86_64__)
        // We originally wrote _cas128bit expecting it to only apply on x86_64
        // but later added a version that calls into a clang/gcc builtin that
        // can handle other architectures. Maybe we should remove this branch
        // and just always use the builtin but I'm hesitant to make that change
        // without doing a performance analysis to ensure that it performs as
        // well (or better) than this branch (I fully expect that it would).
        char result;
        __asm__ __volatile__ ("lock; cmpxchg16b (%6);"
          "setz %7; "
          : "=a" (cmp->lo),
          "=d" (cmp->hi)
          : "0" (cmp->lo),
          "1" (cmp->hi),
          "b" (with->lo),
          "c" (with->hi),
          "r" (src),
          "m" (result)
          : "cc", "memory");
        return result;
      #elif defined(__has_builtin)
        #if __has_builtin (__atomic_compare_exchange)
          return __atomic_compare_exchange(src, cmp, with, 1, 1, 1);
        #else
          #error "Unable to build the AtomicObjects package module " \
                 "so that it can perform 128-bit compare-and-swap operations."
        #endif
      #else
        #error "Unable to build the AtomicObjects package module " \
               "so that it can perform 128-bit compare-and-swap operations."
      #endif
    }

    // srcvp: Address of a 16-byte aligned address or else General Protection Fault (GPF)
    // cmpvp: Expected value
    // withvp: New value to replace old
    // Returns: If successful or not
    static inline int cas128bit(void *srcvp, void *cmpvp, void *withvp) {
      uint128_t __attribute__ ((aligned (16))) cmp_val = * (uint128_t *) cmpvp;
      uint128_t __attribute__ ((aligned (16))) with_val = * (uint128_t *) withvp;
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;
      char result = _cas128bit(src, cmp, with);
      *(uint128_t *) cmpvp = cmp_val;
      return result;
    }

    static inline void write128bit(void *srcvp, void *valvp) {
      uint128_t __attribute__ ((aligned (16))) with_val = * (uint128_t *) valvp;
      uint128_t __attribute__ ((aligned (16))) cmp_val = * (uint128_t *) srcvp;
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;
      while (!_cas128bit(src, cmp, with)) ;
    }

    // Special-case which will update the ABA count of valvp to be
    // one plus the srcvp. This is needed as ABA count needs to be monotonically
    // increasing.
    static inline void write128bit_special(void *srcvp, void *valvp) {
      uint128_t __attribute__ ((aligned (16))) with_val = * (uint128_t *) valvp;
      uint128_t __attribute__ ((aligned (16))) cmp_val = * (uint128_t *) srcvp;
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;

      with->hi = cmp->hi + 1;
      while (!_cas128bit(src, cmp, with)) {
        with->hi = cmp->hi + 1;
      }
    }

    // srcvp: Address of a 16-byte aligned address or else General Protection Fault (GPF)
    // valvp: New value to replace the old
    // retvalp: Stores the old value
    static inline void exchange128bit(void *srcvp, void *valvp, void *retvalvp) {
      uint128_t __attribute__ ((aligned (16))) with_val = * (uint128_t *) valvp;
      uint128_t __attribute__ ((aligned (16))) cmp_val = * (uint128_t *) srcvp;
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;
      while (!_cas128bit(src, cmp, with)) ;
      *(uint128_t *) retvalvp = cmp_val;
    }

    // Special-case which will update the ABA count of valvp to be
    // one plus the srcvp. This is needed as ABA count needs to be monotonically
    // increasing.
    static inline void exchange128bit_special(void *srcvp, void *valvp, void *retvalvp) {
      uint128_t __attribute__ ((aligned (16))) with_val = * (uint128_t *) valvp;
      uint128_t __attribute__ ((aligned (16))) cmp_val = * (uint128_t *) srcvp;
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;

      with->hi = cmp->hi + 1;
      while (!_cas128bit(src, cmp, with)) {
        with->hi = cmp->hi + 1;
      }
      *(uint128_t *) retvalvp = cmp_val;
    }

    static inline void read128bit(void *srcvp, void *dstvp) {
      uint128_t __attribute__ ((aligned (16))) src_val = * (uint128_t *) srcvp;
      uint128_t __attribute__ ((aligned (16))) cmp_val = src_val;
      uint128_t __attribute__ ((aligned (16))) with_val = src_val;
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;
      _cas128bit(src, cmp, with);
      *(uint128_t *)dstvp = cmp_val;
    }
  }

  @chpldoc.nodoc
  extern "chpl_atomic_uint_least64_t" type atomic_uint_least64_t;

  @chpldoc.nodoc
  extern type wide_ptr_t;

  @chpldoc.nodoc
  extern type c_nodeid_t;

  @chpldoc.nodoc
  type type_c_void_ptr = c_ptr(void);

  extern proc chpl_return_wide_ptr_node(c_nodeid_t, type_c_void_ptr) : wide_ptr_t;

  if numLocales >= 2**16 {
    writeln("[WARNING]: AtomicObjects currently only supports up to 65535 locales!");
  }

  @chpldoc.nodoc
  param compressedAddrMask = 0x0000FFFFFFFFFFFF;
  @chpldoc.nodoc
  param compressedLocaleIdMask = 0xFFFF;
  @chpldoc.nodoc
  param tableLocaleIdMask = 0xFFFFFFFF;
  @chpldoc.nodoc
  param tableIdxMask = 0xFFFFFFFF;
  @chpldoc.nodoc
  param compressedLocIdOffset = 48;

  @chpldoc.nodoc
  inline proc castToObj(type objType, addr) {
    return __primitive("cast", objType?, uintToCVoidPtr(addr));
  }

  @chpldoc.nodoc
  inline proc uintToCVoidPtr(addr) {
    return __primitive("cast", c_ptr(void), addr);
  }

  @chpldoc.nodoc
  // This is busted: $CHPL_HOME/test/optimizations/widepointers/return.future
  inline proc widePointerCheck(obj) {
    if !__primitive("is wide pointer", obj) {
      compilerError(
          "Dummy object created was not a wide pointer!",
          " Assumption: Inside a 'local' block"
          );
    }
  }

  @chpldoc.nodoc
  inline proc getAddrAndLocality(obj) : (locale, uint(64)) {
    return (obj.locale, getAddr(obj));
  }

  @chpldoc.nodoc
  inline proc getAddr(obj) : uint(64) {
    return __primitive("cast", uint(64), __primitive("_wide_get_addr", obj));
  }


  @chpldoc.nodoc
  /*
     Compresses an object into a descriptor.
  */
  proc compress(obj : ?objType) : uint {
    if obj == nil then return 0;

    // Perform compression by packing the 48 usable bits of the virtual
    // address with 16 bits of the locale/node id.
    var locId : uint(64) = obj.locale.id : uint(64);
    var addr = getAddr(obj);
    var ret = (locId << compressedLocIdOffset) | (addr & compressedAddrMask);
    if boundsChecking then assert(decompress(objType, ret) == obj);
    return ret;
  }

  @chpldoc.nodoc
  /*
     Decompresses a descriptor into the wide pointer object.
  */
  proc decompress(type objType, descr:uint) : objType? {
    if descr == 0 then return nil;

    // If we have less than 2^16 locales, then we know we performed the
    // faster compression method so we need to decompress it in the same way...
    var locId = descr >> compressedLocIdOffset;
    var addr = descr & compressedAddrMask;
    if compiledForSingleLocale() || locId == here.id then return castToObj(objType, addr);

    // We've created the wide pointer, but unfortunately Chapel does not support
    // the ability to cast it to the actual object, so we have to do some
    // trickery to get it to work. What we do is we allocate a wide pointer on
    // the stack and memcpy our wideptr into the other. This is needed so we
    // have the same type.
    //
    // It would be better if we could write something like
    //    return wideptr: objType?;
    var wideptr = chpl_return_wide_ptr_node(locId, uintToCVoidPtr(addr));
    var newObj : objType?;
    // Ensure that newObj is a wide pointer
    on Locales[here.id] do newObj = nil;
    memcpy(c_addrOf(newObj), c_addrOf(wideptr), 16);
    return newObj;
  }

  /*
     Wrapper for an object protected by an ABA counter. This type forwards to the object
     represented by its underlying pointer and hence can be used as if it were the object
     itself, via 'forwarding'. This type should not be created by the user, and instead
     should be created by LocalAtomicObject. The object protected by this ABA wrapper can
     be extracted via 'getObject'.
  */
  record ABA : serializable {
    type __ABA_objType;
    @chpldoc.nodoc
    var __ABA_ptr : uint(64);
    @chpldoc.nodoc
    var __ABA_cnt : uint(64);

    @chpldoc.nodoc
    proc init(type __ABA_objType, ptr : uint(64), cnt : uint(64)) {
      this.__ABA_objType = __ABA_objType;
      this.__ABA_ptr = ptr;
      this.__ABA_cnt = cnt;
    }

    @chpldoc.nodoc
    proc init(obj : ?objType, cnt : uint(64)) {
      this.__ABA_objType = objType;
      this.__ABA_ptr = compress(obj);
      this.__ABA_cnt = cnt;
    }

    proc init(type __ABA_objType) {
      this.__ABA_objType = __ABA_objType;
    }

    proc init=(other : ABA(?objType)) {
      this.__ABA_objType = objType;
      this.__ABA_ptr = other.__ABA_ptr;
      this.__ABA_cnt = other.__ABA_cnt;
    }

    inline proc getObject() : __ABA_objType? {
      return decompress(__ABA_objType, __ABA_ptr);
    }

    inline proc getABACount() {
      return __ABA_cnt;
    }

    @chpldoc.nodoc
    proc deserialize(reader, ref deserializer) throws {
      compilerWarning("Reading an ABA is not supported");
    }

    @chpldoc.nodoc
    proc init(type __ABA_objType, reader: fileReader, ref deserializer) {
      this.init(__ABA_objType);
      compilerWarning("Deserializing an ABA is not yet supported");
    }

    /* Writes an ABA */
    proc serialize(writer, ref serializer) throws {
      writer.write("(ABA){cnt=", this.__ABA_cnt, ", obj=", this.getObject(), "}");
    }

    forwarding this.getObject()!;
  }
  operator ABA.=(ref lhs: ABA, const ref rhs: lhs.type) {
    lhs.__ABA_ptr = rhs.__ABA_ptr;
    lhs.__ABA_cnt = rhs.__ABA_cnt;
  }

  @chpldoc.nodoc
  record _ABAInternal {
    type objType;
    @chpldoc.nodoc
    var _ABA_ptr : atomic uint(64);
    @chpldoc.nodoc
    var _ABA_cnt : atomic uint(64);

    proc init(type objType, ptr : uint(64), cnt : uint(64)) {
      this.objType = objType;
      init this;
      this._ABA_ptr.write(ptr);
      this._ABA_cnt.write(cnt);
    }

    proc init(type objType, ptr : uint(64)) {
      this.objType = objType;
      init this;
      this._ABA_ptr.write(ptr);
    }

    proc init(type objType) {
      this.objType = objType;
    }

    proc init=(other: _ABAInternal) {
      this.objType = other.objType;
      this._ABA_ptr = other._ABA_ptr.read();
      this._ABA_cnt = other._ABA_cnt.read();
    }
  }

  /*
    Special case operator that compares two ``ABA`` wrappers.
  */
  operator ABA.==(const ref aba1 : ABA, const ref aba2 : ABA) {
    return aba1.__ABA_cnt == aba2.__ABA_cnt && aba1.__ABA_ptr == aba2.__ABA_ptr;
  }

  operator ABA.!=(const ref aba1 : ABA, const ref aba2 : ABA) {
    return aba1.__ABA_cnt != aba2.__ABA_cnt || aba1.__ABA_ptr != aba2.__ABA_ptr;
  }

  record AtomicObject : serializable {
    type objType;
    // If this atomic instance provides ABA support
    param hasABASupport : bool;
    // If this atomic instance provides global atomics
    param hasGlobalSupport : bool;
    var atomicVar : if hasABASupport then _ddata(_ABAInternal(objType?)) else atomic uint(64);

    proc init(type objType, param hasABASupport = false, param hasGlobalSupport = !compiledForSingleLocale()) {
      if !isUnmanagedClass(objType) {
        compilerError ("LocalAtomicObject must take a 'unmanaged' type, not ", objType : string);
      }
      this.objType = objType;
      this.hasABASupport = hasABASupport;
      this.hasGlobalSupport = hasGlobalSupport;
      init this;
      if hasABASupport {
        var ptr : c_ptr(void);
        var retval = posix_memalign(c_addrOf(ptr), 16, c_sizeof(ABA(objType?)));
        if retval then halt();
        this.atomicVar = ptr:_ddata(_ABAInternal(objType?));
        memset(ptr, 0, c_sizeof(ABA(objType?)));
      }
    }

    proc init(type objType, defaultValue : objType, param hasABASupport = false, param hasGlobalSupport = !compiledForSingleLocale()) {
      init(objType, hasABASupport, hasGlobalSupport);
      var ptr : uint(64);
      if hasGlobalSupport {
        ptr = compress(defaultValue);
      } else {
        ptr = getAddr(defaultValue);
      }

      if hasABASupport {
        atomicVar[0]._ABA_ptr.write(ptr);
      } else {
        atomicVar.write(ptr);
      }
    }

    proc init=(other: AtomicObject) {
      this.objType = other.objType;
      this.hasABASupport = other.hasABASupport;
      this.hasGlobalSupport = other.hasGlobalSupport;
      if hasABASupport
        then this.atomicVar = other.atomicVar;
        else this.atomicVar = other.atomicVar.read();
    }

    @chpldoc.nodoc
    inline proc ref atomicVariable ref {
      if hasABASupport {
        return atomicVar[0]._ABA_ptr;
      } else {
        return atomicVar;
      }
    }

    // Object(objType) -> Pointer(uint(64))
    @chpldoc.nodoc
    inline proc toPointer(obj:objType?) : uint(64) {
      if hasGlobalSupport {
        return compress(obj);
      } else {
        // Check if an object is non-local when 'hasGlobalSupport' is false
        // Note: Both `compiledForSingleLocale()` and `boundsChecking` are
        // compile-time constants and will compile this away.
        if !compiledForSingleLocale() && boundsChecking {
          localityCheck(obj);
        }
        return getAddr(obj);
      }
    }

    // Pointer(uint(64)) -> Object(objType)
    @chpldoc.nodoc
    inline proc fromPointer(ptr : uint(64)) : objType? {
      if hasGlobalSupport {
        return decompress(objType, ptr);
      } else {
        return castToObj(objType, ptr);
      }
    }

    @chpldoc.nodoc
    inline proc localityCheck(objs...) {
      if boundsChecking && (|| reduce [obj in objs] obj.locale != this.locale) then
        halt("Locality check failed on ", for obj in objs do getAddrAndLocality(obj), " when expected to be hosted on ", this.locale);
    }

    // Called from ABA API, which ensures that the ABA API is only called on an
    // AtomicObject which supports data
    @chpldoc.nodoc
    inline proc doABACheck() param {
      if !hasABASupport {
        compilerError("Attempt to use ABA API from AtomicObject(hasABASupport=", hasABASupport, ", hasGlobalSupport=", hasGlobalSupport, ")");
      }
    }

    proc readABA() : ABA(objType?) {
      doABACheck();
      var ret : ABA(objType?);
      on this {
        var dest : ABA(objType?);
        read128bit(atomicVar:c_ptr(void), c_addrOf(dest));
        ret = dest;
      }
      return ret;
    }

    proc ref read() : objType? {
      return fromPointer(atomicVariable.read());
    }

    proc ref compareAndSwap(expectedObj : objType?, newObj : objType?) : bool {
      return atomicVariable.compareAndSwap(toPointer(expectedObj), toPointer(newObj));
    }

    proc compareAndSwapABA(expectedObj : ABA(objType?), newObj : objType?) : bool {
      doABACheck();
      var ret : bool;
      on this {
        var cmp = expectedObj;
        // Note that no 'cas128bit_special' is needed here as the 'cas128bit' will detect
        // a change from the expectedObj passed, which of course includes the _ABA_cnt.
        var val = new ABA(objType?, toPointer(newObj), atomicVar[0]._ABA_cnt.read() + 1);
        ret = cas128bit(atomicVar:c_ptr(void), c_addrOf(cmp), c_addrOf(val)) : bool;
      }
      return ret;
    }

    proc compareAndSwapABA(expectedObj : ABA(objType?), newObj : ABA(objType?)) : bool {
      compareAndSwapABA(expectedObj, newObj.getObject());
    }

    proc ref write(newObj:objType?) {
      atomicVariable.write(toPointer(newObj));
    }

    proc write(newObj:ABA(objType?)) {
      write(newObj.getObject());
    }

    proc writeABA(newObj: ABA(objType?)) {
      doABACheck();
      write128bit(atomicVar:c_ptr(void), c_addrOf(newObj));
    }

    proc writeABA(newObj: objType?) {
      doABACheck();
      // Note: We do not invoke `write128bit` here as we _must_ ensure that _ABA_cnt
      // is one plus the previous, otherwise we inject a race condition where a task
      // gets preempted after reading the _ABA_cnt but _before_ it finishes the
      // `write128bit` operation; this race condition makes it possible for an older
      // _ABA_cnt to be written back, which by itself opens the possibility for other
      // ABA race conditions that we're trying to solve. 'write128bit_special' solves this
      // by setting the 'with' upper 64-bits equal to one plus the actual 'cmp' upper 64-bits.
      write128bit_special(new ABA(objType?, toPointer(newObj), 0));
    }

    inline proc ref exchange(newObj:objType?) : objType? {
      return fromPointer(atomicVariable.exchange(toPointer(newObj)));
    }

    inline proc exchangeABA(newObj : objType?) : ABA(objType?) {
      doABACheck();
      var ret : ABA(objType?);
      on this {
        var retval : ABA(objType?);
        var _newObj = newObj;
        var val = new ABA(objType?, toPointer(newObj), 0);
        exchange128bit_special(atomicVar:c_ptr(void), c_addrOf(_newObj), c_addrOf(retval));
        ret = retval;
      }

      return ret;
    }

    inline proc exchangeABA(newObj: ABA(objType?)) : ABA(objType?) {
      doABACheck();
      var ret : ABA(objType?);
      on this {
        var retval : ABA(objType?);
        var _newObj = newObj;
        var val = newObj;
        exchange128bit(atomicVar:c_ptr(void), c_addrOf(_newObj), c_addrOf(retval));
        ret = retval;
      }

      return ret;
    }

    @chpldoc.nodoc
    proc deserialize(reader, ref deserializer) throws {
      compilerWarning("Reading an AtomicObject is not supported");
    }

    @chpldoc.nodoc
    proc init(type objType,
              param hasABASupport: bool,
              param hasGlobalSupport: bool,
              reader: fileReader,
              ref deserializer) {
      init(objType, hasABASupport, hasGlobalSupport);
      compilerWarning("Deserializing an AtomicObject is not yet supported");
    }

    @chpldoc.nodoc
    proc serialize(writer, ref serializer) throws {
      writer.write(atomicVariable.read());
    }
  }
}
