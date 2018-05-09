record R {
  var x : int;

  proc init() {
    writeln("R.init()");
  }
  proc init(r:R) {
    this.x = r.x;
    writeln("R.init(R)");
  }
  proc deinit() {
    writeln("R.deinit");
  }
}

proc bad(n:int) {
  var err_args : R;
  for i in 1..n do err_args.x += 1;

  const ret = err_args:R;
  return ret;
}

proc main() {
  var b = bad(10);
  writeln("b = ", b);
}
