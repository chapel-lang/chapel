
record R {
  var x: int;

  proc init(x=0) {
    this.x = x;
    writeln("init: ", x);
  }

  proc deinit() {
    writeln("deinit: ", x);
  }
}

proc helper(arg: int) {
  var r = new R(arg);
  return r;
}

proc main() {
  var x = helper;
  writeln(x(42));
}
