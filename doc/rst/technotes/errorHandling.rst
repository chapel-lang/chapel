.. _readme-errorHandling:

==============
Error Handling
==============

This README describes the early version of error handling implemented in Chapel
1.15. For a complete explanation of the final design and rationale, refer to
`CHIP 8 <https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/8.rst>`_.


Errors
------
A base class ``Error`` is defined in the Chapel standard modules.
It includes an initializer accepting a string argument, which is the error
message to display if the error is not caught.

Users may use the provided ``Error`` directly, or create their own error
hierarchy with ``Error`` at its root.

.. code-block:: chapel

  class MyError: Error {}

  class MyIntError: MyError {
    var i: int;
  }

While defining a standard set of ``Error`` subclasses is planned for the
future, Chapel only provides ``Error`` at present.


Throwing Errors
---------------
``throw`` statements throw errors from a function to its callee.

.. code-block:: chapel

  // throwing a newly created error
  throw new Error("error message here");

  // throwing an error stored in a variable
  var e: Error;
  // ... set e ...
  throw e;

If a function may throw an error, it must be declared with ``throws``.

.. code-block:: chapel

  proc mayThrowErrors() throws { ... }
  proc mayThrowErrorsAlso(): A throws where ... { ... }
  proc mayNotThrowErrors() { ... }


Handling Errors
---------------

Chapel supports two modes for error handling: strict mode for production code,
and default mode for drafting and iterating. Strict mode forces users to
explicitly mark throwing calls and handle errors, clarifying the program's
control flow to the user and the reader. Default mode loosens those
requirements with implicit error handling.

Strict mode is enabled by passing ``--strict-errors`` to the Chapel compiler,
otherwise the compiler will use default mode.


Strict Mode
+++++++++++

``try``/``try!`` blocks and their associated ``catch`` clauses allow the user
to create a handler for a throwing call.

* Statements that contain throwing calls must be enclosed in a ``try`` block.
  
  * If an error is raised by a call in a ``try`` block, the rest of the block
    is abandoned and control flow is passed to the list of ``catch`` clauses.
 
* Each ``catch`` clause may contain a type filter. If the error's type matches
  the type filter, the block associated with that ``catch`` clause (and only
  that block) will be executed. 

  * These filters will be evaluated in order of declaration, so even if a more
    exact filter is present in the list, it will not be selected if a matching
    general filter precedes it.

  * If no filter is included, or only an error variable name is provided, then
    all errors will match. A clause with such a filter must be declared at the
    end of the list, since clauses that follow it could never be executed.

  * Unless the ``catch`` clause block contains a ``throw``, execution will
    resume after the ``try`` block.

.. code-block:: chapel

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

* If none of the type filters matches the error, a catchall block will be
  inserted. Its functionality will depend on whether a ``try`` or ``try!``
  was used.
 
  * If ``try`` was used, the error will be propagated. (More on this below.)

  * If ``try!`` was used, the program will ``halt()``.

* ``try`` blocks may be used without any ``catch`` clauses, or without brackets
  for single statements.

.. code-block:: chapel

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

.. code-block:: chapel

  proc ioSetup() throws {
    try {
      ioSourceOne();
      ioSourceTwo();
      ioSourceThree();
    }
  }

2. To the catch blocks of an enclosing ``try``.

.. code-block:: chapel

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


Default Mode
++++++++++++

In addition to the features in the *Strict Mode* section, default mode allows
throwing calls to be called without being enclosed by a ``try`` or ``try!``
block. This is done by inserting an implicit handler after the call:

* If the caller is declared with ``throws``, any error encountered will be
  thrown in turn.

* If the caller is not declared with ``throws``, the program will ``halt()``
  if any error is encountered.


Current Limitations
-------------------
* In general it is not possible to use error handling with parallel or
  multilocale constructs.
  
  * ``forall``, ``begin``, ``on``, etc.

  * However it is possible to use throwing calls handled entirely within the
    scope of a given construct, without propagation.

* Errors may not be generic classes. 

* Virtual methods cannot throw. In practice, this means that while a typical
  class may contain throwing methods, a class that is extended or a class
  that is extending another class may not contain throwing methods.
