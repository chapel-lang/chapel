.. _readme-manage:

======================
The 'manage' statement
======================

The ``manage`` statement provides a mechanism for performing actions
automatically at the beginning and end of a scope.

The syntax of the ``manage`` statement is inspired by its Python
counterpart. The main difference is the use of the ``manage``
keyword to open the statement instead of ``with``:

.. code-block:: chapel

    manage myManager() as myResource do
      myResource.doSomething();

The manage statement accepts a `manager` (the `myManager` in the
above example). The statement calls a special method on the manager
which lets it perform actions before executing the managed block.

The manager may optionally return a `resource`. If a developer wants
to make use of the resource, they may capture it after the manager
expression (the `myResource` in the above example).

Any aggregate type may be used as a manager as long as it defines
two special methods:

.. code-block:: chapel

   record myManager {
     var x: int = 0;

     proc enterThis() ref: int {
       writeln('x is: ', x);
       return x;
     }

     proc leaveThis(in err: owned Error?) {
       if err then halt(err:string);
       writeln('x is: ', x);
     }
   }

   var m = new myManager();
   manage m as myResource {
     // Prints '0'
     myResource = 8;
     // Prints '8'
   }

The ``enterThis()`` method is called on the manager before executing
the managed block. The ``leaveThis()`` method is called on the
manager before exiting the block. It accepts an ``owned Error?`` by
``in`` intent in order to take ownership of it. The record author
may decide to rethrow the error or suppress it.

Status and Future Work
----------------------

The behavior of several aspects of the ``manage`` statement have yet
to be formalized, such as:

- If multiple overloads of ``enterThis()`` exist that have different
  return intents, it is unclear what the disambiguation order used
  to select an overload should be.
- It is uncertain whether or not the storage kind (e.g. ``var``) of
  a resource should be allowed to be explicitly specified.
- It is unclear how the ``leaveThis()`` method of a manager should
  interact with errors beyond guaranteeing that ``leaveThis()``
  is called even if an error is thrown from within a managed block.

