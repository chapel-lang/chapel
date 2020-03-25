.. _readme-partial-instantiations:

.. default-domain:: chpl

======================
Partial Instantiations
======================

Overview
--------

A generic class or record can be fully instantiated, fully generic, or
partially instantiated. A fully instantiated type expression has concrete types
for all generic fields. A fully generic type expression is one in which none of
the generic fields have been instantiated. A partially instantiated type
expression has concrete types for one or more, but not all, generic fields.
Partial instantiations can be used to more finely constrain a procedure's
arguments, constrain valid types when initializing variables, or for
conveniently creating other instantiations through incremental instantiation.

Creating Partial Instantiations
-------------------------------

A generic type is instantiated by a call to the type constructor, which accepts
an argument for each generic field in the type in field declaration order.
These arguments can be passed positionally or by using named expressions and
the names of fields. In order to support partial instantiations of generic
types the language allows for arguments to be omitted from the type constructor
call. The resulting instantiation will have its own type constructor that
accepts arguments for the remaining generic fields. For example:

.. code-block:: chapel

  record R {
    type X;
    type Y;
    param p : int;
  }

  type A = R; // 'A' is simply an alias for the generic type 'R'

  // B's type constructor accepts two arguments for fields 'Y' and 'p'
  type B = R(int); // instantiate field 'X'
  writeln(B:string); // "R(int(64))"

  // C's type constructor accepts one argument for field 'Y'
  type C = B(p=5); // instantiate field 'p'
  writeln(C:string); // "R(int(64),p=5)"

  // D is a fully instantiated type and has no type constructor
  type D = C(real); // instantiate field 'Y'
  writeln(D:string); // "R(int(64),real(64),p=5)"

A generic field might depend on a previously declared field to describe its
type. In such cases the later generic field may not be instantiated before
the earlier generic field:

.. code-block:: chapel

  // dependent.chpl
  record R {
    type T;
    param p : T;
  }

  type A = R(p=5);

Output:

.. code-block:: text

  dependent.chpl:7: error: Unable to resolve partial instantiation 'R(p=5)'
  dependent.chpl:7: note: Instantiation of field 'p' depends on uninstantiated fields: 'type T'

Generic fields may have default values that are used to instantiate the field
when the user does not explicitly provide an argument in the type constructor.
In order to allow these fields to remain generic the user can pass ``?`` as an
argument to the type constructor. This argument indicates that generic fields
with default values should not be automatically instantiated.

.. code-block:: chapel

  record MyRange {
    type idxType = int;
    param stridable = false;
  }

  // all arguments must be of type ``MyRange(int,false)``
  proc foo(arg : MyRange) { ... }

  // Any instantiation of ``MyRange`` is accepted
  proc bar(arg : MyRange(?)) { ... }

  // Any ``MyRange`` accepted provided the first field is ``uint``
  proc baz(arg : MyRange(uint, ?)) { ... }

There are some restrictions that accompany the ``?`` argument. First is that
``?`` may only appear once in a type constructor call. A user might be
tempted to use ``?`` as a positional argument in order to more easily pass
an argument to a specific field, e.g. ``R(?, ?, int)``. In order to reduce
confusion and support ``?`` as a convenient catch-all argument, ``?`` may not
be used multiple times in a type constructor call. For example:

.. code-block:: chapel

  record R {
    type X = int;
    type Y = real;
    type Z = string;
  }

  // Error! '?' may not be used multiple times in a type constructor call
  // proc foo(arg : R(?, ?, uint)) { ... }
  // instead, use named-expressions:
  proc foo(arg : R(?, Z=uint)) { ... }

  // alternatively, use the named-expression first:
  proc bar(arg : R(Z=uint, ?)) { ... }

Furthermore, once ``?`` appears in a type constructor call all later arguments
in the same call must used named expressions. This restriction emphasizes that
``?`` may not be used as a positional argument. For example:

.. code-block:: chapel

  record R {
    type A = int;
    type B = real;
  }

  // Error! second argument must use a named expression
  // proc foo(arg : R(?, string)) { ... }
  // instead...
  proc foo(arg : R(?, B=string)) { ... }

Detecting Partial Instantiations
--------------------------------

There are a few features that allow for the detection of partial
instantiations.

The first feature is a standard library function named :proc:`~Types.isGeneric`
that will simply return ``true`` if its argument is a partial instantiation or
a fully-generic type:

.. code-block:: chapel

  record R {
    type T;
    param p : int;
  }

  writeln(isGeneric(R));            // true
  writeln(isGeneric(R(int)));       // true
  writeln(isGeneric(R(p=42)));      // true
  writeln(isGeneric(R(string, 5))); // false

Users may also query individual fields to determine whether the field has
been instantiated by comparing the field against ``?``:

.. code-block:: chapel

  record R {
    type T;
    param p : int;
  }

  type A = R(int);
  
  // '?' can be used to compare against either type or param fields
  writeln(A.T == ?); // false
  writeln(A.p == ?); // true

Finally, a Reflection function named :proc:`~Reflection.isFieldBound` will
return true if the given name of the field in the provided type has been
instantiated:

.. code-block:: chapel

  use Reflection;

  record R {
    type T;
    type U;
    param p : int;
  }

  proc printInstantiated(type T) {
    writeln("type = ", T:string);
    for param i in 1..numFields(T) {
      param name : string = getFieldName(T, i);
      writeln("  field ", name, " = ", isFieldBound(T, name));
    }
  }

  printInstantiated(R);
  printInstantiated(R(U=real));


This program outputs:

.. code-block:: text

  type = R
    field T = false
    field U = false
    field p = false
  type = R(U=real(64))
    field T = false
    field U = true
    field p = false



Passing and Returning Generic Types
-----------------------------------

As of the 1.20 release generic type expressions may be passed to and returned
from functions. This change supports not only partial instantiations, but also
generic management of class types.

.. code-block:: chapel

  class C {
    type T;
    param p : int;
  }

  proc defaultC() type {
    return C(int, 5)?;
  }

  proc intC type {
    return C(int)?;
  }

  // Declare two variables with the same instantiation of 'C' but with different management
  var x : unmanaged defaultC();
  var y : owned defaultC();

  // Declare two variables with different instantiations of 'C', based on a
  // common instantiation returned by 'intC', and with different management
  var a : owned intC(100);
  var b : shared intC(123);

  // An factory function that accepts any kind of type and creates a new
  // instance with param 'p'
  proc make(type T, param p : int) {
    return new T(p);
  }

  // For these calls, 'T' will be a partial instantiation
  var q = make(C(int), 1);
  var r = make(C(real), 2);

