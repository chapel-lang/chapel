use BlockDist, CyclicDist, BlockCycDist, ReplicatedDist, StencilDist;

enum DistType { default, block, cyclic, blockcyclic, replicated, stencil };
config param distType: DistType = DistType.block;

config param makeAHidesInit = true;

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


proc makeInitialArray() {
  if distType == DistType.default {
    var ret: [1..1] int;
    return ret;
  } else if distType == DistType.block {
    return newBlockArr(1..1, int);
  } else if distType == DistType.cyclic {
    return newCyclicArr(1..1, int);
  } else if distType == DistType.blockcyclic {
    var D = {1..1} dmapped BlockCyclic(startIdx=(1,), (3,));
    var ret: [D] int;
    return ret;
  } else if distType == DistType.replicated {
    var D = {1..1} dmapped Replicated();
    var ret: [D] int;
    return ret;
  } else if distType == DistType.stencil {
    var D = {1..1} dmapped Stencil(rank=1, boundingBox={1..1});
    var ret: [D] int;
    return ret;
  } else {
    compilationError("unknown case");
  }
}

var A = makeInitialArray();
writeln("initial array is ", A);
writeln("-");

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
  var B:[A.domain] int = 1..1;
  writeln(B);
}
A = 0;
test4();

proc test5() {
  writeln("test5");
  var B:[A.domain] int = [i in 1..1] i;
  writeln(B);
}
A = 0;
test5();

proc test6() {
  writeln("test6");
  var B:[A.domain] int = {1..1};
  writeln(B);
}
A = 0;
test6();

proc test7() {
  writeln("test7");
  var B:[A.domain] int = 1;
  writeln(B);
}
A = 0;
test7();

proc test8() {
  writeln("test8");
  var B:[A.domain] int = (1,);
  writeln(B);
}
A = 0;
test8();

proc testrDefault() {
  writeln("testrDefault");
  var B:[A.domain] R;
  writeln(B);
}
testrDefault();
writeln("-");

proc makeA() {
  var savePrint = printInitDeinit;
  if makeAHidesInit {
    printInitDeinit = false;
  } else if printInitDeinit {
    writeln("in makeA");
  }

  var ret: [A.domain] R;
  {
    ret[1] = new R(1);
    printInitDeinit = savePrint;
  }
  if printInitDeinit {
    if makeAHidesInit {
      writeln("init 1 1");
    } else {
      writeln("leaving makeA");
    }
  }
  return ret;
}

printInitDeinit = false;
var globalA = makeA();
printInitDeinit = true;
writeln("globalA is ", globalA);
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
  var B:[A.domain] R = globalA;
  writeln(B);
}
testr1();
writeln("-");

proc testr2() {
  writeln("testr2");
  var B:[A.domain] R = makeA();
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
  var B:[A.domain] R = makeTuple();
  writeln(B);
}
testr3();
writeln("-");

proc testr4() {
  writeln("testr4");
  // don't print init/deinit for this test because
  // it varies depending on the number of tasks
  // and the init/deinits can overlap
  var savePrintInitDeinit = printInitDeinit;
  printInitDeinit = false;

  {
    var B:[A.domain] R = new R(1);
    writeln(B);
  }

  printInitDeinit = savePrintInitDeinit;
}
testr4();
writeln("-");

proc testr5() {
  writeln("testr5");
  var B:[A.domain] R = [i in A.domain] new R(1);
  writeln(B);
}
testr5();
writeln("-");

proc testr6() {
  writeln("testr6 (copy required)");
  var B:[A.domain] R = globalA[A.domain];
  writeln(B);
}
testr6();
writeln("-");

class C1 {
  var AA: [A.domain] R = [i in A.domain] new R(1);
}

proc testC1() {
  writeln("testC1");
  var c = new C1();
  writeln(c);
}
testC1();
writeln("-");

class C2 {
  var AA: [A.domain] R;
  proc init() {
    this.AA = [i in A.domain] new R(1);
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
  var AA: [A.domain] R;
  proc init() {
    this.AA = makeA();
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
  var AA: [A.domain] R;
}

proc testC4() {
  writeln("testC4");
  var c = new C4(makeA());
  writeln(c);
}
testC4();
writeln("-");



record R1 {
  var AA: [A.domain] R = [i in A.domain] new R(1);
}

proc testR1() {
  writeln("testR1");
  var c = new R1();
  writeln(c);
}
testR1();
writeln("-");

record R2 {
  var AA: [A.domain] R;
  proc init() {
    this.AA = [i in A.domain] new R(1);
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
  var AA: [A.domain] R;
  proc init() {
    this.AA = makeA();
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
  var AA: [A.domain] R;
}

proc testR4() {
  writeln("testR4");
  var c = new R4(makeA());
  writeln(c);
}
testR4();
writeln("-");

printInitDeinit = false;
