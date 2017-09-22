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
  Working with Errors
  -------------------

  .. _primers-errorHandling-throwing:

  Throwing Errors
  +++++++++++++++
 */

//
// Errors may be thrown from a function to its callee with a ``throw``
// statement. For a function to throw an error, its signature must include
// a ``throws`` declaration. The declaration is put after the return
// type and before any ``where`` clauses.
//

/*
   .. note::

     ``Error`` currently has a constructor accepting a string argument,
     which is the error message displayed if the error is not caught.
     Subclasses of ``Error`` can override ``proc message`` to specify what
     error message should be displayed with the error. The ``Error`` class
     may not store a string argument in the future.
*/

proc canThrow(i: int): int throws {
  if i < 0 then
    throw new Error("i was less than zero");

  return i + 1;
}

proc alwaysThrows():int throws {
  throw new Error("test error");
  // never reached
  return 1;
}

/*
  .. _primers-errorHandling-handling:

  Handling Errors
  +++++++++++++++

  Writing code that handles an error amounts to choosing one
  of these three possibilities:

   1. Halting if an error occurs with ``try!``.
   2. Propagating the error out of the current function with ``throws``.
   3. Handling the error with ``catch`` blocks.

  Halting on Error with ``try!``
  ******************************

  Marking a throwing call with ``try!`` causes the compiler to
  generate code to halt the program if that call results in an error.
 */

proc haltsOnError():int {
  // the try! next to the throwing call requests
  // the program halt if an error occurs.
  return try! canThrow(0);
}

/*
  Propagating an Error with ``throws``
  ************************************

  A function marked with ``throws`` can pass along an error
  raised by a throwing function it calls. The ``try`` keyword
  is available to mark such throwing calls to make control flow
  clear.
 */

proc propagatesError():int throws {
  // returns early if an error was thrown;
  // we could indicate that fact with `try`
  canThrow(0);

  try canThrow(0);

  try {
    canThrow(0);
  }

  var x = try canThrow(1);
  writeln(x);

  return try canThrow(0);
}

// To make control flow clear, it is sometimes desireable to mark all of the
// calls that could throw. For this purpose, ``try`` and ``try!`` are available
// as expressions. See :ref:`primers-errorHandling-strict` below.

proc assignmentTry():int throws {
  var x = try canThrow(1);
  writeln(x);
}

proc returnTry() {
  return try! canThrow(1);
}

/*
  Handling an error with ``catch``
  ********************************


  Errors may be handled using ``try`` and ``catch``. When an error is raised by
  a call, the rest of the block is abandoned and control flow is passed to the
  list of ``catch`` clauses.
 */

proc catchingSomeErrors() throws {
  try {
    var x = alwaysThrows(-1);
    writeln("never reached");
  } catch e:FileNotFoundError {
    writeln("caught an error");
  }
  // if the catch clauses after a try didn't handle the error, it is
  // propagated to the enclosing scope - in this case out of the
  // function.

  // try! is also available in block form and can
  // have catch blocks
  try! {
    var x = alwaysThrows(-1);
    writeln("never reached");
  } catch e:FileNotFoundError {
    writeln("caught a file not found error");
  }
  // if the catch clauses after a try! didn't handle the error
  // the program halts.
}

/*
  .. _primers-errorHandling-propagation:

  propagation
  +++++++++++
 */

//
// Error propagation occurs when no matching ``catch`` clause is found for an
// error raised in a ``try`` block. This can be accomplished in two ways:
//
// 1. Out of the enclosing function, if the function is declared `throws`.
//

proc propagateOut() throws {
  try {
    alwaysThrows(0);
  } catch e: PrimerError {
    writeln("caught a PrimerError");
  }
  // other errors get thrown
}

//
// 2. To the ``catch`` clauses of an enclosing ``try``.
//
proc propagateNested() {
  try {
    try {
      alwaysThrows(0);
    } catch e: PrimerError {
      writeln("caught a PrimerError");
    }
    writeln("never reached");
  } catch {
    writeln("caught an Error");
  }
}


/*
  .. _primers-errorHandling-catch:

  'catch' clauses
  +++++++++++++++
 */

//
// ``catch`` clause type filters are evaluated in order. If a ``catch``
// clause's type filter matches, then its block is executed to the exclusion
// of the others. Hence there is no notion of 'best match', only a first
// match. If no type filter is present on a catch clause, or if no variable is
// present at all, then it is a catchall clause which matches all errors.
//

proc multipleCatchClauses() {
  try {
    alwaysThrows(0);
  } catch e: PrimerError {
    writeln("caught a PrimerError");
  } catch e {
    writeln("caught an Error");
  }
}

/*
  .. _primers-errorHandling-complete:

  complete handling
  +++++++++++++++++


  For a function to successfully handle errors from its calls without itself
  throwing, its ``try``/``catch`` must be *complete*. This may be
  accomplished in two ways:

  1. A catchall clause on the ``try``. This prevents ``try`` from trying to
  propagate the error out of the function as described above.

  2. ``try!`` instead of ``try``. This will halt the program if no matching
  ``catch`` clause is found, instead of propagating to the enclosing scope.

 */

proc warnsOnError(i: int): int {
  try {
    alwaysThrows(i);
  } catch e {
    writeln("Warning: caught a error ", e);
  }
}

proc haltsOnError(i: int): int {
  try! {
    canThrow(i);
  } catch e: PrimerError {
    writeln("caught a PrimerError");
  }
}

/*
  .. _primers-errorHandling-parallel:

  Multilocale and Parallelism
  +++++++++++++++++++++++++++

  Errors can be thrown within `on` statements. In that event, the error
  will be propagated out of the `on` statement.

  Errors can be thrown within a `begin` statement. In that event, the error
  will be propagated to the `sync` statement that waits for that task.

  Errors can be throw from ``coforall`` and ``cobegin`` statements.

  Errors can be throws from ``forall`` too.

 */


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


