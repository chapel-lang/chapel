
record Foo {
  var x : int;
  var y : real;

  proc init() {
    x = 5;
    y = 10.0;
  }
}

proc main() {
  var A : [1..4] Foo;
  writeln(A);
}
