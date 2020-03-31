.. default-domain:: chpl

.. module:: OwnedObject
   :synopsis: `owned` (along with :record:`~SharedObject.shared`) manage the

owned
=====

`owned` (along with :record:`~SharedObject.shared`) manage the
deallocation of a class instance. :record:`owned` is meant to be used when
only one reference to an object needs to manage that object's storage
at a time.

Please see also the language spec section :ref:`Class_Lifetime_and_Borrows`.

Using `owned`
-------------

The ``new`` keyword allocates :record:`owned` classes by default.
Additionally, it is possible to explicitly request an ``owned`` class instance

.. code-block:: chapel

 class MyClass { }

 var myOwnedObject = new MyClass();
 // or, equivalently
 var myOwnedObject = new owned MyClass();

When ``myOwnedObject`` goes out of scope, the class instance it refers to will
be deleted.  It is possible to transfer the ownership to another `owned`
variable before that happens.

Copy initializing from ``myOwnedObject`` or assigning it to another
:record:`owned` will leave ``myOwnedObject`` storing a nil value
and transfer the owned class instance to the other value.

.. code-block:: chapel

 var otherOwnedObject = myOwnedObject;
 // now myOwnedObject stores nil
 // the value it stored earlier has moved to otherOwnedObject

 myOwnedObject = otherOwnedObject;
 // this assignment moves the value from the right-hand-side
 // to the left-hand-side, leaving the right-hand-side empty.
 // after the assignment, otherOwnedObject stores nil
 // and myOwnedObject stores a value that will be deleted
 // when myOwnedObject goes out of scope.


`owned` forms part of a type and can be used in type expressions:

.. code-block:: chapel

 var emptyOwnedObject: owned MyClass;


.. _about-owned-borrowing:

Borrowing from `owned`
----------------------

The :proc:`owned.borrow` method returns the pointer managed by the
:record:`owned`. This pointer is only valid as long as the :record:`owned`
is storing that pointer.

The compiler includes a component called the lifetime checker that
can, in many cases, check that a `borrow` does not refer to an object
that could be deleted before the `borrow`. For example:

.. code-block:: chapel

 proc test() {
   var a: owned MyClass = new owned MyClass();
   // the instance referred to by a is deleted at end of scope
   var c: borrowed MyClass = a.borrow();
   // c "borrows" to the instance managed by a
   return c; // lifetime checker error! returning borrow from local variable
   // a is deleted here
 }

.. _about-owned-coercions:

Coercions for `owned`
---------------------

The compiler includes support for introducing automatic coercions
from :record:`owned` to the borrow type. This is equivalent
to calling the :proc:`owned.borrow` method. For example:

.. code-block:: chapel

 proc f(arg: borrowed MyClass) {
   writeln(arg);
 }

 var myOwned = new owned MyClass();
 f(myOwned); // compiler coerces to borrowed MyClass via borrow()


Additionally, the compiler includes support for coercing a value
of type ``owned T`` to ``owned U`` when ``T`` is a subclass of ``U``.
For example:

.. code-block:: chapel

 class Person { }
 class Student : Person { }

 var myStudent = new owned Student();
 var myPerson:owned Person = myStudent;
 // relies on coercion from owned Student to owned Person
 // moves the instance from myStudent to myPerson, leaving
 // myStudent containing nil.


`owned` Default Intent
----------------------

The default intent for :record:`owned` is ``const ref``.

 

.. record:: owned

   
   :record:`owned` manages the deletion of a class instance assuming
   that this :record:`owned` is the only thing responsible for managing
   the lifetime of the class instance.
   


   .. method:: proc init(type t)

      
      Default-initialize a :record:`owned` to store type `t`
      

   .. method:: proc init=(ref src: owned)

      
      Copy-initializer. Creates a new :record:`owned`
      that takes over ownership from `src`. `src` will
      refer to `nil` after this call.
      

   .. method:: proc type create(in take: owned)

      Creates a new `owned` class reference, taking over the ownership
      of the argument. The result has the same type as the argument.
      If the argument is non-nilable, it must be recognized by the compiler
      as an expiring value. 

   .. method:: proc type create(p: unmanaged)

   .. method:: proc deinit()

      
      The deinitializer for :record:`owned` will destroy the class
      instance it manages when the :record:`owned` goes out of scope.
      

   .. method:: proc ref clear()

      
      Empty this :record:`owned` so that it stores `nil`.
      Deletes the previously managed object, if any.
      

   .. method:: proc ref retain(newPtr: unmanaged)

      
      Change the instance managed by this class to `newPtr`.
      If this record was already managing a non-nil instance,
      that instance will be deleted.
      

   .. method:: proc ref release()

      
      Empty this :record:`owned` so that it manages `nil`.
      Returns the instance previously managed by this :record:`owned`.
      

   .. method:: proc borrow()

      
      Return the object managed by this :record:`owned` without
      impacting its lifetime at all. It is an error to use the
      value returned by this function after the :record:`owned`
      goes out of scope or deletes the contained class instance
      for another reason, such as with `=` or ``owned.retain``.
      In some cases such errors are caught at compile-time.
      

   .. method:: proc type borrow() type

.. function:: proc =(ref lhs: owned, ref rhs: owned)

   
   Assign one :record:`owned` to another. Deletes the object managed by
   ``lhs``, if any. Transfers ownership of the object managed by ``rhs``
   to ``lhs``, leaving ``rhs`` storing `nil`.
   

.. function:: proc <=>(ref lhs: owned, ref rhs: lhs.type )

   
   Swap two :record:`owned` objects.
   

