.. _initequals:

.. default-domain:: chpl

==================
The 'init=' Method
==================

Overview
--------

Historically the Chapel language has not supported a way for users to directly
handle initialization from an arbitrary expression. Typically the compiler
would first default-initialize the variable, and then assign the expression
into that variable. For example, if a user wanted to initialize their own
list type from an array literal they would need to implement the assignment
operator:

.. code-block:: chapel

  record IntList {
    var D : domain(1);
    var A : [D] int;
  }

  proc =(ref lhs : IntList, rhs : []) {
    lhs.D = rhs.domain;
    lhs.A = rhs;
  }

  var i : IntList = [1, 2, 3, 4, 5];
  // becomes...
  //   i.init();
  //   i = [1, 2, 3, 4, 5];

Though this process can work in some situations, there are some downsides to
consider:

- Error messages confusingly mention assignment instead of initialization
- Fields used in this process could not be 'const' (assignment requires mutability)
- This is not true initialization, and may have unnecessary overhead

In order to rectify these issues, a new method named "init=" has been created
to replace the "init" method for copy initialization and for initialization
from arbitrary expressions.

Why a New Method is Necessary
-----------------------------

Since their introduction, initializers have served as the only mechanism for
initializing types. Many existing initializers have been written for use with
new-expressions. If those initializers were invoked when initializing a
variable from an arbitrary expression, then the resulting initialization
patterns may be surprising and unintended. For example, consider a list with an
initializer that accepts an integer representing the list's length:

.. code-block:: chapel

  proc IntList.init(len : int) { ... }

  var x = new IntList(5); // creates a new IntList of length '5': x.init(5)

If this initializer were invoked for initialization from arbitrary expressions,
then users could initialize this list from an integer. This is unlikely to be
what the author of the type intended:

.. code-block:: chapel

  var y : IntList = 10; // becomes: y.init(10)

By creating a new method we can separate the two use-cases and allow for
further control by the type's author.

General Rules for 'init='
-------------------------

The ``init=`` method is in many ways similar to the ``init`` method, and they
share the same semantic rules within the body of the method. For example,
fields must be initialized in declaration order, and must be initialized unless
they have a default value.

Where ``init=`` differs is in its invocation. The ``init=`` method will only be
invoked in two cases: copy initialization and initializing a variable from an
expression. The ``init=`` method will only be invoked with a single value.

The ``init=`` method may also invoke other initializers through
``this.init(...)``, but currently may not invoke other ``init=`` methods.

Classes do not support the ``init=`` method because classes will not be
copy-initialized by the compiler.

The 'init=' Method for Non-Generic Types
-----------------------------------------

The compiler-generated ``init=`` method for non-generic types is simple. It
accepts one argument of the same type:

.. code-block:: chapel

  record R {
    var x : int;
  }

  // identical to compiler-generated implementation
  proc R.init=(other : R) {
    this.x = other.x;
  }

In order to override this compiler-generated implementation, the user must
implement an ``init=`` method that can accept an argument of the same type.
Other user-defined ``init=`` methods will not prevent the compiler from
generating a default implementation for ``init=``. For example:

.. code-block:: chapel

  record R {
    var x : int;
  }

  proc R.init=(other : int) {
    this.x = other;
  }

  var A = new R(10);   // compiler-generated initializer 
  var B = A;           // B.init=(A) , the compiler-generated ``init=``
  var C : R = 10;      // C.init=(10) , user-defined ``init=``
  // var D : R = "hello"; // D.init=("hello") , unresolved call!


The 'init=' Method for Generic Types
------------------------------------

The compiler-generated 'init='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The compiler-generated ``init=`` method for generic types requires knowing the
intended instantiation in order to disallow copy-initialization from different
types. In the following example, there should be a compile-time error when
attempting to initialize a ``R(int)`` from a ``R(real)``.

.. code-block:: chapel

  record R {
    type T;
    var x : T;
  }

  var x : R(real);
  var y : R(int) = x;

This is accomplished by allowing ``init=`` to query the intended instantiation
through the expression ``this.type``. The compiler-generated ``init=`` for
type ``R`` looks like:

.. code-block:: chapel

  proc R.init=(other : this.type) {
    this.T = other.T;
    this.x = other.x;
  }

