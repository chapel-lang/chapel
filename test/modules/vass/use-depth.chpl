// Should the uses of 'x' and 'y' through M1 vs. M2 be in conflict?
// Or should M1's 'x' shadow M2's 'x' because the use chain for
// M2's 'x' is deeper?

// main module

use M1;  // depth of use chain: x: 2, y: 2, z: 2
use M2;  // depth of use chain: x: 3, y: 1, z: 2

compilerWarning("got x: ", typeToString(x.type)); // could prefer M11's x
compilerWarning("got y: ", typeToString(y.type)); // could prefer M2's y
compilerWarning("got z: ", typeToString(z.type)); // error anyway

// modules being used

module M1 {
  use M11;
}

module M11 {
  var x: real;
  var y: real;
  var z: real;
}

module M2 {
  use M22;
  var y: int;
}

module M22 {
  use M222;
  var z: int;
}

module M222 {
  var x: int;
}
