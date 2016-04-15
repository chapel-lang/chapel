/* This file is solely intended as a helper file to the modules primer.  It
   is not intended to be looked at on its own.
*/

// This module is not named after the file in which it lives.  Accessing it from
// another module requires naming the file on the command line.
module anotherHelper {
  var someVar = 19;

  proc someFunc() {
    return someVar + 4;
  }
}
