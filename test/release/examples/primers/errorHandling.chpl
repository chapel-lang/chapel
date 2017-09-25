// Error Handling
//
// This primer is a tutorial on Chapel error handling.
//
/*
  Overview
  --------

  The Chapel language supports ``throw``, ``try``, ``try!``, ``catch``, and
  ``throws`` which are described below. Chapel supports several error handling
  modes, and in particular the default for code not in an explicit module
  declaration is suitable only for prototype code.
 */

/*
  .. _primers-errorHandling-throwing:

  Throwing Errors
  ---------------

  Errors may be thrown from a function to its callee with a ``throw``
  statement. For a function to throw an error, its signature must include
  a ``throws`` declaration. The declaration is put after the return
  type and before any ``where`` clauses.
 */

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

/*
  .. _primers-errorHandling-handling:

  Handling Errors
  ---------------

  There are three ways to handle an error:

   1. Halt if the error occurs with ``try!``.
   2. Handle the error with ``catch`` blocks.
   3. Propagate the error out of the current function with ``throws``.


  .. _primers-errorHandling-trybang:

  Halting on error with ``try!``
  ++++++++++++++++++++++++++++++

  Marking a throwing call with ``try!`` causes the compiler to
  generate code to halt the program if that call results in an error.
 */

proc haltsOnError():int {
  // the try! next to the throwing call requests
  // the program halt if an error occurs.
  return try! canThrow(0);
}

proc haltsOnErrorBlock() {
  try! {
    canThrow(1);
    canThrow(0);
  }
}

/*
  .. _primers-errorHandling-catch:

  Handling an error with ``catch``
  ++++++++++++++++++++++++++++++++

  When an error is raised by a call in a ``try`` block, the rest of the
  block is abandoned and control flow is passed to the ``catch`` clauses.


  'catch' clauses
  ***************

  ``catch`` clause type filters are evaluated in order. If a ``catch``
  clause's type filter matches, then its block is executed to the exclusion
  of the others. Hence there is no notion of best match, only a first match.

  If no type filter is present on a catch clause, or if no variable is
  present at all, then it is a catchall clause which matches all errors.
 */

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

/*
  'try!' and 'catch'
  ******************

  ``try!`` is also available in block form and can have catch blocks.
  If the ``catch`` clauses after a ``try!`` don't handle the error,
  the program halts.
 */

proc catchingErrorsHalt() {
  try! {
    var x = alwaysThrows(-1);
    writeln("never reached");
  } catch e:FileNotFoundError {
    writeln("caught a file not found error");
  }
  // errors other than FileNotFoundError cause a halt
}

/*
  Nested 'try'
  ************

  It is also possible for ``try`` blocks to direct their errors to the
  ``catch`` clauses of an enclosing ``try``, if a matching ``catch`` clause
  is not available.
 */

proc nestedTry() {
  try {
    try {
      alwaysThrows(0);
    } catch e: PrimerError {
      writeln("caught a PrimerError");
    }
    writeln("never reached");
  } catch {
    writeln("caught an Error from inner try");
  }
}

/*
  .. _primers-errorHandling-propagation:

  Propagating an error with ``throws``
  ++++++++++++++++++++++++++++++++++++

  A function marked ``throws`` can pass along an error
  raised by a throwing function it calls.


  After 'catch' clauses
  *********************

  Propagation can occur when no matching ``catch`` clause is found for an
  error raised in a ``try`` block.
 */

proc catchingErrorsPropagate() throws {
  try {
    var x = alwaysThrows(-1);
    writeln("never reached");
  } catch e:FileNotFoundError {
    writeln("caught an error");
  }
  // errors other than FileNotFoundError propagate
}

/*
  catch-less 'try'
  ****************

  A logical extension of the above is the case where no ``catch`` blocks are
  attached to the ``try``. In this case the ``try`` keyword marks throwing
  calls to clarify control flow.
 */

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

