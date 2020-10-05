.. default-domain:: chpl

.. _Chapter-Modules:

Modules
=======

Chapel supports modules to manage namespaces. A program consists of one
or more modules. Every symbol, including variables, functions, and
types, is associated with some module.

Module definitions are described in :ref:`Module_Definitions`.
The relation between files and modules is described
in :ref:`Implicit_Modules`. Nested modules are described
in :ref:`Nested_Modules`. The visibility of a module’s symbols
by users of the module is described
in :ref:`Visibility_Of_Symbols`. The execution of a program
and module initialization/deinitialization are described
in :ref:`Program_Execution`.

.. _Module_Definitions:

Module Definitions
------------------

A module is declared with the following syntax: 

.. code-block:: syntax

   module-declaration-statement:
     privacy-specifier[OPT] prototype-specifier[OPT] 'module' module-identifier block-statement

   privacy-specifier:
     'private'
     'public'

   prototype-specifier:
     'prototype'

   module-identifier:
     identifier

A module's name is specified after the ``module`` keyword. The
``block-statement`` opens the module's scope. Symbols defined in this
block statement are defined in the module’s scope and are called
*module-scope symbols*. The visibility of a module is defined by its
``privacy-specifier``  (:ref:`Visibility_Of_A_Module`).

Module declaration statements are only legal as file-scope or
module-scope statements. For example, module declaration statements may
not occur within block statements, functions, classes, or records.

Any module declaration that is not contained within another module
creates a *top-level module*. Module declarations within other modules
create nested modules (:ref:`Nested_Modules`).

.. _Prototype_Modules:

Prototype Modules
-----------------

Modules that are declared with the ``prototype`` keyword use relaxed
rules for error handling. These relaxed rules are
appropriate for programs in the early stages of development but are not
appropriate for libraries. In particular, within a ``prototype`` module
errors that are not handled will terminate the program
(see :ref:`Errors_Prototype_Mode`).

Implicit modules (:ref:`Implicit_Modules`) are implicitly considered
``prototype`` modules as well.

.. _Implicit_Modules:

Files and Implicit Modules
--------------------------

Multiple modules can be defined within the same file and need not bear
any relation to the file in terms of their names.

   *Example (two-modules.chpl)*.

   The following file contains two explicitly named modules, MX and MY.
   

   .. code-block:: chapel

      module MX {
        var x: string = "Module MX";
        proc printX() {
          writeln(x);
        }
      }

      module MY {
        var y: string = "Module MY";
        proc printY() {
          writeln(y);
        }
      }

   

   .. BLOCK-test-chapelpost

      module Test {
        proc main() {
          use MX;
          use MY;
          MX.printX();
          MY.printY();
        }
      }

   

   .. BLOCK-test-chapeloutput

      Module MX
      Module MY

   Module MX defines module-scope symbols x and printX, while MY defines
   module-scope symbols y and printY.

For any file that contains file-scope statements other than module
declarations, the file itself is treated as a module declaration. In
this case, the module is implicit. Implicit modules are always
``prototype`` modules (:ref:`Prototype_Modules`).
An implicit module takes its name from the base
filename. In particular, the module name is defined as the remaining
string after removing the ``.chpl`` suffix and any path specification
from the specified filename. If the resulting name is not a legal Chapel
identifier, it cannot be referenced in a use statement.

   *Example (implicit.chpl)*.

   The following file, named implicit.chpl, defines an implicitly named
   module called implicit. 

   .. code-block:: chapel

      var x: int = 0;
      var y: int = 1;

      proc printX() {
        writeln(x);
      }
      proc printY() {
        writeln(y);
      }

   

   .. BLOCK-test-chapelpost

      printX();
      printY();

   

   .. BLOCK-test-chapeloutput

      0
      1

   Module implicit defines the module-scope symbols x, y, printX, and
   printY.

.. _Nested_Modules:

Nested Modules
--------------

