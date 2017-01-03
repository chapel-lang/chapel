// Created to verify semantics of records with compiler generated initCopy()s
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
