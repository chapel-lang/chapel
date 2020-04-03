.. _Chapter-Statements:

Statements
==========

Chapel is an imperative language with statements that may have side
effects. Statements allow for the sequencing of program execution.
Chapel provides the following statements:



.. code-block:: syntax

   statement:
     block-statement
     expression-statement
     assignment-statement
     swap-statement
     io-statement
     conditional-statement
     select-statement
     while-do-statement
     do-while-statement
     for-statement
     label-statement
     break-statement
     continue-statement
     param-for-statement
     use-statement
     defer-statement
     empty-statement
     return-statement
     yield-statement
     module-declaration-statement
     procedure-declaration-statement
     external-procedure-declaration-statement
     exported-procedure-declaration-statement
     iterator-declaration-statement
     method-declaration-statement
     type-declaration-statement
     variable-declaration-statement
     remote-variable-declaration-statement
     on-statement
     cobegin-statement
     coforall-statement
     begin-statement
     sync-statement
     serial-statement
     atomic-statement
     forall-statement
     delete-statement

Individual statements are defined in the remainder of this chapter and
additionally as follows:

-  return :ref:`The_Return_Statement`

-  yield :ref:`The_Yield_Statement`

-  module declaration :ref:`Chapter-Modules`

-  procedure declaration :ref:`Function_Definitions`

-  external procedure declaration
   :ref:`Calling_External_Functions`

-  exporting procedure declaration
   :ref:`Calling_Chapel_Functions`

-  iterator declaration :ref:`Iterator_Function_Definitions`

-  method declaration :ref:`Class_Methods`

-  type declaration :ref:`Chapter-Types`

-  variable declaration :ref:`Variable_Declarations`

-  remote variable declaration
    :ref:`remote_variable_declarations`

-  ``on`` statement :ref:`On`

-  cobegin, coforall, begin, sync, serial and atomic statements
   :ref:`Chapter-Task_Parallelism_and_Synchronization`

-  forall :ref:`Chapter-Data_Parallelism`

-  delete :ref:`Class_Delete`

.. _Blocks:

Blocks
------

A block is a statement or a possibly empty list of statements that form
their own scope. A block is given by 

.. code-block:: syntax

   block-statement:
     { statements[OPT] }

   statements:
     statement
     statement statements

Variables defined within a block are local
variables (:ref:`Local_Variables`).

The statements within a block are executed serially unless the block is
in a cobegin statement (:ref:`Cobegin`).

.. _Expression_Statements:

Expression Statements
---------------------

The expression statement evaluates an expression solely for side
effects. The syntax for an expression statement is given by 

.. code-block:: syntax

   expression-statement:
     variable-expression ;
     member-access-expression ;
     call-expression ;
     new-expression ;
     let-expression ;

.. _Assignment_Statements:

Assignment Statements
---------------------

An assignment statement assigns the value of an expression to another
expression, for example, a variable. Assignment statements are given by



.. code-block:: syntax

   assignment-statement:
     lvalue-expression assignment-operator expression

   assignment-operator: one of
      = += -= *= /= %= **= &= |= ^= &&= ||= <<= >>=

The assignment operators that contain a binary operator symbol as a
prefix are *compound assignment* operators. The remaining assignment
operator ``=`` is called *simple assignment*.

The expression on the left-hand side of the assignment operator must be
a valid lvalue (:ref:`LValue_Expressions`). It is evaluated
before the expression on the right-hand side of the assignment operator,
which can be any expression.

When the left-hand side is of a numerical type, there is an implicit
conversion (:ref:`Implicit_Conversions`) of the right-hand side
expression to the type of the left-hand side expression. Additionally,
for simple assignment, if the left-hand side is of Boolean type, the
right-hand side is implicitly converted to the type of the left-hand
side (i.e. a ``bool(?w)`` with the same width ``w``).

For simple assignment, the validity and semantics of assigning between
classes (:ref:`Class_Assignment`),
records (:ref:`Record_Assignment`),
unions (:ref:`Union_Assignment`),
tuples (:ref:`Tuple_Assignment`),
ranges (:ref:`Range_Assignment`),
domains (:ref:`Domain_Assignment`), and
arrays (:ref:`Array_Assignment`) are discussed in these later
sections.