A *nested module* (or *sub-module*) is a module that is defined within
another module, known as the outer, or parent, module.  An outer
module can refer to the names of its sub-modules directly without a
``use`` or ``import`` statement.  However, a sub-module must ``use``
or ``import`` its parent module in order to refer to its name or
symbols.

An inner module's symbols can be referenced without accessing those of
its parent module by naming the inner module in a qualified manner
within the ``use`` statement.

   *Example (nested-use.chpl)*.

   The code 

   .. BLOCK-test-chapelpre

      module libsci {
        writeln("Initializing libsci");
        module blas {
          writeln("\tInitializing blas");
        }
      }
      module testmain { // used to avoid warnings
        proc main() {
   

   .. code-block:: chapel

      use libsci.blas;

    .. BLOCK-test-chapelpost

      } }
   

   .. BLOCK-test-chapeloutput

      Initializing libsci
      	Initializing blas

   uses a module named ``blas`` that is nested within a module named
   ``libsci``.

Files with both module declarations and file-scope statements result in
nested modules.

   *Example (nested.chpl)*.

   The following file, named nested.chpl, defines an implicitly named
   module called nested, with nested modules MX and MY. 

   .. code-block:: chapel

      module MX {
        var x: int = 0;
      }

      module MY {
        var y: int = 0;
      }

      use MX, MY;

      proc printX() {
        writeln(x);
      }

      proc printY() {
        writeln(y);
      }

   

   .. BLOCK-test-chapelpost

      printX();
      printY();

   

   .. BLOCK-test-chapeloutput

      nested.chpl:11: warning: This file-scope code is outside of any explicit module declarations (e.g., module MY), so an implicit module named 'nested' is being introduced to contain the file's contents.
      0
      0

.. _Access_Of_Module_Contents:

Access of Module Contents
-------------------------

A module’s contents can be accessed by code outside of that module depending on
the visibility of the module itself (:ref:`Visibility_Of_A_Module`) and the
visibility of each individual symbol (:ref:`Visibility_Of_Symbols`). This can be
done via the use statement (:ref:`Using_Modules`), the import
statement (:ref:`Importing_Modules`) or qualified
naming (:ref:`Explicit_Naming`).

.. _Visibility_Of_A_Module:

Visibility Of A Module
~~~~~~~~~~~~~~~~~~~~~~

A top-level module is available for use (:ref:`Using_Modules`) or import
(:ref:`Importing_Modules`) anywhere.  A module name is not accessible in other
statements or expressions unless an ``import`` or ``use`` statement has brought
the name into scope.

Additionally, ``use`` and ``import`` can both name a module with a relative
path; for example, ``this.Submodule`` or ``super.Siblingmodule``.  ``use`` and
``import`` differ in their behavior towards a named module when two conditions
are both true: when the named module is not a top-level module and when a
relative path is not provided.

For the purpose of ``use``, the visibility of a nested module is subject to the
rules of :ref:`Visibility_Of_Symbols`, where the nested module is considered a
"module-scope symbol" of its outer module.  If the module is currently in scope,
then it may be used with just its name.  The module may alternatively be
accessed explicitly with all the outer modules surrounding it to the top level,
or relatively from the current scope with ``this`` or ``super`` components as
has already been mentioned.

Now, let's consider how ``import`` interacts with a nested module.  In order to
``import`` it, either all the outer modules surrounding it to the top level must
be provided as part of the path to the module, or a ``super`` or ``this`` prefix
may be provided as has already been mentioned.  The nested module cannot be
imported with just its name, even from the scope in which the module is defined,
unless it has already been brought into scope by another ``use`` or ``import``
statement.

.. _Visibility_Of_Symbols:

Visibility Of A Module’s Symbols
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A symbol defined at module scope is *visible* from outside the module when the
``privacy-specifier`` of its definition is ``public`` or is omitted (i.e. by
default). When a module-scope symbol is declared ``private``, it is not visible
outside of that module. A symbol’s visibility inside its module is controlled by
normal lexical scoping and is not affected by its ``privacy-specifier``. When a
module’s symbol is visible (:ref:`Visibility_Of_A_Module`), the visible symbols
it contains are accessible via the use statement (:ref:`Using_Modules`), import
statement (:ref:`Importing_Modules`), or qualified
naming (:ref:`Explicit_Naming`).

