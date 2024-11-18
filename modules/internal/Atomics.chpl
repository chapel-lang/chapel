/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

/*
   See runtime/include/atomics/README for more information about atomics.

   Note that when compiling with --cache-remote, the compiler
   will add fences to methods in atomic types with order arguments e.g.::

     proc sub (... param order:memoryOrder = memoryOrder.seqCst):void {
       on this do atomic_fetch_sub_explicit_...(_v, val, c_memory_order(order));
     }

   becomes::

     proc sub (... param order:memoryOrder = memoryOrder.seqCst):void {
       chpl_rmem_consist_maybe_release(order);
       on this do atomic_fetch_sub_explicit_...(_v, val, c_memory_order(order));
       chpl_rmem_consist_maybe_acquire(order);
     }

   In addition, when --cache-remote is activated, the normally required
   memory fence for an 'on' statement is omitted for these functions
   (since the maybe_release/maybe_acquire fence takes care of it).

   These methods are detected based on the type of the order argument,
   but to avoid errors, the compiler will give an error if the
   order argument is not explicitly specified (because the relevant
   pass runs before function resolution).

   waitFor methods also do not need the fences written here -
   because they will be added if the cache is enabled - but they
   do need a thread fence after the loop of memoryOrder.relaxed
   transactions in order to correctly run in a comm=none compilation
   where the 'on' statement is omitted.
*/

/*
   A note about const-ness: these atomic types are records but
   in order for record assignment to work, the read() functions
   need to be able to work with a const RHS.

   To enable that, the read/waitFor functions take in `this`
   with const intent. That is reasonable even if the atomic is
   implemented with a lock because the programmer can view it
   as constant, and on good hardware it really will be. If we change
   our mind about this, we will need to revisit the default `=`
   generated for records containing atomic fields.
*/

// Note -- the next docs comment is intended to be included
// directly in the appropriate language spec section.

/*

For example, the following code declares an atomic variable ``x`` that
stores an ``int``:

.. code-block:: chapel

      var x: atomic int;

Such an atomic variable that is declared without an initialization expression
will store the default value of the contained type (i.e. ``0`` or ``false``).

Atomic variables can also be declared with an initial value:

.. code-block:: chapel

      var y: atomic int = 1;

Similarly, a temporary ``atomic`` value can be created by casting to atomic:

.. code-block:: chapel

      var one: int = 1;
      ... one : atomic int... // creates an `atomic int` initialized with 1

Assignment is supported between atomic variables as well:

.. code-block:: chapel

      var x: atomic int = 1;
      var y: atomic int = 2;

      x = y; // equivalent to x.write(y.read())

Chapel currently supports atomic operations for bools, all supported sizes of
signed and unsigned integers, as well as all supported sizes of reals.  Note
that not all operations are supported for all atomic types. The supported types
are listed for each operation.

  *Rationale*.

  The choice of supported atomic variable types as well as the atomic
  operations was strongly influenced by the C11 standard.

Most atomic methods accept an optional argument named ``order`` of type
``memoryOrder``. The ``order`` argument is used to specify the ordering
constraints of atomic operations. The supported memoryOrder values are:

 * memoryOrder.relaxed
 * memoryOrder.acquire
 * memoryOrder.release
 * memoryOrder.acqRel
 * memoryOrder.seqCst


See also :ref:`Chapter-Memory_Consistency_Model` and in particular
:ref:`non_sc_atomics` for more information on the meaning of these memory
orders.

Unless specified, the default for the memoryOrder parameter is
memoryOrder.seqCst.

   *Implementors’ note*.

   Not all architectures or implementations may support all memoryOrder
   values. In these cases, the implementation should default to a more
   conservative ordering than specified.

*/
pragma "atomic module"
module Atomics {

  use ChapelBase;
  public use MemConsistency;  // OK: to get and propagate memoryOrder
  import AutoMath;

  pragma "local fn" pragma "fast-on safe extern function"
  extern proc chpl_atomic_thread_fence(order:memory_order);

