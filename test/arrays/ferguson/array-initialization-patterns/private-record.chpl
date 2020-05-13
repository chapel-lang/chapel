use PrivateDist;

config const test=0;

config var printInitDeinit = true;
  
class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
  proc init() {
    this.x = 0;
    this.ptr = new shared C(0);
    if printInitDeinit then writeln("a init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared C(arg);
    if printInitDeinit then writeln("b init ", arg, " ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    this.ptr = new shared C(other.ptr.xx);
    if printInitDeinit then writeln("c init= ", other.x, " ", other.ptr.xx);
  }
  proc deinit() {
    if printInitDeinit then writeln("z deinit ", x, " ", ptr.xx);
  }
  proc toString() {
    return "(" + this.x:string + " " + this.ptr.xx:string + ")";
  }
  proc ref set1() ref {
    this.x = 1;
    this.ptr.xx = 1;
    return this;
  }
}
proc =(ref lhs:R, rhs:R) {
  if printInitDeinit then writeln("d lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}

proc test0() {
  writeln(" test0");
  var A: [PrivateSpace] R;
  for (loc, i) in zip(Locales, 0..) {
    on loc {
      A[i] = new R(i+1);
    }
  }
  for loc in Locales {
    on loc {
      writeln(A[here.id]);
    }
  }
}
if test == 0 {
  test0();
  writeln();
}

proc test1() {
  writeln(" test1");
  var A: [PrivateSpace] R = for i in 1..#numLocales do new R(i);
}
if test == 1 {
  test1();
  writeln();
}

proc test2() {
  writeln(" test2");
  var A: [PrivateSpace] R = for i in 1..#numLocales do new R(i);
  var B = A;
}
if test == 2 {
  test2();
  writeln();
}

proc test3() {
  writeln(" test3");
  var A: [PrivateSpace] R = for i in 1..#numLocales do new R(i);
  var B = A;
  A; // no copy elision
}
if test == 3 {
  test3();
  writeln();
}

proc test4() {
  writeln(" test4");
  var A: [PrivateSpace] R = for i in 1..#numLocales do new R(i);
  var B: [PrivateSpace] R = A;
}
if test == 4 {
  test4();
  writeln();
}

proc test5() {
  writeln(" test5");
  var A: [PrivateSpace] R = for i in 1..#numLocales do new R(i);
  var B: [PrivateSpace] R = A;
  A; // no copy elision
}
if test == 5 {
  test5();
  writeln();
}
