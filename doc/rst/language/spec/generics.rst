.. _Chapter-Generics:

Generics
========

Chapel supports generic functions and types that are parameterizable
over both types and parameters. The generic functions and types look
similar to non-generic functions and types already discussed.

.. _Generic_Functions:

Generic Functions
-----------------

A function is generic if any of the following conditions hold:

-  Some formal argument is specified with an intent of ``type`` or
   ``param``.

-  Some formal argument has no specified type and no default value.

-  Some formal argument is specified with a queried type.

-  The type of some formal argument is a generic type, e.g., ``List``.
   Queries may be inlined in generic types, e.g., ``List(?eltType)``.

-  The type of some formal argument is an array type where either the
   element type is queried or omitted or the domain is queried or
   omitted.

These conditions are discussed in the next sections.

.. _Formal_Type_Arguments:

Formal Type Arguments
~~~~~~~~~~~~~~~~~~~~~

If a formal argument is specified with intent ``type``, then a type must
be passed to the function at the call site. A copy of the function is
instantiated for each unique type that is passed to this function at a
call site. The formal argument has the semantics of a type alias.

   *Example (build2tuple.chpl)*.

   The following code defines a function that takes two types at the
   call site and returns a 2-tuple where the types of the components of
   the tuple are defined by the two type arguments and the values are
   specified by the types default values. 

   .. code-block:: chapel

      proc build2Tuple(type t, type tt) {
        var x1: t;
        var x2: tt;
        return (x1, x2);
      }

   This function is instantiated with “normal” function call syntax
   where the arguments are types: 

   .. code-block:: chapel

      var t2 = build2Tuple(int, string);
      t2 = (1, "hello");

   

   .. BLOCK-test-chapelpost

      writeln(t2);

   

   .. BLOCK-test-chapeloutput

      (1, hello)

A formal type argument can include a formal type (a colon followed by a
type). This pattern is sometimes useful to create generic functions
accepting type arguments that only apply to a specific group of types.

   *Example (typeColonArgument.chpl)*.

   Suppose that we’d like to define a function that accepts a type
   argument and returns 1 represented in that type. 

   .. code-block:: chapel

      proc getOne(type t:numeric) {
        return 1:t;
      }

   Now calls to this function will resolve to the appropriate version
   based upon the argument type supplied. 

   .. code-block:: chapel

      var anInt8 = getOne(int(8));
      var aReal = getOne(real);

   

   .. BLOCK-test-chapelpost

      writeln(anInt8.type:string, " ", anInt8);
      writeln(aReal.type:string, " ", aReal);

   

   .. BLOCK-test-chapeloutput

      int(8) 1
      real(64) 1.0

.. _Formal_Parameter_Arguments:

Formal Parameter Arguments
~~~~~~~~~~~~~~~~~~~~~~~~~~

If a formal argument is specified with intent ``param``, then a
parameter must be passed to the function at the call site. A copy of the
function is instantiated for each unique parameter that is passed to
this function at a call site. The formal argument is a parameter.

   *Example (fillTuple.chpl)*.

   The following code defines a function that takes an integer parameter
   ``p`` at the call site as well as a regular actual argument of
   integer type ``x``. The function returns a homogeneous tuple of size
   ``p`` where each component in the tuple has the value of ``x``.
   

   .. code-block:: chapel

      proc fillTuple(param p: int, x: int) {
        var result: p*int;
        for i in result.indices do
          result(i) = x;
        return result;
      }

   

   .. BLOCK-test-chapelpost

      writeln(fillTuple(3,3));

   

   .. BLOCK-test-chapeloutput

      (3, 3, 3)

   The function call ``fillTuple(3, 3)`` returns a 3-tuple where each
   component contains the value ``3``.

.. _Formal_Arguments_without_Types:

