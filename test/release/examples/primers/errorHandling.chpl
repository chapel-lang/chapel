// Error Handling
//
// This primer is a short tutorial on Chapel error handling. A detailed
// outline can be found in the :ref:`readme-errorHandling` technote.
//

/*
  .. _primers-errorHandling-handling:

  Handling Errors
  ---------------

  Standard library functions that used to halt now throw errors for
  developers to handle as they see fit. This is especially useful for
  recovering from errors in :mod:`IO`.
 */

use IO;

const f1 = "fake.txt";
const f2 = "still_fake.txt";

var f: file;

  // halts on error
  // f = try! open(f1, iomode.r);

try {
  f = open(f1, iomode.r);
} catch {
  writeln("an error occurred");
}

try {
  f = open(f1, iomode.r);
} catch e: FileNotFoundError {
  writeln(f1 + " does not exist");
  try! {
    f = open(f2, iomode.r);
  } catch e: FileNotFoundError {
    writeln(f2 + " does not exist");
  } // halts if a different error is returned
} catch {
  writeln("unknown error");
}

/*
  .. _primers-errorHandling-throwing:

  Throwing Errors
  ---------------

  Developers can also write their own throwing functions, throwing and
  consuming errors. While there are built-in :class:`SysError.SystemError`
  types, developers can also create their own hierarchies.
 */

class EmptyFilenameError : Error { }

proc checkFilename(f_name: string) throws {
  if f_name.isEmptyString() then
    throw new EmptyFilenameError();
}

proc openFilename(f_name: string) throws {
  var f: file;

  try {
    f = open(f, iomode.r);
  } catch e: FileNotFoundError {
    writeln(f + " does not exist");
  } // throws all other errors

  return f;
}

/*
  .. _primers-errorHandling-modules:

  Modules
  -------

  Implicit modules, prototype modules, and explicit modules have different
  minimum requirements for error handling. This encourages rapid prototyping
  at the outset while enforcing safety in production.

  Implicit modules will halt if an error is unhandled.
 */

checkFilename(f1); // halts on error

proc doesNotThrow() {
  checkFilename(f2); // halts on error
}

proc throwOn() throws {
  checkFilename(f2); // throws on error
}

/*
  Prototype modules will also halt if an error is unhandled.
 */

prototype module P {
  checkFilename(f1); // halts on error

  proc doesNotThrow() {
    checkFilename(f2); // halts on error
  }

  proc throwOn() throws {
    checkFilename(f2); // throws on error
  }
}

/*
  Explicit modules require errors to be completely handled, except when the
  calling function also throws.
 */

module R {
  // not permitted, error must be handled completely
  // checkFilename(f1);
  try! checkFilename(f1);

  proc doesNotThrow() {
    // not permitted, error must be handled completely
    try {
      checkFilename(f2);
    } catch {
      writeln("handled completely");
    }
  }

  proc throwOnExplicit() throws {
    checkFilename(f2); // still works, throws on error
  }
}

/*
  :ref:`technote-errorHandling-strict` may be enabled on any module,
  requiring that all throwing calls be marked with ``try``.
 */
