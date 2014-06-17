/*  This program is conceptually very similar to hello.chpl, but it
 *  uses a more structured programming style, explicitly defining a
 *  module, a configuration constant, and a main() procedure.
 */

//
// define a module named 'Hello'.  If a source file defines no
// modules, the filename minus its .chpl extension serves as the
// module name for the code it contains.  Thus, 'hello' would be
// the automatic module name for hello.chpl.
//
module Hello {

  //
  // Declare the message to be printed out as a configuration
  // constant named "message".  The type is inferred to be a string
  // due to the initializing expression.  Like any configuration
  // constant or variable, the default value can be over-ridden by
  // using on the executable's command line (e.g., --message="hiya!")
  //
  config const message = "Hello, world!";


  // Any top-level code in a module is executed when the module is
  // initialized at the start of a Chapel program's execution.  Thus,
  // in hello.chpl, the presence of a writeln() at the file scope is
  // part of the implicit module's initialization and is executed at
  // program startup (and since there was no explicit main() function
  // or other top-level code, that's all that the program does).


  //
  // Here we define the module's main() procedure, which is the
  // program's entry point that will be run after the module (and any
  // modules that it uses explicitly or implicitly) are initialized.
  //
  proc main() {
    writeln(message);
  }
}