.. _Using_And_Importing:

Using and Importing
+++++++++++++++++++

The ``use`` statement and the ``import`` statement are the two primary ways to
access a module's symbols from outside of the module.  For top-level modules, a
``use`` or ``import`` statement is required before referring to the module’s
name or the symbols it contains within a given lexical scope.

The names that are made visible by a ``use`` or ``import`` statement are
inserted in to a new scope that immediately encloses the scope within which the
statement appears.  This implies that the position of the ``use`` or ``import``
statement within a scope has no effect on its behavior.  If a scope includes
multiple ``use`` statements, multiple ``import`` statements, or a combination of
``import`` and ``use`` statements, then the newly-visible names are inserted
into a common enclosing scope.

.. _Use_And_Import_Conflicts:

Conflicts
+++++++++

The implicit scope added by ``use`` and ``import`` described in the previous
section follows the same rules about conflicting variables as other scopes (see
:ref:`Variable_Conflicts`).  Thus an error will be signaled if multiple
variables with the same name would be inserted into this enclosing scope and
that name is accessed.  Remember that this does not apply to functions unless
they are also indistinguishable in other ways, see :ref:`Function_Overloading`.

Because symbols brought into scope by a ``use`` or ``import`` statement are
placed at a scope enclosing where the statement appears, such symbols will be
shadowed by other symbols with the same name defined in the scope with the
statement.  The symbols that are shadowed will only be accessible via
:ref:`Explicit_Naming`.

Symbols defined by public ``use`` or ``import`` statements can impact the scope
they are inserted into in different ways (see :ref:`Public_Use` and
:ref:`Reexporting` for more information on the ``public`` keyword).  Symbols
that are brought in by a ``public use`` for unqualified access are treated as
at successive distances relative to how many ``public use`` statements were
necessary to obtain them.  For instance,

   *Example (conflict1.chpl)*.

   .. code-block:: chapel

      module A {
        var x: int;
      }

      module B {
        public use A;
      }

      module C {
        var x: bool;
      }

      module MainMod {
        use B, C;

        proc main() {
          writeln(x);
        }
      }

   This code demonstrates a module (MainMod) using two modules, B and C.  Module
   C defines a symbol named x, while module B publicly uses another module, A,
   which also defines a symbol named x.  The program as written will compile and
   will print out the value of ``C.x``, which is ``false``, because A's x is
   considered further away (it is made available to MainMod through `two` use
   statements instead of just one).  Thus, it will generate the following
   output:

   .. code-block:: printoutput

      false

   If, however, C had been publicly used by another module D and that was used
   by MainMod instead, then the compiler cannot determine which of ``C.x`` and
   ``A.x`` was intended for ``writeln(x);``.  The program must use qualified
   access to indicate which x to access.

Symbols brought in directly by a ``public import`` are treated as though defined
*at* the scope with the ``public import`` for the purpose of determining
conflicts (see :ref:`Reexporting`).  This means that if the ``public use`` in
module B of the previous example was instead replaced with a ``public import
A.x``, A's x would conflict with ``C.x`` when resolving the main function's
body.

.. _Using_Modules:

Using Modules
~~~~~~~~~~~~~

By default, use statements make both a module’s name and its public symbols
available for access within a given scope.

The syntax of the use statement is given by:



.. code-block:: syntax

   use-statement:
     privacy-specifier[OPT] 'use' module-or-enum-name-list ;

   module-or-enum-name-list:
     module-or-enum-name limitation-clause[OPT]
     module-or-enum-name , module-or-enum-name-list

   module-or-enum-name:
     rename-base
     identifier . module-or-enum-name

   limitation-clause:
     'except' exclude-list
     'only' rename-list[OPT]

   exclude-list:
     identifier-list
     $ * $

   rename-list:
     rename-base
     rename-base , rename-list

   rename-base:
     identifier 'as' identifier
     identifier 'as' _
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

