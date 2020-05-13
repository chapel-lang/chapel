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
    if printInitDeinit then writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared C(arg);
    if printInitDeinit then writeln("init ", arg, " ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    this.ptr = new shared C(other.ptr.xx);
    if printInitDeinit then writeln("init= ", other.x, " ", other.ptr.xx);
  }
  proc deinit() {
    if printInitDeinit then writeln("deinit ", x, " ", ptr.xx);
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
  if printInitDeinit then writeln("lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}


var A:[1..1] int;

proc test0() {
  writeln("test0");
  var B = A;
  writeln(A, " ", B);
}
A = 0;
test0();

proc test1() {
  writeln("test1");
  var B:[1..1] int = A;
  writeln(A, " ", B);
}
A = 0;
test1();

proc test2() {
  writeln("test2");
  var B:A.type = A;
  writeln(A, " ", B);
}
A = 0;
test2();

proc test3() {
  writeln("test3");
  var B:[A.domain] int = A;
  writeln(A, " ", B);
}
A = 0;
test3();

proc test4() {
  writeln("test4");
  var B:[1..1] int = 1..1;
  writeln(B);
}
A = 0;
test4();

proc test5() {
  writeln("test5");
  var B:[1..1] int = [i in 1..1] i;
  writeln(B);
}
A = 0;
test5();

proc test6() {
  writeln("test6");
  var B:[1..1] int = {1..1};
  writeln(B);
}
A = 0;
test6();

proc test7() {
  writeln("test7");
  var B:[1..1] int = 1;
  writeln(B);
}
A = 0;
test7();

proc test8() {
  writeln("test8");
  var B:[1..1] int = (1,);
  writeln(B);
}
A = 0;
test8();

proc test9() {
  writeln("test9");
  // check that the domain is set correctly in variable initialization
  A = 1;
  var D = {1..1};
  var B:[D] int = A;
  D = {1..2};
  writeln(B.domain, " ", B);
}
A = 0;
test9();

proc inIntentTest(D, in A:[D] int) {
  return A;
}
proc test10() {
  writeln("test10");
  // check that the domain is set correctly in in-intent argument
  A = 1;
  var D = {1..1};
  var B = inIntentTest(D, A);
  D = {1..2};
  writeln(B.domain, " ", B);
}
A = 0;
test10();

record RdomainTest {
  var D: domain;
  var A: [D] int;
}

proc test11() {
  writeln("test11");
  // check that the domain is set correctly in record initialization
  A = 1;
  var D = {1..1};
  var r = new RdomainTest(D, A);
  r.D = {1..2};
  writeln(r.A.domain, " ", r.A);
}
A = 0;
test11();


proc makeA() {
  var savePrint = printInitDeinit;
  printInitDeinit = false;
  var ret: [1..1] R;
  ret[1] = new R(1);
  printInitDeinit = savePrint;
  if printInitDeinit then
    writeln("init 1 1");
  return ret;
}

printInitDeinit = false;
var globalA = makeA();
printInitDeinit = true;
writeln("-");

proc testr0() {
  writeln("testr0");
  var B = makeA();
  writeln(B);
}
testr0();
writeln("-");

proc testr1() {
  writeln("testr1 (copy required)");
  var B:[1..1] R = globalA;
  writeln(B);
}
testr1();
writeln("-");

proc testr2() {
  writeln("testr2");
  var B:[1..1] R = makeA();
  writeln(B);
}
testr2();
writeln("-");

proc makeTuple() {
  writeln("in makeTuple");
  var fullTuple;
  {
    fullTuple = (new R(1),);
  }
  writeln("done makeTuple");
  return fullTuple;
}

proc testr3() {
  writeln("testr3");
  var B:[1..1] R = makeTuple();
  writeln(B);
}
testr3();
writeln("-");

proc testr4() {
  writeln("testr4");
  // goes 1..10 because the forall will make some copies of the record
  // for different locales/tasks and we like to see that.
  var B:[1..10] R = new R(1);
  writeln(B);
}
testr4();
writeln("-");

proc testr5() {
  writeln("testr5");
  var B:[1..1] R = [i in 1..1] new R(1);
  writeln(B);
}
testr5();
writeln("-");

proc testr6() {
  writeln("testr6 (copy required)");
  var B:[1..1] R = globalA[1..1];
  writeln(B);
}
testr6();
writeln("-");

proc returnInArg(D, in arg:[D] R) {
  return arg;
}

proc buildArray(D) {
  var A: [D] R = [i in D] new R(i);
  return returnInArg(D, A);
}

proc testr7() {
  writeln("testr7");
  var D = {1..1};
  var B:[D] R = buildArray(D);
  writeln(B);
}
testr7();
writeln("-");

proc inIntentDefWithDom(D, in A:[D] R=buildArray(D)) {
  return A;
}

proc testr8() {
  writeln("testr8");
  var D = {1..1};
  var B:[D] R = inIntentDefWithDom(D);
  writeln(B);
}
testr8();
writeln("-");

proc testr9() {
  writeln("testr9");
  var D = {1..1};
  var B:[D] R = inIntentDefWithDom(D, buildArray(D));
  writeln(B);
}
testr9();
writeln("-");

proc inIntentDef(in A:[1..1] R=makeA()) {
  return A;
}

proc testr10() {
  writeln("testr10");
  var D = {1..1};
  var B:[D] R = inIntentDef();
  writeln(B);
}
testr10();
writeln("-");

proc testr11() {
  writeln("testr11");
  var D = {1..1};
  var B:[D] R = inIntentDef(makeA());
  writeln(B);
}
testr11();
writeln("-");

proc testr12() {
  writeln("testr12");
  var A = makeA();
  var B = A;
  writeln(A);
}
testr12();
writeln("-");

proc testr13() {
  writeln("testr13");
  var A = makeA();
  var B:A.type = A;
  writeln(A);
}
testr13();
writeln("-");




class C1 {
  var A: [1..1] R = [i in 1..1] new R(1);
}

proc testC1() {
  writeln("testC1");
  var c = new C1();
  writeln(c);
}
testC1();
writeln("-");

class C2 {
  var A: [1..1] R;
  proc init() {
    this.A = [i in 1..1] new R(1);
  }
}

proc testC2() {
  writeln("testC2");
  var c = new C2();
  writeln(c);
}
testC2();
writeln("-");

class C3 {
  var A: [1..1] R;
  proc init() {
    this.A = makeA();
  }
}

proc testC3() {
  writeln("testC3");
  var c = new C3();
  writeln(c);
}
testC3();
writeln("-");

class C4 {
  var A: [1..1] R;
}

proc testC4() {
  writeln("testC4");
  var c = new C4(makeA());
  writeln(c);
}
testC4();
writeln("-");

class C5 {
  var D;
  var A: [D] R;
}

proc testC5() {
  writeln("testC5");
  var D = {1..1};
  var A: [D] R = [i in D] new R(i);
  var c = new C5(D, A);
  writeln(c);
}
testC5();
writeln("-");

class C6 {
  var D;
  var A: [D] R;
  proc init(in D, in A: [D] R) {
    this.D = D;
    this.A = A;
  }
}

proc testC6() {
  writeln("testC6");
  var D = {1..1};
  var A: [D] R = [i in D] new R(i);
  var c = new C6(D, A);
  writeln(c);
}
testC6();
writeln("-");

class C7 {
  var x: R;
  proc init(arg: int) {
    this.x = new R(arg);
  }
}

proc testC7() {
  writeln("testC7");
  var A : [1..1] borrowed C = [new C(1)];
  writeln(A);
}
testC7();
writeln("-");


record R1 {
  var A: [1..1] R = [i in 1..1] new R(1);
}

proc testR1() {
  writeln("testR1");
  var c = new R1();
  writeln(c);
}
testR1();
writeln("-");

record R2 {
  var A: [1..1] R;
  proc init() {
    this.A = [i in 1..1] new R(1);
  }
}

proc testR2() {
  writeln("testR2");
  var c = new R2();
  writeln(c);
}
testR2();
writeln("-");

record R3 {
  var A: [1..1] R;
  proc init() {
    this.A = makeA();
  }
}

proc testR3() {
  writeln("testR3");
  var c = new R3();
  writeln(c);
}
testR3();
writeln("-");

class R4 {
  var A: [1..1] R;
}

proc testR4() {
  writeln("testR4");
  var c = new R4(makeA());
  writeln(c);
}
testR4();
writeln("-");

record R5 {
  var D;
  var A: [D] R;
}

proc testR5() {
  writeln("testR5");
  var D = {1..1};
  var A: [D] R = [i in D] new R(i);
  var c = new R5(D, A);
  writeln(c);
}
testR5();
writeln("-");

record R6 {
  var D;
  var A: [D] R;
  proc init(in D, in A: [D] R) {
    this.D = D;
    this.A = A;
  }
}

proc testR6() {
  writeln("testR6");
  var D = {1..1};
  var A: [D] R = [i in D] new R(i);
  var c = new R6(D, A);
  writeln(c);
}
testR6();
writeln("-");


printInitDeinit = false;
