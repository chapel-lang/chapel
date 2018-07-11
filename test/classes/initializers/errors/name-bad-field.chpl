
record R {
  var a : int;
  var b : real;

  proc init() {
    a = 1; writeln(b);
  }
}

var r = new R();
