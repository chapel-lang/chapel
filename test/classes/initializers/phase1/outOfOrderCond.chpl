
record R {
  var x, y, z : int;

  // would like the 'y' case to error as well, but that's a separate issue
  // about halting after the first out-of-order field.
  proc init(cond: bool) {
    this.z = 5;
    if cond then
      this.x = 3;
    else
      this.y = 42;
  }
}

var r = new R(true);
