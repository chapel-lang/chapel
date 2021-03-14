.. _readme-errorHandling:

==========================================
Error Handling Modes and Prototype Modules
==========================================

Overview
--------

The Chapel language supports ``throw``, ``try``, ``try!``, ``catch``, and
``throws`` which are described below. Chapel supports several error handling
modes, and in particular the default for code not in an explicit module
declaration is suitable only for prototype code.

For a code-oriented introduction to error handling, refer to the
:ref:`primers-errorHandling` primer.

Please see the Error Handling chapter of the language specification for
details.

.. _technote-errorHandling-throwing:

.. _technote-errorHandling-handling:

.. _technote-errorHandling-trybang:

.. _technote-errorHandling-catch:

.. _technote-errorHandling-propagation:

.. _technote-errorHandling-complete:

.. _technote-errorHandling-defer:

.. _technote-errorHandling-methods:

.. _technote-errorHandling-multilocale:

.. _technote-errorHandling-parallel:

.. _technote-errorHandling-classes:

.. _technote-errorHandling-modes:

Error Handling Modes
--------------------

Chapel currently supports three error handling modes: fatal, relaxed, and
strict. Developers can select each mode in the following ways:

 * *fatal mode*

   * writing code in an implicit module
   * writing code in a ``prototype module``
   * passing the ``--permit-unhandled-module-errors`` flag on the
     ``chpl`` command line

 * *relaxed mode*

   * writing code inside of a ``module`` declaration

 * *strict mode*

   * writing code inside of a module marked with a pragma

It is fully permissible to write for a stricter error handling mode
while using a looser one -- in particular, code that compiles in strict
mode will also compile in relaxed or fatal mode.

.. _technote-errorHandling-fatal:

Fatal Mode for Implicit and Prototype Modules
+++++++++++++++++++++++++++++++++++++++++++++

In implicit and prototype modules, it is not necessary to explicitly handle
errors from a function that throws. If an error is thrown and the calling
function throws, the error will be propagated out of the function.  However,
if an error is thrown and the calling function does not throw, the program
will halt.

This is the *fatal* error handling mode.

An *implicit module* is the module the compiler creates to store
code not in a ``module`` declaration. In particular, all of the
statements above this point in this file are in an implicit module called
errorHandling. Since the below statements are also not in a ``module``
declaration, they are also in an *implicit module*.

.. code-block:: chapel

  canThrow(1); // handling can be omitted; halts if an error occurs

  proc throwsErrorsOn() throws {
    // error propagates out of this function
    canThrow(-1);
  }

  proc doesNotThrowErrorsOn() {
    // causes a halt if called
    alwaysThrows();
  }

Fatal error mode can also be activated for explicit modules using the
``prototype module`` declaration:

.. code-block:: chapel

  prototype module PrototypeModule {

    canThrow(1); // handling can be omitted; halts if an error occurs

    proc throwsErrorsOn() throws {
      // error propagates out of this function
      alwaysThrows();
    }

    proc doesNotThrowErrorsOn() {
      // causes a halt if called
      alwaysThrows();
    }
  }

  use PrototypeModule;

.. _technote-errorHandling-relaxed:

Relaxed Mode for Explicit Modules
+++++++++++++++++++++++++++++++++

In non-prototype explicit modules, it is necessary to handle errors if the
calling function does not throw. If the calling function does
throw, then the error will be propagated out as with implicit modules.

This is the *relaxed* error handling mode.

.. code-block:: chapel

  module ProductionModule {
    // this line would cause a compilation error since the error is not handled
    // canThrow(1);

    proc throwsErrorsOn() throws {
      // any error thrown by alwaysThrows will propagate out
      alwaysThrows();
    }

    // this function does not compile because the error is not handled
    // proc doesNotThrowErrorsOn() {
    //   alwaysThrows();
    // }
  }

  use ProductionModule;

.. _technote-errorHandling-strict:

Strict Mode
+++++++++++

It is possible to request a more restricted error handling mode within a
module scope using a pragma. With it, it is necessary to explicitly mark all
throwing calls, whether or not the calling function throws. The intent of
this restricted mode is to make the control flow clear.

This is the *strict* error handling mode.

The difference between relaxed and strict mode is that throwing calls
in throwing functions need to be marked. This may be accomplished
with the catch-less ``try`` block, the single statement ``try``, or
the assignment ``try``.

It is also possible to use ``try!`` in these forms.

.. code-block:: chapel

  pragma "error mode strict"
  module StrictModule {
    proc throwsErrorsOn() throws {
      try {
        alwaysThrows();
        writeln("never reached");
      }
    }

    proc alsoThrowsErrorsOn() throws {
      try alwaysThrows();
    }

    proc doesNotThrowErrorsOn() {
      try! alwaysThrows();
    }

    proc assignmentTry() throws {
      var x = try alwaysThrows();
      writeln(x);
    }
  }

.. _technote-errorHandling-limitations:

Current Limitations
-------------------

* Error handling does not work yet with initializers.
* It is not yet decided whether or not it will be possible to
  throw from a `deinit` function.
* Errors can't currently be thrown from iterators that are not inlined by the
  compiler

  * note that iterators with a single yield as well as leader and standalone
    iterators are generally inlined by the compiler
