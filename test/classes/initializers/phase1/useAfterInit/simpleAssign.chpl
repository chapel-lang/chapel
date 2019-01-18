
record R {
  var x : int;
  var y : real;

  proc init() {
    this.x = 5;
    this.y = 1.0;
    this.x *= 2;
    this.complete();
  }
}

var r = new R();
writeln("r = ", r);
