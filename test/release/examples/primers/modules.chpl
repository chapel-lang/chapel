// Modules

/*
   This primer introduces the concept of modules, a concept for encapsulating
   code for use by other code.  It covers:

    * how to define a module
    * namespace control within a module
    * access to another module's symbols
    * namespace control when using a module, including:

      * unlimited
      * explicit exclusion of symbols
      * explicit inclusion of symbols
      * renaming included symbols

    * cross-file module access
*/


/* A module is a grouping of code - each program consists of at least one
   module, and every symbol is associated with some module.  If all the code of
   a file is not enclosed in an explicit module, defined using the ``module``
   keyword, then the file itself is treated as a module with the same name as
   the file (minus the .chpl suffix).  The compiler can be directed to include
   modules in distinct files by naming them on the command line or by relying
   on the ``-M`` flag (see the :ref:`man page <man-chpl>` for exact details).
   Here, we declare a module `modToUse`:
*/
module modToUse {

  /* In this case, ``foo`` is a public module-level variable that is defined
     within the module ``modToUse``  */
  var foo = 12;

  /* As is ``bar``.  */
  var bar: int = 2;

  /* A symbol can be declared ``private`` - this means that only code defined
     within the same scope as the definition of the symbol (including code in
     nested scopes) can access it.

     Here, ``hiddenFoo`` is a private module-level variable, making it
     only accessible to other code contained in ``modToUse``
   */
  private var hiddenFoo = false;


  /* ``baz`` is a public function which is defined within ``modToUse``  */
  proc baz (x, y) {
    return x * (x + y);
  }

  /* ``hiddenBaz`` is a private function, which is also only accessible by
     symbols contained in ``modToUse``.
   */
  private proc hiddenBaz(a) {
    writeln(a);
    return a + 3;
  }

  /* ``Rec`` is a module-level record, with a field and a method defined inside
     it.
   */
  record Rec {
    var field: int;

    proc method1 () {
      writeln("In Rec.method1()");
    }
  }

  /* ``method2`` is a secondary method defined on ``Rec``. */
  proc Rec.method2() {
    writeln("In Rec.method2()");
  }

} // end of modToUse module

/* In the current implementation, ``private`` cannot be applied to type
   definitions; type aliases, and declarations of enums, records, and
   classes cannot be declared private.  Private also cannot be applied to
   fields or methods yet.

   Here are some other modules which will be used in the remainder of this file
*/


module AnotherModule {
  var a = false;
}

module ThirdModule {
  var b = -13.0;
}


module Conflict {
  /* This variable shares a name with a symbol in ``modToUse``.  */
  var bar = 5;

  var other = 5.0 + 3i;

  var another = false;
} // end of Conflict module


module DifferentArguments {
  /* This function shares a name with a function in ``modToUse``, but takes
     different arguments  */
  proc baz(x) {
    return x - 2;
  }
} // end of DifferentArguments module

