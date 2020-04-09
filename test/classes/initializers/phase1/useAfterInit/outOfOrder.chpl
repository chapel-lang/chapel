
record R {
  var x : int;
  var y : real;

  proc init() {
    this.y = 1.0;
    this.x = 5;
    this.complete();
  }
}
