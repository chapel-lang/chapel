
proc printer(x) {
  writeln("got: ", x);
}

record R {
  var x : int;

  proc init() {
    this.x = 1;
    printer(this.foo());
    init this;
  }
}

var r = new R();
