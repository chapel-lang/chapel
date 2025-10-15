// Test for issue #27411: Multiple fields with R(?) should work
record R {
  type t;
  var x: t;
}

record S {
  var x: R(?);
  var y: R(?);

  proc init() {
    this.x = new R(int);
    this.y = new R(int);
  }
}

var myS = new S();
writeln(myS);
