module OuterModule {

  // Modules are Chapel's way of managing name spaces.
  // The files containing these modules do not need to be named after the modules
  // (as in Java), but files implicitly name modules.
  // In this case, this file implicitly names the 'learnchapel' module

  module OurModule {
    // We can use modules inside of other modules.
    use Time; // Time is one of the standard modules.

    // We'll use this procedure in the parallelism section.
    proc countdown( seconds: int ){
      for i in 1..seconds by -1 {
        writeln( i );
        sleep( 1 );
      }
    }

    // Submodules of OurModule
    // It is possible to create arbitrarily deep module nests.
    module ChildModule {
      proc foo(){
        writeln( "ChildModule.foo()");
      }
    }

    module SiblingModule {
      proc foo(){
        writeln( "SiblingModule.foo()" );
      }
    }
  } // end OurModule
  use OurModule;

  // At this point we have not used ChildModule or SiblingModule so their symbols
  // (i.e. foo ) are not available to us.
  // However, the module names are, and we can explicitly call foo() through them.
  SiblingModule.foo();         // Calls SiblingModule.foo()

  // Super explicit naming.
  OurModule.ChildModule.foo(); // Calls ChildModule.foo()

  use ChildModule;
  foo();   // Less explicit call on ChildModule.foo()

  sync {
      begin { // Start of new task's body
        var a = 0;
        for i in 1..1000 do a += 1;
        writeln( "Done: ", a);
      } // End of new tasks body
      writeln( "spun off a task!");
    }
    writeln( "Back together" );
}