A module or enum being used may optionally be given a new name using the ``as``
keyword.  This new name will be usable from the scope of the use in place of the
old name.  This new name does not affect uses or imports of that module from
other contexts.

The ``as`` keyword can also be used to disable accesses to the module name while
still allowing accesses to the symbols within the module.  See the
:ref:`Disabling_Qualified` section for more information.

.. _Public_Use:

Public and Private Use Statements
+++++++++++++++++++++++++++++++++

Use statements may be explicitly declared ``public`` or ``private``.
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

Making a use ``public`` additionally causes its symbols to be visible as though
they were defined in the scope with the use.  This strategy is called
`re-exporting`.  More information about re-exporting can be found in the
relevant section (:ref:`Reexporting`).

.. _Limitation_Clauses:

Except and Only Lists
+++++++++++++++++++++

An optional ``limitation-clause`` may be provided to limit the symbols made
available by a given use statement. If an ``except`` list is provided, then all
the visible but unlisted symbols in the module or enumerated type will be made
available without prefix. If an ``only`` list is provided, then just the listed
visible symbols in the module or enumerated type will be made available without
prefix. All visible symbols not provided via these limited use statements are
still accessible by prefixing the access with the name of the module or
enumerated type (unless the module has been renamed to ``_``, as described
earlier). It is an error to provide a name in a ``limitation-clause`` that does
not exist or is not visible in the respective module or enumerated type.

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
module scope, uses and imports of that module will also be able to access
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

.. _Using_Enums:

Using Enums
+++++++++++

Aside from modules, only enums can be listed as the last portion of a ``use``
statement's ``module-or-enum-name``.  Doing so enables its constants to be
accessible without the enum's name as a prefix (see :ref:`Explicit_Naming` for
how to access its constants normally).

For more information on enumerated types, please see :ref:`Enumerated_Types`.

.. _Importing_Modules:

Importing Modules
~~~~~~~~~~~~~~~~~

The import statement provides either only qualified access to all of the public
symbols of a module or only unqualified access to the specified public symbols
of a module.

The syntax of the import statement is given by:

.. code-block:: syntax

   import-statement:
     privacy-specifier[OPT] 'import' import-expression-list ;

   import-expression-list:
     import-expression
     import-expression , import-expression-list

   import-expression:
     module-or-symbol-rename
     module-or-symbol-base unqualified-list

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

A module or a public module-level symbol being imported may optionally be given
a new name using the ``as`` keyword.  This new name will be usable from the
scope of the import in place of the old name.  This new name does not affect
imports or uses of that module from other contexts.

Import statements may be explicitly declared ``public`` or ``private``.  By
default, imports are ``private``.  Making an import ``public`` causes its
symbols to be visible as though they were defined in the scope with the import,
a strategy which will be referred to as `re-exporting`.  More information about
re-exporting can be found in the relevant section (:ref:`Reexporting`).

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
be able to access that symbol using the new name instead of the old name.
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

.. _Explicit_Naming:

Qualified Naming of Module Symbols
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When a module’s symbol is visible—via a use or import statement, or lexically
for nested modules—its public symbols can be accessed via qualified naming with
the following syntax:

.. code-block:: syntax

   module-access-expression:
     module-identifier-list . identifier

   module-identifier-list:
     module-identifier
     module-identifier . module-identifier-list

This allows two symbols that have the same name to be distinguished
based on the name of their module. Using qualified naming in a function
call restricts the set of candidate functions to those in the specified
module.