Formal Arguments without Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the type of a formal argument is omitted, the type of the formal
argument is taken to be the type of the actual argument passed to the
function at the call site. A copy of the function is instantiated for
each unique actual type.

   *Example (fillTuple2.chpl)*.

   The example from the previous section can be extended to be generic
   on a parameter as well as the actual argument that is passed to it by
   omitting the type of the formal argument ``x``. Additionally the
   parameter argument can allow any type be passed. The following code
   defines a function that returns a homogeneous tuple of size ``p``
   where each component in the tuple is initialized to ``x``:
   

   .. code-block:: chapel

      proc fillTuple(param p, x) {
        var result: p*x.type;
        for i in result.indices do
          result(i) = x;
        return result;
      }

   

   .. BLOCK-test-chapelpost

      var x = fillTuple(3, 3.14);
      writeln(x);
      writeln(x.type:string);

   

   .. BLOCK-test-chapeloutput

      (3.14, 3.14, 3.14)
      3*real(64)

   In this function, the type of the tuple is taken to be the type of
   the actual argument. The call ``fillTuple(3, 3.14)`` returns a
   3-tuple of real values ``(3.14, 3.14, 3.14)``. The return type is
   ``(real, real, real)``.

.. _Formal_Arguments_with_Queried_Types:

Formal Arguments with Queried Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the type of a formal argument is specified as a queried type, the
type of the formal argument is taken to be the type of the actual
argument passed to the function at the call site. A copy of the function
is instantiated for each unique actual type. The queried type has the
semantics of a type alias.

   *Example (fillTuple3.chpl)*.

   The example from the previous section can be rewritten to use a
   queried type for clarity: 

   .. code-block:: chapel

      proc fillTuple(param p: int, x: ?t) {
        var result: p*t;
        for i in result.indices do
          result(i) = x;
        return result;
      }

   

   .. BLOCK-test-chapelpost

      var x = fillTuple(3, 3.14);
      writeln(x);
      writeln(x.type:string);

   

   .. BLOCK-test-chapeloutput

      (3.14, 3.14, 3.14)
      3*real(64)

..

   *Example (query.chpl)*.

   Type queries can also be used to constrain the types of other
   function arguments and/or the return type. In this example, the type
   query on the first argument establishes type constraints on the other
   arguments and also determines the return type.

   The code 

   .. code-block:: chapel

      writeln(sumOfThree(1,2,3));
      writeln(sumOfThree(4.0,5.0,3.0));

      proc sumOfThree(x: ?t, y:t, z:t):t {
         var sum: t;

         sum = x + y + z;
         return sum;
      }

   produces the output 

   .. code-block:: printoutput

      6
      12.0

.. _Formal_Arguments_of_Generic_Type:

Formal Arguments of Generic Type
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the type of a formal argument is a generic type, there must exist an
instantiation of that type that the actual argument can be implicitly
coerced to (:ref:`Implicit_Conversions`). A copy of the function is
instantiated for each unique instantiation of the formal’s type.

   *Example*.

   The following code defines a function ``writeTop`` that takes an
   actual argument that is a generic stack
   (see :ref:`Example_Generic_Stack`) and outputs the top element
   of the stack. The function is generic on the type of its argument.
   

   .. code-block:: chapel

      proc writeTop(s: Stack) {
        write(s.top.item);
      }

Types and parameters may be queried from the types of formal arguments
as well. In the example above, the formal argument’s type could also be
specified as ``Stack(?t)`` in which case the symbol ``t`` is equivalent
to ``s.itemType``.

Note that generic types which have default values for all of their
generic fields, *e.g. range*, are not generic when simply specified and
require a query to mark the argument as generic. For simplicity, the
identifier may be omitted.

   *Example*.

   The following code defines a class with a type field that has a
   default value. Function ``f`` is defined to take an argument of this
   class type where the type field is instantiated to the default.
   Function ``g``, on the other hand, is generic on its argument because
   of the use of the question mark. 

   .. code-block:: chapel

      class C {
        type t = int;
      }
      proc f(c: C) {
        // c.type is always int
      }
      proc g(c: C(?)) {
        // c.type may not be int
      }

.. _Formal_Arguments_of_Partially_Generic_Type:

