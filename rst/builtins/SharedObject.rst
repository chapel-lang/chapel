.. default-domain:: chpl

.. module:: SharedObject
   :synopsis: :record:`shared` (along with :record:`~OwnedObject.owned`) manage the

shared
======

:record:`shared` (along with :record:`~OwnedObject.owned`) manage the
deallocation of a class instance. :record:`shared` is meant to be used when many
different references will exist to the object at the same time and these
references need to keep the object alive.

Please see also the language spec section :ref:`Class_Lifetime_and_Borrows`.

Using `shared`
--------------

To use :record:`shared`, allocate a class instance following this
pattern:

.. code-block:: chapel

 var mySharedObject = new shared MyClass(...));

When ``mySharedObject`` and any copies of it go out of scope, the class
instance it refers to will be deleted.

Copy initializing or assigning from mySharedObject will make
other variables refer to the same class instance. The class instance
will be deleted after all of these references go out of scope.

.. code-block:: chapel

 var globalSharedObject:shared MyClass;

 proc makeGlobalSharedObject() {
   var mySharedObject = new shared MyClass(...);
   globalSharedObject = mySharedObject;
   // the reference count is decremented when mySharedObject
   // goes out of scope. Since it's not zero after decrementing, the
   // MyClass instance is not deleted until globalSharedObject
   // goes out of scope.
 }

Borrowing from `shared`
-----------------------

The :proc:`shared.borrow` method returns the pointer managed by the
:record:`shared`. This pointer is only valid as long as the :record:`shared` is
storing that pointer. The compiler includes some checking for errors in this
case. In these ways, :record:`shared` is similar to
:record:`~OwnedObject.owned`.

See :ref:`about-owned-borrowing` for more details and examples.

Coercions for `shared`
----------------------

As with :record:`~OwnedObject.owned`, :record:`shared` supports
coercions to the class type as well as
coercions from a ``shared(T)`` to ``shared(U)`` where ``T`` is a
subclass of ``U``.

See :ref:`about-owned-coercions` for more details and examples.

`shared` Default Intent
-----------------------

The default intent for :record:`shared` types is ``const ref``.

 

.. record:: shared

   
   
   :record:`shared` manages the deletion of a class instance in a way
   that supports multiple owners of the class instance.
   
   This is currently implemented with task-safe reference counting.
   
   


   .. method:: proc init(type t)

      
      Default-initialize a :record:`shared`.
      

   .. method:: proc init(in take: owned)

      
      Initialize a :record:`shared` taking a pointer from
      a :record:`~OwnedObject.owned`.
      
      This :record:`shared` will take over the deletion of the class
      instance. It is an error to directly delete the class instance
      while it is managed by :record:`shared`.
      
      :arg take: the owned value to take ownership from
      

   .. method:: proc init=(const ref src: shared)

      
      Copy-initializer. Creates a new :record:`shared`
      that refers to the same class instance as `src`.
      These will share responsibility for managing the instance.
      

   .. method:: proc type create(in take: owned)

      Changes the memory management strategy of the argument from `owned`
      to `shared`, taking over the ownership of the argument.
      The result type preserves nilability of the argument type.
      If the argument is non-nilable, it must be recognized by the compiler
      as an expiring value. 

   .. method:: proc type create(in src: shared)

      Creates a new `shared` class reference to the argument.
      The result has the same type as the argument. 

   .. method:: proc type create(p: unmanaged)

   .. method:: proc deinit()

      
      The deinitializer for :record:`shared` will destroy the class
      instance once there are no longer any copies of this
      :record:`shared` that refer to it.
      

   .. method:: proc ref retain(newPtr: unmanaged)

      
      Change the instance managed by this class to `newPtr`.
      If this record was the last :record:`shared` managing a
      non-nil instance, that instance will be deleted.
      

   .. method:: proc ref clear()

      
      Empty this :record:`shared` so that it stores `nil`.
      Deletes the managed object if this :record:`shared` is the
      last :record:`shared` managing that object.
      Does not return a value.
      
      Equivalent to ``shared.retain(nil)``.
      

   .. method:: proc borrow()

      
      Return the object managed by this :record:`shared` without
      impacting its lifetime at all. It is an error to use the
      value returned by this function after the last :record:`shared`
      goes out of scope or deletes the contained class instance
      for another reason, including calls to
      `=`, or ``shared.retain`` when this is the last :record:`shared`
      referring to the instance.
      In some cases such errors are caught at compile-time.
      

.. function:: proc =(ref lhs: shared, rhs: shared)

   
   Assign one :record:`shared` to another.
   Deletes the object managed by ``lhs`` if there are
   no other :record:`shared` referring to it. On return,
   ``lhs`` will refer to the same object as ``rhs``.
   

.. function:: proc =(ref lhs: shared, in rhs: owned)

   
   Set a :record:`shared` from a :record:`~OwnedObject.owned`.
   Deletes the object managed by ``lhs`` if there are
   no other :record:`shared` referring to it.
   On return, ``lhs`` will refer to the object previously
   managed by ``rhs``, and ``rhs`` will refer to `nil`.
   

.. function:: proc <=>(ref lhs: shared, ref rhs: shared)

   
   Swap two :record:`shared` objects.
   