/*
  'try' expressions
  *****************

  ``try`` and ``try!`` are also available as expressions to clarify
  control flow for assignments and returns. These may not be used
  with ``catch`` clauses.
*/

proc expressionTry():int throws {
  var x = try canThrow(1);
  writeln(x);

  return try canThrow(0);
}

/*
  For more information on enforcing clear control flow with the compiler,
  see :ref:`primers-errorHandling-strict` below.


  .. _primers-errorHandling-complete:

  complete handling
  +++++++++++++++++

  For a function to handle errors from its calls without itself throwing,
  its ``try``/``catch`` must be *complete*. This may be accomplished
  in two ways:

  1. A catchall clause on ``try``. This prevents ``try`` from
  propagating the error out of the function as described above.
 */

proc warnsOnError(i: int): int {
  try {
    alwaysThrows(i);
  } catch e {
    writeln("Warning: caught a error ", e);
  }
}

/*
  2. ``try!`` instead of ``try``. This will halt the program if no matching
  ``catch`` clause is found, instead of propagating.
 */

proc haltsOnError(i: int): int {
  try! {
    canThrow(i);
  } catch e: PrimerError {
    writeln("caught a PrimerError");
  }
}

/*
  .. _primers-errorHandling-multilocale:

  Multilocale
  -----------

  Errors can be thrown within `on` statements. In that event, the error
  will be propagated out of the `on` statement.
 */

proc handleFromOn() {
  try {
    on Locales[0] {
      canThrow(1);
    }
  } catch {
    writeln("caught from Locale 0");
  }
}

/*
  .. _primers-errorHandling-parallel:

  Parallelism
  -----------

  TaskErrors
  ++++++++++

  If multiple tasks can throw errors, the errors from those tasks will be
  collected into a built-in error type, `TaskErrors`. It can be iterated on
  and filtered for different kinds of errors.


  'begin'
  +++++++

  Errors can be thrown within a `begin` statement. In that event, the error
  will be propagated to the `sync` statement that waits for that task.
 */

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

/*
  'coforall', 'cobegin'
  +++++++++++++++++++++

  Errors can be thrown from ``coforall`` and ``cobegin`` statements, handled
  as ``TaskErrors``.
 */

proc handleFromCobegin() {
  try! {
    writeln("before cobegin block");
    cobegin {
      throw new PrimerError();
      throw new PrimerError();
    }
    writeln("after cobegin block");
  } catch errors: TaskErrors {
    for e in errors {
      writeln("Caught task error e ", e.message());
    }
  }
}

proc handleFromCoforall() {
  try! {
    writeln("before coforall block");
    coforall i in 1..2 {
      throw new PrimerError();
    }
    writeln("after coforall block");
  } catch errors: TaskErrors {
    for e in errors {
      writeln("Caught task error e ", e.message());
    }
  }
}

/*
  'forall'
  ++++++++

  Errors can be thrown from ``forall`` too. The implementation of
  ``TaskErrors`` prevents nested ``forall`` statements from producing
  nested ``TaskErrors``. Instead, the nested errors will flatten into the
  outer loop's ``TaskErrors``.
 */

proc handleFromCoforall() {
  try! {
    writeln("before forall block");
    forall i in 1..2 {
      forall j in 1..2 {
        throw new PrimerError();
      }
    }
    writeln("after forall block");
  } catch errors: TaskErrors { // not nested
    for e in errors {
      writeln("Caught task error e ", e.message());
    }
  }
}

/*
  .. _primers-errorHandling-errors:
  .. _primers-errorHandling-classes:

  Creating New Error Types
  ------------------------
 */

//
// Errors in Chapel are implemented as classes, with a base class ``Error``
// defined in the standard modules. ``Error`` may be used directly, and new
// hierarchies may be created from it.
//
// A hierarchy for system errors is included in the ``SysError`` module,
// accessed with a ``use`` statement.
//

use SysError;

class PrimerError : Error { }