  // non user-facing fence that is called by the compiler
  @chpldoc.nodoc
  proc atomic_fence(order:memory_order = memory_order_seq_cst) {
    chpl_atomic_thread_fence(order);
    if CHPL_CACHE_REMOTE then
      chpl_rmem_consist_fence(order);
  }

  /* An atomic fence that establishes an ordering of non-atomic and relaxed
     atomic operations. */
  inline proc atomicFence(param order: memoryOrder = memoryOrder.seqCst) {
    atomic_fence(c_memory_order(order));
  }

  private proc isSupported(type valType) param {
    return valType == bool || isInt(valType) || isUint(valType) || isReal(valType);
  }

  // Compute the C/Runtime type from the Chapel type
  // TODO support extern type renaming?
  private proc externT(type valType) type {
    extern "chpl_atomic_bool" type atomic_bool;

    extern "chpl_atomic_int_least8_t" type atomic_int_least8_t;
    extern "chpl_atomic_int_least16_t" type atomic_int_least16_t;
    extern "chpl_atomic_int_least32_t" type atomic_int_least32_t;
    extern "chpl_atomic_int_least64_t" type atomic_int_least64_t;

    extern "chpl_atomic_uint_least8_t" type atomic_uint_least8_t;
    extern "chpl_atomic_uint_least16_t" type atomic_uint_least16_t;
    extern "chpl_atomic_uint_least32_t" type atomic_uint_least32_t;
    extern "chpl_atomic_uint_least64_t" type atomic_uint_least64_t;

    extern "chpl_atomic__real64" type atomic__real64;
    extern "chpl_atomic__real32" type atomic__real32;

    select valType {
      when bool     do return atomic_bool;

      when int(8)   do return atomic_int_least8_t;
      when int(16)  do return atomic_int_least16_t;
      when int(32)  do return atomic_int_least32_t;
      when int(64)  do return atomic_int_least64_t;

      when uint(8)  do return atomic_uint_least8_t;
      when uint(16) do return atomic_uint_least16_t;
      when uint(32) do return atomic_uint_least32_t;
      when uint(64) do return atomic_uint_least64_t;

      when real(32) do return atomic__real32;
      when real(64) do return atomic__real64;
    }
  }

  private proc externTString(type valType) param {
    if valType == bool then return "bool";
    if isInt(valType)  then return "int_least"  + numBits(valType):string + "_t";
    if isUint(valType) then return "uint_least" + numBits(valType):string + "_t";
    if isReal(valType) then return "_real"      + numBits(valType):string;
  }

  private proc externFunc(param s: string, type valType, param explicit=true) param {
    if explicit then return "atomic_" + s + "_explicit_" + externTString(valType);
                else return "atomic_" + s + "_"          + externTString(valType);
  }

  proc chpl__processorAtomicType(type valType) type {
    if valType == bool           then return AtomicBool;
    else if isSupported(valType) then return AtomicT(valType);
    else compilerError("Unsupported atomic type: " + valType:string);
  }

  // Parser hook
  proc chpl__atomicType(type valType) type {
    use ChplConfig;
    if CHPL_NETWORK_ATOMICS == "none" {
      return chpl__processorAtomicType(valType);
    } else {
      return chpl__networkAtomicType(valType);
    }
  }

  extern proc chpl_comm_ensure_progress(): void;

  pragma "atomic type"
  pragma "ignore noinit"
  record AtomicBool : writeSerializable {
    // Support `valType` on atomic bool type and instances for symmetry with
    // numeric atomics
    @chpldoc.nodoc
    proc type valType type { return bool; }
    @chpldoc.nodoc
    proc valType type { return bool; }

    @chpldoc.nodoc
    var _v:externT(bool);

    @chpldoc.nodoc
    proc ref init_helper(val:bool) {
      pragma "fn synchronization free"
      pragma "local fn" pragma "fast-on safe extern function"
      pragma "no gpu codegen"
      extern externFunc("init", bool, explicit=false)
        proc atomic_init(ref obj:externT(bool), val:bool): void;

      atomic_init(_v, val);
    }