A compound assignment is shorthand for applying the binary operator to
the left- and right-hand side expressions and then assigning the result
to the left-hand side expression. For numerical types, the left-hand
side expression is evaluated only once, and there is an implicit
conversion of the result of the binary operator to the type of the
left-hand side expression. Thus, for example, ``x += y`` is equivalent
to ``x = x + y`` where the expression ``x`` is evaluated once.

For all other compound assignments, Chapel provides a completely generic
catch-all implementation defined in the obvious way. For example:



.. code-block:: chapel

   inline proc +=(ref lhs, rhs) {
     lhs = lhs + rhs;
   }

Thus, compound assignment can be used with operands of arbitrary types,
provided that the following provisions are met: If the type of the
left-hand argument of a compound assignment operator ``op=`` is
:math:`L` and that of the right-hand argument is :math:`R`, then a
definition for the corresponding binary operator ``op`` exists, such
that :math:`L` is coercible to the type of its left-hand formal and
:math:`R` is coercible to the type of its right-hand formal. Further,
the result of ``op`` must be coercible to :math:`L`, and there must
exist a definition for simple assignment between objects of type
:math:`L`.

Both simple and compound assignment operators can be overloaded for
different types using operator
overloading (:ref:`Function_Overloading`). In such an overload,
the left-hand side expression should have ``ref`` intent and be modified
within the body of the function. The return type of the function should
be ``void``.

.. _The_Swap_Statement:

The Swap Statement
------------------

The swap statement indicates to swap the values in the expressions on
either side of the swap operator. Since both expressions are assigned
to, each must be a valid lvalue
expression (:ref:`LValue_Expressions`).

The swap operator can be overloaded for different types using operator
overloading (:ref:`Function_Overloading`). 

.. code-block:: syntax

   swap-statement:
     lvalue-expression swap-operator lvalue-expression

   swap-operator:
     <=>

To implement the swap operation, the compiler uses temporary variables
as necessary.

   *Example*.

   When resolved to the default swap operator, the following swap
   statement 

   .. code-block:: chapel

      var a, b: real;

      a <=> b;

   is semantically equivalent to: 

   .. code-block:: chapel

      const t = b;
      b = a;
      a = t;

.. _The_IO_Statement:

The I/O Statement
-----------------

The I/O operator indicates writing to the left-hand-side the value in
the right-hand-side; or reading from the left-hand-side and storing the
result in the variable on the right-hand-side. This operator can be
chained with other I/O operator calls.

The I/O operator can be overloaded for different types using operator
overloading (:ref:`Function_Overloading`). 

.. code-block:: syntax

   io-statement:
     io-expression io-operator expression

   io-expression:
     expression
     io-expression io-operator expression

   io-operator:
     <~>

See the module documentation on I/O for details on how to use the I/O
statement.

   *Example*.

   In the example below, 

   .. code-block:: chapel

      var w = opentmp().writer(); // a channel
      var a: real;
      var b: int;

      w <~> a <~> b;

   the I/O operator is left-associative and indicates writing ``a`` and
   then ``b`` to ``w`` in this case.

.. _The_Conditional_Statement:

The Conditional Statement
-------------------------

The conditional statement allows execution to choose between two
statements based on the evaluation of an expression of ``bool`` type.
The syntax for a conditional statement is given by 

.. code-block:: syntax

   conditional-statement:
     `if' expression `then' statement else-part[OPT]
     `if' expression block-statement else-part[OPT]

   else-part:
     `else' statement

A conditional statement evaluates an expression of bool type. If the
expression evaluates to true, the first statement in the conditional
statement is executed. If the expression evaluates to false and the
optional else-clause exists, the statement following the ``else``
keyword is executed.

If the expression is a parameter, the conditional statement is folded by
the compiler. If the expression evaluates to true, the first statement
replaces the conditional statement. If the expression evaluates to
false, the second statement, if it exists, replaces the conditional
statement; if the second statement does not exist, the conditional
statement is removed.

Each statement embedded in the *conditional-statement* has its own scope
whether or not an explicit block surrounds it.

