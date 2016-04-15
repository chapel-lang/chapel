/* This file is solely intended as a helper file to the modules primer.  It
   is not intended to be looked at on its own.
*/

// This module does not live in the same directory as the program which intends
// to use it.  Accessing it from another module requires naming the directory
// where it lives on the command line.
module helperInAnotherDir {
  var someVar = 19;

  proc someFunc() {
    return someVar + 4;
  }
}
