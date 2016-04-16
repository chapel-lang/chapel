/*
 * Module Primer
 *
 * This primer introduces the concept of modules, a strategy for encapsulating
 * code for later use.  It covers:
 *   motivation for creating a module
 *   namespace control for a module's symbols
 *   outside access of a module's symbols
 *   namespace control when using a module in an outside context, including:
 *     unlimited
 *     explicit exclusion of symbols
 *     explicit inclusion of symbols
 *     renaming included symbols
 *   outside access of modules in different files
 */


/* Once programs become large, it is useful to group related code into units.
   If the related code extends beyond mere object orientation, then it is best
   to utilize what is known in Chapel as a "module".

   Modules 

   TODO!!!!!!!!!!!!!!!!!!!!!!
 */
module modToUse {

  /* In this case, foo is a public global variable which lives in modToUse */
  var foo = 12;

  // As is bar.
  var bar: int = 2;

  /* baz is a public function which lives in modToUse */
  proc baz (x, y) {
    return x * (x + y);
  }

  // TO COVER: nested modules



  /* Often, the functionality of a group of code doesn't need to be completely
     visible to outside clients.  Sometimes it would be harmful for the client
     to access a function that isn't part of the blessed API, and sometimes it
     just adds unnecessary clutter to the namespace.  To avoid these cases, a
     symbol can be declared "private" - this means that only code defined within
     the same scope as the definition of this symbol can access it.

  /* Here, hiddenFoo is a private global variable, which is only accessible by
     symbols defined within modToUse
  */
  private var hiddenFoo = false;

  /* hiddenBaz is a private function, which is also only accessible by symbols
     defined within modToUse.
  */
  private proc hiddenBaz(a) {
    writeln(a);
    return a + 3;
  }

  module Inner {
    /* Since the module Inner is defined within modToUse, it can access the
       private variable hiddenFoo and the private function hiddenBaz.  However,
       any private symbol defined within Inner will not be visible to symbols
       defined outside of Inner.
     */

    private var innerOnly = -17;
    var canSeeHidden = !hiddenFoo;
  }

  // At the moment, private cannot be applied to type definitions; type
  // aliases, and declarations of enums, records, and classes cannot be declared
  // private.  Private also cannot be applied to fields or methods yet.

}

module AnotherModule {
  var a = false;
}

module ThirdModule {
  var b = -13.0;
}


module Conflict {
  /* This parenthesis-less function shares a name with a symbol in modToUse. */
  proc bar {
    writeln("In Conflict.bar");
    return 5;
  }

  var other = 5.0 + 3i;
}


