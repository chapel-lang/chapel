
proc printer(const val) {
  writeln("val = ", val);
}

record R {
  var x : int;
  var y : real;

  proc init() {
    this.x = 1;
    this.y = 1.0;

    // Ensure compiler catches method calls nested inside other calls
    printer(this.foo());
    this.complete();
  }

  proc foo() {
    return x * y;
  }
}
