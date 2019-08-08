/*
 * Obtained from https://github.com/LouisJenkinsCS/LocalAtomics/blob/master/src/LocalAtomics.chpl
 */

module LocalAtomics {
  /*
     Planned usage:
     var head : LocalAtomicObject(unmanaged Node(int));
  // Adding a new node...
  head.write(new unmanaged Node(int)); // Need 'write(objType)'
  */

  extern {
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

    struct uint128 {
      uint64_t lo;
      uint64_t hi;
    };

    typedef struct uint128 uint128_t;
    static inline int cas128bit(void *srcvp, void *cmpvp, void *withvp) {
      uint128_t __attribute__ ((aligned (16))) cmp_val = * (uint128_t *) cmpvp;
      uint128_t __attribute__ ((aligned (16))) with_val = * (uint128_t *) withvp;      
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;
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
      *(uint128_t *) cmpvp = cmp_val;
      return result;
    }

    static inline void write128bit(void *srcvp, void *valvp) {
      uint128_t __attribute__ ((aligned (16))) with_val = * (uint128_t *) valvp;
      uint128_t __attribute__ ((aligned (16))) cmp_val = * (uint128_t *) srcvp;
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;
      char successful = 0;

      while (!successful) {
        __asm__ __volatile__ ("lock; cmpxchg16b (%6);"
            "setz %7; "
            : "=a" (cmp->lo),
            "=d" (cmp->hi)
            : "0" (cmp->lo),
            "1" (cmp->hi),
            "b" (with->lo),
            "c" (with->hi),
            "r" (src),
            "m" (successful)
            : "cc", "memory");
      }
    }

    static inline void read128bit(void *srcvp, void *dstvp) {
      uint128_t __attribute__ ((aligned (16))) src_val = * (uint128_t *) srcvp;
      uint128_t __attribute__ ((aligned (16))) cmp_val = src_val;
      uint128_t __attribute__ ((aligned (16))) with_val = src_val;
      uint128_t *src = srcvp;
      uint128_t *cmp = &cmp_val;
      uint128_t *with = &with_val;
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

      *(uint128_t *)dstvp = cmp_val;
    }
  }

  extern type atomic_uint_least64_t;
  // We require this to create a wide pointer after decompression.
  extern type wide_ptr_t;
  extern type c_nodeid_t;
  extern proc chpl_return_wide_ptr_node(c_nodeid_t, c_void_ptr) : wide_ptr_t;

  /*
     The core `GlobalAtomicObject` which manages compression and decompression
     for atomic objects. It should be noted that if numLocales >= 2^17, then any
     object being operated on is added to the descriptor table, which will leak data
     if never freed with '_delete' (although if the memory is reused,
     so will the memory managed by this object). The descriptor table will be cleaned
     up automatically when this goes out of scope.
     */

  param compressedAddrMask = 0x0000FFFFFFFFFFFF;
  param compressedLocaleIdMask = 0xFFFF;
  param tableLocaleIdMask = 0xFFFFFFFF;
  param tableIdxMask = 0xFFFFFFFF;
  param compressedLocIdOffset = 48;

  inline proc uintToCVoidPtr(addr) {
    return __primitive("cast", c_void_ptr, addr);
  }

  // This is busted: $CHPL_HOME/test/optimizations/widepointers/return.future
  inline proc widePointerCheck(obj) {
    if !__primitive("is wide pointer", obj) {
      compilerError(
          "Dummy object created was not a wide pointer!",
          " Assumption: Inside a 'local' block"
          );
    }
  }

  inline proc getAddrAndLocality(obj) : (locale, uint(64)) {
    return (obj.locale, getAddr(obj));
  }

  inline proc getAddr(obj) : uint(64) {
    return __primitive("cast", uint(64), __primitive("_wide_get_addr", obj));
  }


  /*
     Compresses an object into a descriptor.
     */
  proc compress(obj) : uint {
    if obj == nil then return 0;

    // Perform a faster compression by packing the 48 usable bits of the virtual
    // address with 16 bits of the locale/node id.
    var locId : uint(64) = obj.locale.id : uint(64);
    var addr = getAddr(obj);
    return (locId << compressedLocIdOffset) | (addr & compressedAddrMask);
  }