The first line of this ``init=`` may seem unnecessary, since ``this.type`` must
already be known. The line ``this.T = other.T`` is currently used by the
compiler to ensure that the types match. If the user attempts to initialize
``this.T`` with a type different from ``this.type.T`` the compiler will issue
an error. Future releases may allow this field initialization and type check to
be omitted, and instead infer the type from ``this.type.T``.

Field-Based Constraints
^^^^^^^^^^^^^^^^^^^^^^^

The ``this.type`` query can also be used to constrain the given value based on
generic fields. For example, consider the following generic record that simply
wraps any given type:

.. code-block:: chapel

  record Wrapper {
    type T;
    var x : T;
  }

A simple ``init=`` for this type may try to infer ``T`` from the given value:

.. code-block:: chapel

  proc Wrapper.init=(value : ?T) {
    this.T = T;
    this.x = value;
  }

This only works as long as the desired instantiation of ``T`` and the type of
the value match. What if a user tried to initialize a ``Wrapper(int(8))`` from
an integer literal?

.. code-block:: chapel

  var x : Wrapper(int(8)) = 5;

The type of ``5`` is actually ``int(64)``, and the ``init=`` would fail at the
line ``this.T = T;``. Furthermore, because ``value`` is a fully-generic
argument this ``init=`` would also resolve as the copy initializer, and attempt
to initialize some sort of nested ``Wrapper(Wrapper(int(8)))`` type.

A better approach is to constrain ``value`` using ``this.type``:

.. code-block:: chapel

  proc Wrapper.init=(value : this.type.T) {
    this.T = value.type;
    this.x = value;
  }

  var x : Wrapper(int(8)) = 5; // x.init=(5)
  var y = x; // compiler-generated init=

The literal ``5`` will now coerce from ``int(64)`` to ``int(8)`` following
regular Chapel semantics, and the compiler-generated ``init=`` will be invoked
when initializing variable ``y``.

Using 'this.type' Inside 'init='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A type may be initialized from a value that represents only part (or none) of
the required instantiation information. For example, consider initializing a
distributed list type from an array:

.. code-block:: chapel

  record DistList {
    type DistType;
    type eltType;
    // ...
  }

  proc DistList.init=(arr : [] ?eltType) {
    this.DistType = this.type.DistType; // from variable declaration
    this.eltType = eltType; // from 'arr'
    // ... initialize data, etc. ...
  }

  // Initializing a Block-distributed list from an array literal
  var x : DistList(Block(1), int) = [1, 2, 3, 4, 5];

In this example snippet, ``this.type`` is used within the ``init=`` body in
order to achieve the desired instantiation. Part of what was needed was
available from the given value (i.e. the element type), but the rest was
taken from ``this.type``.

Note that only fully instantiated types can be initialized in this manner.
Future releases may add support for fully or partially generic ``this.type``
expressions.


Initializing with a Generic Expression
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the variable declaration's type expression is fully generic, then the value
expression must be a subtype of of that generic type expression. In such cases
the compiler infers the type of the variable to be the same as the value's
type:

.. code-block:: chapel

  record R {
    type T;
    var x : T;
  }

  var A = new R(int, 5);
  var B : R = A; // 'B' inferred to be of type 'R(int)'

If the value is not a subtype of the generic expression, then there will be a
compile-time error. This may change in future releases.


Disabling Copyability
---------------------
If a user wishes to indicate that their record cannot be copied, they can do
so by implementing an ``init=`` method with a ``false`` where-clause:

.. code-block:: chapel

  proc R.init=(other: R) where false {
   // method body may be empty in this case
  }

A call to the ``compilerError`` utility function can be used for the same
purpose:

.. code-block:: chapel

  proc R.init=(other: R) {
    compilerError("Cannot copy R");
  }

Relation to Assignment Operator
-------------------------------

In the 1.20 release users could choose to implement either the ``init=`` method
or ``=`` operator for a given type, or implement both, or rely entirely on the
compiler-generated implementation. This could lead to hard-to-debug problems
when both functions appeared to be user-defined, but a user mistake in the
function signature caused it to be ignored and the compiler-generated version
to be used instead.

In the 1.21 release users are now required to implement both the ``init=``
method and ``=`` operator for a given type, or rely entirely on the
compiler-generated implementations. If only one implementation is found, the
compiler will issue an error and any potentially-incorrect function signatures
will hopefully be exposed.
