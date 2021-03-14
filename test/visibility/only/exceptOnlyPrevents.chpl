// Verifies the behavior of use chains that start with an 'except' and have
// an 'only' in them
module Outer {
  use Inner only a;

  var b: bool;
  var c = 14.7;
}

module Inner {
  var a = "tee hee";
  var d: int;
}

module M {
  use Outer except c;

  proc main() {
    writeln(d); // Should fail, the only should hide it.
  }
}