Formal Arguments of Partially Generic Type
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The generic type for a formal argument may be specified with some
queries or generic types and some concrete types or values. Using
concrete types or values in this manner makes the argument *partially
concrete* for the purpose of function resolution.

   *Example (nested-type-queries.chpl)*.

   Given the code: 

   .. code-block:: chapel

      class C {
        type elementType;
        type indexType;
        type containerType;
      }
      class Container {
        type containedType;
      }
      proc f(c: C(real,?t,?u)) {
        // ...
      }

   The function ``f`` can only apply when the ``c.elementType==real``.

   It’s also possible to use a generic type as an argument to ``C``. The
   following function, ``g``, can only apply when ``c.containerType`` is
   an instance of ``Container``:

   

   .. code-block:: chapel

      proc g(c: C(?t,?u,Container)) {
        // ...
      }

   

   .. BLOCK-test-chapelpost

      var cc = new Container(int);
      var c = new C(real, int, cc.type);
      f(c);
      g(c);

Similarly, a tuple type with query arguments forms a *partially
concrete* argument.

   *Example*.

   The function definition 

   .. code-block:: chapel

      proc f(tuple: (?t,real)) {
        // body
      }

   specifies that ``tuple.size == 2 && tuple(2).type == real``.

Homogeneous tuple arguments of generic type are also supported:

   *Example (partially-concrete-star-tuple.chpl)*.

   

   .. code-block:: chapel

      record Number {
        var n;
      }
      proc f(tuple: 2*Number) {
      }

   

   .. BLOCK-test-chapelpost

      f( (new Number(0), new Number(1)) );

   specifies that ``f`` accepts a tuple with 2 elements, where each
   element has the same type, and that type is instantiation of
   ``Number``.

Note that specifying a tuple consisting entirely of queried types does
create a *partially concrete argument* because the size of the tuple is
constrained.

   *Example (partially-concrete-tuple-ambiguity.chpl)*.

   The following program results in an ambiguity error: 

   .. code-block:: chapel

      proc f(tuple: (?,real)) {
      }
      proc f(tuple: (?,?)) {
      }
      f( (1.0, 2.0) );

   since the ``tuple`` arguments in both versions of ``f`` are
   *partially concrete*. 

   .. BLOCK-test-chapelprediff

      #!/usr/bin/env sh
      # This prediff exists to avoid underscores in the output
      # which confuse tex
      testname=$1
      outfile=$2
      head -n 1 $outfile > $outfile.2
      mv $outfile.2 $outfile

   

   .. BLOCK-test-chapeloutput

      partially-concrete-tuple-ambiguity.chpl:5: error: ambiguous call 'f(2*real(64))'

.. _Formal_Arguments_of_Generic_Array_Types:

Formal Arguments of Generic Array Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the type of a formal argument is an array where either the domain or
the element type is queried or omitted, the type of the formal argument
is taken to be the type of the actual argument passed to the function at
the call site. If the domain is omitted, the domain of the formal
argument is taken to be the domain of the actual argument.

A queried domain may not be modified via the name to which it is bound
(see :ref:`Association_of_Arrays_to_Domains` for rationale).

.. _Function_Visibility_in_Generic_Functions:

Function Visibility in Generic Functions
----------------------------------------

When resolving function calls made within generic functions, there is an
additional source of visible functions. Besides functions visible to the
generic function’s point of declaration, visible functions are also
taken from one of the call sites at which the generic function is
instantiated for each particular instantiation. The specific call site
chosen is arbitrary and it is referred to as the *point of
instantiation*.

   *Example (point-of-instantiation.chpl)*.

   Consider the following code which defines a generic function ``bar``:
   

   .. code-block:: chapel

      module M1 {
        record R {
          var x: int;
          proc foo() { }
        }
      }

      module M2 {
        proc bar(x) {
          x.foo();
        }
      }

      module M3 {
        use M1, M2;
        proc main() {
          var r: R;
          bar(r);
        }
      }

   In the function ``main``, the variable ``r`` is declared to be of
   type ``R`` defined in module ``M1`` and a call is made to the generic
   function ``bar`` which is defined in module ``M2``. This is the only
   place where ``bar`` is called in this program and so it becomes the
   point of instantiation for ``bar`` when the argument ``x`` is of type
   ``R``. Therefore, the call to the ``foo`` method in ``bar`` is
   resolved by looking for visible functions from within ``main`` and
   going through the use of module ``M1``.