If code tries to access a symbol that conflicts with one or more other symbols
defined in other modules, the compiler will issue an error. Qualified naming can
be used to disambiguate the symbols in this case.

   *Example (ambiguity.chpl)*.

   In the following example, 

   .. code-block:: chapel

      module M1 {
        var x: int = 1;
        var y: int = -1;
        proc printX() {
          writeln("M1's x is: ", x);
        }
        proc printY() {
          writeln("M1's y is: ", y);
        }
      }
       
      module M2 {
        use M3;
        use M1;

        var x: int = 2;

        proc printX() {
          writeln("M2's x is: ", x);
        }

        proc main() {
          M1.x = 4;
          M1.printX();
          writeln(x);
          printX(); // This is not ambiguous
          printY(); // ERROR: This is ambiguous
        }
      }

      module M3 {
        var x: int = 3;
        var y: int = -3;
        proc printY() {
          writeln("M3's y is: ", y);
        }
      }

   

   .. BLOCK-test-chapeloutput

      ambiguity.chpl:22: In function 'main':
      ambiguity.chpl:27: error: ambiguous call 'printY()'
      ambiguity.chpl:34: note: candidates are: printY()
      ambiguity.chpl:7: note:                 printY()

   The call to printX() is not ambiguous because M2’s definition shadows
   that of M1. On the other hand, the call to printY() is ambiguous
   because it is defined in both M1 and M3. This will result in a
   compiler error. The call could be qualified via M1.printY() or
   M3.printY() to resolve this ambiguity.

.. _Disabling_Qualified:

Disabling Qualified Access
~~~~~~~~~~~~~~~~~~~~~~~~~~

While import statements are naturally set up to choose between enabling
qualified or unqualified access, use statements by default always enable at
least qualified access to a module's symbols.  This can be disabled for a
particular use statement by providing ``_`` as the new name for the module.

When a module is renamed to ``_``, symbols within it will only be accessible
without a module name prefix - no prefix will be usable to access the symbol.
For example:

    *Example (use-no-qualified.chpl)*.

    .. code-block:: chapel

       module B {
         var bSymbol = 3;
       }

       module A {
         use B as _;

         proc main() {
           writeln(bSymbol);
           // writeln(B.bSymbol); // Would not work
         }
       }

    .. BLOCK-test-chapeloutput

       3

    In this code, module A uses a module B and renames B to ``_``.  B defines a
    symbol ``bSymbol``, which means that A can contain accesses to ``bSymbol``,
    but cannot contain accesses to ``B.bSymbol`` or ``_.bSymbol``.

.. _Reexporting:

Re-exporting
~~~~~~~~~~~~

Making a use or import ``public`` causes the symbols brought in by that
statement to be visible as though they were defined in the scope with the use or
import, a strategy which will be referred to as `re-exporting`.  However,
symbols with the same name in the scope with the use or import will still take
precedence.

   *Example (use-reexport1.chpl)*.

   Say we have a module A that uses a module B, and module B contains a public
   use of module C:

   .. code-block:: chapel

      module C {
        var cSymbol: int;
      }

      module B {
        public use C;
      }

      module A {
        proc main() {
          use B;
          writeln(B.C.cSymbol);
          writeln(B.cSymbol);
        }
      }

   In this case, C will be visible to A as though it was a submodule of B, and
   its symbols can also be treated as though they were defined within B.  This
   means that A can contain mentions like ``B.C.cSymbol`` if cSymbol was a
   symbol defined in C, regardless of if C was actually a submodule of B.

   This also means that A can contain mentions like ``B.cSymbol`` which would
   access C's cSymbol, assuming these symbols were not shadowed by symbols with
   the same name in B.

   .. BLOCK-test-chapeloutput

      0
      0

   *Example (use-reexport2.chpl)*.

   However, if the public use of C also disabled accesses to the module name
   using the ``as`` keyword, e.g.

   .. code-block:: chapel

      module C {
        var cSymbol: int;
      }

      module B {
        public use C as _;
      }

      module A {
        proc main() {
          use B;
          // writeln(B.C.cSymbol); // Would not work
          writeln(B.cSymbol);
        }
      }

   Then A could only contain mentions like ``B.cSymbol``, it could not access
   ``cSymbol`` using ``B.C.cSymbol``.  This is because C is not present as a
   public name in B's scope.

   .. BLOCK-test-chapeloutput

      0