module MainModule {
  proc main() {
    writeln("Access from outside a module");

    /* Access From Outside a Module
       ----------------------------
       In multi-module programs, it is common for modules to access
       the contents of other modules.  The starting point for doing so
       is the "``use`` statement" in Chapel.  These statements can be
       inserted at any lexical scope that contains executable code.

       By default, a ``use`` statement makes all of a specific
       module's visible symbols available to the scope that contains
       that ``use`` statement.  These symbols may then be accessed
       directly in an unqualified manner (without a module name prefix).

       In this case, ``bazBarFoo`` should store the result of calling
       ``modToUse.baz`` on ``modToUse.bar`` and ``modToUse.foo``, which is
       in this case ``28``.
    */
    {
      use modToUse;

      var bazBarFoo = baz(bar, foo);
      writeln(bazBarFoo);

    }
    /* Since ``use`` statements only affect their containing scope, when we
       leave a scope like this, we lose access to the module's symbols.  For
       instance, since the following line isn't within a scope that contains a
       ``use`` of ``modToUse``, it would generate an error if uncommented.
       This is because ``foo`` is not visible within our lexical scope or
       via any ``use`` statements in that scope.
     */
    // var twiceFoo = 2 * foo;



    /* ``use`` statements apply to the entire scope in which they are defined.
       Even if the ``use`` statement occurs after code which would directly
       refer to its symbols, these references are still valid.  This is
       similar to other Chapel forms of introducing symbols - for instance,
       function declaration order does not prevent a function declared earlier
       in a scope from calling one declared later.

       Thus, as in an earlier example, the following declaration of
       ``bazBarFoo`` will store the result of calling ``modToUse.baz``
       on ``modToUse.bar`` and ``modToUse.foo``, which is again
       ``28``.
    */
    {
      var bazBarFoo = baz(bar, foo);

      use modToUse;

      writeln(bazBarFoo);
    }

    /* Modules that are being used can be given a different name than the ones
       they were declared with.  In this example, ``modToUse.bar`` can be
       accessed using the new name, ``other``, as the module prefix.  The old
       name is not visible in that scope, though, so writing just
       ``modToUse.bar`` will not work.  However, unprefixed access to the
       module's symbols remains unchanged.
    */
    {
      use modToUse as other;

      writeln(other.bar);
      // writeln(modToUse.bar); // would be an error, modToUse not visible
      writeln(bar);
    }


    /* The symbols provided by a ``use`` statement are only considered when
       the name in question cannot be resolved directly within the local
       scope. Thus, because another ``bar`` is defined within this scope, the
       reference to ``bar`` within the ``writeln`` will refer to the local
       variable ``bar`` rather than to ``modToUse.bar``.
    */
    {
      var bar = 4.0;

      use modToUse;

      writeln(bar);
      // Will output the value of the bar defined in this scope (which is
      // '4.0'), rather than the value of modToUse.bar (which is '2')
    }


    /* If a symbol cannot be resolved directly within the local scope, then
       the symbols provided by a ``use`` statement are considered before the
       symbols defined outside of the scope where the ``use`` statement
       applies.  Thus, because the other ``bar`` was defined outside of
       these curly braces, the compiler will find the ``bar`` from
       ``modToUse`` when resolving the access within the ``writeln``,
       rather than the outer ``bar``. The ``bar`` from ``modToUse`` is said
       to be "shadowing" the definition at the outer scope.
    */
    {
      var bar = false;
      {

        use modToUse;
        writeln(bar);
        // Will output the value of modToUse.bar (which is '2'), rather
        // than the value of the bar defined outside of this scope (which
        // is 'false')
      }
    }

    /* Multiple modules may be named in a single ``use`` statement  */
    {
      use modToUse, AnotherModule, ThirdModule;

      if (a || b < 0) {
        // Refers to AnotherModule.a (which is 'false') and ThirdModule.b (which
        // is '-13.0')
        writeln(foo); // Refers to modToUse.foo
      } else {
        writeln(bar); // Refers to modToUse.bar
      } // Will output modToUse.foo (which is '12')
    }


    /* Equivalently, a scope may contain multiple ``use`` statements  */
    {
      use modToUse;
      use AnotherModule, ThirdModule;

      writeln(a && foo > 15);
      // outputs false (because AnotherModule.a is 'false' and modToUse.foo is
      // '12')
    }


    /* In either case, the modules used in this way are considered in concert
       (after symbols defined at this scope but before symbols defined outside
       of it) - the ordering within a ``use`` statement or across multiple
       ``use`` statements does not affect the precedence of symbols that share
       a name. This means that if two modules each define a symbol with the
       same name, and both modules are used at the same scope, attempts to
       access a symbol by that name will result in a naming conflict.

       The commented-out line below would fail because both ``modToUse`` and
       ``Conflict`` define a symbol named ``bar``:
    */
    {
      use modToUse, Conflict;

      writeln(foo); // Outputs modToUse.foo ('12')
      // writeln(bar);
      writeln(other); // Outputs Conflict.other ('5.0 + 3.0i')
    }

    /* When the symbol being accessed is the name of a function, the rules
       become more complex.  If the two function definitions are overloads (or
       define different arguments), then the best match will be found, no
       matter where the function is defined relative to the other function
       definitions.

       More details on when overloading applies, when functions may shadow
       other functions, etc. can be found in the relevant section of the
       language specification.  They will not be covered further in this
       primer.

       Finally, the names of the modules themselves are made available
       by a ``use`` statement at a scope just outside of the modules'
       contents and just inside the next lexical scope surrounding the
       current one.
    */
    {

      use modToUse, DifferentArguments;

      writeln(baz(2, 3));
      // Accesses the function modToUse.baz using the two arguments.  Should
      // output 2 * (2 + 3) or '10'
      writeln(baz(3));
      // Access the function DifferentArguments.baz using the single argument.
      // Should output 3 - 2, or '1'
    }

    /* Limiting a Use
       --------------
       To get around such conflicts, there are multiple strategies.  If only a
       small number of symbols are desired from a particular module, you can
       specify the symbols to bring in via an ``only`` list.

       Here, because of the ``only`` clause in the ``use`` of ``Conflict``,
       Conflict's ``bar`` is not directly accessible here.
    */
    writeln();
    writeln("Limiting a use");


    {
      use modToUse;
      use Conflict only other, another;

      writeln(foo); // Outputs modToUse.foo ('12')
      writeln(bar); // Outputs modToUse.bar ('2')
      writeln(other); // Outputs Conflict.other ('5.0 + 3.0i')
    }


    /* Using an ``except`` list will cause every symbol other than the ones
       listed to be available.
    */
    {
      use Conflict;
      use modToUse except bar;

      writeln(foo); // Outputs modToUse.foo ('12')
      writeln(bar); // Outputs Conflict.bar ('5')
      writeln(other); // Outputs Conflict.other ('5.0 + 3.0i')
    }


    /* If both symbols which conflict are desired, or if the ``use`` causes
       symbols to be shadowed which are necessary, you can choose to rename a
       symbol when including it via the ``as`` keyword, so long as the new name
       does not cause any conflicts with other included symbols.
    */
    {
      use modToUse;
      use Conflict only bar as boop;
      writeln(bar); // Outputs modToUse.bar ('2')
      writeln(boop); // Outputs Conflict.bar ('5')
    }

    /* You can also ``use`` a module without making any symbols
       available in an unqualified manner using an asterisk after
       ``except``...
    */
    {
      use modToUse except *;
      use Conflict except *;
      writeln(modToUse.bar);  // Outputs modToUse.bar ('2')
      writeln(Conflict.bar);  // Outputs Conflict.bar ('5')
      // writeln(bar);        // this won't resolve since bar isn't available
    }

    /* ...or equivalently, an empty identifier list after ``only``.
       These forms are typically used by programmers who prefer to
       always fully qualify references to their modules' symbols. */
    {
      use modToUse only;
      use Conflict only;
      writeln(modToUse.bar);  // Outputs modToUse.bar ('2')
      writeln(Conflict.bar);  // Outputs Conflict.bar ('5')
      // writeln(bar);        // this won't resolve since bar isn't available
    }

    /* When either of these are present, any instances of classes or records
       will be able to access their symbols defined in that module.
    */
    {
      use modToUse only;
      var rec = new modToUse.Rec(4); // Only accessible via the module prefix
      writeln(rec.field);            // Accessible because we have an instance
      rec.method1();                 // Ditto to the field case
      rec.method2();
    }

    writeln();

    /* Application to Enums
       --------------------
       ``use`` statements can also be called on enums.  Normally to access one
       of an enum's constants, you must provide a prefix of the enum name.
       With a ``use`` of that enum, such a prefix is no longer necessary.
    */
    writeln("Application to enums");

    {

      enum color {red, blue, yellow};

      {
        // Normally you must prefix the constant with the name of the enum
        var aColor = color.blue;
        writeln(aColor);
      }

      {
        use color;

        // The 'use' statement allows you to access an enum's symbols without
        // the prefix
        var anotherColor = yellow; // color.yellow
        writeln(anotherColor);
      }

    }

    writeln();
    /* All of the above rules for using modules also apply to using
       enums */

    /* Nested Modules
       --------------
       A ``use`` of a nested module (see the module ``OuterNested`` and its
       submodules for an example of a nested module) is similar to that of a
       top-level module.  Its name is treated like any other visible symbol
       in the outer module, so if the outer module has not been used then
       the inner module must be explicitly named.
    */
    {
      use OuterNested.Inner1;

      writeln(foobar); // Will output Inner1.foobar, or '14'
    }

  } // end of main() function
} // end of MainModule module

