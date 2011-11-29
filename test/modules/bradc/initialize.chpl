//
// define a module named "Hello".  If a source file defines no
// modules, the filename minus the .chpl extension serves as the
// module name for the code it contains.
//
module Hello {

  //
  // the message to be printed out, defined as a configuration
  // constant named "message".  The type is inferred to be a string
  // due to the initializing expression.  This message can be
  // over-ridden by using --message="new message" on the executable's
  // command line
  //
  config const message = "Hello, world!";

  //
  // the module's initialization routine, which is run when the module
  // is first used.  Since this module contains the main() routine, it
  // will be initialized at program startup.  
  //
  // For convenience, the init() routine can be omitted in which
  // case all top-level executable statements in a module are used to
  // create an automatic initialize() routine.  This is what allows
  // top-level code like that contained in hello.chpl and
  // hello-config.chpl to work.
  //
  proc initialize() {
    writeln("from the Hello module's initialization routine: ", message);
  }

  //
  // the module's main() routine, which is the program's entry point
  // and will be run after the module (and any modules that it uses)
  // are initialized.
  //
  proc main() {
    writeln("from the main routine: ", message);
  }
}