Conversely, if B's use of C was ``private`` then A would not be able to see C's
symbols at all due to that ``use``.

The situation for ``import`` is similar.  Because import statements only
enable either qualified or unqualified access to a symbol, it more closely
resembles the second example instead of the first.

   *Example (import-reexport1.chpl)*.

   Say we have a module A that imports module B, and module B contains a public
   import of module C:

   .. code-block:: chapel

      module C {
        var cSymbol: int;
      }

      module B {
        public import C;
      }

      module A {
        proc main() {
          import B;
          writeln(B.C.cSymbol);
        }
      }

   In this case, C will be visible to A as though it was a submodule of B.  This
   means that A can contain mentions like ``B.C.cSymbol`` if cSymbol was a
   symbol defined in C, regardless of if C was actually a submodule of B.

   .. BLOCK-test-chapeloutput

      0

   *Example (import-reexport2.chpl)*.

   Alternatively, if module B contains a public import of some public symbols
   defined in module C, then those symbols will be visible to A as though they
   were defined in module B, unless they are shadowed by symbols of the same
   name in B.

   .. code-block:: chapel

      module C {
        var cSymbol: int;
      }

      module B {
        public import C.cSymbol;
      }

      module A {
        proc main() {
          import B;
          writeln(B.cSymbol);
        }
      }

   Here, A's mention of ``B.cSymbol`` accesses ``cSymbol`` from C.

   .. BLOCK-test-chapeloutput

      0

Again, if B's import of C is ``private`` then A will not be able to see C's
symbols due to that ``import``.

This notion of re-exporting extends to the case in which a scope uses multiple
modules.

   *Example (use-reexport3.chpl)*.

   Say we have a module A that uses a module B, and module B contains a
   public use of modules C1, C2, and C3.

   .. code-block:: chapel

      module C1 {
        var c1Symbol: int;
      }

      module C2 {
        var c2Symbol: bool;
      }

      module C3 {
        var c3Symbol = 3;
      }

      module B {
        public use C1, C2, C3;
      }

      module A {
        proc main() {
          use B;
          writeln(B.C1.c1Symbol);
          writeln(B.C2.c2Symbol);
          writeln(B.C3.c3Symbol);

          writeln(B.c1Symbol);
          writeln(B.c2Symbol);
          writeln(B.c3Symbol);
        }
      }

   In this case all three of those modules will be accessible by A as though
   they were submodules of B.  This also means that symbols in C1, C2, and C3
   will be accessible as though they were defined in B, assuming these symbols
   were not shadowed by symbols with the same name in B and that these symbols
   do not conflict with each other.

   .. BLOCK-test-chapeloutput

      0
      false
      3
      0
      false
      3

This similarly applies to import statements that contain multiple
subexpressions.

.. _Module_Initialization:

Module Initialization
~~~~~~~~~~~~~~~~~~~~~

Module initialization occurs at program start-up. All module-scope
statements within a module other than function and type declarations are
executed during module initialization. Modules that are not referred to,
including both top-level modules and sub-modules, will not be
initialized.

   *Example (init.chpl)*.

   In the code, 

   .. BLOCK-test-chapelpre

      proc foo() {
          return 1;
      }

   

   .. code-block:: chapel

      var x = foo();       // executed at module initialization
      writeln("Hi!");      // executed at module initialization
      proc sayGoodbye {
        writeln("Bye!");   // not executed at module initialization
      }

   

   .. BLOCK-test-chapeloutput

      Hi!

   The function foo() will be invoked and its result assigned to x. Then
   “Hi!” will be printed.

Module initialization order is discussed
in :ref:`Module_Initialization_Order`.

.. _Module_Deinitialization:

Module Deinitialization
~~~~~~~~~~~~~~~~~~~~~~~

Module deinitialization occurs at program tear-down. During module
deinitialization:

-  If the module contains a deinitializer, which is a module-scope
   function named ``deinit()``, it is executed first.