/*
  .. _primers-errorHandling-modes:

  Error Handling Modes
  --------------------

  Chapel currently supports 3 error handling modes: fatal, relaxed, and strict.
  It is fully permissible to write for a stricter error handling mode while
  using a looser one -- for example, code that compiles in strict mode
  will also compile in relaxed or fatal mode.

  Developers can select error handling mode by:

   * for *fatal mode*:

     * writing code in an implicit module
     * writing code in a ``prototype module``
     * passing the ``--permit-unhandled-module-errors`` flag on the ``chpl``
       command line

   * for *relaxed mode*:

     * writing code inside of a `module` declaration

   * for *strict mode*:

     * writing code inside of a module marked with a pragma


  .. _primers-errorHandling-fatal:

  Fatal Mode for Implicit and Prototype Modules
  +++++++++++++++++++++++++++++++++++++++++++++
 */

//
// In implicit and prototype modules, it is not necessary to explicitly handle
// errors from a function that throws. If an error is thrown and the calling
// function throws, the error will be propagated out of the function.  However,
// if an error is thrown and the calling function does not throw, the program
// will halt.
//
// This is the *fatal* error handling mode.
//
// An *implicit module* is the module the compiler creates to store
// code not in a ``module`` declaration. In particular, all of the
// statements above this point in this file are in an implicit module called
// errorHandling. Since the below statements are also not in a ``module``
// declaration, they are also in an *implicit module*.

canThrow(1); // handling can be omitted; halts if an error occurs

proc throwsErrorsOn() throws {
  // error propagates out of this function
  canThrow(-1);
}

proc doesNotThrowErrorsOn() {
  // causes a halt if called
  alwaysThrows();
}

// Fatal error mode can also be activated for explicit modules using the
// ``prototype module`` declaration:

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

/*
  .. _primers-errorHandling-relaxed:

  Relaxed Mode for Explicit Modules
  +++++++++++++++++++++++++++++++++
 */

//
// In non-prototype explicit modules, it is necessary to handle errors if the
// calling function does not throw. If the calling function does
// throw, then the error will be propagated out as with implicit modules.
//
// This is the *relaxed* error handling mode.
//

module ProductionModule {
  // this line would cause compilation error since the error is not handled
  //canThrow(1);

  proc throwsErrorsOn() throws {
    // any error thrown in thrownErrors will propagates out of this function
    alwaysThrows();
  }
}

use ProductionModule;

/*
  .. _primers-errorHandling-strict:

  Strict Mode
  +++++++++++
 */

//
// It is possible to specify a more restricted error handling mode within a
// module scope using a pragma. With it, it is necessary to explicitly mark all
// throwing calls, whether or not the calling function throws.
//
// This is the *strict* error handling mode.
//
// Statements containing throwing calls must be enclosed in a ``try`` block.

pragma "error mode strict"
module StrictModule {

/*
  .. _primers-errorHandling-alternate:

  alternate 'try' forms
  +++++++++++++++++++++
 */

//
// The major difference between relaxed and strict mode is that throwing
// calls in throwing functions need to be marked. This may be accomplished
// with the catch-less ``try`` block, the single statement ``try``, or the
// assignment ``try``.
//
// It is also possible to use ``try!`` in these forms.
//

  proc throwsErrorsOn() throws {
    try {
      alwaysThrows();
      writeln("never reached");
    }
  }

  proc alsoThrowsErrorsOn() {
    try alwaysThrows();
  }

  proc assignmentTry() throws {
    var x = try alwaysThrows();
    writeln(x);
  }
}

/*
  .. _primers-errorHandling-future:

  Future Changes
  --------------

  * Error handling does not work yet with initializers.
  * It is not yet decided whether or not it will be possible to
    throw from a `deinit` function.
  * In the future, there will be a better way to select strict mode.
  * Errors can't currently be thrown from iterators that are not inlined by the
    compiler

    * note that iterators with a single yield as well as leader and standalone
      iterators are generally inlined by the compiler

 */


