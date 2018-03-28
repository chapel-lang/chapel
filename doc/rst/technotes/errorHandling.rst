.. _readme-errorHandling:

==============
Error Handling
==============

Overview
--------

The Chapel language supports ``throw``, ``try``, ``try!``, ``catch``, and
``throws`` which are described below. Chapel supports several error handling
modes, and in particular the default for code not in an explicit module
declaration is suitable only for prototype code.

For a code-oriented introduction to error handling, refer to the
:ref:`primers-errorHandling` primer.

.. _technote-errorHandling-throwing:

Throwing Errors
---------------

Errors may be thrown from a function to its callee with a ``throw``
statement. For a function to throw an error, its signature must include
a ``throws`` declaration. The declaration is put after the return
type and before any ``where`` clauses.

.. code-block:: chapel

  proc canThrow(i: int): int throws {
    if i < 0 then
      throw new Error();

    return i + 1;
  }

  proc alwaysThrows():int throws {
    throw new Error();
    // never reached
    return 1;
  }

.. _technote-errorHandling-handling:

Handling Errors
---------------

There are three ways to handle an error:

 1. Halt with ``try!``.
 2. Handle the error with ``catch`` blocks.
 3. Propagate the error out of the current function with ``throws``.


.. _technote-errorHandling-trybang:

Halting on error with ``try!``
++++++++++++++++++++++++++++++

Marking a throwing call with ``try!`` causes the compiler to
generate code to halt the program if that call results in an error.

.. code-block:: chapel

  proc haltsOnError():int {
    // the try! next to the throwing call
    // halts the program if an error occurs.
    return try! canThrow(0);
  }

  proc haltsOnErrorBlock() {
    try! {
      canThrow(1);
      canThrow(0);
    }
  }

.. _technote-errorHandling-catch:

Handling an error with ``catch``
++++++++++++++++++++++++++++++++

When an error is raised by a call in a ``try`` block, the rest of the
block is abandoned and control flow is passed to the ``catch`` clauses.

``catch`` clauses
*****************

``catch`` clause type filters are evaluated in order. If a ``catch``
clause's type filter matches, then its block is executed to the exclusion
of the others. Hence there is no notion of best match, only a first match.

If no type filter is present on a catch clause, or if no variable is
present at all, then it is a catchall clause which matches all errors.

.. code-block:: chapel

  proc catchingErrors() throws {
    try {
      alwaysThrows(0);
    } catch {
      writeln("caught an error, unnamed catchall");
    }

    try {
      var x = alwaysThrows(-1);
      writeln("never reached");
    } catch e:FileNotFoundError {
      writeln("caught a file not found error");
    } catch e {
      writeln("caught an error, named catchall");
    }
  }

``try!`` with ``catch``
***********************

``try!`` is also available in block form and can have catch blocks.
If the ``catch`` clauses after a ``try!`` don't handle the error,
the program halts.

.. code-block:: chapel

  proc catchingErrorsHalt() {
    try! {
      var x = alwaysThrows(-1);
      writeln("never reached");
    } catch e:FileNotFoundError {
      writeln("caught a file not found error");
    }
    // errors other than FileNotFoundError cause a halt
  }

Nested ``try``
**************

It is also possible for ``try`` blocks to direct their errors to the
``catch`` clauses of an enclosing ``try``, if a matching ``catch`` clause
is not available.

.. code-block:: chapel

  proc nestedTry() {
    try {
      try {
        alwaysThrows(0);
      } catch e: DemoError {
        writeln("caught a DemoError");
      }
      writeln("never reached");
    } catch {
      writeln("caught an Error from inner try");
    }
  }

.. _technote-errorHandling-propagation:

Propagating an error with ``throws``
++++++++++++++++++++++++++++++++++++

A function marked ``throws`` can pass along an error thrown by a
function called within it.


After ``catch`` clauses
***********************

Propagation can occur when no matching ``catch`` clause is found for an
error raised in a ``try`` block.

.. code-block:: chapel

  proc catchingErrorsPropagate() throws {
    try {
      var x = alwaysThrows(-1);
      writeln("never reached");
    } catch e:FileNotFoundError {
      writeln("caught an error");
    }
    // errors other than FileNotFoundError propagate
  }

catch-less ``try``
******************

A logical extension of the above is the case where no ``catch`` blocks
are attached to the ``try``. Here the ``try`` keyword marks throwing
calls to clarify control flow.

.. code-block:: chapel

  proc propagatesError() throws {
    // control flow changes if an error was thrown;
    // could be indicated more clearly with try
    canThrow(0);

    try canThrow(0);

    try {
      canThrow(0);
    }

    var x = try canThrow(1);
    writeln(x);

    return try canThrow(0);
  }

``try`` expressions
*******************

``try`` and ``try!`` are available as expressions to clarify control flow
for assignments and returns. The expression form may not be used with
``catch`` clauses.

.. code-block:: chapel

  proc expressionTry(): int throws {
    var x = try canThrow(1);
    writeln(x);

    return try canThrow(0);
  }

For more information on enforcing clear control flow with the compiler,
see :ref:`technote-errorHandling-strict` below.

.. _technote-errorHandling-complete:

Complete handling
+++++++++++++++++

For a function to handle errors from its calls without itself throwing,
its ``try``/``catch`` must be *complete*. This may be accomplished
in two ways:

1. A catchall clause on ``try``. This prevents ``try`` from
propagating the error out of the function as described above.

.. code-block:: chapel

  proc warnsOnError(i: int): int {
    try {
      alwaysThrows(i);
    } catch e {
      writeln("Warning: caught a error ", e);
    }
  }

2. ``try!`` instead of ``try``. This will halt the program if no matching
``catch`` clause is found, instead of propagating.

.. code-block:: chapel

  proc haltsOnError(i: int): int {
    try! {
      canThrow(i);
    } catch e: DemoError {
      writeln("caught a DemoError");
    }
  }

.. _technote-errorHandling-defer:

``defer``
---------

When an error is thrown, it is sometimes necessary to clean up state and
allocated memory. ``defer`` statements facilitate that by running when a
scope is exited, regardless of how it is exited.

.. code-block:: chapel

  proc deferredDelete(i: int) {
    try {
      var huge = allocateLargeObject();
      defer {
        delete huge;
        writeln("huge has been deleted");
      }

      canThrow(i);
      processObject(huge);
    } catch {
      writeln("no memory leaks");
    }
  }

.. note::

  It is not possible to throw errors out of a ``defer`` statement because the
  atomicity of all ``defer`` statements must be guaranteed, and the handling
  context would be unclear.

  Errors also cannot be thrown by ``deinit()`` for similar reasons.

.. _technote-errorHandling-methods:

Methods
-------

Errors can be thrown by methods, just as with any other function.
An overriding method must throw if the overridden method throws,
or not throw if the overridden method does not throw.

.. code-block:: chapel

  class ThrowingObject {
    proc f() throws {
      throw new Error();
    }
  }

  class SubThrowingObject : ThrowingObject {
    // must be marked throws even though it doesn't throw
    proc f() throws {
      writeln("this version doesn't throw");
    }
  }

.. _technote-errorHandling-multilocale:

Multilocale
-----------

Errors can be thrown within ``on`` statements. In that event, the error
will be propagated out of the ``on`` statement.

.. code-block:: chapel

  proc handleFromOn() {
    try {
      on Locales[0] {
        canThrow(1);
      }
    } catch {
      writeln("caught from Locale 0");
    }
  }

.. _technote-errorHandling-parallel:

Parallelism
-----------

``TaskErrors``
++++++++++++++

``TaskErrors`` helps coordinate errors among groups of tasks by collecting
them for centralized handling. It can be iterated on and filtered for
different kinds of errors.

The implementation of ``TaskErrors`` prevents nested ``coforall`` statements
from producing nested ``TaskErrors``. Instead, the nested errors will flatten
into the outer loop's ``TaskErrors``.

``begin``
+++++++++

Errors can be thrown within a ``begin`` statement. In that event, the error
will be propagated to the ``sync`` statement that waits for that task.

.. code-block:: chapel

  proc handleFromBegin() {
    try! {
      sync {
        begin canThrow(0);
        begin canThrow(1);
      }
    } catch e: TaskErrors {
      writeln("caught from Locale 0");
    }
  }

``coforall``, ``cobegin``
+++++++++++++++++++++++++

Errors can be thrown from ``coforall`` and ``cobegin`` statements, handled
as ``TaskErrors``. Note the nested ``coforall`` loops, which as mentioned
earlier will emit a flattened ``TaskErrors``.

.. code-block:: chapel

  proc handleFromCoforall() {
    try! {
      writeln("before coforall block");
      coforall i in 1..2 {
        coforall j in 1..2 {
          throw new DemoError();
        }
      }
      writeln("after coforall block");
    } catch errors: TaskErrors { // not nested
      // all of e will be of runtime type DemoError in this example
      for e in errors {
        writeln("Caught task error e ", e.message());
      }
    }
  }

  proc handleFromCobegin() {
    try! {
      writeln("before cobegin block");
      cobegin {
        throw new DemoError();
        throw new DemoError();
      }
      writeln("after cobegin block");
    } catch errors: TaskErrors {
      for e in errors {
        writeln("Caught task error e ", e.message());
      }
    }
  }

``forall``
++++++++++

Errors can be thrown from ``forall`` too. Although the ``forall`` may execute
serially on a single task, it will always throw ``TaskErrors`` if an error
is thrown by the inner loop.

.. code-block:: chapel

  proc handleFromCoforall() {
    try! {
      writeln("before forall block");
      forall i in 1..2 {
        throw new DemoError();
      }
      writeln("after forall block");
    } catch errors: TaskErrors {
      for e in errors {
        writeln("Caught task error e ", e.message());
      }
    }
  }

.. _technote-errorHandling-classes:

Creating New Error Types
------------------------

Errors in Chapel are implemented as classes, with a base class ``Error``
defined in the standard modules. ``Error`` may be used directly, and new
hierarchies may be created from it.

A hierarchy for system errors is included in :mod:`SysError`,
accessed with a ``use`` statement.

.. code-block:: chapel

  use SysError;

  class DemoError : Error { }

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
