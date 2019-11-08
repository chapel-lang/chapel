// Verifies the behavior of public use chains that start with an 'except' and
// have an 'only' in them
module Outer {
  public use Inner only a;

  var b: bool;
  var c = 14.7;
}

module Inner {
  var a = "tee hee";
  var d: int;
}

module M {
  public use Outer except c;

  proc main() {
    writeln(a);
    writeln(b);
  }
}