If the statement that immediately follows the optional ``then`` keyword
is a conditional statement and it is not in a block, the else-clause is
bound to the nearest preceding conditional statement without an
else-clause. The statement in the else-clause can be a conditional
statement, too.

   *Example (conditionals.chpl)*.

   The following function prints ``two`` when ``x`` is ``2`` and
   ``B,four`` when ``x`` is ``4``. 

   .. code-block:: chapel

      proc condtest(x:int) {
        if x > 3 then
          if x > 5 then
            write("A,");
          else
            write("B,");

        if x == 2 then
          writeln("two");
        else if x == 4 then
          writeln("four");
        else
          writeln("other");
      }

   

   .. BLOCK-test-chapelpost

      for i in 2..6 do condtest(i);

   

   .. BLOCK-test-chapeloutput

      two
      other
      B,four
      B,other
      A,other

.. _The_Select_Statement:

The Select Statement
--------------------

The select statement is a multi-way variant of the conditional
statement. The syntax is given by: 

.. code-block:: syntax

   select-statement:
     `select' expression { when-statements }

   when-statements:
     when-statement
     when-statement when-statements

   when-statement:
     `when' expression-list `do' statement
     `when' expression-list block-statement
     `otherwise' statement
     `otherwise' `do' statement

   expression-list:
     expression
     expression , expression-list

The expression that follows the keyword ``select``, the select
expression, is evaluated once and its value is then compared with the
list of case expressions following each ``when`` keyword. These values
are compared using the equality operator ``==``. If the expressions
cannot be compared with the equality operator, a compile-time error is
generated. The first case expression that contains an expression where
that comparison is ``true`` will be selected and control transferred to
the associated statement. If the comparison is always ``false``, the
statement associated with the keyword ``otherwise``, if it exists, will
be selected and control transferred to it. There may be at most one
``otherwise`` statement and its location within the select statement
does not matter.

Each statement embedded in the *when-statement* or the
*otherwise-statement* has its own scope whether or not an explicit block
surrounds it.

.. _The_While_and_Do_While_Loops:

The While Do and Do While Loops
-------------------------------

There are two variants of the while loop in Chapel. The syntax of the
while-do loop is given by: 

.. code-block:: syntax

   while-do-statement:
     `while' expression `do' statement
     `while' expression block-statement

The syntax of the do-while loop is given by: 

.. code-block:: syntax

   do-while-statement:
     `do' statement `while' expression ;

In both variants, the expression evaluates to a value of type ``bool``
which determines when the loop terminates and control continues with the
statement following the loop.

The while-do loop is executed as follows:

#. The expression is evaluated.

#. If the expression evaluates to ``false``, the statement is not
   executed and control continues to the statement following the loop.

#. If the expression evaluates to ``true``, the statement is executed
   and control continues to step 1, evaluating the expression again.

The do-while loop is executed as follows:

#. The statement is executed.

#. The expression is evaluated.

#. If the expression evaluates to ``false``, control continues to the
   statement following the loop.

#. If the expression evaluates to ``true``, control continues to step 1
   and the the statement is executed again.

In this second form of the loop, note that the statement is executed
unconditionally the first time.

   *Example (while.chpl)*.

   The following example illustrates the difference between the
   ``do-while-statement`` and the ``while-do-statement``. The body of
   the do-while loop is always executed at least once, even if the loop
   conditional is already false when it is entered. The code
   

   .. code-block:: chapel

      var t = 11;

      writeln("Scope of do while loop:");
      do {
        t += 1;
        writeln(t);
      } while (t <= 10);

      t = 11;
      writeln("Scope of while loop:");
      while (t <= 10) {
        t += 1;
        writeln(t);
      }

   produces the output 

   .. code-block:: printoutput

      Scope of do while loop:
      12
      Scope of while loop:

Chapel do-while loops differ from those found in most other languages in
one important regard. If the body of a do-while statement is a block
statement and new variables are defined within that block statement,
then the scope of those variables extends to cover the loop’s
termination expression.

   *Example (do-while.chpl)*.

   The following example demonstrates that the scope of the variable t
   includes the loop termination expression. 

   .. code-block:: chapel

      var i = 0;
      do {
        var t = i;
        i += 1;
        writeln(t);
      } while (t != 5);

   produces the output 

   .. code-block:: printoutput

      0
      1
      2
      3
      4
      5