-  If the module declares module-scope variables, they are deinitialized in
   the reverse order of their initialization.

Module deinitialization order is discussed
in :ref:`Module_Deinitialization_Order`.

.. _Program_Execution:

Program Execution
-----------------

Chapel programs start by initializing all modules and then executing the
main function (:ref:`The_main_Function`).

.. _The_main_Function:

The *main* Function
~~~~~~~~~~~~~~~~~~~

The main function must be called ``main`` and must have zero arguments.
It can be specified with or without parentheses. In any Chapel program,
there is a single main function that defines the program’s entry point.
If a program defines multiple potential entry points, the implementation
may provide a compiler flag that disambiguates between main functions in
multiple modules.

   *Implementation Notes*.

   In the current Chapel compiler implementation, the *– –main-module* flag
   can be used to specify the module from which the main function
   definition will be used.

..

   *Example (main-module.chpl)*.

   Because it defines two ``main`` functions, the following code will
   yield an error unless a main module is specified on the command line.
   

   .. code-block:: chapel

      module M1 {
        const x = 1;
        proc main() {
          writeln("M", x, "'s main");
        }
      }
       
      module M2 {
        use M1;

        const x = 2;
        proc main() {
          M1.main();
          writeln("M", x, "'s main");
        }
      }

   

   .. BLOCK-test-chapelcompopts

      --main-module M1 # main_module.M1.good
      --main-module M2 # main_module.M2.good

   If M1 is specified as the main module, the program will output:
   

   .. BLOCK-test-chapeloutputname

      main_module.M1.good

   

   .. code-block:: printoutput

      M1's main

   If M2 is specified as the main module the program will output:
   

   .. BLOCK-test-chapeloutputname

      main_module.M2.good

   

   .. code-block:: printoutput

      M1's main
      M2's main

   Notice that main is treated like just another function if it is not
   in the main module and can be called as such.

To aid in exploratory programming, a default main function is created if
the program does not contain a user-defined main function. The default
main function is equivalent to 

.. code-block:: chapel

   proc main() {}

..

   *Example (no-main.chpl)*.

   The code 

   .. code-block:: chapel

      writeln("hello, world");

   

   .. BLOCK-test-chapeloutput

      hello, world

   is a legal and complete Chapel program. The startup code for a Chapel
   program first calls the module initialization code for the main
   module and then calls ``main()``. This program’s initialization
   function is the file-scope writeln() statement. The module
   declaration is taken to be the entire file, as described
   in :ref:`Implicit_Modules`.

.. _Module_Initialization_Order:

Module Initialization Order
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Module initialization is performed using the following algorithm.

Starting from the module that defines the main function, the modules named in
its use and import statements are visited depth-first and initialized in
post-order. If a use or import statement names a module that has already been
visited, it is not visited a second time. Thus, infinite recursion is avoided.

Modules used or imported by a given module are visited in the order in which
they appear in the program text. For nested modules, the parent module and its
uses are initialized before the nested module and its uses or imports.

   *Example (init-order.chpl)*.

   The code 

   .. code-block:: chapel

      module M1 {
        use M2.M3;
        use M2;
        writeln("In M1's initializer");
        proc main() {
          writeln("In main");
        }
      }

      module M2 {
        use M4;
        writeln("In M2's initializer");
        module M3 {
          writeln("In M3's initializer");
        }
      }

      module M4 {
        writeln("In M4's initializer");
      }

   prints the following 

   .. code-block:: printoutput

      In M4's initializer
      In M2's initializer
      In M3's initializer
      In M1's initializer
      In main

   M1, the main module, uses M2.M3 and then M2, thus M2.M3 must be
   initialized. Because M2.M3 is a nested module, M4 (which is used by
   M2) must be initialized first. M2 itself is initialized, followed by
   M2.M3. Finally M1 is initialized, and the main function is run.

.. _Module_Deinitialization_Order:

Module Deinitialization Order
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Module deinitialization is performed in the reverse order of module
initialization, as specified in
:ref:`Module_Initialization_Order`.
