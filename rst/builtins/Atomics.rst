.. default-domain:: chpl

.. module:: Atomics
   :synopsis: Atomic variables are variables that support atomic operations. Chapel

Atomics
=======
Atomic variables are variables that support atomic operations. Chapel
currently supports atomic operations for bools, all supported sizes of
signed and unsigned integers, as well as all supported sizes of reals.

Most atomic methods accept an optional argument named ``order`` of type
``memoryOrder``. The ``order`` argument is used to specify the ordering
constraints of atomic operations. The supported values are:

  * memoryOrder.relaxed
  * memoryOrder.acquire
  * memoryOrder.release
  * memoryOrder.acqRel
  * memoryOrder.seqCst

.. function:: proc atomicFence(param order: memoryOrder = memoryOrder.seqCst)

   An atomic fence that establishes an ordering of non-atomic and relaxed
   atomic operations. 

.. type:: atomic (bool)

   .. method:: proc read(param order: memoryOrder = memoryOrder.seqCst): bool

      
      :returns: The stored value.
      

   .. method:: proc write(value: bool, param order: memoryOrder = memoryOrder.seqCst): void

      
      Stores `value` as the new value.
      

   .. method:: proc exchange(value: bool, param order: memoryOrder = memoryOrder.seqCst): bool

      
      Stores `value` as the new value and returns the original value.
      

   .. method:: proc compareExchange(ref expected: bool, desired: bool, param order: memoryOrder = memoryOrder.seqCst): bool

      
      Stores `desired` as the new value, if and only if the original value is
      equal to `expected`. Returns `true` if `desired` was stored, otherwise
      updates `expected` to the original value.
      

   .. method:: proc compareExchange(ref expected: bool, desired: bool, param success: memoryOrder, param failure: memoryOrder): bool

   .. method:: proc compareExchangeWeak(ref expected: bool, desired: bool, param order: memoryOrder = memoryOrder.seqCst): bool

      
      Similar to :proc:`compareExchange`, except that this function may
      return `false` even if the original value was equal to `expected`. This
      may happen if the value could not be updated atomically.
      

   .. method:: proc compareExchangeWeak(ref expected: bool, desired: bool, param success: memoryOrder, param failure: memoryOrder)

   .. method:: proc compareAndSwap(expected: bool, desired: bool, param order: memoryOrder = memoryOrder.seqCst): bool

      
      Stores `desired` as the new value, if and only if the original value is
      equal to `expected`. Returns `true` if `desired` was stored.
      

   .. method:: proc testAndSet(param order: memoryOrder = memoryOrder.seqCst): bool

      
      Stores `true` as the new value and returns the old value.
      

   .. method:: proc clear(param order: memoryOrder = memoryOrder.seqCst): void

      
      Stores `false` as the new value.
      

   .. method:: proc waitFor(value: bool, param order: memoryOrder = memoryOrder.seqCst): void

      
      :arg value: Value to compare against.
      
      Waits until the stored value is equal to `value`. The implementation may
      yield the running task while waiting.
      

.. type:: atomic (T)

   .. method:: proc read(param order: memoryOrder = memoryOrder.seqCst): T

      
      :returns: The stored value.
      

   .. method:: proc write(value: T, param order: memoryOrder = memoryOrder.seqCst): void

      
      Stores `value` as the new value.
      

   .. method:: proc exchange(value: T, param order: memoryOrder = memoryOrder.seqCst): T

      
      Stores `value` as the new value and returns the original value.
      

   .. method:: proc compareExchange(ref expected: T, desired: T, param order: memoryOrder = memoryOrder.seqCst): bool

      
      Stores `desired` as the new value, if and only if the original value is
      equal to `expected`. Returns `true` if `desired` was stored, otherwise
      updates `expected` to the original value.
      

   .. method:: proc compareExchange(ref expected: T, desired: T, param success: memoryOrder, param failure: memoryOrder): bool

   .. method:: proc compareExchangeWeak(ref expected: T, desired: T, param order: memoryOrder = memoryOrder.seqCst): bool

      
      Similar to :proc:`compareExchange`, except that this function may
      return `false` even if the original value was equal to `expected`. This
      may happen if the value could not be updated atomically.
      

   .. method:: proc compareExchangeWeak(ref expected: T, desired: T, param success: memoryOrder, param failure: memoryOrder): bool

   .. method:: proc compareAndSwap(expected: T, desired: T, param order: memoryOrder = memoryOrder.seqCst): bool

      
      Stores `desired` as the new value, if and only if the original value is
      equal to `expected`. Returns `true` if `desired` was stored.
      

   .. method:: proc fetchAdd(value: T, param order: memoryOrder = memoryOrder.seqCst): T

      
      :returns: The original value.
      
      Adds `value` to the original value and stores the result. Defined for
      integer and real atomic types.
      

   .. method:: proc add(value: T, param order: memoryOrder = memoryOrder.seqCst): void

      
      Adds `value` to the original value and stores the result. Defined for
      integer and real atomic types.
      

   .. method:: proc fetchSub(value: T, param order: memoryOrder = memoryOrder.seqCst): T

      
      :returns: The original value.
      
      Subtracts `value` from the original value and stores the result. Defined
      for integer and real atomic types.
      

   .. method:: proc sub(value: T, param order: memoryOrder = memoryOrder.seqCst): void

      
      Subtracts `value` from the original value and stores the result. Defined
      for integer and real atomic types.
      

   .. method:: proc fetchOr(value: T, param order: memoryOrder = memoryOrder.seqCst): T

      
      :returns: The original value.
      
      Applies the ``|`` operator to `value` and the original value, then stores
      the result.
      
      Only defined for integer atomic types.
      

   .. method:: proc or(value: T, param order: memoryOrder = memoryOrder.seqCst): void

      
      Applies the ``|`` operator to `value` and the original value, then stores
      the result.
      
      Only defined for integer atomic types.
      

   .. method:: proc fetchAnd(value: T, param order: memoryOrder = memoryOrder.seqCst): T

      
      :returns: The original value.
      
      Applies the ``&`` operator to `value` and the original value, then stores
      the result.
      
      Only defined for integer atomic types.
      

   .. method:: proc and(value: T, param order: memoryOrder = memoryOrder.seqCst): void

      
      Applies the ``&`` operator to `value` and the original value, then stores
      the result.
      
      Only defined for integer atomic types.
      

   .. method:: proc fetchXor(value: T, param order: memoryOrder = memoryOrder.seqCst): T

      
      :returns: The original value.
      
      Applies the ``^`` operator to `value` and the original value, then stores
      the result.
      
      Only defined for integer atomic types.
      

   .. method:: proc xor(value: T, param order: memoryOrder = memoryOrder.seqCst): void

      
      Applies the ``^`` operator to `value` and the original value, then stores
      the result.
      
      Only defined for integer atomic types.
      

   .. method:: proc waitFor(value: T, param order: memoryOrder = memoryOrder.seqCst): void

      
      Waits until the stored value is equal to `value`. The implementation may
      yield the running task while waiting.
      