  /*
     Decompresses a descriptor into the wide pointer object.
     */
  proc decompress(type objType, descr:uint) : objType {
    if descr == 0 then return nil;

    // If we have less than 2^16 locales, then we know we performed the
    // faster compression method so we need to decompress it in the same way...
    var locId = descr >> compressedLocIdOffset;
    var addr = descr & compressedAddrMask;
    if locId == 0 then return __primitive("cast", objType, uintToCVoidPtr(addr));

    // We've created the wide pointer, but unfortunately Chapel does not support
    // the ability to cast it to the actual object, so we have to do some
    // trickery to get it to work. What we do is we allocate a wide pointer on
    // the stack and memcpy our wideptr into the other. This is needed so we
    // have the same type.
    var wideptr = chpl_return_wide_ptr_node(locId, uintToCVoidPtr(addr));
    var newObj : objType;
    on Locales[here.id] do newObj = nil;
    c_memcpy(c_ptrTo(newObj), c_ptrTo(wideptr), 16);
    return newObj;
  }

  /*
     Wrapper for an object protected by an ABA counter. This type forwards to the object
     represented by its underlying pointer and hence can be used as if it were the object
     itself, via 'forwarding'. This type should not be created by the user, and instead
     should be created by LocalAtomicObject. The object protected by this ABA wrapper can
     be extracted via 'getObject'.
     */
  record ABA {
    type objType;
    // Runtime version of atomics so that we can read these without overhead
    var _ABA_ptr : atomic uint(64);
    var _ABA_cnt : atomic uint(64);

    proc init(type objType, ptr : uint(64), cnt : uint(64)) {
      this.objType = objType;
      this.complete();
      this._ABA_ptr.write(ptr);
      this._ABA_cnt.write(cnt);
    }

    proc init(type objType, ptr : uint(64)) {
      this.objType = objType;
      this.complete();
      this._ABA_ptr.write(ptr);
    }

    proc init(type objType) {
      this.objType = objType;
    }

    inline proc getObject() {
      var ptr = this._ABA_ptr;
      return __primitive("cast", objType, decompress(objType, this._ABA_ptr.read()));
    }

    inline proc getABACounter() {
      return this._ABA_cnt.read();
    }


    proc readWriteThis(f) {
      f <~> "ptr: " <~> getObject() <~> ", cnt: " <~> getABACounter();
    }

    forwarding getObject();
  }

  proc ==(const ref _arg1 : ABA, const ref _arg2 : ABA) {
    if (_arg1._ABA_ptr.read() == _arg2._ABA_ptr.read() && _arg1._ABA_cnt.read() == _arg2._ABA_cnt.read()) then return true;
    return false;
  }

  /*
     Provides a software solution to the problem of applying atomic operations using
     128-bit wide pointers via compression. The algorithm used for compression (and
     decompression) rely on the number locales. In majority of cases where numLocales
     is within a 16 bit range, we can encode it in the upper 16 bits of the address
     as only 2^48 bits of the virtual address space is ever used.
     */
  record LocalAtomicObject {
    type objType;
    var atomicVar : _ddata(ABA(objType));

    proc init(type objType) {
      if !isUnmanagedClass(objType) then compilerError("LocalAtomicObject must take a 'unmanaged' type, not ", objType : string);
      this.objType = objType;
      this.complete();
      var ptr : c_void_ptr;
      posix_memalign(c_ptrTo(ptr), 16, c_sizeof(ABA(objType)));
      this.atomicVar = ptr:_ddata(ABA(objType));
      c_memset(ptr, 0, c_sizeof(ABA(objType)));
    }

    proc init(type objType, defaultValue : objType) {
      if !isUnmanagedClass(objType) then compilerError("LocalAtomicObject must take a 'unmanaged' type, not ", objType : string);
      this.objType = objType;
      this.complete();
      localityCheck(defaultValue);
      var ptr : c_void_ptr;
      posix_memalign(c_ptrTo(ptr), 16, c_sizeof(ABA(objType)));
      this.atomicVar = ptr:_ddata(ABA(objType));
      c_memset(atomicVar, 0, c_sizeof(ABA(objType)));      
      this.atomicVar[0]._ABA_ptr.write(compress(defaultValue));
    }


