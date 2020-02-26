config var print = true;

class C { }
record R {
  var x: int = 0;
  var ptr: shared C = new shared C();
  proc init() {
    this.x = 0;
    if print then writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    if print then writeln("init ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    if print then writeln("init= ", other.x);
  }
  proc deinit() {
    if print then writeln("deinit ", x);
  }
}
proc =(ref lhs:R, rhs:R) {
  if print then writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}

proc val(arg: R) {
  return arg.x;
}

iter iterateAllOf(tuple...?k) {
  for param i in 1..k {
    for j in tuple(i) {
      yield j;
    }
  }
}

proc main() {
  var A:[1..2, 1..2] R = for (i,j) in {1..2, 1..2} do new R(10*i + j);
  if 121 == + reduce val(iterateAllOf(A, A[1..2, 1..1], A[1, ..])) {
    writeln("OK");
  }
}
