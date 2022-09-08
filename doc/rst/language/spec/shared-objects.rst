.. default-domain:: chpl

.. _Chapter-Shared-Objects:

==============
Shared Objects
==============

:record:`~SharedObject.shared` (along with :record:`~OwnedObject.owned`) manage
the deallocation of a class instance. :record:`~SharedObject.shared` is meant
to be used when many different references will exist to the object at the same
time and these references need to keep the object alive.

Please see also the section on :ref:`Class_Lifetime_and_Borrows`.

Using `shared`
--------------

:record:`~SharedObject.shared`, allocate a class instance following this
pattern:

.. code-block:: chapel

    var mySharedObject = new shared MyClass(...));

When ``mySharedObject`` and any copies of it go out of scope, the class
instance it refers to will be deleted.

Copy initializing or assigning from mySharedObject will make other variables
refer to the same class instance. The class instance will be deleted after
all of these references go out of scope.

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
~~~~~~~~~~~~~~~~~~~~~~~

The :proc:`~SharedObject.shared.borrow` method returns the pointer managed by
the :record:`~SharedObject.shared`. This pointer is only valid as long as the
:record:`~SharedObject.shared` is storing that pointer. The compiler includes
some checking for errors in this case. In these ways,
:record:`~SharedObject.shared` is similar to :record:`~OwnedObject.owned`.

See :ref:`about-owned-borrowing` for more details and examples.

Coercions for `shared`
~~~~~~~~~~~~~~~~~~~~~~

As with :record:`~OwnedObject.owned`, :record:`~SharedObject.shared` supports
coercions to the class type as well as coercions from a ``shared(T)`` to
``shared(U)`` where ``T`` is a subclass of ``U``.

See :ref:`about-owned-coercions` for more details and examples.

`shared` Default Intent
~~~~~~~~~~~~~~~~~~~~~~~

The default intent for :record:`~SharedObject.shared` types is ``const ref``.


The `shared` Type Definition
----------------------------

.. include:: /builtins/SharedObject.rst