    @chpldoc.nodoc
    proc init() {
      init this;
      const default: bool;
      init_helper(default);
    }

    @chpldoc.nodoc
    proc init=(other:AtomicBool) {
      init this;
      init_helper(other.read());
    }

    @chpldoc.nodoc
    proc init=(other:bool) {
      init this;
      init_helper(other);
    }

    @chpldoc.nodoc
    proc ref deinit() {
      pragma "fn synchronization free"
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("destroy", bool, explicit=false)
        proc atomic_destroy(ref obj:externT(bool)): void;

      on this do atomic_destroy(_v);
    }

    /*
       Returns the stored value.
    */
    inline proc const read(param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("load", bool)
        proc atomic_load(const ref obj:externT(bool), order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_load(_v, c_memory_order(order));
      return ret;
    }

    /*
       Stores `val` as the new value.
    */
    inline proc ref write(val:bool, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("store", bool)
        proc atomic_store(ref obj:externT(bool), val:bool, order:memory_order): void;

      on this do atomic_store(_v, val, c_memory_order(order));
    }

    /*
       Stores `val` as the new value and returns the original value.
    */
    inline proc ref exchange(val:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("exchange", bool)
        proc atomic_exchange(ref obj:externT(bool), val:bool, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_exchange(_v, val, c_memory_order(order));
      return ret;
    }

    /*
       Stores `desired` as the new value, if and only if the original value is
       equal to `expected`. Returns `true` if `desired` was stored, otherwise
       updates `expected` to the original value.
     */
    inline proc ref compareExchange(ref expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order, readableOrder(order));
    }
    inline proc ref compareExchange(ref expected:bool, desired:bool, param success: memoryOrder, param failure: memoryOrder): bool {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("compare_exchange_strong", bool)
        proc atomic_compare_exchange_strong(ref obj:externT(bool), ref expected:bool, desired:bool, succ:memory_order, fail:memory_order): bool;

      var ret:bool;
      on this {
        var localizedExpected = expected;
        ret = atomic_compare_exchange_strong(_v, localizedExpected, desired, c_memory_order(success), c_memory_order(failure));
        if !ret then expected = localizedExpected;
      }
      return ret;
    }

    /*
       Similar to :proc:`compareExchange`, except that this function may
       return `false` even if the original value was equal to `expected`. This
       may happen if the value could not be updated atomically.

       This weak version is allowed to spuriously fail, but when
       compareExchange is already in a loop, it can offer better
       performance on some platforms.
    */
    inline proc ref compareExchangeWeak(ref expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchangeWeak(expected, desired, order, readableOrder(order));
    }
    inline proc ref compareExchangeWeak(ref expected:bool, desired:bool, param success: memoryOrder, param failure: memoryOrder) {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("compare_exchange_weak", bool)
        proc atomic_compare_exchange_weak(ref obj:externT(bool), ref expected:bool, desired:bool, succ:memory_order, fail:memory_order): bool;

      var ret:bool;
      on this {
        var localizedExpected = expected;
        ret = atomic_compare_exchange_weak(_v, localizedExpected, desired, c_memory_order(success), c_memory_order(failure));
        if !ret then expected = localizedExpected;
      }
      return ret;
    }

    /*
       Stores `desired` as the new value, if and only if the original value is
       equal to `expected`. Returns `true` if `desired` was stored.
    */
    @unstable("'compareAndSwap' is unstable")
    inline proc ref compareAndSwap(expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("compare_exchange_strong", bool)
        proc atomic_compare_exchange_strong(ref obj:externT(bool), ref expected:bool, desired:bool, succ:memory_order, fail:memory_order): bool;

      var ret:bool;
      on this {
        var mutableExpected = expected;
        ret = atomic_compare_exchange_strong(_v, mutableExpected, desired, c_memory_order(order), c_memory_order(readableOrder(order)));
      }
      return ret;
    }

    /*
       Stores `true` as the new value and returns the old value.
    */
    inline proc ref testAndSet(param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.exchange(true, order);
    }

    /*
       Stores `false` as the new value.
    */
    inline proc ref clear(param order: memoryOrder = memoryOrder.seqCst): void {
      this.write(false, order);
    }

    /*
       Waits until the stored value is equal to `val`. The implementation may
       yield the running task while waiting.
    */
    inline proc const waitFor(val:bool, param order: memoryOrder = memoryOrder.seqCst): void {
      on this {
        while (this.read(order=memoryOrder.relaxed) != val) {
          chpl_comm_ensure_progress();
          currentTask.yieldExecution();
        }
        chpl_atomic_thread_fence(c_memory_order(order));
      }
    }

    @chpldoc.nodoc
    proc const serialize(writer, ref serializer) throws {
      writer.write(read());
    }

  }

