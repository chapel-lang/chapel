.. default-domain:: chpl

.. _Chapter-Procedures:

Procedures
==========

A *function* is a code abstraction that can be invoked by a call
expression. Throughout this specification the term “function” is used in
this programming-languages sense, rather than in the mathematical sense.
A function has zero or more *formal arguments*, or simply *formals*.
Upon a function call each formal is associated with the corresponding
*actual argument*, or simply *actual*. Actual arguments are provided as
part of the call expression, or at the the *call site*. Direct and
indirect recursion is supported.

A function can be a *procedure*, which completes and returns to the call
site exactly once, returning no result, a single result, or multiple
results aggregated in a tuple. A function can also be an iterator, which
can generate, or *yield*, multiple results (in sequence and/or in
parallel). A function (either a procedure or an iterator) can be a
*method* if it is bound to a type (often a class). An *operator* in this
chapter is a procedure with a special name, which can be invoked using
infix notation, i.e., via a unary or binary expression. This chapter
defines procedures, but most of its contents apply to iterators and
methods as well.

Functions are presented as follows:

-  procedures (this chapter)

-  operators :ref:`Function_Definitions`,
   :ref:`Binary_Expressions`

-  iterators :ref:`Chapter-Iterators`

-  methods (when bound to a class) :ref:`Class_Methods`

-  function calls :ref:`Function_Calls`

-  various aspects of defining a procedure
   :ref:`Function_Definitions`–:ref:`Nested_Functions`

-  calling external functions from Chapel
   :ref:`Calling_External_Functions`

-  calling Chapel functions from external
   functions :ref:`Calling_Chapel_Functions`

-  determining the function to invoke for a given call site: function
   and operator overloading :ref:`Function_Overloading`,
   function resolution :ref:`Function_Resolution`

.. _Function_Calls:

Function Calls
--------------

The syntax to call a non-method function is given by: 

.. code-block:: syntax

   call-expression:
     lvalue-expression ( named-expression-list )
     lvalue-expression [ named-expression-list ]
     parenthesesless-function-identifier

   named-expression-list:
     named-expression
     named-expression , named-expression-list

   named-expression:
     expression
     identifier = expression

   parenthesesless-function-identifier:
     identifier

A ``call-expression`` is resolved to a particular function according to
the algorithm for function resolution described
in :ref:`Function_Resolution`.

Functions can be called using either parentheses or brackets.

   *Rationale*.

   This provides an opportunity to blur the distinction between an array
   access and a function call and thereby exploit a possible space/time
   tradeoff.

Functions that are defined without parentheses must be called without
parentheses as defined by scope resolution. Functions without
parentheses are discussed
in :ref:`Functions_without_Parentheses`.

A ``named-expression`` is an expression that may be optionally named. It
provides an actual argument to the function being called. The optional
``identifier`` refers to a named formal argument described
in :ref:`Named_Arguments`.

Calls to methods are defined in
Section :ref:`Class_Method_Calls`.

.. _Function_Definitions:

Procedure Definitions
---------------------

Procedures are defined with the following syntax: 

.. code-block:: syntax

   procedure-declaration-statement:
     privacy-specifier[OPT] procedure-kind[OPT] 'proc' identifier argument-list[OPT] return-intent[OPT] return-type[OPT] where-clause[OPT] function-body
     privacy-specifier[OPT] procedure-kind[OPT] 'operator' operator-name argument-list return-intent[OPT] return-type[OPT] where-clause[OPT] function-body

   procedure-kind:
     'inline'
     'export'
     'extern'
     'override'

   operator-name: one of
     'align' 'by'
     + - * / % ** : ! == != <= >= < > << >> & | ^ ~
     = += -= *= /= %= **= &= |= ^= <<= >>= <=> <~> #

   argument-list:
     ( formals[OPT] )

   formals:
     formal
     formal , formals

   formal:
     formal-intent[OPT] identifier formal-type[OPT] default-expression[OPT]
     formal-intent[OPT] identifier formal-type[OPT] variable-argument-expression
     formal-intent[OPT] tuple-grouped-identifier-list formal-type[OPT] default-expression[OPT]
     formal-intent[OPT] tuple-grouped-identifier-list formal-type[OPT] variable-argument-expression

   formal-type:
     : type-expression
     : ? identifier[OPT]

   default-expression:
     = expression

   variable-argument-expression:
     ... expression
     ... ? identifier[OPT]
     ...

   formal-intent:
     'const'
     'const in'
     'const ref'
     'in'
     'out'
     'inout'
     'ref'
     'param'
     'type'

   return-intent:
     'const'
     'const ref'
     'ref'
     'param'
     'type'

   return-type:
     : type-expression

   where-clause:
     'where' expression

   function-body:
     block-statement
     return-statement

Functions do not require parentheses if they have no arguments. Such
functions are described in :ref:`Functions_without_Parentheses`.

Formal arguments can be grouped together using a tuple notation as
described in :ref:`Formal_Argument_Declarations_in_a_Tuple`.

Default expressions allow for the omission of actual arguments at the
call site, resulting in the implicit passing of a default value. Default
values are discussed in :ref:`Default_Values`.

The intents ``const``, ``const in``, ``const ref``, ``in``, ``out``,
``inout`` and ``ref`` are discussed in :ref:`Argument_Intents`.
The intents ``param`` and ``type`` make a function generic and are
discussed in :ref:`Generic_Functions`. If the formal argument’s
type is omitted, generic, or prefixed with a question mark, the function
is also generic and is discussed in :ref:`Generic_Functions`.

Functions can take a variable number of arguments. Such functions are
discussed in :ref:`Variable_Length_Argument_Lists`.

The ``return-intent`` can be used to indicate how the value is returned
from a function. ``return-intent`` is described further in
:ref:`Return_Intent`.

   *Open issue*.

   Parameter and type procedures are supported. Parameter and type
   iterators are currently not supported.

The ``return-type`` is optional and is discussed
in :ref:`Return_Types`. A type function may not specify a return
type.

The ``where-clause`` is optional and is discussed
in :ref:`Where_Clauses`.

Function and operator overloading is supported in Chapel and is
discussed in :ref:`Function_Overloading`. Operator overloading
is supported on the operators listed above (see ``operator-name``).

