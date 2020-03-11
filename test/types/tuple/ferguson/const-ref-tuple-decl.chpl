config const cond = false;
config const otherCond = true;
var print = true;

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

proc commonPrefixLength(const a1: [] R, const a2: [] R) {
  writeln("in commonPrefixLength");
  const ref (a, b) = if a1.size < a2.size then (a1, a2) else (a2, a1);
  /* this alternative functions correctly:
  const ref a;
  const ref b;
  if a1.size < a2.size {
    a = a1;
    b = a2;
  } else {
    a = a2;
    b = a1;
  }*/

  for x in a {
    writeln(x);
  }

  writeln("end commonPrefixLength");
}


proc main() {
  var A:[1..2] R = for i in 1..2 do new R(i);
  var B:[1..3] R = for i in 1..3 do new R(10*i);

  commonPrefixLength(A, B);
}
