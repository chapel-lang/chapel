// Verifies the behavior of public use chains that start with an 'only' and
// have an 'except' in them
module Outer {
  public use Inner except a;

  var b: bool;
  var c = 14.7;
}

module Inner {
  var a = "tee hee";
  var d: int;
}

module M {
  public use Outer only c, d;

  proc main() {
    writeln(c);
    writeln(d);
  }
}
