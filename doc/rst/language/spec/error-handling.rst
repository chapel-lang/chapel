.. _Chapter-Error_Handling:

Error Handling
==============

The Chapel language supports ``throw``, ``try``, ``try``!, ``catch``,
and ``throws`` which are described below. Chapel supports several error
handling modes, including a mode suitable for prototype development and
a less-permissive mode intended for production code.

   *Implementation Notes*.

   | Additional information about the current implementation of
     error handling and the *strict* error handling mode, which is not
     defined here, is available online in the technical note:
   | https://chapel-lang.org/docs/technotes/errorHandling.html

.. _Throwing_Errors:

Throwing Errors
---------------

Errors may be thrown from a function to its callee with a ``throw``
statement. For a function to throw an error, its signature must include
a ``throws`` declaration. The declaration is put after the return type
and before any ``where`` clauses.

Only ``owned`` instances of a type inheriting from ``Error`` can be
thrown.

   *Example (throwing.chpl)*.

   

   .. code-block:: chapel

      proc canThrow(i: int): int throws {
        if i < 0 then
          throw new owned Error();

        return i + 1;
      }

      proc alwaysThrows():int throws {
        throw new owned Error();
        // never reached
        return 1;
      }

.. _Handling_Errors:

Handling Errors
---------------

There are three ways to handle an error:

-  Halt with ``try``!.

-  Handle the error with ``catch`` blocks.

-  Propagate the error out of the current function with ``throws``.

.. _Halting_on_error_with_try_bang:

Halting on error with try!
~~~~~~~~~~~~~~~~~~~~~~~~~~

If an error is thrown by a call within the lexical scope of a ``try``!
block or a ``try``! expression prefix, the program halts.

   *Example (try-bang.chpl)*.

   

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

.. _Handling_an_error_with_catch:

Handling an error with catch
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When an error is raised by a call in a ``try`` or ``try``! block, the
rest of the block is abandoned and control flow is passed to its
``catch`` clause(s), if any.

.. _Catch_clauses:

Catch clauses
^^^^^^^^^^^^^

A ``try`` or ``try``! block can have one or more ``catch`` clauses.

A ``catch`` clause can specify the variable that refers to the caught
error within the ``catch`` block. If the variable is given a type, for
example ``catch e:SomeError``, it is a *type filter*. The corresponding
``catch`` clause *matches* the errors that are of the class
``SomeError`` or its subclass. If no type filter is present on a catch
clause, or if no variable is present at all, then it is a *catchall*
clause, which matches all errors.

The type filters are evaluated in the order that the ``catch`` clauses
appear in the program. If a ``catch`` clause’s type filter matches, then
its block is executed to the exclusion of the others. Hence there is no
notion of best match, only a first match.

If the ``catch`` block itself throws an error, it is handled in the same
manner as if that error were thrown by a statement adjacent to the
``try``-``catch`` blocks. Otherwise, after the execution of the
``catch`` block completes, the program execution proceeds to the next
statement after the ``try``-``catch`` blocks.

   *Example (catching-errors.chpl)*.

   

   .. code-block:: chapel

      use SysError;

      proc catchingErrors() throws {
        try {
          alwaysThrows(0);
        } catch {
          writeln("caught an error, unnamed catchall clause");
        }

        try {
          var x = alwaysThrows(-1);
          writeln("never reached");
        } catch e:FileNotFoundError {
          writeln("caught an error, FileNotFoundError type filter matched");
        } catch e {
          writeln("caught an error in a named catchall clause");
        }
      }

.. _try_bang_with_catch:

try! with catch
^^^^^^^^^^^^^^^

If an error is thrown within a ``try``! block and none of its ``catch``
clauses, if any, match that error, the program halts.

   *Example (catching-errors-halt.chpl)*.

   

   .. code-block:: chapel

      use SysError;

      proc catchingErrorsHalt() {
        try! {
          var x = alwaysThrows(-1);
          writeln("never reached");
        } catch e:FileNotFoundError {
          writeln("caught a file not found error");
        }
        // errors other than FileNotFoundError cause a halt
      }

.. _Nested_try:

Nested try
^^^^^^^^^^

If an error is thrown within a ``try`` block and none of its ``catch``
clauses, if any, match that error, the error is directed to the
enclosing ``try`` block, when present.

   *Example (nested-try.chpl)*.

   

   .. code-block:: chapel

      class DemoError : Error { }

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

   

   .. BLOCK-test-chapelpost

      proc alwaysThrows():int throws {
        throw new owned Error();
        // never reached
        return 1;
      }