.. _The_For_Loop:

The For Loop
------------

The for loop iterates over ranges, domains, arrays, iterators, or any
class that implements an iterator named ``these``. The syntax of the for
loop is given by: 

.. code-block:: syntax

   for-statement:
     `for' index-var-declaration `in' iteratable-expression `do' statement
     `for' index-var-declaration `in' iteratable-expression block-statement
     `for' iteratable-expression `do' statement
     `for' iteratable-expression block-statement

   index-var-declaration:
     identifier
     tuple-grouped-identifier-list

   iteratable-expression:
     expression
     `zip' ( expression-list )

The ``index-var-declaration`` declares new variables for the scope of
the loop. It may specify a new identifier or may specify multiple
identifiers grouped using a tuple notation in order to destructure the
values returned by the iterator expression, as described
in :ref:`Indices_in_a_Tuple`.

The ``index-var-declaration`` is optional and may be omitted if the
indices do not need to be referenced in the loop.

If the iteratable-expression begins with the keyword ``zip`` followed by
a parenthesized expression-list, the listed expressions must support
zipper iteration.

.. _Zipper_Iteration:

Zipper Iteration
~~~~~~~~~~~~~~~~

When multiple iterators are iterated over in a zipper context, on each
iteration, each expression is iterated over, the values are returned by
the iterators in a tuple and assigned to the index, and then statement
is executed.

The shape of each iterator, the rank and the extents in each dimension,
must be identical.

   *Example (zipper.chpl)*.

   The output of 

   .. code-block:: chapel

      for (i, j) in zip(1..3, 4..6) do
        write(i, " ", j, " ");

   

   .. BLOCK-test-chapelpost

      writeln();

   is 

   .. code-block:: printoutput

      1 4 2 5 3 6 

.. _Parameter_For_Loops:

Parameter For Loops
~~~~~~~~~~~~~~~~~~~

Parameter for loops are unrolled by the compiler so that the index
variable is a parameter rather than a variable. The syntax for a
parameter for loop statement is given by: 

.. code-block:: syntax

   param-for-statement:
     `for' `param' identifier `in' param-iteratable-expression `do' statement
     `for' `param' identifier `in' param-iteratable-expression block-statement

   param-iteratable-expression:
     range-literal
     range-literal `by' integer-literal

Parameter for loops are restricted to iteration over range literals with
an optional by expression where the bounds and stride must be
parameters. The loop is then unrolled for each iteration.

.. _Label_Break_Continue:

The Break, Continue and Label Statements
----------------------------------------

The break- and continue-statements are used to alter the flow of control
within a loop construct. A break-statement causes flow to exit the
containing loop and resume with the statement immediately following it.
A continue-statement causes control to jump to the end of the body of
the containing loop and resume execution from there. By default, break-
and continue-statements exit or skip the body of the
immediately-containing loop construct.

The label-statement is used to name a specific loop so that ``break``
and ``continue`` can exit or resume a less-nested loop. Labels can only
be attached to for-, while-do- and do-while-statements. When a break
statement has a label, execution continues with the first statement
following the loop statement with the matching label. When a continue
statement has a label, execution continues at the end of the body of the
loop with the matching label. If there is no containing loop construct
with a matching label, a compile-time error occurs.

The syntax for label, break, and continue statements is given by:


.. code-block:: syntax

   break-statement:
     `break' identifier[OPT] ;

   continue-statement:
     `continue' identifier[OPT] ;

   label-statement:
     `label' identifier statement

A ``break`` statement cannot be used to exit a parallel loop
:ref:`Forall`.

   *Rationale*.

   Breaks are not permitted in parallel loops because the execution
   order of the iterations of parallel loops is not defined.

..

   *Future*.

   We expect to support a *eureka* concept which would enable one or
   more tasks to stop the execution of all current and future iterations
   of the loop.

   *Example*.

   In the following code, the index of the first element in each row of
   ``A`` that is equal to ``findVal`` is printed. Once a match is found,
   the continue statement is executed causing the outer loop to move to
   the next row. 

   .. code-block:: chapel

      label outer for i in 1..n {
        for j in 1..n {
          if A[i, j] == findVal {
            writeln("index: ", (i, j), " matches.");
            continue outer;
          }
        }
      }

.. _The_Use_Statement:

The Use Statement
-----------------

The use statement provides access to the constants in an enumerated type
or to the public symbols of a module without the need to use a fully
qualified name. When using a module, the statement also ensures that the
module symbol itself is visible within the current scope (top-level
modules are not otherwise visible without a ``use``).

The syntax of the use statement is given by:



.. code-block:: syntax

   use-statement:
     privacy-specifier[OPT] `use' module-or-enum-name-list ;

   module-or-enum-name-list:
     module-or-enum-name limitation-clause[OPT]
     module-or-enum-name , module-or-enum-name-list

   module-or-enum-name:
     rename-base
     identifier . module-or-enum-name

   limitation-clause:
     `except' exclude-list
     `only' rename-list[OPT]

   exclude-list:
     identifier-list
     $ * $

   rename-list:
     rename-base
     rename-base , rename-list

   rename-base:
     identifier `as' identifier
     identifier

For example, the program

   *Example (use1.chpl)*.

   

   .. code-block:: chapel

      module M1 {
        proc foo() {
          writeln("In M1's foo.");
        }
      }

      module M2 {
        use M1;
        proc main() {
          writeln("In M2's main.");
          M1.foo();
        }
      }

   prints out 

   .. code-block:: printoutput

      In M2's main.
      In M1's foo.

This program is equivalent to:

   *Example (use2.chpl)*.

   

   .. code-block:: chapel

      module M1 {
        proc foo() {
          writeln("In M1's foo.");
        }
      }

      module M2 {
        proc main() {
          use M1;

          writeln("In M2's main.");
          foo();
        }
      }

   which also prints out 

   .. code-block:: printoutput

      In M2's main.
      In M1's foo.

The ``module-or-enum-name`` in a ``use`` statement must begin with one of
the following:

 * a top-level module name
 * a submodule of the current module
 * a module name currently in scope due to another ``use`` or ``import``
   statement
 * any number of ``super`` components to indicate a number of parents of
   the current module (e.g. ``super.super.SomeModule``)
 * ``this`` to indicate the requested module is a submodule of the
   current module

The names that are made visible by a ``use`` statement are inserted in to a new
scope that immediately encloses the scope within which the statement
appears. This implies that the position of the ``use`` statement within a scope
has no effect on its behavior. If a scope includes multiple ``use`` statements
or a combination of ``use`` and ``import`` statements, then the newly-visible
names are inserted in to a common enclosing scope.

An error is signaled if multiple enumeration constants or public
module-level symbols would be inserted into this enclosing scope with
the same name, and that name is referenced by other statements in the
same scope as the use.

A module or enum being used may optionally be given a new name using the ``as``
keyword.  This new name will be usable from the scope of the use in place of the
old name.  This new name does not affect uses or imports of that module from
other contexts.

Use statements may be explicitly delared ``public`` or ``private``.
By default, uses are ``private``.  Making a use ``public`` causes its
symbols to be transitively visible: if module A uses module B, and
module B contains a public use of a module or enumerated type C, then
C’s public symbols will also be visible to A unless they are shadowed
by symbols of the same name in B.  Conversely, if B's use of C is
``private`` then A will not be able to see C's symbols due to that
``use``.

This notion of transitivity extends to the case in which a scope
imports symbols from multiple modules or constants from multiple
enumeration types. For example if a module A uses modules B1, B2, B3
and modules B1, B2, B3 publicly use modules C1, C2, C3 respectively,
then all of the public symbols in B1, B2, B3 have the potential to
shadow the public symbols of C1, C2, and C3. However an error is
signaled if C1, C2, C3 have conflicting public module-level
definitions of the same symbol.

An optional ``limitation-clause`` may be provided to limit the symbols
made available by a given use statement. If an ``except`` list is
provided, then all the visible but unlisted symbols in the module or
enumerated type will be made available without prefix. If an ``only``
list is provided, then just the listed visible symbols in the module or
enumerated type will be made available without prefix. All visible
symbols not provided via these limited use statements are still
accessible by prefixing the access with the name of the module or
enumerated type. It is an error to provide a name in a
``limitation-clause`` that does not exist or is not visible in the
respective module or enumerated type.

If a type or type's secondary methods are defined in the used module, then any
instances of the type obtained in the scope of the use may access the fields and
methods of that type, regardless of the ``limitation-clause``. These fields
and methods cannot be specified in a ``limitation-clause`` on their own.  The
privacy of use statements is also ignored when determining if an instance can
access the fields and methods, for similar reasons.

If an ``only`` list is left empty or ``except`` is followed by :math:`*`
then no symbols are made available to the scope without prefix. However,
any methods or fields defined within a module used in this way will
still be accessible on instances of the type. For example:

   *Example (limited-access.chpl)*.

   

   .. code-block:: chapel

      module M1 {
        record A {
          var x = 1;

          proc foo() {
            writeln("In A.foo()");
          }
        }
      }

      module M2 {
        proc main() {
          use M1 only;

          var a = new M1.A(3); // Only accessible via the module prefix
          writeln(a.x); // Accessible because we have a record instance
          a.foo(); // Ditto
        }
      }

   will print out 

   .. code-block:: printoutput

      3
      In A.foo()

Within an ``only`` list, a visible symbol from that module may optionally be
given a new name using the ``as`` keyword. This new name will be usable from the
scope of the use in place of the old name unless the old name is additionally
specified in the ``only`` list. If a use which renames a symbol is present at
module scope, uses and imports of that module will also be able to reference
that symbol using the new name instead of the old name. Renaming does not affect
accesses to that symbol via the source module’s or enumerated type’s prefix, nor
does it affect uses or imports of that module or enumerated type from other
contexts. It is an error to attempt to rename a symbol that does not exist or is
not visible in the respective module or enumerated type, or to rename a symbol
to a name that is already present in the same ``only`` list. It is, however,
perfectly acceptable to rename a symbol to a name present in the respective
module or enumerated type which was not specified via that ``only`` list.

If a use statement mentions multiple modules or enumerated types or a
mix of these symbols, only the last module or enumerated type can have a
``limitation-clause``. Limitation clauses are applied transitively as
well - in the first example, if module A’s use of module B contains an
``except`` or ``only`` list, that list will also limit which of C’s
symbols are visible to A.

For more information on enumerated types, please
see :ref:`Enumerated_Types`. For use statement rules which are
only applicable to modules, please see :ref:`Using_Modules`.
For more information on modules in general, please
see :ref:`Chapter-Modules`.

.. _The_Import_Statement:

The Import Statement
--------------------

The import statement provides either only qualified access to all of the public
symbols of a module or only unqualified access to the specified public symbols
of a module.

The syntax of the import statement is given by:

.. code-block:: syntax

   import-statement:
     privacy-specifier[OPT] `import' module-or-symbol-rename ;
     privacy-specifier[OPT] `import' module-or-symbol-base unqualified-list ;

   module-or-symbol-rename:
     rename-base
     identifier . module-or-symbol-rename

   module-or-symbol-base:
     identifier
     identifier . module-or-symbol-base

   unqualified-list:
     . { rename-list }

For example, the program

   *Example (import1.chpl)*.

   .. code-block:: chapel

      module M1 {
        proc foo() {
          writeln("In M1's foo.");
        }
      }

      module M2 {
        import M1;
        proc main() {
          writeln("In M2's main.");
          M1.foo();
        }
      }

   prints out

   .. code-block:: printoutput

      In M2's main.
      In M1's foo.

This program is equivalent to:

   *Example (import2.chpl)*.

   .. code-block:: chapel

      module M1 {
        proc foo() {
          writeln("In M1's foo.");
        }
      }

      module M2 {
        proc main() {
          import M1.foo;

          writeln("In M2's main.");
          foo();
        }
      }

   which also prints out

   .. code-block:: printoutput

      In M2's main.
      In M1's foo.

And both programs are also equivalent to:

   *Example (import3.chpl)*.

   .. code-block:: chapel

      module M1 {
        proc foo() {
          writeln("In M1's foo.");
        }
      }

      module M2 {
        proc main() {
          import M1.{foo};

          writeln("In M2's main.");
          foo();
        }
      }

   which also prints out

   .. code-block:: printoutput

      In M2's main.
      In M1's foo.

The ``module-or-symbol-rename`` or ``module-or-symbol-base`` in an ``import``
statement must begin with one of the following:

 * a top-level module name
 * a module name currently in scope due to another ``use`` or ``import``
   statement
 * any number of ``super`` components to indicate a number of parents of the
   current module (e.g. ``super.super.SomeModule``)
 * ``this`` to indicate the requested module is a submodule of the current
   module

A submodule may not be imported without either the full path to it, or a
``super`` or ``this`` prefix at the beginning of the path.

The names that are made visible by an ``import`` statement are inserted in to a
new scope that immediately encloses the scope within which the statement
appears.  This implies that the position of the ``import`` statement within a
scope has no effect on its behavior.  If a scope includes multiple ``import``
statements, or a combination of ``import`` and ``use`` statements, then the
newly-visible names are inserted into a common enclosing scope.

An error is signalled if multiple public module-level symbols would be inserted
into this enclosing scope with the same name, and that name is referenced by
other statements in the same scope as the import.

A module or a public module-level symbol being imported may optionally be given
a new name using the ``as`` keyword.  This new name will be usable from the
scope of the import in place of the old name.  This new name does not affect
imports or uses of that module from other contexts.

Import statements may be explicitly declared ``public`` or ``private``.  By
default, imports are ``private``.  Making an import ``public`` causes its
symbols to be visible as though they were defined in the scope with the import,
a strategy which will be referred to as `re-exporting`.  However, symbols with
the same name in the scope with the import will still take precedence.  If
module A imports module B, and module B contains a public import of module C,
then C will be visible to A as though it was a submodule of B.  This means that
A could contain references like ``B.C.cSymbol`` if cSymbol was a symbol defined
in C, regardless of if C was actually a submodule of B.  Similarly, if module B
contains a public import of some public symbols defined in module C, then those
symbols will be visible to A as though they were defined in module B, unless
they are shadowed by symbols of the same name in B.  This means that A could
contain references like ``B.cSymbol`` and it would reference C's cSymbol.
Conversely, if B's import of C is ``private`` then A will not be able to see C's
symbols due to that ``import``.

This notion of re-exporting extends to the case in which a scope imports symbols
from multiple modules.  For example, if a module A imports a module B, and
module B contains a public import of modules C1, C2, and C3, then all three of
those modules will be referenceable by A as though they were submodules of B.
Similarly, if module B instead publicly imports specific symbols from C1, C2,
and C3, A will be able to reference those symbols as though they were defined
directly in B.  However, an error is signaled if symbols with the same name are
imported from these modules.

The import statement may specify a single module or module-level symbol, or it
may specify multiple module-level symbols in the ``unqualified-list``.  Unlike
``use`` statements, symbols specified for unqualified access are not able to be
accessed with the module qualifier.  A separate import statement may be provided
to enable this behavior.  It is an error to provide a name in an
``unqualified-list`` that does not exist or is not visible in the respective
module.

If a type or type's secondary methods are defined in the imported module, then
any instances of the type obtained in the scope of the import may access the
fields and methods of that type, regardless of the ``unqualified-list``. These
fields and methods cannot be specified in an ``unqualified-list`` on their own.
The privacy of import statements is also ignored when determining if an instance
can access the fields and methods, for similar reasons.

Within an ``unqualified-list``, a visible symbol from that module may optionally
be given a new name using the ``as`` keyword.  This new name will be usable from
the scope of the import in place of the old name unless the old name is
additionally specified in the ``unqualified-list``.  If an import which renames
a symbol is present at module scope, imports and uses of that module will also
be able to reference that symbol using the new name instead of the old name.
Renaming does not affect accesses to that symbol via the source module's prefix,
nor does it affect imports or uses of that module from other contexts.  It is an
error to attempt to rename a symbol that does not exist or is not visible in the
respective module, or to rename a symbol to a name that is already present in
the same ``unqualified-list``.  It is, however, perfectly acceptable to rename a
symbol to a name present in the respective module which was not specified via
that ``unqualified-list``.

The list of symbols for unqualified access can also be applied transitively -
in the second example of re-exporting, if module A's import of B only allowed
access to certain symbols, that list will also limit which of the symbols from
C1, C2, and C3 will be available to A.

For more information on modules in general, please see :ref:`Chapter-Modules`.

.. _The_Defer_Statement:

The Defer Statement
-------------------

A ``defer`` statement declares a clean-up action to be run when exiting
a block. ``defer`` is useful because the clean-up action will be run no
matter how the block is exited.

The syntax is:



.. code-block:: syntax

   defer-statement:
     `defer' statement