The optional ``privacy-specifier`` keywords indicate the visibility of
module level procedures to outside modules. By default, procedures are
publicly visible. More details on visibility can be found in
 :ref:`Visibility_Of_Symbols`.

The linkage specifier ``inline`` indicates that the function body must
be inlined at every call site.

   *Rationale*.

   A Chapel compiler is permitted to inline any function if it
   determines there is likely to be a performance benefit to do so.
   Hence an error must be reported if the compiler is unable to inline a
   procedure with this specifier. One example of a preventable inlining
   error is to define a sequence of inlined calls that includes a cycle
   back to an inlined procedure.

See the chapter on interoperability
(:ref:`Chapter-Interoperability`) for details on exported
and imported functions.

.. _Functions_without_Parentheses:

Functions without Parentheses
-----------------------------

Functions do not require parentheses if they have empty argument lists.
Functions declared without parentheses around empty argument lists must
be called without parentheses.

   *Example (function-no-parens.chpl)*.

   Given the definitions 

   .. code-block:: chapel

      proc foo { writeln("In foo"); }
      proc bar() { writeln("In bar"); }

   

   .. BLOCK-test-chapelpost

      foo;
      bar();

   

   .. BLOCK-test-chapeloutput

      In foo
      In bar

   the procedure ``foo`` can be called by writing ``foo`` and the
   procedure ``bar`` can be called by writing ``bar()``. It is an error
   to use parentheses when calling ``foo`` or omit them when calling
   ``bar``.

.. _Formal_Arguments:

Formal Arguments
----------------

A formal argument’s intent (:ref:`Argument_Intents`) specifies
how the actual argument is passed to the function. If no intent is
specified, the default intent (:ref:`The_Default_Intent`) is
applied, resulting in type-dependent behavior.

.. _Named_Arguments:

Named Arguments
~~~~~~~~~~~~~~~

A formal argument can be named at the call site to explicitly map an
actual argument to a formal argument.

   *Example (named-args.chpl)*.

   Running the code 

   .. code-block:: chapel

      proc foo(x: int, y: int) { writeln(x); writeln(y); }

      foo(x=2, y=3);
      foo(y=3, x=2);

   will produce the output 

   .. code-block:: printoutput

      2
      3
      2
      3

   named argument passing is used to map the actual arguments to the
   formal arguments. The two function calls are equivalent.

Named arguments are sometimes necessary to disambiguate calls or ignore
arguments with default values. For a function that has many arguments,
it is sometimes good practice to name the arguments at the call site for
compiler-checked documentation.

.. _Default_Values:

Default Values
~~~~~~~~~~~~~~

Default values can be specified for a formal argument by appending the
assignment operator and a default expression to the declaration of the
formal argument. If the actual argument is omitted from the function
call, the default expression is evaluated when the function call is made
and the evaluated result is passed to the formal argument as if it were
passed from the call site. While the default expression is evaluated at
the time of the function call, it is resolved in the scope of the
definition of the called function, immediately before the called function
is resolved. As a result, a default value expression can refer to
previous formal arguments.

When a default value is provided for a formal argument without a type,
the argument type will be inferred to match the type of the default
value.  This inference is similar to the type inference for variable
declarations (see :ref:`Local_Type_Inference`). However, there is one
difference: when the call provides a corresponding actual argument, and
the actual argument is of a type that includes a runtime component (see
:ref:`Types_with_Runtime_Components`), the runtime component of the
formal argument's type will come from the actual argument, rather than
from the default value expression.

   *Example (default-values.chpl)*.

   The code 

   .. code-block:: chapel

      proc foo(x: int = 5, y: int = 7) { writeln(x); writeln(y); }

      foo();
      foo(7);
      foo(y=5);

   writes out 

   .. code-block:: printoutput

      5
      7
      7
      7
      5
      5

   Default values are specified for the formal arguments ``x`` and
   ``y``. The three calls to ``foo`` are equivalent to the following
   three calls where the actual arguments are explicit: ``foo(5, 7)``,
   ``foo(7, 7)``, and ``foo(5, 5)``. The example ``foo(y=5)`` shows how
   to use a named argument for ``y`` in order to use the default value
   for ``x`` in the case when ``x`` appears earlier than ``y`` in the
   formal argument list.


   *Example (default-array-runtime-type.chpl)*.

   This example shows that the runtime type of the default expression
   does not impact the runtime type of the formal argument in the case
   that an actual argument was provided.

   .. code-block:: chapel

      var D = {1..4};
      proc createArrayOverD() {
        var A:[D] int;
        return A;
      }

      proc bar(arg = createArrayOverD()) {
        writeln(arg.domain);
      }

      bar(); // arg uses the default, so outputs {1..4}

      var B:[0..2] int;
      bar(B); // arg refers to B and so has the runtime type from B
              // so outputs {0..2}

   .. BLOCK-test-chapeloutput

      {1..4}
      {0..2}


.. _Argument_Intents:

Argument Intents
----------------

Argument intents specify how an actual argument is passed to a function
where it is represented by the corresponding formal argument.

Argument intents are categorized as being either *concrete* or
*abstract*. Concrete intents are those in which the semantics of the
intent keyword are independent of the argument’s type. Abstract intents
are those in which the keyword (or lack thereof) expresses a general
intention that will ultimately be implemented via one of the concrete
intents. The specific choice of concrete intent depends on the
argument’s type and may be implementation-defined. Abstract intents are
provided to support productivity and code reuse.

.. _Concrete Intents:

Concrete Intents
~~~~~~~~~~~~~~~~

The concrete intents are ``in``, ``out``, ``inout``, ``ref``,
``const in``, and ``const ref``.

.. _The_In_Intent:

The In Intent
^^^^^^^^^^^^^

When ``in`` is specified as the intent, the formal argument represents a
variable that is initialized from the value of the actual argument.
This initialization will be copy-initialization or move-initialization
according to :ref:`Copy_and_Move_Initialization`.

For example, for integer arguments, the formal argument will store a copy
of the actual argument.

An implicit conversion for a function call occurs from the actual
argument to the type of the formal.

The formal can be modified within the function, but such changes are
local to the function and not reflected back to the call site.

.. _The_Out_Intent:

The Out Intent
^^^^^^^^^^^^^^

The ``out`` intent on a formal argument supports return-like behavior.
As such, the type of an ``out`` formal is not considered when determining
candidate functions or choosing the best candidate (see
:ref:`Function_Resolution`).

When a function with the ``out`` intent returns, the actual argument is
set to the formal argument using assignment or possibly initialized
from the formal argument according to :ref:`Split_Initialization`.

Within the function body, an ``out`` formal argument is initialized
according :ref:`Split_Initialization`. It will start with its default
value if one is supplied and can use the default value for the declared
type if no initialization point is found. The formal argument can be
modified within the function.

.. _The_Inout_Intent:

The Inout Intent
^^^^^^^^^^^^^^^^

When ``inout`` is specified as the intent, the actual argument is
copy-initialized into the formal argument, the called function body is
run, and then the actual argument is set to the formal argument with
assignment. As a result the behavior of the ``inout`` intent is a
combination of the ``in`` and ``out`` intents.

``inout`` intent formals behave the same as ``in`` formals for the
purposes of determining candidate functions and choosing the best
candidate (see :ref:`Function_Resolution`).

The actual argument must be a valid lvalue. The formal argument can be
modified within the function.

.. _The_Ref_Intent:

The Ref Intent
^^^^^^^^^^^^^^

When ``ref`` is specified as the intent, the actual argument is passed
by reference. Any reads of, or modifications to, the formal argument are
performed directly on the corresponding actual argument at the call
site. The actual argument must be a valid lvalue. The type of the actual
argument must be the same as the type of the formal.

The ``ref`` intent differs from the ``inout`` intent in that the
``inout`` intent requires copying from/to the actual argument on the way
in/out of the function, while ``ref`` allows direct access to the actual
argument through the formal argument without copies. Note that
concurrent modifications to the ``ref`` actual argument by other tasks
may be visible within the function, subject to the memory consistency
model.

.. _The_Const_In_Intent:

The Const In Intent
^^^^^^^^^^^^^^^^^^^

The ``const in`` intent is identical to the ``in`` intent, except that
modifications to the formal argument are prohibited within the function.

.. _The_Const_Ref_Intent:

The Const Ref Intent
^^^^^^^^^^^^^^^^^^^^

The ``const ref`` intent is identical to the ``ref`` intent, except that
modifications to the formal argument are prohibited within the dynamic
scope of the function. Note that concurrent tasks may modify the actual
argument while the function is executing and that these modifications
may be visible to reads of the formal argument within the function’s
dynamic scope (subject to the memory consistency model).

.. _Summary_of_Concrete_Intents:

Summary of Concrete Intents
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following table summarizes the differences between the concrete
intents:

================================ ====== ========= ========= =========== ============ =============
\                                ``in`` ``out``   ``inout`` ``ref``     ``const in`` ``const ref``
================================ ====== ========= ========= =========== ============ =============
initializes formal from actual?  yes    no        yes       no          yes          no
sets actual from formal?         no     yes       yes       no          no           no
refers to actual argument?       no     no        no        yes         no           yes
formal can be read?              yes    yes       yes       yes         yes          yes
formal can be modified?          yes    yes       yes       yes         no           no
local changes affect the actual? no     on return on return immediately N/A          N/A
================================ ====== ========= ========= =========== ============ =============

.. _Abstract_Intents:

Abstract Intents
~~~~~~~~~~~~~~~~

The abstract intents are ``const`` and the *default intent* (when no
intent is specified).

.. _Abstract_Intents_Table:

Abstract Intents Table
^^^^^^^^^^^^^^^^^^^^^^

The following table summarizes what these abstract intents mean for each
type:

=================== ================ ======================= ====================================================
\                   meaning of       meaning of             
type                ``const`` intent default intent          notes
``bool``            ``const in``     ``const in``           
``int``             ``const in``     ``const in``           
``uint``            ``const in``     ``const in``           
``real``            ``const in``     ``const in``           
``imag``            ``const in``     ``const in``           
``complex``         ``const in``     ``const in``           
``range``           ``const in``     ``const in``           
``owned class``     ``const ref``    ``const ref``          
``shared class``    ``const ref``    ``const ref``          
``borrowed class``  ``const in``     ``const in``           
``unmanaged class`` ``const in``     ``const in``           
``atomic``          ``const ref``    ``ref``                
``single``          ``const ref``    ``ref``                
``sync``            ``const ref``    ``ref``                
``string``          ``const ref``    ``const ref``          
``bytes``           ``const ref``    ``const ref``          
``record``          ``const ref``    ``const ref``           see :ref:`Default_Intent_for_Arrays_and_Record_this`
``union``           ``const ref``    ``const ref``          
``dmap``            ``const ref``    ``const ref``          
``domain``          ``const ref``    ``const ref``          
array               ``const ref``    ``ref`` / ``const ref`` see :ref:`Default_Intent_for_Arrays_and_Record_this`
tuple               per element      per element             see :ref:`Tuple_Argument_Intents`
=================== ================ ======================= ====================================================

.. _The_Const_Intent:

The Const Intent
^^^^^^^^^^^^^^^^

The ``const`` intent specifies the intention that the function will not
and cannot modify the formal argument within its dynamic scope. Whether
the actual argument will be passed by ``const in`` or ``const ref``
intent depends on its type. In general, small values, such as scalar
types, will be passed by ``const in``; while larger values, such as
domains and arrays, will be passed by ``const ref`` intent. The
:ref:`Abstract_Intents_Table` earlier in this sub-section lists the
meaning of the const intent for each type.

.. _The_Default_Intent:

The Default Intent
^^^^^^^^^^^^^^^^^^

When no intent is specified for a formal argument, the *default intent*
is applied. It is designed to take the most natural/least surprising
action for the argument, based on its type.
The :ref:`Abstract_Intents_Table` earlier in this sub-section lists the
meaning of the default intent for each type.

Default argument passing for tuples generally matches the default
argument passing strategy that would be applied if each tuple element
was passed as a separate argument. See :ref:`Tuple_Argument_Intents`.

.. _Default_Intent_for_Arrays_and_Record_this:

Default Intent for Arrays and Record ’this’
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The default intent for arrays and for a ``this`` argument of record
type (see :ref:`Method_receiver_and_this`) is ``ref`` or
``const ref``. It is ``ref`` if the formal argument is modified inside
the function, otherwise it is ``const ref``. Note that neither of these
cause an array or record to be copied by default. The choice between
``ref`` and ``const ref`` is similar to and interacts with return intent
overloads (see :ref:`Return_Intent_Overloads`).

.. _Default_Intent_for_owned_and_shared:

Default Intent for ’owned’ and ’shared’
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The default intent for ``owned`` and ``shared`` arguments is
``const ref``. Arguments can use the ``in`` or ``const in`` intents to
transfer or share ownership if those arguments apply to ``owned`` or
``shared`` types.

   *Example (owned-any-intent.chpl)*.

   

   .. code-block:: chapel

      proc defaultGeneric(arg) {
        writeln(arg.type:string);
      }
      class SomeClass { }
      var own = new owned SomeClass();
      defaultGeneric(own);
      writeln(own != nil);

   

   .. BLOCK-test-chapeloutput

      owned SomeClass
      true

.. _Variable_Length_Argument_Lists:

Variable Number of Arguments
----------------------------

Functions can be defined to take a variable number of arguments where
those arguments can have any intent or can be types. A variable number
of parameters is not supported. This allows the call site to pass a
different number of actual arguments. There must be at least one actual
argument.

If the variable argument expression contains an identifier prepended by
a question mark, the number of actual arguments can vary, and the
identifier will be bound to an integer parameter value indicating the
number of arguments at a given call site. If the variable argument
expression contains an expression without a question mark, that
expression must evaluate to an integer parameter value requiring the
call site to pass that number of arguments to the function.

Within the function, the formal argument that is marked with a
variable argument expression is a tuple of the actual arguments.  If
the actual arguments all have the same type, the formal will be a
homogeneous tuple, otherwise it will be a heterogeneous tuple.

   *Example (varargs.chpl)*.

   The code 

   .. code-block:: chapel

      proc mywriteln(xs ...?k) {
        for x in xs do
          writeln(x);
      }

   

   .. BLOCK-test-chapelpost

      mywriteln("hi", "there");
      mywriteln(1, 2.0, 3, 4.0);

   

   .. BLOCK-test-chapeloutput

      hi
      there
      1
      2.0
      3
      4.0

   defines a generic procedure called ``mywriteln`` that takes a
   variable number of arguments of any type and then writes them out
   on separate lines.  The type of ``xs`` can also be constrained in
   the formal argument list to require that the actuals all have the
   same type.  For example ``xs: string...?k`` would accept a variable
   number of string arguments.

..

   *Example (varargs-with-type.chpl)*.

   Either or both the number of variable arguments and their types can
   be specified. For example, a basic procedure to sum the values of
   three integers can be written as 

   .. code-block:: chapel

      proc sum(x: int...3) return x(0) + x(1) + x(2);

   

   .. BLOCK-test-chapelpost

      writeln(sum(1, 2, 3));
      writeln(sum(-1, -2, -3));

   

   .. BLOCK-test-chapeloutput

      6
      -6

   Specifying the type is useful if it is important that each argument
   have the same type. Specifying the number is useful in, for example,
   defining a method on a class that is instantiated over a rank
   parameter.

   *Example (varargs-returns-tuples.chpl)*.

   The code 

   .. code-block:: chapel

      proc tuple(x ...) return x;

   

   .. BLOCK-test-chapelpost

      writeln(tuple(1));
      writeln(tuple("hi", "there"));
      writeln(tuple(tuple(1, 2), tuple(3, 4)));

   

   .. BLOCK-test-chapeloutput

      (1)
      (hi, there)
      ((1, 2), (3, 4))

   defines a generic procedure that is equivalent to building a tuple.
   Therefore the expressions ``tuple(1, 2)`` and ``(1,2)`` are
   equivalent, as are the expressions ``tuple(1)`` and ``(1,)``.

.. _Return_Intent:

Return Intents
--------------

The ``return-intent`` specifies how the value is returned from a
function, and in what contexts that function is allowed to be used. By
default, or if the ``return-intent`` is ``const``, the function returns
a value that cannot be used as an lvalue.

.. _Ref_Return_Intent:

The Ref Return Intent
~~~~~~~~~~~~~~~~~~~~~

When using a ``ref`` return intent, the function call is an lvalue
(specifically, a call expression for a procedure and an iterator
variable for an iterator).

The ``ref`` return intent is specified by following the argument list
with the ``ref`` keyword. The function must return or yield an lvalue.

   *Example (ref-return-intent.chpl)*.

   The following code defines a procedure that can be interpreted as a
   simple two-element array where the elements are actually module level
   variables: 

   .. code-block:: chapel

      var x, y = 0;

      proc A(i: int) ref {
        if i < 0 || i > 1 then
          halt("array access out of bounds");
        if i == 0 then
          return x;
        else
          return y;
      }

   Calls to this procedure can be assigned to in order to write to the
   “elements” of the array as in 

   .. code-block:: chapel

      A(0) = 1;
      A(1) = 2;

   It can be called as an expression to access the “elements” as in
   

   .. code-block:: chapel

      writeln(A(0) + A(1));

   This code outputs the number ``3``.

   

   .. BLOCK-test-chapeloutput

      3

.. _Const_Ref_Return_Intent:

The Const Ref Return Intent
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``const ref`` return intent is also available. It is a restricted
form of the ``ref`` return intent. Calls to functions marked with the
``const ref`` return intent are not lvalue expressions.

.. _Return_Intent_Overloads:

Return Intent Overloads
~~~~~~~~~~~~~~~~~~~~~~~

In some situations, it is useful to choose the function called based
upon how the returned value is used. In particular, suppose that there
are two functions that have the same formal arguments and differ only in
their return intent. One might expect such a situation to result in an
error indicating that it is ambiguous which function is called. However,
the Chapel language includes a special rule for determining which
function to call when the candidate functions are otherwise ambiguous
except for their return intent. This rule enables data structures such
as sparse arrays.