.. _Propagating_an_error_with_throws:

Propagating an error with throws
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A function marked ``throws`` can pass along an error thrown by a
function called within it. This can be done in several ways.

.. _After_catch_clauses:

After catch clauses
^^^^^^^^^^^^^^^^^^^

Propagation can occur when no matching ``catch`` clause is found for an
error raised in a ``try`` block.

   *Example (catching-errors-propagate.chpl)*.

   

   .. code-block:: chapel

      use SysError;

      proc catchingErrorsPropagate() throws {
        try {
          var x = alwaysThrows(-1);
          writeln("never reached");
        } catch e:FileNotFoundError {
          writeln("caught a file not found error");
        }
        // errors other than FileNotFoundError propagate
      }

.. _catch_less_try:

catch-less try
^^^^^^^^^^^^^^

A logical extension of the above is the case where no ``catch`` blocks
are attached to the ``try``. Here the ``try`` keyword marks throwing
calls to clarify control flow.

   *Example (propagates-error.chpl)*.

   

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

.. _try_expressions:

try expressions
^^^^^^^^^^^^^^^

``try`` and ``try``! are available as expressions to clarify control
flow at expression granularity. The expression form may not be used with
``catch`` clauses.

   *Example (expression-try.chpl)*.

   

   .. code-block:: chapel

      proc expressionTry(): int throws {
        var x = try canThrow(1);
        writeln(x);

        return try canThrow(0);
      }

.. _Complete_handling:

Complete handling
~~~~~~~~~~~~~~~~~

For a function to handle errors from its calls without itself throwing,
its ``try``/``catch`` must be complete. This may be accomplished in two
ways:

-  A catchall clause on ``try``. This prevents ``try`` from propagating
   the error out of the function as described above.

      *Example (warns-on-error.chpl)*.

      

      .. code-block:: chapel

         proc warnsOnError(i: int): int {
           try {
             alwaysThrows(i);
           } catch e {
             writeln("Warning: caught a error ", e);
           }
         }

-  ``try``! instead of ``try``. This will halt the program if no
   matching ``catch`` clause is found, instead of propagating.

      *Example (halts-on-error.chpl)*.

      

      .. code-block:: chapel

         class DemoError : Error { }
         proc haltsOnError(i: int): int {
           try! {
             canThrow(i);
           } catch e: DemoError {
             writeln("caught a DemoError");
           }
         }

.. _Errors_defer:

Defer statement
---------------

When an error is thrown, it is sometimes necessary to clean up state and
allocated memory. ``defer`` statements facilitate that by running when a
scope is exited, regardless of how it is exited.

   *Example (defer.chpl)*.

   

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

It is not possible to throw errors out of a ``defer`` statement because
the atomicity of all ``defer`` statements must be guaranteed, and the
handling context would be unclear.

Errors also cannot be thrown by ``deinit()`` for similar reasons.

.. _Errors_Methods:

Methods
-------

Errors can be thrown by methods, just as with any other function. An
overriding method must throw if the overridden method throws, or not
throw if the overridden method does not throw.

   *Example (throwing-methods.chpl)*.

   

   .. code-block:: chapel

      class ThrowingObject {
        proc f() throws {
          throw new owned Error();
        }
      }

      class SubThrowingObject : ThrowingObject {
        // must be marked throws even though it doesn't throw
        proc f() throws {
          writeln("this version doesn't throw");
        }
      }

.. _Errors_Multilocale:

Multilocale
-----------

Errors can be thrown within ``on`` statements. In that event, the error
will be propagated out of the ``on`` statement.

   *Example (handle-from-on.chpl)*.

   

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

.. _Errors_Parallelism:

Parallelism
-----------

.. _TaskErrors:

TaskErrors
~~~~~~~~~~

``TaskErrors`` class helps coordinate errors among groups of tasks by
collecting them for centralized handling. It can be iterated on and
filtered for different kinds of errors. See also
https://chapel-lang.org/docs/builtins/ChapelError.html#ChapelError.TaskErrors.

Nested ``coforall`` statements do not produce nested ``TaskErrors``.
Instead, the nested errors are flattened into the ``TaskErrors`` error
thrown by the outer loop.

.. _Errors_begin:

begin
~~~~~

