// Test for issue #27411: Multiple fields with R(?) can have different types
record R {
  type t;
  var x: t;
}

record S {
  var x: R(?);
  var y: R(?);

  proc init() {
    this.x = new R(int, 42);
    this.y = new R(real, 3.14);
  }
}

var myS = new S();
writeln(myS);