  // TODO: should this be an operator method AtomicBool.: ?
  pragma "do not resolve unless called"
  @chpldoc.nodoc
  operator :(rhs: bool, type t:AtomicBool) {
    var lhs: AtomicBool = rhs; // use init=
    return lhs;
  }

  pragma "atomic type"
  pragma "ignore noinit"
  record AtomicT : writeSerializable {
    @chpldoc.nodoc
    type valType;

    @chpldoc.nodoc
    var _v:externT(valType);

    @chpldoc.nodoc
    proc ref init_helper(val:valType) {
      pragma "fn synchronization free"
      pragma "local fn" pragma "fast-on safe extern function"
      pragma "no gpu codegen"
      extern externFunc("init", valType, explicit=false)
        proc atomic_init(ref obj:externT(valType), val:valType): void;

      atomic_init(_v, val);
    }

    @chpldoc.nodoc
    proc init(type valType) {
      this.valType = valType;
      init this;
      const default: valType;
      init_helper(default);
    }

    @chpldoc.nodoc
    proc init=(const ref other:this.type) {
      this.valType = other.valType;
      init this;
      init_helper(other.read());
    }

    @chpldoc.nodoc
    proc init=(other:this.type.valType) {
      this.valType = other.type;
      init this;
      init_helper(other);
    }

    @chpldoc.nodoc
    proc ref deinit() {
      pragma "fn synchronization free"
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("destroy", valType, explicit=false)
        proc atomic_destroy(ref obj:externT(valType)): void;

      on this do atomic_destroy(_v);
    }

    /*
       Returns the stored value.
    */
    inline proc const read(param order: memoryOrder = memoryOrder.seqCst): valType {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("load", valType)
        proc atomic_load(const ref obj:externT(valType), order:memory_order): valType;

      var ret:valType;
      on this do ret = atomic_load(_v, c_memory_order(order));
      return ret;
    }

    /*
       Stores `val` as the new value.
    */
    inline proc ref write(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("store", valType)
        proc atomic_store(ref obj:externT(valType), val:valType, order:memory_order): void;

      on this do atomic_store(_v, val, c_memory_order(order));
    }

    /*
       Stores `val` as the new value and returns the original value.
    */
    inline proc ref exchange(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("exchange", valType)
        proc atomic_exchange(ref obj:externT(valType), val:valType, order:memory_order): valType;

      var ret:valType;
      on this do ret = atomic_exchange(_v, val, c_memory_order(order));
      return ret;
    }

    /*
       Stores `desired` as the new value, if and only if the original value is
       equal to `expected`. Returns `true` if `desired` was stored, otherwise
       updates `expected` to the original value.
     */
    inline proc ref compareExchange(ref expected:valType, desired:valType, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order, readableOrder(order));
    }
    inline proc ref compareExchange(ref expected:valType, desired:valType, param success: memoryOrder, param failure: memoryOrder): bool {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("compare_exchange_strong", valType)
        proc atomic_compare_exchange_strong(ref obj:externT(valType), ref expected:valType, desired:valType, succ:memory_order, fail:memory_order): bool;

      var ret:bool;
      on this {
        var localizedExpected = expected;
        ret = atomic_compare_exchange_strong(_v, localizedExpected, desired, c_memory_order(success), c_memory_order(failure));
        if !ret then expected = localizedExpected;
      }
      return ret;
    }