If the generic function is only called indirectly through dynamic
dispatch, the point of instantiation is defined as the point at which
the derived type (the type of the implicit ``this`` argument) is defined
or instantiated (if the derived type is generic).

   *Rationale*.

   Visible function lookup in Chapel’s generic functions is handled
   differently than in C++’s template functions in that there is no
   split between dependent and independent types.

   Also, dynamic dispatch and instantiation is handled differently.
   Chapel supports dynamic dispatch over methods that are generic in
   some of its formal arguments.

   Note that the Chapel lookup mechanism is still under development and
   discussion. Comments or questions are appreciated.

.. _Generic_Types:

Generic Types
-------------

Generic types comprise built-in generic types, generic classes, and
generic records.

.. _Built_in_Generic_types:

Built-in Generic Types
~~~~~~~~~~~~~~~~~~~~~~

The types ``integral``, ``numeric`` and ``enum`` are generic types that
can only be instantiated with, respectively, the signed and unsigned
integral types, all of the numeric types, and all enumerated types. The
type ``enumerated`` is currently available as a synonym for ``enum``.

The type ``record`` can be instantiated with any record type.

The memory management strategies ``owned``, ``shared``, ``borrowed``,
and ``unmanaged`` (see :ref:`Class_Types`) are also generic
types that can be instantiated with any class using that memory
management strategy. These types indicate generic nilability.

The types ``class`` and ``class?``, on their own or in combination with
memory management strategies, are also generic types. They can be
instantiated as follows:

-  ``class`` can instantiate with any non-nilable class using any memory
   management strategy

-  ``class?`` can instantiate with any class using any memory management
   strategy but will use the nilable variant of that class in an
   instantiation. When used as an argument type, a value of non-nilable
   class type will be implicitly converted to the nilable type on the
   call. As a result, a formal of type ``class?`` can accept an actual
   of any class type.

-  ``owned`` can instantiate with any ``owned`` class - taking the
   nilability from whatever it instantiated from.

-  ``owned class`` can instantiate with any non-nilable ``owned`` class.

-  ``owned class?`` can instantiate from any nilable ``owned`` class. As
   with ``class?``, it can also instantiate from a non-nilable ``owned``
   class, in which case a implicit conversion would occur in a call.

-  ``shared``, ``shared class``, ``shared class?`` behave similarly to
   the above but with ``shared`` management strategy.

-  ``borrowed``, ``borrowed class``, ``borrowed class?`` behave
   similarly to the above but with ``borrowed`` management strategy.

-  ``unmanaged``, ``unmanaged class``, ``unmanaged class?`` behave
   similarly to the above but with ``unmanaged`` management strategy.

Generic Classes and Records
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The remainder of this section :ref:`Generic_Types` specifies
generic class and record types that are not built-in types
(:ref:`Built_in_Generic_types`).

A class or record is generic if it contains one or more generic fields.
A generic field is one of:

-  a specified or unspecified type alias,

-  a parameter field, or

-  a ``var`` or ``const`` field that has no type and no initialization
   expression.

For each generic field, the class or record is parameterized over:

-  the type bound to the type alias,

-  the value of the parameter field, or

-  the type of the ``var`` or ``const`` field, respectively.

Correspondingly, the class or record is instantiated with a set of types
and parameter values, one type or value per generic field.

.. _Type_Aliases_in_Generic_Types:

Type Aliases in Generic Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If a class or record defines a type alias, the class or record is
generic over the type that is bound to that alias. Such a type alias is
accessed as if it were a field from either a class or record instance or
from the instantiated class or record type itself. Similar to a
parameter field, it cannot be assigned except in its declaration.

The type alias becomes an argument with intent ``type`` to the
compiler-generated initializer
(:ref:`Generic_Compiler_Generated_Initializers`) for its class
or record. This makes the compiler-generated initializer generic. The
type alias also becomes an argument with intent ``type`` to the type
constructor (:ref:`Type_Constructors`). If the type alias
declaration binds it to a type, that type becomes the default for these
arguments, otherwise they have no defaults.

