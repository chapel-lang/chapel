.. default-domain:: chpl

.. _Chapter-Procedures:

==========
Procedures
==========

A *function* is a code abstraction that can be invoked by a call
expression. Throughout this specification the term “function” is used in
this programming-languages sense, rather than in the mathematical sense.
A function has zero or more *formal arguments*, or simply *formals*.
Upon a function call each formal is associated with the corresponding
*actual argument*, or simply *actual*. Actual arguments are provided as
part of the call expression, or at the *call site*. Direct and
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
parentheses. Functions without parentheses are discussed
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
     = += -= *= /= %= **= &= |= ^= <<= >>= <=> #

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

   default-expression:
     = expression

   variable-argument-expression:
     ... expression
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
     'do' statement
     block-statement

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

The ``function-body`` defines the function's behavior and is defined
in :ref:`The_Function_Body`.  Function bodies may contain return
statements (see :ref:`The_Return_Statement`).

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
variable that is copy-initialized from the value of the actual argument,
see :ref:`Copy_and_Move_Initialization`.

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

Note that the way that type inference works with generic ``out`` formal
arguments is very different from other formal arguments. In particular,
the type of a generic ``out`` formal argument is inferred from the
function body rather than from the call site.

.. note::

   If the type of an ``out`` argument needs to be inferred based upon the
   call site, there are currently two approaches available:

     * use a separate ``type`` argument to pass the type
     * use the ``ref`` intent instead of the ``out`` intent

   There is proposal that including a type query (e.g.  ``?t`` in an
   ``out`` argument will cause the type to be inferred based upon the
   call site. However this is not yet implemented, at the time of this
   writing.

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
scope of the function. Note that the same or concurrent tasks may modify the
actual argument while the function is executing and that these modifications
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

.. _The_Const_Intent:

The Const Intent
^^^^^^^^^^^^^^^^

The ``const`` intent specifies that the function will not and cannot
modify the formal argument within its dynamic scope. Whether ``const``
is interpreted as ``const in`` or ``const ref`` intent depends on the
argument type.  Generally, small values, such as scalar types, will be
passed by ``const in``; while larger values, such as domains and
arrays, will be passed by ``const ref`` intent. The
:ref:`Abstract_Intents_Table` below lists the meaning of the ``const``
intent for each type.

.. _The_Default_Intent:

The Default Intent
^^^^^^^^^^^^^^^^^^

When no intent is specified for a formal argument, the *default
intent* is applied.  It is designed to take the most natural/least
surprising action for the argument, based on its type.  In practice,
this is ``const`` for most types (as defined by
:ref:`The_Const_Intent`) to avoid surprises for programmers coming
from languages where everything is passed by ``in`` or ``ref`` intent
by default.  Exceptions are made for types where modification is
considered part of their nature, such as types used for synchronization
(like ``atomic``) and arrays.

Default argument passing for tuples applies the default
argument passing strategy to each tuple component as if it
was passed as a separate argument. See :ref:`Tuple_Argument_Intents`.

The :ref:`Abstract_Intents_Table` that follows defines the default
intent for each type.

.. _Abstract_Intents_Table:

Abstract Intents Table
^^^^^^^^^^^^^^^^^^^^^^

The following table summarizes what these abstract intents mean for each
type:

.. table::
    :widths: 28 18 22 32

    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    |                         | ``const`` intent     |                         |                                                      |
    | Type                    | meaning              | Default intent meaning  | Notes                                                |
    +=========================+======================+=========================+======================================================+
    | scalar types            |  ``const in``        | ``const in``            |                                                      |
    |                         |                      |                         |                                                      |
    | (``bool``,              |                      |                         |                                                      |
    | ``int``, ``uint``,      |                      |                         |                                                      |
    | ``real``, ``imag``,     |                      |                         |                                                      |
    | ``complex``)            |                      |                         |                                                      |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | string-like types       | ``const ref``        | ``const ref``           |                                                      |
    |                         |                      |                         |                                                      |
    | (``string``, ``bytes``) |                      |                         |                                                      |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | ranges                  | ``const in``         | ``const in``            |                                                      |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | domains / domain maps   | ``const ref``        | ``const ref``           |                                                      |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | arrays                  | ``const ref``        | ``ref`` / ``const ref`` | see :ref:`Default_Intent_for_Arrays_and_Record_this` |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | records                 | ``const ref``        | ``const ref``           | see :ref:`Default_Intent_for_Arrays_and_Record_this` |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | auto-managed classes    | ``const ref``        | ``const ref``           | see :ref:`Default_Intent_for_owned_and_shared`       |
    | (``owned``, ``shared``) |                      |                         |                                                      |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | non-managed classes     | ``const in``         | ``const in``            |                                                      |
    |                         |                      |                         |                                                      |
    | (``borrowed``,          |                      |                         |                                                      |
    | ``unmanaged``)          |                      |                         |                                                      |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | tuples                  | per-element          | per-element             | see :ref:`Tuple_Argument_Intents`                    |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | unions                  | ``const ref``        | ``const ref``           |                                                      |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+
    | synchronization types   | ``const ref``        | ``ref``                 |                                                      |
    | (``atomic``,            |                      |                         |                                                      |
    | ``sync``, ``single``)   |                      |                         |                                                      |
    +-------------------------+----------------------+-------------------------+------------------------------------------------------+

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

      proc sum(x: int...3) do return x(0) + x(1) + x(2);

   

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

      proc tuple(x ...) do return x;

   

   .. BLOCK-test-chapelpost

      writeln(tuple(1));
      writeln(tuple("hi", "there"));
      writeln(tuple(tuple(1, 2), tuple(3, 4)));

   

   .. BLOCK-test-chapeloutput

      (1,)
      (hi, there)
      ((1, 2), (3, 4))

   defines a generic procedure that is equivalent to building a tuple.
   Therefore the expressions ``tuple(1, 2)`` and ``(1,2)`` are
   equivalent, as are the expressions ``tuple(1)`` and ``(1,)``.

.. _Return_Intent:

Return Intents
--------------

The ``return-intent`` determines how the value is returned from a
function and in what contexts that function is allowed to be used.
The rules for returning tuples are specified in :ref:`Tuple_Return_Behavior`.

.. _Default_Return_Intent:

The Default Return Intent
~~~~~~~~~~~~~~~~~~~~~~~~~

When no ``return-intent`` is specified explicitly, the function returns
a value that cannot be used as an lvalue. This value is obtained
by copy-initialization from the returned expression,
see :ref:`Copy_and_Move_Initialization`.

.. _Const_Return_Intent:

The Const Return Intent
~~~~~~~~~~~~~~~~~~~~~~~~~

The ``const`` return intent is identical to the default return intent.

.. _Ref_Return_Intent:

The Ref Return Intent
~~~~~~~~~~~~~~~~~~~~~

When using a ``ref`` return intent, the function call is an lvalue
(specifically, a call expression for a procedure and an iterator
variable for an iterator).

The ``ref`` return intent is specified by following the argument list
with the ``ref`` keyword. The function must return an lvalue that
exists outside of the function's scope.

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

      proc sumOfSquares(param a: int, param b: int) param do
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


.. _The_Function_Body:

Function Bodies
---------------

The body of a procedure or iterator is made up of one or more
statements that are executed when a call to the function is made.
Function bodies can always be specified using a compound or _block_
statement (:ref:`Blocks`), set off by curly brackets.  When a
function's body is just a single statement, the `do` keyword can be
used as a shorthand for defining the body instead, similar to other
forms of control flow.


.. _The_Return_Statement:

The Return Statement
~~~~~~~~~~~~~~~~~~~~

The return statement can only appear in a function body. It causes
control to exit that function, returning it to the point at which that
function was called.

A procedure can return a value by executing a return statement that
includes an expression. If it does, that expression’s value becomes the
value of the invoking call expression.

A return statement in a procedure of a ``void`` return type
(:ref:`Return_Types`) or in an iterator must not include an
expression.  A return statement in a procedure of a non-\ ``void``
return type must include an expression.  For procedures with ``ref``
or ``const ref`` return intent, the expression must have storage
associated with it that will outlive the procedure itself.

The statements following a return statement in the same block
are ignored by the compiler because they cannot be executed.

The syntax of the return statement is given by 

.. code-block:: syntax

   return-statement:
     'return' expression[OPT] ;

..

   *Example (return.chpl)*.

   The following code defines a procedure that returns the sum of three
   integers: 

   .. code-block:: chapel

      proc sum(i1: int, i2: int, i3: int) do
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
binary   ``&=`` ``|=`` ``^=`` ``<<=`` ``>>=`` ``<=>``
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
   specific functions as described in
    :ref:`Determining_Most_Specific_Functions`. In most cases, if the set
   of most specific functions contains more than one element, it will
   result in an ambiguity error. However, there can be several if they
   differ only in return intent.

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

* An actual argument under consideration is :math:`A_i` of type
  :math:`T(A_i)`

* The formal argument in function :math:`X` that will receive :math:`A_i`
  is :math:`X_i` and it has type :math:`T(X_i)`. When :math:`X` is a
  generic function, :math:`X_i` refers to the possibly generic argument
  and :math:`T(X_i)` refers to the instantiated type.

* When needed in the exposition, :math:`Y` is another function under
  consideration, where :math:`A_i` is passed to the formal :math:`Y_i` of
  type :math:`T(Y_i)`.

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

- :math:`X` is a method and it is defined in the same module that defines
  the receiver type.

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

An actual argument :math:`A_i` of type :math:`T(A_i)` can be legally mapped to
a formal argument :math:`X_i` according to the following rules.

First, if :math:`A_i` is a ``type`` but :math:`X_i` does not use the
``type`` intent, then it is not a legal argument mapping.

Then, if :math:`X_i` is a generic argument:

 * if :math:`X_i` uses ``param`` intent, then :math:`A_i` must also be a
   ``param``
 * if :math:`X_i` uses ``type`` intent, then :math:`A_i` must also be a
   ``type``
 * there must exist an instantiation :math:`T(X_i)` of the generic declared
   type of :math:`X_i`, if any, that is compatible with the type
   :math:`T(A_i)` according to the rules below.

Next, the type :math:`T(X_i)` - which is either the declared type of the
formal argument :math:`X_i` if it is concrete or the instantiated type if
:math:`X_i` is generic - must be compatible with the type :math:`T(A_i)`
according to the concrete intent of :math:`X_i`:

 * if :math:`X_i` uses ``ref`` intent, then :math:`T(A_i)`
   must be the same type as :math:`T(X_i)`
 * if :math:`X_i` uses ``const ref`` intent, then :math:`T(A_i)` and
   :math:`T(X_i)` must be the same type or a subtype of :math:`T(X_i)` (see
   :ref:`Subtype_Arg_Conversions`)
 * if :math:`X_i` uses ``in`` or ``inout`` intent, then :math:`T(A_i)`
   must be the same type, a subtype of, or implicitly convertible to
   :math:`T(X_i)`.
 * if :math:`X_i` uses  the ``out`` intent, it is always a legal
   argument mapping regardless of the type of the actual and formal.
   In the event that setting :math:`T(A_i)` from :math:`X_i` is not
   possible then a compilation error will be emitted if this function
   is chosen as the best candidate.
 * if :math:`X_i` uses the ``type`` intent, then :math:`T(A_i)`
   must be the same type or a subtype of :math:`T(X_i)` (see
   :ref:`Subtype_Arg_Conversions`).

Finally, if the above compatibility cannot be established, the mapping is
checked for promotion. If :math:`T(A_i)` is scalar promotable to :math:`T(X_i)`
(see :ref:`Promotion`), then the above rules are checked with the element
type, index type, or yielded type.  For example, if :math:`T(A_i)` is an
array of ``int`` and :math:`T(X_i)` is ``int``, then promotion occurs and
the above rules will be checked with :math:`T(A_i)` == ``int``.

.. _Determining_More_Specific_Functions:

.. _Determining_Most_Specific_Functions:

Determining Most Specific Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Given a set of candidate functions, the following steps are applied to
remove candidates from the set. After the process completes, the
remaining candidates in the set are the most specific functions.

1. If any candidate is more visible (or shadows) another candidate,
   discard all candidates that are less visible than (or shadowed by)
   another candidate.

2. If at least one candidate requires promotion and at least one
   candidate does not use promotion, discard all candidates that use
   promotion.

3. Discard any function that has a less specific argument mapping than
   any other function. See :ref:`More_Specific_Argument_Mappings` below
   for details on the more specific argument mapping relation.

4. Discard any candidates that have more formals that require implicit
   conversion than other candidates. For this step, implicit conversions
   between ``real(w)``, ``imag(w)``, and ``complex(2*w)`` are not
   considered.

5. Discard any candidates that have more formals that require a negative
   ``param`` value is converted to an unsigned integral type of any width
   (i.e. a ``uint(?w)``).

6. Discard any candidates that have more formals that require ``param``
   narrowing conversions than another candidate. A ``param`` narrowing
   conversion is when a ``param`` value is implicitly converted to a type
   that would not normally be allowed. For example, an ``int`` value
   cannot normally implicitly convert to an ``int(8)`` value. However,
   the ``param`` value ``1``, which is an ``int``, can implicitly convert to
   ``int(8)`` because the value is known to fit. See also
   :ref:`Implicit_Compile_Time_Constant_Conversions`.


Note that ``where`` clauses are also considered but that happens in a
later step. See :ref:`Determining_Best_Functions`.

.. _More_Specific_Argument_Mappings:

More Specific Argument Mappings
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Given candidate functions :math:`X` and :math:`Y` with formal arguments
:math:`X_1` :math:`X_2` ... and :math:`Y_1` :math:`Y_2` ... that
correspond to actual arguments :math:`A_1` :math:`A_2` ..., which
candidate function is more specific is determined in two steps. First,
the non-param actual arguments and their corresponding formal arguments
are considered. Then, any param actual arguments and their corresponding
formal arguments are considered.

Within each of those steps, the candidate function :math:`X` has a more
specific argument mapping if:

* for each argument :math:`i` considered, the argument mapping from
  :math:`A_i` to :math:`Y_i` is not better than the argument mapping for
  the argument :math:`A_i` to :math:`X_i`, and

* for at least one argument :math:`j` considered, the argument mapping
  from :math:`A_j` to :math:`X_j` is better than the argument mapping
  from :math:`A_j` to :math:`Y_j`.

.. _Better_Argument_Mapping:

Better Argument Mapping
^^^^^^^^^^^^^^^^^^^^^^^

Given an actual argument :math:`A_i` and the corresponding formal
arguments :math:`X_i` and :math:`Y_i` (in the functions :math:`X` and
:math:`Y` being considered), the following rules are applied in order to
determine whether :math:`X_i` or :math:`Y_i` is a better argument
mapping:

1. If one of the formals requires promotion and the other does not, the
   formal not requiring promotion is better

2. If both of the formals have the same type after instantiation and one
   of the formals is less generic than the other formal, the less-generic
   formal is better

3. If one of the formals is ``param`` and the other is not, the ``param``
   formal is better

4. If one of the formals requires a param narrowing conversion and the
   other is not, the one not requiring such narrowing is better

5. If the actual and both formals are numeric types and one formals is a
   preferred numeric conversion target, that formal is better

6. If one of the formals matches the actual type exactly and the other
   does not, the matching formal is better

7. If the actual's scalar type for promotion matches one of the formals
   but not the other, the matching formal is better

8. If an implicit conversion is possible from the type of one formal to
   the other, but not vice versa, then the formal that can be converted
   from is better. (I.e. if the type of :math:`X_i` can implicitly
   convert to the type of :math:`Y_i`, then :math:`X_i` is better).
   Similarly, if the type of one formal can be instantiated to produce
   the type of another formal, the type of the more-instantiated formal
   is better.

.. _Preferred_Numeric_Conversion_Target:

Preferred Numeric Conversion Target
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To compute if a formal is a preferred numeric conversion target, apply
the following rules in order:

1. Classify the actual and formals by numeric kind. If one formal has the
   same kind as the actual but the other does not, the formal with the
   same kind is better. Each of the following bullets represents a
   different numeric kind for this rule:

   * ``bool``

   * ``int(?w)`` or ``uint(?w)``, that is, a signed or unsigned integral
     type of any width

   * ``real(?w)``

   * ``imag(?w)``

   * ``complex(?w)``

   * all other types

2. Classify the actual and formals by numeric width. If one formal has
   the same numeric width as the actual but the other does not, the
   formal with the same width is better. Each of the following bullets
   represents a different numeric width for this rule:

   * All numeric types that match the default width.  This includes
     ``bool``, ``int``, ``uint``, ``real``, ``imag``, and ``complex``
     as well as their more specific names ``int(64)``, ``uint(64)``,
     ``real(64)``, ``imag(64)``, ``complex(128)``

   * All numeric types with 32-bit width: ``int(32)``, ``uint(32)``,
     ``real(32)``, ``imag(32)``, ``complex(64)``. ``complex(64)`` is in
     this category because the real element width is 32 bits.

   * All numeric types with 16-bit width: ``int(16)``, ``uint(16)``

   * All numeric types with 8-bit width: ``int(8)``, ``uint(8)``

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