Errors can be thrown within a ``begin`` statement. In that event, the
error will be propagated to the ``sync`` statement that waits for that
task.

   *Example (handle-from-begin.chpl)*.

   

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

.. _Errors_coforall_and_cobegin:

coforall and cobegin
~~~~~~~~~~~~~~~~~~~~

Errors can be thrown from ``coforall`` and ``cobegin`` statements and
handled as ``TaskErrors``. The nested ``coforall`` loops will emit a
flattened ``TaskErrors`` error.

   *Example (handle-from-coforall.chpl)*.

   
   .. BLOCK-test-chapelpre

     class DemoError : Error { }

   .. code-block:: chapel

      proc handleFromCoforall() {
        try! {
          writeln("before coforall block");
          coforall i in 1..2 {
            coforall j in 1..2 {
              throw new owned DemoError();
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

..

   *Example (handle-from-cobegin.chpl)*.

   .. BLOCK-test-chapelpre

     class DemoError : Error { }

   .. code-block:: chapel

      proc handleFromCobegin() {
        try! {
          writeln("before cobegin block");
          cobegin {
            throw new owned DemoError();
            throw new owned DemoError();
          }
          writeln("after cobegin block");
        } catch errors: TaskErrors {
          for e in errors {
            writeln("Caught task error e ", e.message());
          }
        }
      }

.. _Errors_forall:

forall
~~~~~~

Errors can be thrown from ``forall`` loops, too. Although the ``forall``
may execute serially within a single task, it will always throw a
``TaskErrors`` error if error(s) are thrown in the loop body.

   *Example (handle-from-forall.chpl)*.

   .. BLOCK-test-chapelpre

     class DemoError : Error { }

   .. code-block:: chapel

      proc handleFromForall() {
        try! {
          writeln("before forall block");
          forall i in 1..2 {
            throw new owned DemoError();
          }
          writeln("after forall block");
        } catch errors: TaskErrors {
          for e in errors {
            writeln("Caught task error e ", e.message());
          }
        }
      }

.. _Creating_New_Error_Types:

Creating New Error Types
------------------------

Errors in Chapel are implemented as classes, with a base class ``Error``
defined in the standard modules. ``Error`` may be used directly, and new
subclass hierarchies may be created from it. See also
https://chapel-lang.org/docs/builtins/ChapelError.html.

A hierarchy for system errors is included in the ``SysError`` module,
accessed with a ``use`` statement. See also
https://chapel-lang.org/docs/modules/standard/SysError.html

   *Example (defining-errors.chpl)*.

   

   .. code-block:: chapel

      use SysError;

      class DemoError : Error { }

      class DemoSysError : SystemError { }

.. _Error_Handling_Modes:

Error Handling Modes
--------------------

Certain error handling details depend on the *error handling mode*:

-  Code in ``prototype`` modules (:ref:`Prototype_Modules`),
   including implicit modules (:ref:`Implicit_Modules`), is
   handled in the *prototype* mode.

-  Otherwise, code is handled in the *production* mode.

Code that is legal in the production mode is always legal in the
prototype mode.

.. _Errors_Prototype_Mode:

Prototype Mode
~~~~~~~~~~~~~~

In the prototype mode, it is not necessary to explicitly handle errors
from a function that throws. If an error is thrown and the calling
function throws, the error will be propagated out of the function.
However, if an error is thrown and the calling function does not include
a ``throws`` declaration, the program will halt.

In the following example, the code is in an implicit module. It is legal
in the prototype mode:

   *Example (fatal-mode.chpl)*.

   

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

   

   .. BLOCK-test-chapelpost

      proc canThrow(i: int): int throws {
        if i < 0 then
          throw new owned Error();

        return i + 1;
      }

The following module is explicitly marked as a prototype module, so the
prototype mode applies here, too.

   *Example (PrototypeModule.chpl)*.

   

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

        proc canThrow(i: int): int throws {
          if i < 0 then
            throw new owned Error();

          return i + 1;
        }
      }

.. _Production_Mode_for_Explicit_Modules:

Production Mode
~~~~~~~~~~~~~~~

In the production mode, it is necessary to handle errors if the calling
function does not throw. If the calling function does throw, then the
error will be propagated out, as with the prototype mode.

   *Example (ProductionModule.chpl)*.

   

   .. code-block:: chapel

      module ProductionModule {
        // This would cause a compilation error since the error is not handled:
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