The class or record is instantiated by binding the type alias to the
actual type passed to the corresponding argument of a user-defined
(:ref:`Generic_User_Initializers`) or compiler-generated
initializer or type constructor. If that argument has a default, the
actual type can be omitted, in which case the default will be used
instead.

   *Example (NodeClass.chpl)*.

   The following code defines a class called ``Node`` that implements a
   linked list data structure. It is generic over the type of the
   element contained in the linked list. 

   .. code-block:: chapel

      class Node {
        type eltType;
        var data: eltType;
        var next: unmanaged Node(eltType)?;
      }

   

   .. BLOCK-test-chapelpost

      var n: unmanaged Node(real) = new unmanaged Node(real, 3.14);
      writeln(n.data);
      writeln(n.next);
      writeln(n.next.type:string);
      delete n;

   

   .. BLOCK-test-chapeloutput

      3.14
      nil
      unmanaged Node(real(64))?

   The call ``new Node(real, 3.14)`` creates a node in the linked list
   that contains the value ``3.14``. The ``next`` field is set to nil.
   The type specifier ``Node`` is a generic type and cannot be used to
   define a variable. The type specifier ``Node(real)`` denotes the type
   of the ``Node`` class instantiated over ``real``. Note that the type
   of the ``next`` field is specified as ``Node(eltType)``; the type of
   ``next`` is the same type as the type of the object that it is a
   field of.

.. _Parameters_in_Generic_Types:

Parameters in Generic Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~

If a class or record defines a parameter field, the class or record is
generic over the value that is bound to that field. The field can be
accessed from a class or record instance or from the instantiated class
or record type itself.

The parameter becomes an argument with intent ``param`` to the
compiler-generated initializer
(:ref:`Generic_Compiler_Generated_Initializers`) for that class
or record. This makes the compiler-generated initializer generic. The
parameter also becomes an argument with intent ``param`` to the type
constructor (:ref:`Type_Constructors`). If the parameter
declaration has an initialization expression, that expression becomes
the default for these arguments, otherwise they have no defaults.

The class or record is instantiated by binding the parameter to the
actual value passed to the corresponding argument of a user-defined
(:ref:`Generic_User_Initializers`) or compiler-generated
initializer or type constructor. If that argument has a default, the
actual value can be omitted, in which case the default will be used
instead.

   *Example (IntegerTuple.chpl)*.

   The following code defines a class called ``IntegerTuple`` that is
   generic over an integer parameter which defines the number of
   components in the class. 

   .. code-block:: chapel

      class IntegerTuple {
        param size: int;
        var data: size*int;
      }

   

   .. BLOCK-test-chapelpost

      var x = new unmanaged IntegerTuple(3);
      writeln(x.data);
      delete x;

   

   .. BLOCK-test-chapeloutput

      (0, 0, 0)

   The call ``new IntegerTuple(3)`` creates an instance of the
   ``IntegerTuple`` class that is instantiated over parameter ``3``. The
   field ``data`` becomes a 3-tuple of integers. The type of this class
   instance is ``IntegerTuple(3)``. The type specified by
   ``IntegerTuple`` is a generic type.

.. _Fields_without_Types:

Fields without Types
~~~~~~~~~~~~~~~~~~~~

If a ``var`` or ``const`` field in a class or record has no specified
type or initialization expression, the class or record is generic over
the type of that field. The field becomes an argument with default
intent to the compiler-generated initializer
(:ref:`Generic_Compiler_Generated_Initializers`). That argument
has no specified type and no default value. This makes the
compiler-generated initializer generic. The field also becomes an
argument with ``type`` intent and no default to the type constructor
(:ref:`Type_Constructors`). Correspondingly, an actual value
must always be passed to the default initializer argument and an actual
type to the type constructor argument.

