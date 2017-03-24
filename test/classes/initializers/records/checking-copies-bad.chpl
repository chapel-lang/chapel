// Created to verify semantics of records which define initializers and are
// moved
record R {
  var x: int;
  var y: bool;

  proc init() {
    x = 10;
    y = true;
  }
}

proc main() {
  var r = new R();
  var copy = r;
  writeln(copy);
}