    /*
       Similar to :proc:`compareExchange`, except that this function may
       return `false` even if the original value was equal to `expected`. This
       may happen if the value could not be updated atomically.

       This weak version is allowed to spuriously fail, but when
       compareExchange is already in a loop, it can offer better
       performance on some platforms.
    */
    inline proc ref compareExchangeWeak(ref expected:valType, desired:valType, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchangeWeak(expected, desired, order, readableOrder(order));
    }
    inline proc ref compareExchangeWeak(ref expected:valType, desired:valType, param success: memoryOrder, param failure: memoryOrder): bool {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("compare_exchange_weak", valType)
        proc atomic_compare_exchange_weak(ref obj:externT(valType), ref expected:valType, desired:valType, succ:memory_order, fail:memory_order): bool;

      var ret:bool;
      on this {
        var localizedExpected = expected;
        ret = atomic_compare_exchange_weak(_v, localizedExpected, desired, c_memory_order(success), c_memory_order(failure));
        if !ret then expected = localizedExpected;
      }
      return ret;
    }

    /*
       Stores `desired` as the new value, if and only if the original value is
       equal to `expected`. Returns `true` if `desired` was stored.
    */
    @unstable("'compareAndSwap' is unstable")
    inline proc ref compareAndSwap(expected:valType, desired:valType, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("compare_exchange_strong", valType)
        proc atomic_compare_exchange_strong(ref obj:externT(valType), ref expected:valType, desired:valType, succ:memory_order, fail:memory_order): bool;

      var ret:bool;
      on this {
        var mutableExpected = expected;
        ret = atomic_compare_exchange_strong(_v, mutableExpected, desired, c_memory_order(order), c_memory_order(readableOrder(order)));
      }
      return ret;
    }