    inline proc localityCheck(objs...) {
      if boundsChecking && (|| reduce [obj in objs] obj.locale != this.locale) then
        halt("Locality check failed on ", for obj in objs do getAddrAndLocality(obj), " when expected to be hosted on ", this.locale);
    }

    proc readABA() : ABA(objType) {
      var ret : ABA(objType);
      on this {
        var dest : ABA(objType);
        read128bit(atomicVar:c_void_ptr, c_ptrTo(dest));
        ret = dest;
      }
      return ret;
    }

    proc read() : objType {
      return __primitive("cast", objType, atomicVar[0].getObject());
    }

    proc compareExchange(expectedObj : objType, newObj : objType) : bool {
      return atomicVar[0]._ABA_ptr.compareExchange(compress(expectedObj), compress(newObj));
    }

    proc compareExchangeABA(expectedObj : ABA(objType), newObj : objType) : bool {
      var ret : bool;
      on this {
        var cmp = expectedObj;
        var val = new ABA(objType, compress(newObj), atomicVar[0].getABACounter() + 1);
        ret = cas128bit(atomicVar:c_void_ptr, c_ptrTo(cmp), c_ptrTo(val)) : bool;
      }
      return ret;
    }

    proc compareExchangeABA(expectedObj : ABA(objType), newObj : ABA(objType)) : bool {
      compareExchangeABA(expectedObj, newObj.getObject());
    }

    proc write(newObj:objType) {
      atomicVar[0]._ABA_ptr.write(compress(newObj));
    }

    proc write(newObj:ABA(objType)) {
      write(newObj.getObject());
    }

    proc writeABA(newObj: ABA(objType)) {
      write128bit(atomicVar, c_ptrTo(newObj));
    }

    proc writeABA(newObj: objType) {
      writeABA(new ABA(objType, compress(objType), atomicVar[0].getABACounter() + 1));
    }

    inline proc exchange(newObj:objType) {
      return decompress(objType, atomicVar[0]._ABA_ptr.exchange(compress(newObj)));
    }

    // handle wrong types
    inline proc write(newObj) {
      compilerError("Incompatible object type in LocalAtomicObject.write: ",
          newObj.type : string);
    }

    inline proc compareExchange(expectedObj, newObj) {
      compilerError("Incompatible object type in LocalAtomicObject.compareExchange: (",
          expectedObj.type : string, ",", newObj.type : string, ")");
    }

    inline proc exchange(newObj) {
      compilerError("Incompatible object type in LocalAtomicObject.exchange: ",
          newObj.type : string);
    }

    proc readWriteThis(f) {
      f <~> atomicVar[0];
    }
  }

  class C {
    var x : int;
  }

  proc main() {
    var x = new unmanaged C(1);
    var atomicObj = new LocalAtomicObject(unmanaged C);
    atomicObj.write(x);
    var y = atomicObj.read();
    writeln(atomicObj.read().type:string);
    writeln(y);
    var z = atomicObj.readABA();
    writeln(z.type : string);
    writeln(z);
    var w = new unmanaged C(2);
    writeln(atomicObj.compareExchange(x, z.getObject()));
    writeln(atomicObj.read());
    writeln(atomicObj.readABA());
    writeln(atomicObj.compareExchangeABA(z, w));
    writeln(atomicObj.read());
    writeln(atomicObj.readABA());
    writeln(atomicObj.compareExchange(w, x));
    writeln(atomicObj.read());
    writeln(atomicObj.readABA());
    writeln(atomicObj.exchange(nil));
    writeln(atomicObj.read());
    writeln(atomicObj.readABA());

    writeln();

    var a = atomicObj.readABA();
    var b = atomicObj.readABA();

    writeln(a==b);
    atomicObj.compareExchangeABA(a, x);
    writeln(a==b);

    var c = atomicObj.readABA();

    writeln(a==c);

  }
}
