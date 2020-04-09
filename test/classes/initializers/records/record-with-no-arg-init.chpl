// Created to verify semantics of records which do not define a copy initializer
record R {
  var x: int;
  var y: bool;

  proc init() {
    x = 10;
    y = true;
  }
}

proc main() {
  var r: R;
  writeln(r);
}