The class or record is instantiated by binding the type of the field to
the type of the value passed to the corresponding argument of a
user-defined (:ref:`Generic_User_Initializers`) or
compiler-generated initializer
(:ref:`Generic_Compiler_Generated_Initializers`). When the type
constructor is invoked, the class or record is instantiated by binding
the type of the field to the actual type passed to the corresponding
argument.

   *Example (fieldWithoutType.chpl)*.

   The following code defines another class called ``Node`` that
   implements a linked list data structure. It is generic over the type
   of the element contained in the linked list. This code does not
   specify the element type directly in the class as a type alias but
   rather omits the type from the ``data`` field. 

   .. code-block:: chapel

      class Node {
        var data;
        var next: unmanaged Node(data.type)? = nil;
      }

   A node with integer element type can be defined in the call to the
   initializer. The call ``new Node(1)`` defines a node with the value
   ``1``. The code 

   .. code-block:: chapel

      var list = new unmanaged Node(1);
      list.next = new unmanaged Node(2);

   

   .. BLOCK-test-chapelpost

      writeln(list.data);
      writeln(list.next!.data);
      delete list.next;
      delete list;

   

   .. BLOCK-test-chapeloutput

      1
      2

   defines a two-element list with nodes containing the values ``1`` and
   ``2``. The type of each object could be specified as ``Node(int)``.

.. _Type_Constructors:

The Type Constructor
~~~~~~~~~~~~~~~~~~~~

A type constructor is automatically created for each class or record. A
type constructor is a type function (:ref:`Type_Return_Intent`)
that has the same name as the class or record. It takes one argument per
the class’s or record’s generic field, including fields inherited from
the superclasses, if any. The formal argument has intent ``type`` for a
type alias field and is a parameter for a parameter field. It accepts
the type to be bound to the type alias and the value to be bound to the
parameter, respectively. For a generic ``var`` or ``const`` field, the
corresponding formal argument also has intent ``type``. It accepts the
type of the field, as opposed to a value as is the case for a parameter
field. The formal arguments occur in the same order as the fields are
declared and have the same names as the corresponding fields. Unlike the
compiler-generated initializer, the type constructor has only those
arguments that correspond to generic fields.

A call to a type constructor accepts actual types and parameter values
and returns the type of the class or record that is instantiated
appropriately for each field
(:ref:`Type_Aliases_in_Generic_Types`,
:ref:`Parameters_in_Generic_Types`,
:ref:`Fields_without_Types`). Such an instantiated type must
be used as the type of a variable, array element, non-generic formal
argument, and in other cases where uninstantiated generic class or
record types are not allowed.

When a generic field declaration has an initialization expression or a
type alias is specified, that initializer becomes the default value for
the corresponding type constructor argument. Uninitialized fields,
including all generic ``var`` and ``const`` fields, and unspecified type
aliases result in arguments with no defaults; actual types or values for
these arguments must always be provided when invoking the type
constructor.

.. _Generic_Methods:

Generic Methods
~~~~~~~~~~~~~~~

All methods bound to generic classes or records, including initializers,
are generic over the implicit ``this`` argument. This is in addition to
being generic over any other argument that is generic.

.. _Generic_Compiler_Generated_Initializers:

The Compiler-Generated Initializer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If no user-defined initializers are supplied for a given generic class,
the compiler generates one in a manner similar to that for
concrete classes (:ref:`The_Compiler_Generated_Initializer`).
However, the compiler-generated initializer for a generic class or
record (:ref:`The_Compiler_Generated_Initializer`) is generic
over each argument that corresponds to a generic field, as specified
above.

The argument has intent ``type`` for a type alias field and has intent
``param`` for a parameter field. It accepts the type to be bound to the
type alias and the value to be bound to the parameter, respectively.
This is the same as for the type constructor. For a generic ``var`` or
``const`` field, the corresponding formal argument has the default
intent and accepts the value for the field to be initialized with. The
type of the field is inferred automatically to be the type of the
initialization value.

The default values for the generic arguments of the compiler-generated
initializer are the same as for the type constructor
(:ref:`Type_Constructors`). For example, the arguments
corresponding to the generic ``var`` and ``const`` fields, if any, never
have defaults, so the corresponding actual values must always be
provided.

.. _Generic_User_Initializers:

User-Defined Initializers
~~~~~~~~~~~~~~~~~~~~~~~~~

