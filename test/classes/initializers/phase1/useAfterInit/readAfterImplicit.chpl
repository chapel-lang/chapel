
record R {
  var x : int;
  var y : real;

  proc init() {
    this.y = x * 2.0;
    this.initDone();
  }
}