At a given place where control flow exits a block, the in-scope
``defer`` statements and the local variables will be handled in reverse
declaration order. Handling a ``defer`` statement consists of executing
the contained clean-up action. Handling a local variable consists of
running its deinitializer if it is of record type.

When an iterator contains a ``defer`` statement at the top level, the
associated clean-up action will be executed when the loop running the
iterator exits. ``defer`` actions inside a loop body are executed when
that iteration completes.

The following program demonstrates a simple use of ``defer`` to create
an action to be executed when returning from a function:

   *Example (defer1.chpl)*.

   

   .. code-block:: chapel

      class Integer {
        var x:int;
      }
      proc deferInFunction() {
        var c = new unmanaged Integer(1);
        writeln("created ", c);
        defer {
          writeln("defer action: deleting ", c);
          delete c;
        }
        // ... (function body, possibly including return statements)
        // The defer action is executed no matter how this function returns.
      }
      deferInFunction();

   produces the output 

   .. BLOCK-test-chapeloutput

      created {x = 1}
      defer action: deleting {x = 1}

   .. code-block:: bash

      created {x = 1}
      defer action: deleting {x = 1}

The following example uses a nested block to demonstrate that ``defer``
is handled when exiting the block in which it is contained:

   *Example (defer2.chpl)*.

   

   .. code-block:: chapel

      class Integer {
        var x:int;
      }
      proc deferInNestedBlock() {
        var i = 1;
        writeln("before inner block");
        {
          var c = new unmanaged Integer(i);
          writeln("created ", c);
          defer {
            writeln("defer action: deleting ", c);
            delete c;
          }
          writeln("in inner block");
          // note, defer action is executed no matter how this block is exited
        }
        writeln("after inner block");
      }
      deferInNestedBlock();

   produces the output 

   .. BLOCK-test-chapeloutput

      before inner block
      created {x = 1}
      in inner block
      defer action: deleting {x = 1}
      after inner block

   .. code-block:: bash

      before inner block
      created {x = 1}
      in inner block
      defer action: deleting {x = 1}
      after inner block

Lastly, this example shows that when ``defer`` is used in a loop, the
action will be executed for every loop iteration, whether or not loop
body is exited early.

   *Example (defer3.chpl)*.

   

   .. code-block:: chapel

      class Integer {
        var x:int;
      }
      proc deferInLoop() {
        for i in 1..10 {
          var c = new unmanaged Integer(i);
          writeln("created ", c);
          defer {
            writeln("defer action: deleting ", c);
            delete c;
          }
          writeln(c);
          if i == 2 then
            break;
        }
      }
      deferInLoop();

   produces the output 

   .. BLOCK-test-chapeloutput

      created {x = 1}
      {x = 1}
      defer action: deleting {x = 1}
      created {x = 2}
      {x = 2}
      defer action: deleting {x = 2}

   .. code-block:: bash

      created {x = 1}
      {x = 1}
      defer action: deleting {x = 1}
      created {x = 2}
      {x = 2}
      defer action: deleting {x = 2}

.. _The_Empty_Statement:

The Empty Statement
-------------------

An empty statement has no effect. The syntax of an empty statement is
given by 

.. code-block:: syntax

   empty-statement:
     ;

