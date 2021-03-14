// Error Handling
//
// This primer is a short tutorial on Chapel error handling. A detailed
// description can be found in the :ref:`readme-errorHandling` technote.
//

/*
  .. _primers-errorHandling-handling:

  Handling Errors
  ---------------
  Several important standard library functions throw errors so that developers
  can handle the errors as they see fit. This is especially useful for
  recovering from errors in :mod:`IO`. The ``try`` and ``catch`` constructs
  enable handling of errors. Any errors caught by a ``catch`` block are
  ``owned`` errors and can be rethrown. ``try!`` is available to halt if an
  error is thrown but not handled.
 */

use IO;

const f1 = "input.txt";
const f2 = "backup_input.txt";

var f: file;

  // halts on error
  // f = try! open(f1, iomode.r);

try {
  f = open(f1, iomode.r);
  // if open() raises an error, jump to the catch block
  writeln("everything is fine");
} catch {
  // catch block are used to handle errors
  writeln("an error occurred");
}

try {
  f = open(f1, iomode.r);
} catch e: FileNotFoundError {
  // catch block can be directed to only handle certain errors
  // the error caught will be `owned FileNotFoundError` in this case
  writeln("Warning: ", f1, " does not exist");
  try! {
    f = open(f2, iomode.r);
  } catch e: FileNotFoundError {
    writeln("Warning: ", f2, " does not exist");
  }
  // halts if a different error is returned
} catch {
  // catchall is needed because main() does not throw
  writeln("unknown error");
}

/*
  .. _primers-errorHandling-throwing:

  Throwing Errors
  ---------------

  Developers can also write their own throwing functions, throwing and
  consuming errors. While there are built-in :class:`SysError.SystemError`
  types, developers can also create their own hierarchies.

  Only ``owned`` instances that are subtypes of ``Error`` can be thrown.
 */

class EmptyFilenameError : Error {
  proc init() {
  }
}

proc checkFilename(f_name: string) throws {
  if f_name.isEmpty() then
    throw new EmptyFilenameError();
}

proc openFilename(f_name: string) throws {
  var f: file;

  try {
    f = open(f, iomode.r);
  } catch e: FileNotFoundError {
    writeln("Warning: ", f, " does not exist");
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