See :ref:`Choosing_Return_Intent_Overload` for a detailed
description of how return intent overloads are chosen based upon calling
context.

   *Example (ref-return-intent-pair.chpl)*.

   Return intent overload can be used to ensure, for example, that the
   second element in the pseudo-array is only assigned a value if the
   first argument is positive. The following is an example: 

   .. code-block:: chapel

      var x, y = 0;

      proc doA(param setter, i: int) ref {
        if i < 0 || i > 1 then
          halt("array access out of bounds");

        if setter && i == 1 && x <= 0 then
          halt("cannot assign value to A(1) if A(0) <= 0");

        if i == 0 then
          return x;
        else
          return y;
      }
      proc A(i: int) ref {
        return doA(true, i);
      }
      proc A(i: int) {
        return doA(false, i);
      }

      A(0) = 0;
      A(1) = 1; 

   

   .. BLOCK-test-chapeloutput

      ref-return-intent-pair.chpl:8: error: halt reached - cannot assign value to A(1) if A(0) <= 0

.. _Param_Return_Intent:

The Param Return Intent
~~~~~~~~~~~~~~~~~~~~~~~

A *parameter function*, or a *param function*, is a function that
returns a parameter expression. It is specified by following the
function’s argument list by the keyword ``param``. It is often, but not
necessarily, generic.

