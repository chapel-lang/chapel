.. default-domain:: chpl

.. module:: AtomicObjects
   :synopsis: .. note:: 

AtomicObjects
=============
**Usage**

.. code-block:: chapel

   use AtomicObjects;


.. note:: 

  This package relies on Chapel ``extern`` code blocks and so requires that
  ``CHPL_LLVM=llvm`` or ``CHPL_LLVM=system`` and that the Chapel compiler is
  built with LLVM enabled. As well, currently only ``CHPL_TARGET_ARCH=x86_64``
  is supported as we make use of the x86-64 instruction: CMPXCHG16B_.

  .. _CMPXCHG16B: https://www.felixcloutier.com/x86/cmpxchg8b:cmpxchg16b
  
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



.. record:: ABA

   
   Wrapper for an object protected by an ABA counter. This type forwards to the object
   represented by its underlying pointer and hence can be used as if it were the object
   itself, via 'forwarding'. This type should not be created by the user, and instead
   should be created by LocalAtomicObject. The object protected by this ABA wrapper can
   be extracted via 'getObject'.
   


   .. attribute:: type __ABA_objType

   .. method:: proc init(type __ABA_objType)

   .. method:: proc init=(other: ABA(?objType))

   .. method:: proc getObject(): nilable __ABA_objType

   .. method:: proc getABACount()

   .. method:: proc readWriteThis(f) throws

.. function:: proc =(ref lhs: ABA, const ref rhs: lhs.type )

.. function:: proc ==(const ref aba1: ABA, const ref aba2: ABA)

   
   Special case operator that compares two ``ABA`` wrappers.
   

.. function:: proc !=(const ref aba1: ABA, const ref aba2: ABA)

.. record:: AtomicObject

   .. attribute:: type objType

   .. attribute:: param hasABASupport: bool

   .. attribute:: param hasGlobalSupport: bool

   .. attribute:: var atomicVar: AppendExpr.01

   .. method:: proc init(type objType, param hasABASupport = false, param hasGlobalSupport = !_local)

   .. method:: proc init(type objType, defaultValue: objType, param hasABASupport = false, param hasGlobalSupport = !_local)

   .. method:: proc readABA(): ABA(nilable objType)

   .. method:: proc read(): nilable objType

   .. method:: proc compareAndSwap(expectedObj: nilable objType, newObj: nilable objType): bool

   .. method:: proc compareAndSwapABA(expectedObj: ABA(nilable objType), newObj: nilable objType): bool

   .. method:: proc compareAndSwapABA(expectedObj: ABA(nilable objType), newObj: ABA(nilable objType)): bool

   .. method:: proc write(newObj: nilable objType)

   .. method:: proc write(newObj: ABA(nilable objType))

   .. method:: proc writeABA(newObj: ABA(nilable objType))

   .. method:: proc writeABA(newObj: nilable objType)

   .. method:: proc exchange(newObj: nilable objType): nilable objType

   .. method:: proc exchangeABA(newObj: nilable objType): ABA(nilable objType)

   .. method:: proc exchangeABA(newObj: ABA(nilable objType)): ABA(nilable objType)

   .. method:: proc readWriteThis(f) throws

