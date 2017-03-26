.. _readme-errorHandling:

==============
Error Handling
==============

This README describes the error handling feature as currently implemented.


Errors
------
A base class ``Error`` is defined in the Chapel standard modules.

::

  class Error {
    var msg: string;

    proc Error(_msg: string) {
      msg = _msg;
    }
  }

Users may use the provided ``Error`` directly, or create their own error
hierarchy with ``Error`` at its root.

::

  class MyError: Error {}

  class MyIntError: MyError {
    var i: int;
  }

Chapel does not provide a standard set of ``Error`` classes.


Throwing Errors
---------------
``throw`` statements throw errors from a function to its callee.

::

  throw new Error("with a message");

  var e: Error;
  // ... set e ...
  throw e;

If a function may throw an error, it must be declared with ``throws``.

::

  proc mayThrowErrors() throws { ... }
  proc mayThrowErrorsAlso(): A throws where ... { ... }
  proc mayNotThrowErrors() { ... }


Handling Errors
---------------

Default Policy
++++++++++++++

When a throwing function is called, the default error handling policy of the
calling function is as follows:

* If the calling function is declared with ``throws``, any error encountered
  will be thrown in turn.

* If the calling function is not declared with ``throws``, the program will
  ``halt()`` if any error is encountered.


Specified Policy
++++++++++++++++

``try``/``try!`` blocks and their associated ``catch`` clauses allow the user
to specify an explicit error handling policy for a throwing call.

* Statements that contain throwing calls should be enclosed in a ``try`` block.
  
  * If an error is raised by a call in a ``try`` block, the rest of the block
    is abandoned and control flow is passed to the list of ``catch`` clauses.
 
* Each ``catch`` clause may contain an associated type filter. If the error's
  type matches the type filter, the block associated with that ``catch`` clause
  (and only that block) will be executed. 

  * These filters will be evaluated in order of declaration, so even if a more
    exact filter is present in the list, it will not be selected if a matching
    general filter precedes it.

  * If no filter is included, or only an error variable name is provided, then
    all errors will match. Combined with the rule above, a clause with such a
    filter must be declared at the end of the list.

  * Unless the ``catch`` clause block contains a throw, execution will resume
    after the ``try`` block.

::

  try {
    mayThrowErrors();
    mayThrowErrorsAlso();
  } catch e: MyIntError {
    writeln("caught MyIntError " + e.i + ", continue");
  } catch e: MyError {
    throw e; 
  } catch {
    writeln("unexpected error");
  }

* If none of the type filters matches the error, the final policy will depend
  on whether a ``try`` or ``try!`` was used.
 
  * If ``try`` was used, the error will be propagated. (More on this below.)

  * If ``try!`` was used, the program will ``halt()``.

* ``try`` blocks may be used without any ``catch`` clauses, or without brackets
  for single statements, using the same policy rules as above.

::

  try {
    ioSourceOne();
    ioSourceTwo();
    ioSourceThree();
  }

  try! cannotFail();
  try! var x = requiredVariable();


Propagation
+++++++++++
Propagation means that the error will be passed off to a set of handlers
beyond the current scope. This can be accomplished in two ways:

1. Out of the enclosing function, if the function is declared with ``throws``.

::

  proc ioSetup() throws {
    try {
      ioSourceOne();
      ioSourceTwo();
      ioSourceThree();
    }
  }

2. To the catch blocks of an enclosing ``try``.

::

  try {
    writeln("outer try");
    try {
      writeln("inner try");
      throw new MyError();
    } catch err: MyIntError {
      writeln("MyError should not be caught here");
    }
  } catch err: MyError {
    writeln("caught MyError");
  }


Strict Error Mode
+++++++++++++++++
When the ``--strict-errors`` flag is passed to the Chapel compiler, it will
disallow the *Default Policy* described above. As such, every throwing call
must be handled by a *Specified Policy*. This forces throwing calls to be 
explicitly marked and handled by the user.


Current Limitations
-------------------
* In general it is not possible to use error handling together with parallel or
  multilocale constructs.
  
  * ``forall``, ``begin``, ``on``, etc.

  * One exception: throwing calls handled entirely within the scope of a
    given construct, without propagation.

* Errors may not be generic classes. 

* Virtual methods cannot throw.