It is a compile-time error if a parameter function does not return a
parameter expression. The result of a parameter function is computed
during compilation and substituted for the call expression.

   *Example (param-functions.chpl)*.

   In the code 

   .. code-block:: chapel

      proc sumOfSquares(param a: int, param b: int) param
        return a**2 + b**2;

      var x: sumOfSquares(2, 3)*int;

   

   .. BLOCK-test-chapelpost

      writeln(x);

   

   .. BLOCK-test-chapeloutput

      (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

   ``sumOfSquares`` is a parameter procedure that takes two parameters
   as arguments. Calls to this procedure can be used in places where a
   parameter expression is required. In this example, the call is used
   in the declaration of a homogeneous tuple and so is required to be a
   parameter.

Parameter functions may not contain control flow that is not resolved at
compile-time. This includes loops other than the parameter for
loop :ref:`Parameter_For_Loops` and conditionals with a
conditional expressions that is not a parameter.

.. _Type_Return_Intent:

The Type Return Intent
~~~~~~~~~~~~~~~~~~~~~~

A *type function* is a function that returns a type, not a value. It is
specified by following the function’s argument list by the keyword
``type``, without the subsequent return type. It is often, but not
necessarily, generic.

It is a compile-time error if a type function does not return a type.
The result of a type function is computed during compilation.

As with parameter functions, type functions may not contain control flow
that is not resolved at compile-time. This includes loops other than the
parameter for loop :ref:`Parameter_For_Loops` and conditionals
with a conditional expression that is not a parameter.

   *Example (type-functions.chpl)*.

   In the code 

   .. code-block:: chapel

      proc myType(x) type {
        if numBits(x.type) <= 32 then return int(32);
        else return int(64);
      }

   

   .. BLOCK-test-chapelpost

      var a = 4: int(32),
          b = 4.0;
      var at: myType(a),
          bt: myType(b);
      writeln((numBits(at.type), numBits(bt.type)));

   

   .. BLOCK-test-chapeloutput

      (32, 64)

   ``myType`` is a type procedure that takes a single argument ``x`` and
   returns ``int(32)`` if the number of bits used to represent ``x`` is
   less than or equal to 32, otherwise it returns ``int(64)``.
   ``numBits`` is a param procedure defined in the standard Types
   module.

.. _The_Return_Statement:

The Return Statement
--------------------

The return statement can only appear in a function. It causes control to
exit that function, returning it to the point at which that function was
called.

A procedure can return a value by executing a return statement that
includes an expression. If it does, that expression’s value becomes the
value of the invoking call expression.

A return statement in a procedure of a non-\ ``void`` return type
(:ref:`Return_Types`) must include an expression. A return
statement in a procedure of a ``void`` return type or in an iterator
must not include an expression. A return statement of a variable
procedure must contain an lvalue expression.

The syntax of the return statement is given by 

.. code-block:: syntax

   return-statement:
     'return' expression[OPT] ;

..

   *Example (return.chpl)*.

   The following code defines a procedure that returns the sum of three
   integers: 

   .. code-block:: chapel

      proc sum(i1: int, i2: int, i3: int)
        return i1 + i2 + i3;

   

   .. BLOCK-test-chapelpost

      writeln(sum(1, 2, 3));

   

   .. BLOCK-test-chapeloutput

      6

.. _Return_Types:

Return Types
------------

Every procedure has a return type. The return type is either specified
explicitly via ``return-type`` in the procedure declaration, or is
inferred implicitly.

.. _Explicit_Return_Types:

Explicit Return Types
~~~~~~~~~~~~~~~~~~~~~

If a return type is specified and is not ``void``, each return statement
of the procedure must include an expression. For a non-\ ``ref`` return
intent, an implicit conversion occurs from each return expression to the
specified return type. For a ``ref`` return
intent (:ref:`Ref_Return_Intent`), the return type must match
the type returned in all of the return statements exactly, when checked
after generic instantiation and parameter folding (if applicable).

.. _Implicit_Return_Types:

Implicit Return Types
~~~~~~~~~~~~~~~~~~~~~

If a return type is not specified, it is inferred from the return
statements. It is illegal for a procedure to have a return statement
with an expression and a return statement without an expression. For
procedures without any return statements, or when none of the return
statements include an expression, the return type is ``void``.

Otherwise, the types of the expressions in all of the procedure’s return
statements are considered. If a function has a ``ref`` return intent
(:ref:`Ref_Return_Intent`), they all must be the same exact
type, which becomes the inferred return type. Otherwise, there must
exist exactly one type such that an implicit conversion is allowed
between every other type and that type, and that type becomes the
inferred return type. If the above requirements are not satisfied, it is
an error.

.. _Where_Clauses:

Where Clauses
-------------

The list of function candidates can be constrained by *where clauses*. A
where clause is specified in the definition of a
function (:ref:`Function_Definitions`). The expression in the
where clause must be a boolean parameter expression that evaluates to
either ``true`` or ``false``. If it evaluates to ``false``, the function
is rejected and thus is not a possible candidate for function
resolution.

   *Example (whereClause.chpl)*.

   Given two overloaded function definitions 

   .. code-block:: chapel

      proc foo(x) where x.type == int { writeln("int"); }
      proc foo(x) where x.type == real { writeln("real"); }

   

   .. BLOCK-test-chapelpost

      foo(3);
      foo(3.14);

   

   .. BLOCK-test-chapeloutput

      int
      real

   the call foo(3) resolves to the first definition because the where
   clause on the second function evaluates to false.

.. _Nested_Functions:

Nested Functions
----------------

A function defined in another function is called a nested function.
Nesting of functions may be done to arbitrary degrees, i.e., a function
can be nested in a nested function.

Nested functions are only visible to function calls within the lexical
scope in which they are defined.

Nested functions may refer to variables defined in the function(s) in
which they are nested.

.. _Function_Overloading:

Function and Operator Overloading
---------------------------------

Functions that have the same name but different argument lists are
called overloaded functions. Function calls to overloaded functions are
resolved according to the function resolution algorithm
in :ref:`Function_Resolution`.

To define an overloaded operator, use the ``operator`` keyword to define a
function with the same name as the operator.  The operators that may be
overloaded are listed in the following table:

======== ===============================
arity    operators
======== ===============================
unary    ``+`` ``-`` ``!`` ``~``
binary   ``+`` ``-`` ``*`` ``/`` ``%`` ``**`` ``:``
binary   ``==`` ``<=`` ``>=`` ``<`` ``>``
binary   ``<<`` ``>>`` ``&`` ``|`` ``^`` ``#`` ``align`` ``by``
binary   ``=`` ``+=`` ``-=`` ``*=`` ``/=`` ``%=`` ``**=``
binary   ``&=`` ``|=`` ``^=`` ``<<=`` ``>>=`` ``<=>`` ``<~>``
======== ===============================

The arity and precedence of the operator must be maintained when it is
overloaded. Operator resolution follows the same algorithm as function
resolution.

Assignment overloads are not supported for class types.

.. _Function_Resolution:

Function Resolution
-------------------

*Function resolution* is the algorithm that determines which
*target function* to invoke for a given call expression.
Function resolution is defined as follows.

-  Identify the set of visible functions for the function call. A
   *visible function* is any function that satisfies the criteria
   in :ref:`Determining_Visible_Functions`. If no visible
   function can be found, the compiler will issue an error stating that
   the call cannot be resolved.

-  From the set of visible functions for the function call, determine
   the set of candidate functions for the function call. A *candidate
   function* is any function that satisfies the criteria
   in :ref:`Determining_Candidate_Functions`. If no candidate
   function can be found and the call is within a generic function,
   its point of instantiation(s) are visited searching for candidates
   as defined in :ref:`Function_Visibility_in_Generic_Functions`.
   If still no candidate functions are found,
   the compiler will issue an error stating that
   the call cannot be resolved. If exactly one candidate function is
   found, this is determined to be the target function.

-  From the set of candidate functions, determine the set of most
   specific functions. In most cases, there is one most specific
   function, but there can be several if they differ only in return
   intent. The set of most specific functions is the set of functions
   that are not *more specific* than each other but that are *more
   specific* than every other candidate function. The *more specific*
   relationship is defined in
    :ref:`Determining_More_Specific_Functions`.

-  From the set of most specific functions, the compiler determines a
   best function for each return intent as described in
    :ref:`Determining_Best_Functions`. If there is more than
   one best function for a given return intent, the compiler will issue
   an error stating that the call is ambiguous. Otherwise, it will choose
   the target function from these best functions based on the calling
   context as described in :ref:`Choosing_Return_Intent_Overload`.

Notation
~~~~~~~~

This section uses the following notation:

 * :math:`X` is a function under consideration
 * The actual argument under consideration is :math:`A` of type
   :math:`T_A`
 * :math:`M_X` represents the argument mapping from :math:`A` to the
   formal argument :math:`F_X` from function :math:`X`. :math:`F_X` has
   type :math:`T_X`. When :math:`X` is a generic function, :math:`F_X`
   refers to the possibly generic argument and :math:`T_X` refers to
   the instantiated type.
 * When needed in the exposition, :math:`Y` is another function under
   consideration, with mapping :math:`M_Y` from :math:`A` to a formal
   argument :math:`F_Y` of type :math:`T_Y`.

.. _Determining_Visible_Functions:

Determining Visible Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Given a function call, a function :math:`X` is determined to be a
*visible function* if its name is the same as the name of the
function call and one of the following conditions is met:

- :math:`X` is defined in the same scope as the
  function call or in a lexical outer scope of the function call, or

- :math:`X` is ``public`` and is declared in a module that is used from
  the same scope as the function call or from its lexical outer scope,
  see also :ref:`Using_Modules`, or

- :math:`X` is ``public`` and is declared in a module that is imported from
  the same scope as the function call or from its lexical outer scope,
  and the call qualifies the function name with the module name,
  see also :ref:`Importing_Modules`.

   *Open issue*.

   What should be the visibility of methods? Applying the above rules
   excludes, for example, the methods defined in the same module as the
   receiver type when that module is neither visible nor reachable
   through module uses or imports from the scope of the function call.

.. _Determining_Candidate_Functions:

Determining Candidate Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Given a function call, a function is determined to be a *candidate
function* if there is a *valid mapping* from the function call to the
function where each actual argument is mapped to a formal argument with
a *legal argument mapping*.

.. _Valid_Mapping:

Valid Mapping
^^^^^^^^^^^^^

The following algorithm determines a valid mapping from a function call
to a function if one exists:

-  Each actual argument that is passed by name is matched to the formal
   argument with that name. If there is no formal argument with that
   name, there is no valid mapping.

-  The remaining actual arguments are mapped in order to the remaining
   formal arguments in order. If there are more actual arguments then
   formal arguments, there is no valid mapping. If any formal argument
   that is not mapped to by an actual argument does not have a default
   value, there is no valid mapping.

-  The valid mapping is the mapping of actual arguments to formal
   arguments plus default values to formal arguments that are not mapped
   to by actual arguments.

.. _Legal_Argument_Mapping:

Legal Argument Mapping
^^^^^^^^^^^^^^^^^^^^^^

An actual argument :math:`A` of type :math:`T_A` can be legally mapped to
a formal argument :math:`F_X` according to the following rules.

First, if :math:`A` is a ``type`` but :math:`F_X` does not use the
``type`` intent, then it is not a legal argument mapping.

Then, if :math:`F_X` is a generic argument:

 * if :math:`F_X` uses ``param`` intent, then :math:`A` must also be a
   ``param``
 * if :math:`F_X` uses ``type`` intent, then :math:`A` must also be a
   ``type``
 * there must exist an instantiation :math:`T_X` of the generic declared
   type of :math:`F_X`, if any, that is compatible with the type
   :math:`T_A` according to the rules below.

Next, the type :math:`T_X` - which is either the declared type of the
formal argument :math:`F_X` if it is concrete or the instantiated type if
:math:`F_X` is generic - must be compatible with the type :math:`T_A`
according to the concrete intent of :math:`F_X`:

 * if :math:`F_X` uses ``ref`` intent, then :math:`T_A`
   must be the same type as :math:`T_X`
 * if :math:`F_X` uses ``const ref`` intent, then :math:`T_A` and
   :math:`T_X` must be the same type or a subtype of :math:`T_X`
 * if :math:`F_X` uses ``in`` or ``inout`` intent, then :math:`T_A`
   must be the same type, a subtype of, or implicitly convertible to
   :math:`T_X`.
 * if :math:`F_X` uses  the ``out`` intent, it is always a legal
   argument mapping regardless of the type of the actual and formal.
   In the event that setting :math:`T_A` from :math:`F_X` is not
   possible then a compilation error will be emitted if this function
   is chosen as the best candidate.
 * if :math:`F_X` uses the ``type`` intent, then :math:`T_A`
   must be the same type or a subtype of :math:`T_X`.

Finally, if the above compatibility cannot be established, the mapping is
checked for promotion. If :math:`T_A` is scalar promotable to :math:`T_X`
(see :ref:`Promotion`), then the above rules are checked with the element
type, index type, or yielded type.  For example, if :math:`T_A` is an
array of ``int`` and :math:`T_X` is ``int``, then promotion occurs and
the above rules will be checked with :math:`T_A` == ``int``.

.. _Determining_More_Specific_Functions:

Determining More Specific Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Given two candidate functions, :math:`X` and :math:`Y`, the more specific
function is determined by the first of the following steps that applies:

-  If :math:`X` does not require promotion and :math:`Y` does
   require promotion, then :math:`X` is more specific.

-  If :math:`Y` does not require promotion and :math:`X` does
   require promotion, then :math:`Y` is more specific.

-  If at least one of the legal argument mappings to :math:`X` is a
   *more specific argument mapping* than the corresponding legal
   argument mapping to :math:`Y` and none of the legal argument
   mappings to :math:`Y` is a more specific argument mapping than the
   corresponding legal argument mapping to :math:`X`, then :math:`X`
   is more specific.

-  If at least one of the legal argument mappings to :math:`Y` is a
   *more specific argument mapping* than the corresponding legal
   argument mapping to :math:`X` and none of the legal argument
   mappings to :math:`X` is a more specific argument mapping than the
   corresponding legal argument mapping to :math:`Y`, then :math:`Y`
   is more specific.

-  If :math:`X` shadows :math:`Y`, then :math:`X` is more
   specific.

-  If :math:`Y` shadows :math:`X`, then :math:`Y` is more
   specific.

-  If at least one of the legal argument mappings to :math:`X` is
   *weak preferred* and none of the legal argument mappings to
   :math:`Y` are *weak preferred*, then :math:`X` is more specific.

-  If at least one of the legal argument mappings to :math:`Y` is
   *weak preferred* and none of the legal argument mappings to
   :math:`X` are *weak preferred*, then :math:`Y` is more specific.

-  If at least one of the legal argument mappings to :math:`X` is
   *weaker preferred* and none of the legal argument mappings to
   :math:`Y` are *weaker preferred*, then :math:`X` is more
   specific.

-  If at least one of the legal argument mappings to :math:`Y` is
   *weaker preferred* and none of the legal argument mappings to
   :math:`X` are *weaker preferred*, then :math:`Y` is more
   specific.

-  If at least one of the legal argument mappings to :math:`X` is
   *weakest preferred* and none of the legal argument mappings to
   :math:`Y` are *weakest preferred*, then :math:`X` is more
   specific.

-  If at least one of the legal argument mappings to :math:`Y` is
   *weakest preferred* and none of the legal argument mappings to
   :math:`X` are *weakest preferred*, then :math:`Y` is more
   specific.

-  Otherwise neither function is more specific.

The next section discusses the level of preference for an argument
mapping. As discussed above, :math:`M_X` represents the argument mapping
from :math:`A` to the formal argument :math:`F_X` from function :math:`X`
with type :math:`T_X`. When :math:`X` is a generic function, :math:`F_X`
refers to the argument before instantiation and :math:`T_X` represents
the type of :math:`F_X` after instantiation. :math:`M_Y`, :math:`F_Y`,
and :math:`T_Y` are defined in a similar manner to represent the argument
mapping for :math:`Y`.

The level of preference for one of these argument mappings is determined
by the first of the following steps that applies:

-  If :math:`F_X` or :math:`F_Y` uses the ``out`` intent, then neither
   argument mapping is preferred.

-  If :math:`T_X` and :math:`T_Y` are the same type, :math:`F_X` is
   an instantiated parameter, and :math:`F_Y` is not an instantiated
   parameter, :math:`M_X` is more specific.

-  If :math:`T_X` and :math:`T_Y` are the same type, :math:`F_Y` is
   an instantiated parameter, and :math:`F_X` is not an instantiated
   parameter, :math:`M_Y` is more specific.

-  If :math:`M_X` does not require scalar promotion and :math:`M_Y`
   requires scalar promotion, :math:`M_X` is more specific.

-  If :math:`M_X` requires scalar promotion and :math:`M_Y` does not
   require scalar promotion, :math:`M_Y` is more specific.

-  If :math:`T_X` and :math:`T_Y` are the same type, :math:`F_X` is
   generic, and :math:`F_Y` is not generic, :math:`M_X` is more specific.

-  If :math:`T_X` and :math:`T_Y` are the same type, :math:`F_Y` is
   generic, and :math:`F_X` is not generic, :math:`M_Y` is more specific.

-  If :math:`F_X` is not generic over all types and :math:`F_Y` is generic
   over all types, :math:`M_X` is more specific.

-  If :math:`F_X` is generic over all types and :math:`F_Y` is not generic
   over all types, :math:`M_Y` is more specific.

-  If :math:`F_X` and :math:`F_Y` are both generic, and :math:`F_X` is
   partially concrete but :math:`F_Y` is not, then :math:`M_X` is more
   specific.

-  If :math:`F_X` and :math:`F_Y` are both generic, and :math:`F_Y` is
   partially concrete but :math:`F_X` is not, then :math:`M_Y` is more
   specific.

-  If :math:`F_X` is a ``param`` argument but :math:`F_Y` is not, then
   :math:`M_X` is weak preferred.

-  If :math:`F_Y` is a ``param`` argument but :math:`F_X` is not, then
   :math:`M_Y` is weak preferred.

-  If :math:`A` is not a ``param`` argument with a default size and
   :math:`F_Y` requires a narrowing conversion but :math:`F_X` does not,
   then :math:`M_X` is weak preferred.

-  If :math:`A` is not a ``param`` argument with a default size and
   :math:`F_X` requires a narrowing conversion but :math:`F_Y` does not,
   then :math:`M_Y` is weak preferred.

-  If :math:`T_A` and :math:`T_X` are the same type and :math:`T_A`
   and :math:`T_Y` are not the same type, :math:`M_X` is more
   specific.

-  If :math:`T_A` and :math:`T_X` are not the same type and
   :math:`T_A` and :math:`T_Y` are the same type, :math:`M_Y` is more
   specific.

-  If :math:`A` uses a scalar promotion type equal to :math:`T_X` but
   different from :math:`T_Y`, then :math:`M_X` will be preferred as
   follows:

   -  if :math:`A` is a ``param`` argument with a default size, then
      :math:`M_X` is weakest preferred

   -  if :math:`A` is a ``param`` argument with non-default size, then
      :math:`M_X` is weaker preferred

   -  otherwise, :math:`M_X` is more specific

-  If :math:`A` uses a scalar promotion type equal to :math:`T_Y` but
   different from :math:`T_X`, then :math:`M_Y` will be preferred as
   follows:

   -  if :math:`A` is a ``param`` argument with a default size, then
      :math:`M_Y` is weakest preferred

   -  if :math:`A` is a ``param`` argument with non-default size, then
      :math:`M_Y` is weaker preferred

   -  otherwise, :math:`M_Y` is more specific

-  If :math:`T_A` or its scalar promotion type prefers conversion to
   :math:`T_X` over conversion to :math:`T_Y`, then :math:`M_X` is
   preferred. If :math:`A` is a ``param`` argument with a default size,
   then :math:`M_X` is weakest preferred. Otherwise, :math:`M_X` is
   weaker preferred.

   Type conversion preferences are as follows:

   -  Prefer converting a numeric argument to a numeric argument of a
      different width but the same category over converting to another
      type. Categories are

      -  bool

      -  enum

      -  int or uint

      -  real

      -  imag

      -  complex

   -  Prefer an enum or bool cast to int over uint

   -  Prefer an enum or bool cast to a default-sized int or uint over
      another size of int or uint

   -  Prefer an enum, bool, int, or uint cast to a default-sized real
      over another size of real or complex

   -  Prefer an enum, bool, int, or uint cast to a default-sized complex
      over another size of complex

   -  Prefer real/imag cast to the complex with that component size (ie
      total width of twice the real/imag) over another size of complex

-  If :math:`T_A` or its scalar promotion type prefers conversion to
   :math:`T_Y` over conversion to :math:`T_X`, then :math:`M_Y` is
   preferred. If :math:`A` is a ``param`` argument with a default size,
   then :math:`M_Y` is weakest preferred. Otherwise, :math:`M_Y` is
   weaker preferred.

-  If :math:`T_X` is derived from :math:`T_Y`, then :math:`M_X` is
   more specific.

-  If :math:`T_Y` is derived from :math:`T_X`, then :math:`M_Y` is
   more specific.

-  If there is an implicit conversion from :math:`T_X` to
   :math:`T_Y`, then :math:`M_X` is more specific.

-  If there is an implicit conversion from :math:`T_Y` to
   :math:`T_X`, then :math:`M_Y` is more specific.

-  If :math:`T_X` is any ``int`` type and :math:`T_Y` is any
   ``uint`` type, :math:`M_X` is more specific.

-  If :math:`T_Y` is any ``int`` type and :math:`T_X` is any
   ``uint`` type, :math:`M_Y` is more specific.

-  Otherwise neither mapping is more specific.

.. _Determining_Best_Functions:

Determining Best Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~

Given the set of most specific functions for a given return intent, only
the following function(s) are selected as best functions:

-  all functions, if none of them contain a ``where`` clause;

-  only those functions that have a ``where`` clause, otherwise.

.. _Choosing_Return_Intent_Overload:

Choosing Return Intent Overloads Based on Calling Context
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See also :ref:`Return_Intent_Overloads`.

The compiler can choose between overloads differing in return intent
when:

-  there are zero or one best functions for each of ``ref``,
   ``const ref``, ``const``, or the default (blank) return intent

-  at least two of the above return intents have a best function.

In that case, the compiler is able to choose between ``ref`` return,
``const ref`` return, and value return functions based upon the context
of the call. The compiler chooses between these return intent overloads
as follows:

If present, a ``ref`` return version will be chosen when:

-  the call appears on the left-hand side of a variable initialization
   or assignment statement

-  the call is passed to another function as a formal argument with
   ``out``, ``inout``, or ``ref`` intent

-  the call is captured into a ``ref`` variable

-  the call is returned from a function with ``ref`` return intent

Otherwise, the ``const ref`` return or value return version will be
chosen. If only one of these is in the set of most specific functions,
it will be chosen. If both are present in the set, the choice will be
made as follows:

The ``const ref`` version will be chosen when:

-  the call is passed to another function as a formal argument with
   ``const ref`` intent

-  the call is captured into a ``const ref`` variable

-  the call is returned from a function with ``const ref`` return intent

Otherwise, the value version will be chosen.
