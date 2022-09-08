.. default-domain:: chpl

.. _Chapter-Owned-Objects:

=============
Owned Objects
=============

:record:`~OwnedObject.owned` (along with :record:`~SharedObject.shared`) manage
the deallocation of a class instance. :record:`~OwnedObject.owned` is meant to
be used when only one reference to an object needs to manage that object's
storage at a time.

Please see also the section on :ref:`Class_Lifetime_and_Borrows`.

Using `owned`
-------------

The ``new`` keyword allocates :record:`~OwnedObject.owned` classes by default.
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
:record:`~OwnedObject.owned` will leave ``myOwnedObject`` storing a nil value
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
~~~~~~~~~~~~~~~~~~~~~~

The :proc:`~OwnedObject.owned.borrow` method returns the pointer managed by the
:record:`~OwnedObject.owned`. This pointer is only valid as long as the
:record:`~OwnedObject.owned` is storing that pointer.

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
~~~~~~~~~~~~~~~~~~~~~

The compiler includes support for introducing automatic coercions from
:record:`~OwnedObject.owned` to the borrow type. This is equivalent to
calling the :proc:`~OwnedObject.owned.borrow` method. For example:

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
~~~~~~~~~~~~~~~~~~~~~~~

The default intent for :record:`~OwnedObject.owned` is ``const ref``.

.. _owned_type_def:

The `owned` Type Definition
---------------------------

.. include:: /builtins/OwnedObject.rst
