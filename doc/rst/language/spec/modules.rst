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
     privacy-specifier[OPT] prototype-specifier[OPT] `module' module-identifier block-statement

   privacy-specifier:
     `private'
     `public'

   prototype-specifier:
     `prototype'

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
another module, known as the outer, or parent, module. A nested module
automatically has access to all of the symbols in its outer module.
However, an outer module needs to explicitly name or use a nested module
in order to access its symbols.

A nested module can be used without using the outer module by explicitly
naming the outer module in the use statement.

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

   uses a module named ``blas`` that is nested inside a module named
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

A module’s contents can be accessed by code outside of that module
depending on the visibility of the module
itself (:ref:`Visibility_Of_A_Module`) and the visibility of
each individual symbol (:ref:`Visibility_Of_Symbols`). This
can be done via the use statement (:ref:`Using_Modules`) or
qualified naming (:ref:`Explicit_Naming`).

.. _Visibility_Of_A_Module:

Visibility Of A Module
~~~~~~~~~~~~~~~~~~~~~~

A top-level module is available for use (:ref:`Using_Modules`)
anywhere. The visibility of a nested module is subject to the rules
of :ref:`Visibility_Of_Symbols`, where the nested module is
considered a "module-scope symbol" of its outer module.

.. _Visibility_Of_Symbols:

Visibility Of A Module’s Symbols
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A symbol defined at module scope is *visible* from outside the module
when the ``privacy-specifier`` of its definition is ``public`` or is
omitted (i.e. by default). When a module-scope symbol is declared
``private``, it is not visible outside of that module. A symbol’s
visibility inside its module is controlled by normal lexical scoping and
is not affected by its ``privacy-specifier``. When a module’s symbol is
visible (:ref:`Visibility_Of_A_Module`), the visible symbols
it contains are accessible via the use
statement (:ref:`Using_Modules`) or qualified
naming (:ref:`Explicit_Naming`).

.. _Using_Modules:

Using Modules
~~~~~~~~~~~~~

The ``use`` statement provides the primary means of accessing a module’s
symbols from outside of the module. Use statements make both the
module’s name and its public symbols available for reference within a
given scope. For top-level modules, a use statement is required before
referring to the module’s name or the symbols it contains within a given
lexical scope.

Use statements can also restrict or rename the set of module symbols
that are available within the scope. For further information about use
statements, see :ref:`The_Use_Statement`.

.. _Explicit_Naming:

Qualified Naming of Module Symbols
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When a module’s symbol is visible—via a use statement, or lexically for
nested modules—its public symbols can be referred to via qualified
naming with the following syntax: 

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

If code refers to symbols that are defined by multiple modules, the
compiler will issue an error. Qualified naming can be used to
disambiguate the symbols in this case.

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

-  If the module declares global variables, they are deinitialized in
   the reverse declaration order.

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

Starting from the module that defines the main function, the modules
named in its use statements are visited depth-first and initialized in
post-order. If a use statement names a module that has already been
visited, it is not visited a second time. Thus, infinite recursion is
avoided.

Modules used by a given module are visited in the order in which they
appear in the program text. For nested modules, the parent module and
its uses are initialized before the nested module and its uses.

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
