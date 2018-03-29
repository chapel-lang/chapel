
proc plusOne(ref x) {
  x += 1;
}

record R {
  var x : int;
  var y : real;

  proc init() {
    this.x = -1;
    this.y = 1.0;
    plusOne(x);
    this.complete();
  }
}

var r = new R();
writeln("r = ", r);
