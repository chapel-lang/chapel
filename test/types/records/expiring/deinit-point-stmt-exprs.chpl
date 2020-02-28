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

proc secondDeclaration() {
  writeln("in secondDeclaration");
  return 1;
}

proc testMultiInit() {
  writeln("testMultiInit");
  var A = new R(1),
      B = secondDeclaration();
}
testMultiInit();

proc makeArr() {
  print = false;
  var A:[1..1] R;
  writeln("init 1 (setupArr)");
  A[1] = new R(1);
  print = true;
  return A;
}

proc testReduceExpr1() {
  writeln("testReduceExpr1");
  var A = makeArr();

  if 1 == + reduce A.x {
    writeln("in conditional");
  }
}
testReduceExpr1();

proc val(arg: R) {
  return arg.x;
}

proc testReduceExpr2() {
  writeln("testReduceExpr2");
  var A = makeArr();

  if 1 == + reduce val(A) {
    writeln("in conditional");
  }
}
testReduceExpr2();

iter iterateAllOf(tuple...?k) ref {
  for param i in 1..k {
    for j in tuple(i) {
      yield j;
    }
  }
}

proc testReduceExpr3() {
  writeln("testReduceExpr3");
  var A = makeArr();

  if 2 == + reduce val(iterateAllOf(A, A)) {
    writeln("in conditional");
  }
}
testReduceExpr3();

proc testReduceExpr4() {
  writeln("testReduceExpr4");
  var A = makeArr();

  var B = + reduce(val(A)),
      C = secondDeclaration();
}
testReduceExpr4();


proc testIfExpr1() {
  writeln("testIfExpr1");
  var A = makeArr();

  if 1 == if A[1].x == 1 then 1 else 2 {
    writeln("in conditional");
  }
}
testIfExpr1();

proc testIfExpr2() {
  writeln("testIfExpr2");
  var A = makeArr();

  var B = if A[1].x == 1 then 1 else 2,
      C = secondDeclaration();
}
testIfExpr2();

proc sumint(it) {
  var tot = 0;
  for x in it {
    tot += x;
  }
  return tot;
}

proc testForExpr1() {
  writeln("testForExpr1");
  var A = makeArr();

  if 1 == sumint(for r in A do r.x) {
    writeln("in conditional");
  }
}
testForExpr1();

proc testForExpr2() {
  writeln("testForExpr2");
  var A = makeArr();

  var B = sumint(for r in A do r.x),
      C = secondDeclaration();
}
testForExpr2();


proc testScanExpr1() {
  writeln("testScanExpr1");
  var A = makeArr();

  if 1 == sumint(+ scan A.x) {
    writeln("in conditional");
  }
}
testScanExpr1();

proc testScanExpr2() {
  writeln("testScanExpr2");
  var A = makeArr();

  var B = sumint(+ scan A.x),
      C = secondDeclaration();
}
testScanExpr2();

proc testForallExpr1() {
  writeln("testForallExpr1");
  var A = makeArr();

  var B = forall r in A do r.x,
      C = secondDeclaration();
}
testForallExpr1();

proc testForallExpr2() {
  writeln("testForallExpr2");
  var A = makeArr();

  var B = [r in A] r.x,
      C = secondDeclaration();
}
testForallExpr2();

proc testForallExpr3() {
  writeln("testForallExpr3");
  var A = makeArr();

  if 1 == sumint([r in A] r.x) {
    writeln("in conditional");
  }
}
testForallExpr3();
