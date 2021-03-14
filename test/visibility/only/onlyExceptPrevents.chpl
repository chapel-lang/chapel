// Verifies the behavior of use chains that start with an 'only' and have
// an 'except' in them
module Outer {
  use Inner except a;

  var b: bool;
  var c = 14.7;
}

module Inner {
  var a = "tee hee";
  var d: int;
}

module M {
  use Outer only c, d;

  proc main() {
    writeln(a); // Should fail, the except should hide it.
  }
}