If a generic field of a class or record does not have a default value or
type alias, each user-defined initializer for that class must explicitly
initialize that field.

   *Example (initializersForGenericFields.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class MyGenericClass {
        type t1;
        param p1;
        const c1;
        var v1;
        var x1: t1; // this field is not generic

        type t5 = real;
        param p5 = "a string";
        const c5 = 5.5;
        var v5 = 555;
        var x5: t5; // this field is not generic

        proc init(c1, v1, type t1, param p1) {
          this.t1 = t1;
          this.p1 = p1;
          this.c1 = c1;
          this.v1 = v1;
          // compiler inserts initialization for remaining fields
        }
        proc init(type t5, param p5, c5, v5, x5,
                  type t1, param p1, c1, v1, x1) {
          this.t1 = t1;
          this.p1 = p1;
          this.c1 = c1;
          this.v1 = v1;
          this.x1 = x1;
          this.t5 = t5;
          this.p5 = p5;
          this.c5 = c5;
          this.v5 = v5;
          this.x5 = x5;
        }
      }  // class MyGenericClass

      var g1 = new MyGenericClass(11, 111, int, 1);
      var g2 = new MyGenericClass(int, "this is g2", 3.3, 333, 3333,
                                  real, 2, 222, 222.2, 22);

   

   .. BLOCK-test-chapelpost

      writeln(g1.p1);
      writeln(g1.p5);
      writeln(g1);
      writeln(g2.p1);
      writeln(g2.p5);
      writeln(g2);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      1
      a string
      {c1 = 11, v1 = 111, x1 = 0, c5 = 5.5, v5 = 555, x5 = 0.0}
      2
      this is g2
      {c1 = 222, v1 = 222.2, x1 = 22.0, c5 = 3.3, v5 = 333, x5 = 3333}

   The initializers are required to initialize fields ``t1``, ``p1``,
   ``c1``, and ``v1``. Otherwise, field initializations may be omitted
   according to previously-described initializer semantics.

.. _User_Defined_Compiler_Errors:

User-Defined Compiler Diagnostics
---------------------------------

The special compiler diagnostic function calls ``compilerError`` and
``compilerWarning`` generate compiler diagnostic of the indicated
severity if the function containing these calls may be called when the
program is executed and the function call is not eliminated by parameter
folding.

The compiler diagnostic is defined by the actual arguments which must be
string parameters. The diagnostic points to the spot in the Chapel
program from which the function containing the call is called.
Compilation halts if a ``compilerError`` is encountered whereas it will
continue after encountering a ``compilerWarning``.

   *Example (compilerDiagnostics.chpl)*.

   The following code shows an example of using user-defined compiler
   diagnostics to generate warnings and errors: 

   .. code-block:: chapel

      proc foo(x, y) {
        if (x.type != y.type) then
          compilerError("foo() called with non-matching types: ",
                        x.type:string, " != ", y.type:string);
        writeln("In 2-argument foo...");
      }

      proc foo(x) {
        compilerWarning("1-argument version of foo called");
        writeln("In generic foo!");
      }

   The first routine generates a compiler error whenever the compiler
   encounters a call to it where the two arguments have different types.
   It prints out an error message indicating the types of the arguments.
   The second routine generates a compiler warning whenever the compiler
   encounters a call to it.

   Thus, if the program foo.chpl contained the following calls:

   

   .. code-block:: chapel
      :linenos:

      foo(3.4);
      foo("hi");
      foo(1, 2);
      foo(1.2, 3.4);
      foo("hi", "bye");
      foo(1, 2.3);
      foo("hi", 2.3);

   compiling the program would generate output like:

   

   .. code-block:: bash

      foo.chpl:1: warning: 1-argument version of foo called with type: real
      foo.chpl:2: warning: 1-argument version of foo called with type: string
      foo.chpl:6: error: foo() called with non-matching types: int != real

   

   .. BLOCK-test-chapeloutput

      compilerDiagnostics.chpl:14: warning: 1-argument version of foo called
      compilerDiagnostics.chpl:15: warning: 1-argument version of foo called
      compilerDiagnostics.chpl:19: error: foo() called with non-matching types: int(64) != real(64)

.. _Creating_General_and_Specialized_Versions_of_a_Function:

Creating General and Specialized Versions of a Function
-------------------------------------------------------

The Chapel language facility supports three mechanisms for using generic
functions along with concrete functions. These mechanisms allow users to
create a general generic implementation and also a special
implementation for specific concrete types.

The first mechanism applies to functions. According to the function
resolution rules described in :ref:`Function_Resolution`,
functions accepting concrete arguments are selected in preference to
those with a totally generic argument. So, creating a second version of
a generic function that declares a concrete type will cause the concrete
function to be used where possible:

   *Example (specializeGenericFunction.chpl)*.

   

   .. code-block:: chapel

      proc foo(x) {
        writeln("in generic foo(x)");
      }
      proc foo(x:int) {
        writeln("in specific foo(x:int)");
      }

      var myReal:real;
      foo(myReal); // outputs "in generic foo(x)"
      var myInt:int;
      foo(myInt); // outputs "in specific foo(x:int)"

   

   .. BLOCK-test-chapeloutput

      in generic foo(x)
      in specific foo(x:int)

This program will run the generic foo function if the argument is a
real, but it runs the specific version for int if the argument is an
int.

The second mechanism applies when working with methods on generic types.
When declaring a secondary method, the receiver type can be a
parenthesized expression. In that case, the compiler will evaluate the
parenthesized expression at compile time in order to find the concrete
receiver type. Then, the resolution rules described above will cause the
concrete method to be selected when applicable. For example:

   *Example (specializeGenericMethod.chpl)*.

   

   ::

      record MyNode {
        var field;  // since no type is specified here, MyNode is a generic type
      }

      proc MyNode.foo() {
        writeln("in generic MyNode.foo()");
      }
      proc (MyNode(int)).foo() {
        writeln("in specific MyNode(int).foo()");
      }

      var myRealNode = new MyNode(1.0);
      myRealNode.foo(); // outputs "in generic MyNode.foo()"
      var myIntNode = new MyNode(1);
      myIntNode.foo(); // outputs "in specific MyNode(int).foo()"

   .. BLOCK-test-chapelnoprint

      record MyNode {
        var field;  // since no type is specified here, MyNode is a generic type
      }

      proc MyNode.foo() {
        writeln("in generic MyNode.foo()");
      }
      proc (MyNode(int)).foo() {
        writeln("in specific MyNode(int).foo()");
      }

      var myRealNode = new MyNode(1.0);
      myRealNode.foo(); // outputs "in generic MyNode.foo()"
      var myIntNode = new MyNode(1);
      myIntNode.foo(); // outputs "in specific MyNode(int).foo()"



   .. BLOCK-test-chapeloutput

      in generic MyNode.foo()
      in specific MyNode(int).foo()

The third mechanism is to use a where clause. Where clauses limit a
generic method to particular cases. Unlike the previous two cases, a
where clause can be used to declare special implementation of a function
that works with some set of types - in other words, the special
implementation can still be a generic function. See also
:ref:`Where_Clauses`.

.. _Example_Generic_Stack:

Example: A Generic Stack
------------------------

   *Example (genericStack.chpl)*.

   

   .. code-block:: chapel

      class MyNode {
        type itemType;              // type of item
        var item: itemType;         // item in node
        var next: unmanaged MyNode(itemType)?; // reference to next node (same type)
      }

      record Stack {
        type itemType;             // type of items
        var top: unmanaged MyNode(itemType)?; // top node on stack linked list

        proc push(item: itemType) {
          top = new unmanaged MyNode(itemType, item, top);
        }

        proc pop() {
          if isEmpty then
            halt("attempt to pop an item off an empty stack");
          var oldTop = top;
          var oldItem = top!.item;
          top = top!.next;
          delete oldTop;
          return oldItem;
        }

        proc isEmpty return top == nil;
      }

   

   .. BLOCK-test-chapelpost

      var s: Stack(int);
      s.push(1);
      s.push(2);
      s.push(3);
      while !s.isEmpty do
        writeln(s.pop());

   

   .. BLOCK-test-chapeloutput

      3
      2
      1
