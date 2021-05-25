class Foo {
  deprecated "Field p1 is deprecated, use p2 instead"
  param p1 = 3;
  param p2 = 3;

  proc init(param val) {
    p2 = val;
  }
}

proc main() {
  var f = new Foo(5);
  writeln(f.p1);
  writeln(f.p2);
}
