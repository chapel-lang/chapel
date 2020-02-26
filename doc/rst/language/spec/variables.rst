.. _Chapter-Variables:

Variables
=========

A variable is a symbol that represents memory. Chapel is a
statically-typed, type-safe language so every variable has a type that
is known at compile-time and the compiler enforces that values assigned
to the variable can be stored in that variable as specified by its type.

.. _Variable_Declarations:

Variable Declarations
---------------------

Variables are declared with the following syntax:

.. code-block:: syntax

   variable-declaration-statement:
     privacy-specifier[OPT] config-extern-or-export[OPT] variable-kind variable-declaration-list ;

   config-or-extern: one of
     `config' `extern' `export'

   variable-kind:
     `param'
     `const'
     `var'
     `ref'
     `const ref'

   variable-declaration-list:
     variable-declaration
     variable-declaration , variable-declaration-list

   variable-declaration:
     identifier-list type-part[OPT] initialization-part[OPT]

   type-part:
     : type-expression

   initialization-part:
     = expression

   identifier-list:
     identifier
     identifier , identifier-list
     tuple-grouped-identifier-list
     tuple-grouped-identifier-list , identifier-list

   tuple-grouped-identifier-list:
     ( identifier-list )

A ``variable-declaration-statement`` is used to define one or more
variables. If the statement is a top-level module statement, the
variables are module level; otherwise they are local. Module level
variables are discussed in :ref:`Module_Level_Variables`. Local
variables are discussed in :ref:`Local_Variables`.

The optional ``privacy-specifier`` keywords indicate the visibility of
module level variables to outside modules. By default, variables are
publicly visible. More details on visibility can be found in
 :ref:`Visibility_Of_Symbols`.

The optional keyword ``config`` specifies that the variables are
configuration variables, described in
Section :ref:`Configuration_Variables`. The optional keyword
``extern`` indicates that the variable is externally defined. Its name
and type are used within the Chapel program for resolution, but no space
is allocated for it and no initialization code emitted. See
:ref:`Shared_Data` for further details.

The ``variable-kind`` specifies whether the variables are parameters
(``param``), constants (``const``), ref variables (``ref``), or regular
variables (``var``). Parameters are compile-time constants whereas
constants are runtime constants. Both levels of constants are discussed
in :ref:`Constants`. Ref variables are discussed in
:ref:`Ref_Variables`.

The ``type-part`` of a variable declaration specifies the type of the
variable. It is optional.

The ``initialization-part`` of a variable declaration specifies an
initialization expression for the variable. It is optional. When present,
the initialization expression will be stored into the variable as its
initial value.

If the ``initialization-part`` is omitted, the compiler will consider if
split initialization can be applied to this variable as described in
:ref:`Split_Initialization`. If split initialization can be applied, the
compiler will identify one or more later assignment statements and the
right-hand-side of such statements will form the initialization
expression. If the ``initialization-part`` is omitted and split
initialization cannot be applied, then the variable will need to be
initialized to a default value. Only `var` and `const` variable
declarations can be initialized to a default value. Not all types have a
default value. Default values are described in
:ref:`Default_Values_For_Types`.

If the ``type-part`` is omitted or refers to a generic type, an
initialization expression as described above is required. Note that such
initialization expressions can be in later statements if
:ref:`Split_Initialization` us used. When the ``type-part`` is omitted or
generic, the type of the variable is inferred from the initialization
expression using local type inference described
in :ref:`Local_Type_Inference`. If the ``type-part`` is present, the
initialization expression must be coercible to the specified type or, if
``type-part`` is generic, to its instantiation.

Multiple variables can be defined in the same
``variable-declaration-list``. The semantics of declaring multiple
variables that share an ``initialization-part`` and/or ``type-part`` is
defined in :ref:`Multiple_Variable_Declarations`.

Multiple variables can be grouped together using a tuple notation as
described in :ref:`Variable_Declarations_in_a_Tuple`.

.. _Split_Initialization:

Split Initialization
~~~~~~~~~~~~~~~~~~~~

Split initialization is a feature that allows an initialization
expression for a variable to be in a statement after the variable
declaration statement.

If the ``initialization-part`` of a local variable declaration is
omitted, the compiler will search forward in the function for the
earliest assignment statement(s) setting that variable that occur before
the variable is otherwise mentioned. It will search only within block
declarations ``{ }``, ``try`` blocks, ``try!`` blocks, and conditionals.
These assignment statements are called applicable assignment statements.
They perform initialization, not assignment, of that variable.

   *Example (simple-split-init.chpl)*

   The combination of statements ``const x;`` and ``x = 5;`` in the below
   example are equivalent to the declaration ``const x = 5;``.

   .. code-block:: chapel

      proc main() {
        const x;
        x = 5;
        writeln(x);
      }

   .. BLOCK-test-chapeloutput

      5


   *Example (no-split-init.chpl)*

   In the following code, the variable ``x`` is used before it is
   assigned to, and so split initialization cannot apply to that
   variable.

   .. code-block:: chapel

      proc main() {
        const x;
        writeln(x);
        x = 5;
      }

   .. BLOCK-test-chapeloutput

      no-split-init.chpl:1: In function 'main':
      no-split-init.chpl:2: error: 'x' cannot be default initialized
      no-split-init.chpl:3: note: 'x' is used here before being initialized
      no-split-init.chpl:2: error: Variable 'x' is not initialized and has no type


   *Example (split-cond-blocks-init.chpl)*

   Split initialization can find the applicable assignment statement
   within a nested block or conditional. When conditionals are involved,
   there might be multiple applicable assignment statements representing
   different branches.

   .. code-block:: chapel

      config const option = false;
      proc main() {
        const x;
        if option {
          x = 6;
        } else {
          {
            x = 4;
          }
        }
        writeln(x);
      }

   .. BLOCK-test-chapeloutput

      4

Split initialization does not apply:

 * when the variable is a module-level variable, fields, config variable,
   or ``extern`` variable.
 * when an applicable assignment statement setting the variable could not
   be identified
 * when the variable is mentioned before the earliest assignment
   statement(s)
 * when an applicable assignment statement is in a loop, ``on``
   statement, or ``begin`` statement
 * when an applicable assignment statement is in one branch of a
   conditional but not in the other, and when the other branch
   does not always return or throw. This rule prevents
   split-initialization when the applicable assignment statement is
   in a conditional that has no ``else`` branch.
 * when an applicable assignment statement is in a ``try`` or ``try!``
   block which has ``catch`` clauses that mention the variable
   or which has ``catch`` clauses that do not always throw or return.

In the case that the variable is declared without a ``type-part`` and
where multiple applicable assignment statements are identified, all of
the assignment statements need to contain an initialization expression of
the same type.

.. _Default_Values_For_Types:

Default Initialization
~~~~~~~~~~~~~~~~~~~~~~

If a variable declaration has no initialization expression, a variable
is initialized to the default value of its type. The default values are
as follows:

=========== =======================================
**Type**    **Default Value**
=========== =======================================
bool(*)     false
int(*)      0
uint(*)     0
real(*)     0.0
imag(*)     0.0i
complex(*)  0.0 + 0.0i
string      ""
bytes       b""
enums       first enum constant
classes     nil
records     default constructed record
ranges      1..0 :math:`` :math:`` (empty sequence)
arrays      elements are default values
tuples      components are default values
sync/single base default value and *empty* status
atomic      base default value
=========== =======================================

.. _Local_Type_Inference:

Local Type Inference
~~~~~~~~~~~~~~~~~~~~

If the type is omitted from a variable declaration, the type of the
variable is defined to be the type of the initialization expression.
With the exception of sync and single expressions, the declaration


.. code-block:: chapel

   var v = e;

is equivalent to

.. code-block:: chapel

   var v: e.type = e;

for an arbitrary expression ``e``. If ``e`` is of sync or single type,
the type of ``v`` is the base type of ``e``.

.. _Multiple_Variable_Declarations:

Multiple Variable Declarations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

All variables defined in the same ``identifier-list`` are defined such
that they have the same type and value, and so that the type and
initialization expression are evaluated only once.

   *Example (multiple.chpl)*.

   In the declaration

   .. code-block:: chapel

      proc g() { writeln("side effect"); return "a string"; }
      var a, b = 1.0, c, d:int, e, f = g();



   .. BLOCK-test-chapelpost

      writeln((a,b,c,d,e,f));

   variables ``a`` and ``b`` are of type ``real`` with value ``1.0``.
   Variables ``c`` and ``d`` are of type ``int`` and are initialized to
   the default value of ``0``. Variables ``e`` and ``f`` are of type
   ``string`` with value ``"a string"``. The string ``"side effect"``
   has been written to the display once. It is not evaluated twice.



   .. BLOCK-test-chapeloutput

      side effect
      (1.0, 1.0, 0, 0, a string, a string)

The exact way that multiple variables are declared is defined as
follows:

-  If the variables in the ``identifier-list`` are declared with a type,
   but without an initialization expression as in 

   .. code-block:: chapel

      var v1, v2, v3: t;

   for an arbitrary type expression ``t``, then the declarations are
   rewritten so that the first variable is declared to be of type ``t``
   and each later variable is declared to be of the type of the first
   variable as in 

   .. code-block:: chapel

      var v1: t; var v2: v1.type; var v3: v1.type;

-  If the variables in the ``identifier-list`` are declared without a
   type, but with an initialization expression as in 

   .. code-block:: chapel

      var v1, v2, v3 = e;

   for an arbitrary expression ``e``, then the declarations are
   rewritten so that the first variable is initialized by expression
   ``e`` and each later variable is initialized by the first variable as
   in 

   .. code-block:: chapel

      var v1 = e; var v2 = v1; var v3 = v1;

-  If the variables in the ``identifier-list`` are declared with both a
   type and an initialization expression as in 

   .. code-block:: chapel

      var v1, v2, v3: t = e;

   for an arbitrary type expression ``t`` and an arbitrary expression
   ``e``, then the declarations are rewritten so that the first variable
   is declared to be of type ``t`` and initialized by expression ``e``,
   and each later variable is declared to be of the type of the first
   variable and initialized by the result of calling the function
   ``readXX`` on the first variable as in 

   .. code-block:: chapel

      var v1: t = e; var v2: v1.type = readXX(v1); var v3: v1.type = readXX(v1);

   where the function ``readXX`` is defined as follows: 

   .. code-block:: chapel

      proc readXX(x: sync) return x.readXX();
      proc readXX(x: single) return x.readXX();
      proc readXX(x) return x;

   Note that the use of the helper function ``readXX()`` in this code
   fragment is solely for the purposes of illustration. It is not
   actually a part of Chapel’s semantics or implementation.

..

   *Rationale*.

   This algorithm is complicated by the existence of *sync* and *single*
   variables. If these did not exist, we could rewrite any
   multi-variable declaration such that later variables were simply
   initialized by the first variable and the first variable was defined
   as if it appeared alone in the ``identifier-list``. However, both
   *sync* and *single* variables require careful handling to avoid
   unintentional changes to their *full*/*empty* state.

.. _Module_Level_Variables:

Module Level Variables
----------------------

Variables declared in statements that are in a module but not in a
function or block within that module are module level variables. Module
level variables can be accessed anywhere within that module after the
declaration of that variable. If they are public, they can also be
accessed in other modules that use that module.

.. _Local_Variables:

Local Variables
---------------

Local variables are declared within block statements. They can only be
accessed within the scope of that block statement (including all inner
nested block statements and functions).

A local variable only exists during the execution of code that lies
within that block statement. This time is called the lifetime of the
variable. When execution has finished within that block statement, the
local variable and the storage it represents is removed. Variables of
class type are the sole exception. Initializers of class types create
storage that is not associated with any scope. Such storage can be
reclaimed as described in :ref:`Class_Delete`.

.. _Constants:

Constants
---------

Constants are divided into two categories: parameters, specified with
the keyword ``param``, are compile-time constants and constants,
specified with the keyword ``const``, are runtime constants.

.. _Compile-Time_Constants:

Compile-Time Constants
~~~~~~~~~~~~~~~~~~~~~~

A compile-time constant, or “parameter”, must have a single value that
is known statically by the compiler. Parameters are restricted to
primitive and enumerated types.

Parameters can be assigned expressions that are parameter expressions.
Parameter expressions are restricted to the following constructs:

-  Literals of primitive or enumerated type.

-  Parenthesized parameter expressions.

-  Casts of parameter expressions to primitive or enumerated types.

-  Applications of the unary operators ``+ ``-``, ``!``, and ``~`` on operands
   that are bool or integral parameter expressions.

-  Applications of the unary operators ``+`` and ``-`` on operands that are
   real, imaginary or complex parameter expressions.

-  Applications of the binary operators ``+``, ``-``, ``*``, ``/``,
   ``%``, ``**``, ``&&``, ``||``, ``&``, ``|``, ``^``, ``<<``, ``>>``,
   ``==``, ``!=``, ``<=``, ``>=``, ``<``, and ``>`` on operands that are
   bool or integral parameter expressions.

-  Applications of the binary operators ``+``, ``-``, ``*``, ``/``,
   ``**``, ``==``, ``!=``, ``<=``, ``>=``, ``<``, and ``>`` on operands
   that are real, imaginary or complex parameter expressions.

-  Applications of the string concatenation operator ``+``, string
   comparison operators ``==``, ``!=``, ``<=``, ``>=``, ``<``, ``>``, and
   the string length and byte methods on parameter string expressions.

-  The conditional expression where the condition is a parameter and the
   then- and else-expressions are parameters.

-  Call expressions of parameter functions.
   See :ref:`Param_Return_Intent`.

.. _Runtime_Constants:

Runtime Constants
~~~~~~~~~~~~~~~~~

Runtime constants, or simply “constants”, do not have the restrictions
that are associated with parameters. Constants can be of any type.
Whether initialized explicitly or via its type’s default value, a
constant stores the same value throughout its lifetime.

A variable of a class type that is a constant is a constant reference.
That is, the variable always points to the object that it was
initialized to reference. However, the fields of that object are allowed
to be modified.

.. _Configuration_Variables:

Configuration Variables
-----------------------

If the keyword ``config`` precedes the keyword ``var``, ``const``, or
``param``, the variable, constant, or parameter is called a
configuration variable, configuration constant, or configuration
parameter respectively. Such variables, constants, and parameters must
be declared at the module level.

The default initialization of such variables can be overridden via
implementation-dependent means, such as command-line switches or
configuration files.  When overridden in this manner, the initialization
expression in the program is ignored.

Configuration parameters are set at compilation time via compilation
flags or other implementation-defined means. The value passed via these
means can be an arbitrary Chapel expression as long as the expression
can be evaluated at compile-time. If present, the value thus supplied
overrides the default value appearing in the Chapel code.

   *Example (config-param.chpl)*.

   For example, 

   .. code-block:: chapel

      config param rank = 2;

   

   .. BLOCK-test-chapelnoprint

      writeln(rank);

   

   .. BLOCK-test-chapeloutput

      2

   sets an integer parameter ``rank`` to ``2``. At compile-time, this
   default value of ``rank`` can be overridden with another parameter
   expression, such as ``3`` or ``2*n``, provided ``n`` itself is a
   parameter. The ``rank`` configuration variable can be used to write
   rank-independent code.

.. _Ref_Variables:

Ref Variables
-------------

A *ref* variable is a variable declared using the ``ref`` keyword. A ref
variable serves as an alias to another variable, field or array element.
The declaration of a ref variable must contain ``initialization-part``,
which specifies what is to be aliased and can be a variable or any
lvalue expression.

Access or update to a ref variable is equivalent to access or update to
the variable being aliased. For example, an update to a ref variable is
visible via the original variable, and visa versa.

If the expression being aliased is a runtime constant variable, a formal
argument with a ``const ref`` concrete intent
(:ref:`Concrete Intents`), or a call to a function with a
``const ref`` return intent (:ref:`Const_Ref_Return_Intent`),
the corresponding ref variable must be declared as ``const ref``.
Parameter constants and expressions cannot be aliased.

   *Open issue*.

   The behavior of a ``const ref`` alias to a non-\ ``const`` variable
   is an open issue. The options include disallowing such an alias,
   disallowing changes to the variable while it can be accessed via a
   ``const ref`` alias, making changes visible through the alias, and
   making the behavior undefined.

..

   *Example (refVariables.chpl)*.

   For example, the following code:

   

   .. code-block:: chapel

      var myInt = 51;
      ref refInt = myInt;                   // alias of a local or global variable
      myInt = 62;
      writeln("refInt = ", refInt);
      refInt = 73;
      writeln("myInt = ", myInt);

      var myArr: [1..3] int = 51;
      proc arrayElement(i) ref  return myArr[i];
      ref refToExpr = arrayElement(3);      // alias to lvalue returned by a function
      myArr[3] = 62;
      writeln("refToExpr = ", refToExpr);
      refToExpr = 73;
      writeln("myArr[3] = ", myArr[3]);

      const constArr: [1..3] int = 51..53;
      const ref myConstRef = constArr[2];   // would be an error without 'const'
      writeln("myConstRef = ", myConstRef);

   prints out:

   

   .. code-block:: printoutput

      refInt = 62
      myInt = 73
      refToExpr = 62
      myArr[3] = 73
      myConstRef = 52
