
record R {
  var x : int = 2;
  var y : real;

  proc init() {
    this.y = x * 2.0;
    this.complete();
  }
}

var r = new R();
writeln("r = ", r);
