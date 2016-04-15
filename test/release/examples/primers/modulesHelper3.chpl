/* This file is solely intended as a helper file to the modules primer.  It
   is not intended to be looked at on its own.
*/

// This module is not named after the file in which it lives.  Accessing it from
// another module requires naming the file on the command line.  It also
// contains a main() function, which means programs built with it will have to
// disambiguate which main() to run using the --main-module flag
module helperWithMain {
  var someVar = 19;

  proc someFunc() {
    return someVar + 4;
  }

  proc main() {
    writeln("I am Spartacus!");
  }
}