    /*
       :returns: The original value.

       Adds `val` to the original value and stores the result. Defined for
       integer and real atomic types.
    */
    inline proc ref fetchAdd(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_add", valType)
        proc atomic_fetch_add(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      var ret:valType;
      on this do ret = atomic_fetch_add(_v, val, c_memory_order(order));
      return ret;
    }

    /*
       Adds `val` to the original value and stores the result. Defined for
       integer and real atomic types.
    */
    inline proc ref add(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_add", valType)
        proc atomic_fetch_add(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      on this do atomic_fetch_add(_v, val, c_memory_order(order));
    }

    /*
       :returns: The original value.

       Subtracts `val` from the original value and stores the result. Defined
       for integer and real atomic types.
    */
    inline proc ref fetchSub(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_sub", valType)
        proc atomic_fetch_sub(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      var ret:valType;
      on this do ret = atomic_fetch_sub(_v, val, c_memory_order(order));
      return ret;
    }

    /*
       Subtracts `val` from the original value and stores the result. Defined
       for integer and real atomic types.
    */
    inline proc ref sub(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_sub", valType)
        proc atomic_fetch_sub(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      on this do atomic_fetch_sub(_v, val, c_memory_order(order));
    }

    /*
       :returns: The original value.

       Applies the ``|`` operator to `val` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc ref fetchOr(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      if !isIntegral(valType) then compilerError("fetchOr is only defined for integer atomic types");
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_or", valType)
        proc atomic_fetch_or(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      var ret:valType;
      on this do ret = atomic_fetch_or(_v, val, c_memory_order(order));
      return ret;
    }

    /*
       Applies the ``|`` operator to `val` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc ref or(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(valType) then compilerError("or is only defined for integer atomic types");
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_or", valType)
        proc atomic_fetch_or(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      on this do atomic_fetch_or(_v, val, c_memory_order(order));
    }

    /*
       :returns: The original value.

       Applies the ``&`` operator to `val` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc ref fetchAnd(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      if !isIntegral(valType) then compilerError("fetchAnd is only defined for integer atomic types");
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_and", valType)
        proc atomic_fetch_and(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      var ret:valType;
      on this do ret = atomic_fetch_and(_v, val, c_memory_order(order));
      return ret;
    }

    /*
       Applies the ``&`` operator to `val` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc ref and(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(valType) then compilerError("and is only defined for integer atomic types");
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_and", valType)
        proc atomic_fetch_and(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      on this do atomic_fetch_and(_v, val, c_memory_order(order));
    }

    /*
       :returns: The original value.

       Applies the ``^`` operator to `val` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc ref fetchXor(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      if !isIntegral(valType) then compilerError("fetchXor is only defined for integer atomic types");
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_xor", valType)
        proc atomic_fetch_xor(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      var ret:valType;
      on this do ret = atomic_fetch_xor(_v, val, c_memory_order(order));
      return ret;
    }

    /*
       Applies the ``^`` operator to `val` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc ref xor(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(valType) then compilerError("xor is only defined for integer atomic types");
      pragma "local fn" pragma "fast-on safe extern function"
      extern externFunc("fetch_xor", valType)
        proc atomic_fetch_xor(ref obj:externT(valType), operand:valType, order:memory_order): valType;

      on this do atomic_fetch_xor(_v, val, c_memory_order(order));
    }

    @unstable("'fetchMin' on an atomic is unstable")
    inline proc ref fetchMin(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      var t = if orderIncludesRelease(order)
              then fetchAdd(0, order)
              else read(readableOrder(order));
      while AutoMath.min(val, t) != t {
        if compareExchangeWeak(t, val, order) {
          return t;
        }
      }
      return t;
    }

    @unstable("'min' on an atomic is unstable")
    inline proc ref min(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      fetchMin(val, order=order);
    }

    @unstable("'fetchMax' on an atomic is unstable")
    inline proc ref fetchMax(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      var t = if orderIncludesRelease(order)
              then fetchAdd(0, order)
              else read(readableOrder(order));
      while AutoMath.max(val, t) != t {
        if compareExchangeWeak(t, val, order) {
          return t;
        }
      }
      return t;
    }

    @unstable("'max' on an atomic is unstable")
    inline proc ref max(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      fetchMax(val, order=order);
    }

    /*
       Waits until the stored value is equal to `val`. The implementation may
       yield the running task while waiting.
    */
    inline proc const waitFor(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      on this {
        while (this.read(order=memoryOrder.relaxed) != val) {
          chpl_comm_ensure_progress();
          currentTask.yieldExecution();
        }
        chpl_atomic_thread_fence(c_memory_order(order));
      }
    }

    @chpldoc.nodoc
    proc const serialize(writer, ref serializer) throws {
      writer.write(read());
    }

  }

  // TODO: should this be an operator method AtomicT.: ?
  @chpldoc.nodoc
  operator :(rhs, type t:AtomicT)
  where rhs.type == t.valType {
    var lhs: t = rhs; // use init=
    return lhs;
  }

  //
  // For the first cut, we will be making assignment and other normal
  // operations illegal.  In addition, we are also punting on
  // the default argument intent since it's not obvious what it
  // should be.
  //

  // We need to explicitly define these for all types because the atomic
  // types are records and unless explicitly defined, it will resolve
  // to the normal record version of the function.  Sigh.

  /* Equivalent to ``a.write(b.read())`` */
  @chpldoc.nodoc
  inline operator AtomicBool.=(ref a:AtomicBool, const ref b:AtomicBool) {
    a.write(b.read());
  }
  @chpldoc.nodoc
  inline operator AtomicBool.=(ref a:AtomicBool, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  /* Equivalent to ``a.write(b.read())`` */
  @chpldoc.nodoc
  inline operator AtomicT.=(ref a:AtomicT, const ref b:AtomicT) {
    a.write(b.read());
  }
  @chpldoc.nodoc
  inline operator AtomicT.=(ref a:AtomicT, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  @chpldoc.nodoc
  inline operator AtomicT.+(a:AtomicT, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  @chpldoc.nodoc
  inline operator AtomicT.-(a:AtomicT, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  @chpldoc.nodoc
  inline operator AtomicT.*(a:AtomicT, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  @chpldoc.nodoc
  inline operator AtomicT./(a:AtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  @chpldoc.nodoc
  inline operator AtomicT.%(a:AtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
}