module OuterNested {
  var foo = 12;
  var bar: int = 2;
  private var hiddenFoo = false;

  proc baz (x, y) {
    return x * (x + y);
  }

  private proc hiddenBaz(a) {
    writeln(a);
    return a + 3;
  }

  /* A module defined within another module is called a nested module.  These
     submodules cannot refer to symbols defined within their parent module,
     without a ``use`` or ``import`` of the parent module.  The
     parent module can only access the contents of the nested module using
     a ``use`` or ``import`` statement or a fully qualified name.

     The variable ``foobar`` references OuterNested's ``foo`` and ``bar``
     variables.
   */
  module Inner1 {
    use OuterNested;
    var foobar = foo + bar;
  }

  /* Since the module ``Inner2`` is defined within ``OuterNested``, it can
     access the private variable ``hiddenFoo`` and the private function
     ``hiddenBaz``. However, any private symbol defined within ``Inner2``
     will not be visible within scopes defined outside of ``Inner2``.
  */
  module Inner2 {
    private var innerOnly = -17;
    var canSeeHidden = !hiddenFoo;
  }
} // end of OuterNested module

/* Public vs. Private Uses
   -----------------------

   Use statements can be labeled as being either ``public`` or ``private``.
   By default ``use`` statements are ``private``.  This means that if one
   module uses a second, it will only see the symbols defined by that module,
   not by other modules that it happens to use.  For example, consider the
   following library module:
*/


module ModuleThatIsUsed {
  proc publiclyAvailableProc() {
    writeln("This function is accessible!");
  }
}

/* And a module that uses it:
 */
module UserModule {
  use ModuleThatIsUsed;  // or `private use ModuleThatIsUsed`
}

/* When a scope has a ``use`` of ``UserModule``, the symbols from
   ``ModuleThatIsUsed`` will not be available due to the ``private`` nature of
   ``UserModule`` 's ``use``, so the following code would not compile.
*/

module UsesTheUser {
  proc func1() {
    use UserModule;
    // publiclyAvailableProc(); // Won't compile, since ``UserModule``'s ``use`` is ``private``
  }
}

/* By contrast, a ``public use`` will permit symbols used by one module to
   be seen by those that use it.  For example, consider the following
   variation of the previous example:
*/
module UserModule2 {
  public use ModuleThatIsUsed;
}


/* Since its use is ``public``, A scope with a ``use`` of ``UserModule2`` will 
   also be able to see the symbols defined by ``ModuleThatIsUsed``.
*/
module UsesTheUser2 {
  proc func2() {
    use UserModule2;
    publiclyAvailableProc(); // available due to ``use`` of ModuleThatIsUsed
  }
}