proc main() {
  writeln("Access from outside a module");

  /* Often, it is desirable for the contents of a module to be accessed from
     outside that context.  There are several strategies for accomplishing this:

     First, if only one symbol defined in the module is desired, and it is
     desired only once, the symbol can be referenced explicitly - this is done
     using the module name and a separating '.' as a prefix to the name of the
     symbol desired.
  */
  var thriceFoo = 3 * modToUse.foo; // should be 36
  writeln(thriceFoo);


  {
    /* If many of the module's symbols are desired, or the same symbol is
       desired multiple times, then it is best to utilize what is known as a
       "use statement".
    */
    use modToUse;

    /*
      Use statements can be inserted at the global scope, as well as any
      arbitrary inner scope defined.
    */

    /* A use statement brings all of the visible symbols of a module into the
       scope which defines the use statement.  These symbols may then be
       accessed without the module name prefix.
    */
    var bazBarFoo = baz(bar, foo); // should be 28
    writeln(bazBarFoo);
  }



  /* Once the scope defining a use statement is left, however, the symbols must
     be accessed with a prefix.  The following line would cause an error,
     because the previous use of the module 'modToUse' occurred within the curly
     braces above:
  */
  // var twiceFoo = 2 * foo;



  {
    var bazBarFoo = baz(bar, foo); // should be 28

    /* Use statements apply to the entire scope in which they are defined.  Even
       if the use statement occurs after code which would access the symbols
       without prefix, these accesses are still valid.
    */
    use modToUse;

    writeln(bazBarFoo);
  }


  {
    /* The symbols provided by a use statement are considered at an outer scope
       to the symbols defined at the same level as the use itself.  Thus, if
       another bar were defined here, and an access was attempted, the compiler
       would find the bar at this scope, rather than modToUse.bar.
    */
    var bar = 4.0;

    use modToUse;

    writeln(bar); // Will output 4.0, not 2
  }


  var bar = false;
  {
    /* The symbols provided by a use statement are considered at an inner scope
       to the symbols defined outside of the scope where the use statement
       applies.  Thus, if another bar were defined outside of these curly
       braces, and an access was attempted, the compiler would find the bar from
       modToUse, rather than the outer bar.
    */

    use modToUse;
    writeln(bar); // Will output 2, not false
  }


  {
    /* Multiple modules may be used in the same use statement */
    use modToUse, AnotherModule, ThirdModule;

    if (a || b < 0) {
      writeln(foo);
    } else {
      writeln(bar);
    } // Will output foo (12)
  }


  {
    /* And a scope may contain multiple use statements */
    use modToUse;
    use AnotherModule;

    writeln(a && foo > 15); // outputs false
  }


  {
    /* In either case, the modules used in this way are considered at the same
       scope.  This means that if two modules each define a symbol with the same
       name, and both modules are used at the same scope, attempts to access a
       symbol by that name will result in a naming conflict:
    */
    use modToUse, Conflict;

    writeln(foo); // Outputs 12
    /* The following line would fail because both modToUse and Conflict define a
       bar:
    */
    // writeln(bar);
    writeln(other); // Outputs 5.0 + 3.0i
  }

  writeln();
  writeln("Limiting a use");


  {
    /* To get around such conflicts, there are multiple strategies.  If only a
       small number of symbols are desired from a particular module, you can
       specify the symbols to bring in via an 'only' list.
    */
    use modToUse;
    use Conflict only other;

    writeln(foo); // Outputs 12
    writeln(bar); // Outputs 2
    writeln(other); // Outputs 5.0 + 3.0i
  }


  {
    /* If every symbol other than the one which causes a conflict is desired,
       you can specify the symbols to exclude via an 'except' list.
    */
    use Conflict;
    use modToUse except bar;

    writeln(foo); // Outputs 12
    writeln(bar); // Outputs 5 after output in Conflict.bar function
    writeln(other); // Outputs 5.0 + 3.0i
  }


  {
    /* If both symbols which conflict are desired, or if the use causes symbols
       to be shadowed which are necessary, you can choose to rename a symbol
       when including it, so long as the new name does not cause any conflicts
       with other included symbols.
    */
    use modToUse;
    use Conflict only bar as boop;
    writeln(bar); // Outputs 2
    writeln(boop); // Outputs 5 after output in Conflict.bar function
  }

  writeln();
  writeln("Application to enums");

  {
    /* 'Use' statements can also be called on enums.  Normally to access one of
       an enum's constants, you must provide a prefix of the enum name.  With a
       'use' of that enum, such a prefix is no longer necessary.
    */

    enum color {red, blue, yellow};

    // Normally you must use the enum name as a prefix
    var aColor = color.blue;
    writeln(aColor);

    use color;

    // The use statement allows you to access it without the prefix
    var anotherColor = yellow; // color.yellow
    writeln(anotherColor);

    // All of the above rules for 'use' statements also apply to 'use's of enums
  }

  writeln();
  writeln("Accessing modules from other files");

  {
    /* Modules that live outside of the file sometimes have special rules for
       how to access them.  If the module you wish to access has the same name
       as the file in which it lives, and this file is in the same directory as
       your program, no additional steps are necessary.
    */

    use modulesHelper;

    writeln(someVar); // Should be 19
  }


  {
    /* When the module you wish to access is not named after the file in which
       it lives, you must name the file as part of the compilation step:

       chpl modules.chpl modulesHelper2.chpl
    */

    use anotherHelper;

    writeln(someFunc()); // Should be 23

    /* Note, the above will only compile if all the code after this closing
       brace until the end of the main() function is removed or commented out.
       A summary compilation line will be provided at the end
    */
  }


  {
    /* If the helper module also defines a main() function, or this module does
       not, then you must specify on the command line at compilation time which
       module should be the main module for the program, using the --main-module
       flag:

       chpl modules.chpl modulesHelper3.chpl --main-module modules
    */

    use helperWithMain;

    writeln(someVar); // Should be 19

    /* Note, the above will only compile if all the code after this closing
       brace until the end of the main() function is removed or commented out,
       as well as the code for accessing anotherHelper.  A summary compilation
       line will be provided at the end
    */
  }


  {
    /* If the module you wish to use lives in a different directory, you must
       specify the directory where it lives as part of compilation using the
       -M flag.  The -M flag can take a relative or exact path.

       chpl modules.chpl -M modulesPrimerDir/
     */

    use helperInAnotherDir;
    writeln(someFunc()); // Should be 23

    /* Note, the above will only compile if all the code for accessing
       anotherHelper and helperWithMain is removed or commented out.  A summary
       compilation line will be provided at the end
    */
  }

  {
    /* If the module you wish to use is one of the Chapel provided library
       modules (living in $CHPL_HOME/modules/), then merely using the module
       is sufficient for the program to compile
    */

    use IO;
    writeln(iomode.r);
  }

  /* To compile all the examples in this file, use the command line:

     chpl modules.chpl modulesHelper2.chpl modulesHelper3.chpl --main-module modules -M modulesPrimerDir/
   */
}
