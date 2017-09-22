// Error Handling

//
// This primer is a tutorial on Chapel error handling.
//

/*
  .. _primers-errorHandling-errors:

  Errors as Classes
  -----------------
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
  .. _primers-errorHandling-throwing:

  Throwing Errors
  ---------------
 */

//
// Errors may be thrown from a function to its callee with a ``throw``
// statement. For a function to throw an error, its signature must include
// a ``throws`` declaration. The declaration is put after the return
// type and before any ``where`` clauses.
//
// Note: ``Error`` has a constructor accepting a string argument, which is the
// error message displayed if the error is not caught.
//

proc throwsErrors(i: int): int throws {
  if i < 0 then
    throw new Error("i was less than zero");

  return i + 1;
}

/*
  .. _primers-errorHandling-fatal:

  Implicit Modules and Fatal Mode
  -------------------------------
 */

//
// In implicit modules, it is not necessary to explicitly handle errors
// from a function that throws. If an error is thrown and the calling function
// throws, the error will be thrown on. However, if an error is thrown and the
// calling function does not throw, the program will halt.
//
// This is the *fatal* error handling mode.
//

throwsErrors(1); // no handling needed

proc throwsErrorsOn() throws {
  // error is thrown on
  throwsErrors(0);
}

proc doesNotThrowErrorsOn() {
  // would halt if called
  throwsErrors(0);
}

/*
  .. _primers-errorHandling-relaxed:

  Explicit Modules and Relaxed Mode
  ---------------------------------
 */

//
// In explicit modules, it is necessary to explicitly handle errors if the
// calling function does not throw. However, if the calling function does
// throw, then the error will be thrown on like in implicit modules.
//
// This is the *relaxed* error handling mode.
//

module M {
  // no handling needed
  throwsErrors(1);

  // error is thrown on
  proc throwsErrorsOn() throws {
    throwsErrors(0);
  }

/*
  .. _primers-errorHandling-handling:

  handling errors with 'try'
  ++++++++++++++++++++++++++
 */

//
// Errors may be handled using ``try`` and ``catch``. Statements containing
// throwing calls must be enclosed in a ``try`` block. When an error is
// raised by a call, the rest of the block is abandoned and control flow is
// passed to the list of ``catch`` clauses.
//

  proc doesNotThrowErrorsOn() {
    try {
      var x = throwsErrors(0);
      writeln("never reached");
    } catch {
      writeln("caught an error");
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
      throwsErrors(0);
    } catch e: PrimerError {
      writeln("caught a PrimerError");
    } catch e {
      writeln("caught an Error");
    }
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
      throwsErrors(0);
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
        throwsErrors(0);
      } catch e: PrimerError {
        writeln("caught a PrimerError");
      }
      writeln("never reached");
    } catch {
      writeln("caught an Error");
    }
  }

/*
  .. _primers-errorHandling-complete:

  complete handling
  +++++++++++++++++
 */

//
// For a function to successfully handle errors from its calls without itself
// throwing, its ``try``/``catch`` must be considered complete. This may be
// accomplished in two ways:
//
// 1. A catchall clause on the ``try``. This prevents ``try`` from trying to
// propagate the error out of the function as described above.
//
// 2. ``try!`` instead of ``try``. This will halt the program if no matching
// ``catch`` clause is found, instead of propagating to the enclosing scope.
//

  proc haltsOnError(i: int): int {
    try! {
      throwsErrors(i);
      throwsErrors(i - 1);
    } catch e: PrimerError {
      writeln("caught a PrimerError");
    }
  }
}

/*
  .. _primers-errorHandling-strict:

  Strict Mode
  -----------
 */

//
// It is possible to specify a more restricted error handling mode within a
// module scope using a pragma. With it, it is necessary to explicitly mark
// all throwing calls, whether or not the calling function throws.
//
// This is the *strict* error handling mode.
//

module S {
  pragma "error mode strict"

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
      throwsErrors(0);
      writeln("never reached");
    }
  }

  proc alsoThrowsErrorsOn() {
    try throwsErrors(1);
  }

  proc assignmentTry() throws {
    var x = try throwsErrors(1);
    writeln(x);
  }
}

/*
  .. _primers-errorHandling-modes:

  More on Error Modes
  -------------------
 */

//
// It is fully permissible to write for a stricter error handling mode while
// using a looser one -- for example, all error handling code that passes
// strict mode will also pass relaxed and fatal mode.
//
// Any arbitrary error handling mode may be used in any module, implicit or
// explicit, through the use of ``pragma "error mode <mode>"``. Above is an
// example of enabling strict mode in an explicit module, and below is an
// example of enabling fatal mode in an explicit module.
//

module F {
  pragma "error mode fatal"
}

//
// The default mode for explicit modules can also be overridden to fatal using
// the ``--permit-unhandled-module-errors`` flag on the ``chpl`` command line.
//

/*
  .. _primers-errorHandling-constructs:

  Multilocale and Parallelism
  ---------------------------
 */

/*
  .. _primers-errorHandling-classes:

  Classes
  -------
 */


