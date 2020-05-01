record R {
  proc postinit() {
    writeln("init");
  }
  proc deinit() {
    writeln("deinit");
  }
}

proc acceptsIn(in arg) {
}

proc acceptsOut(out arg) {
}

proc makeR() {
  return new R();
}

proc acceptsInAndReturnsNew(in arg) {
  return makeR();
}

proc acceptsOutAndReturnsNew(out arg) {
  return makeR();
}

proc acceptTwoAndReturnNew(a, b) {
  return makeR();
}

proc acceptOutAndTwo(out arg, a, b) {
}

proc testIn1() {
  writeln("testIn1");
  var xx = new R();
  acceptsIn(xx); // note: copy is elided
  writeln("end");
}
testIn1();

proc testIn2() {
  writeln("testIn2");
  var xx = new R();
  acceptsIn(xx);
  xx; // preventing copy elision
  writeln("end");
}
testIn2();

proc testIn3() {
  writeln("testIn3");
  var xx = new R();
  const ref reffy = acceptsInAndReturnsNew(xx);
  xx; // preventing copy elision
  writeln("end");
}
testIn3();

proc testIn4() {
  writeln("testIn4");
  var xx = new R();
  acceptsInAndReturnsNew(xx);
  xx; // preventing copy elision
  writeln("end");
}
testIn4();

proc testOut1() {
  writeln("testOut1");
  var xx:R;
  acceptsOut(xx); // note: split init
  writeln("end");
}
testOut1();

proc testOut2() {
  writeln("testOut2");
  var xx:R;
  xx; // preventing split-init
  acceptsOut(xx);
  writeln("end");
}
testOut2();

proc testOut3() {
  writeln("testOut3");
  var xx:R;
  xx; // preventing split-init
  const ref reffy = acceptsOutAndReturnsNew(xx);
  writeln("end");
}
testOut3();

proc testOut4() {
  writeln("testOut4");
  var xx:R;
  xx; // preventing split-init
  acceptsOutAndReturnsNew(xx);
  writeln("end");
}
testOut4();

proc testOut5() {
  writeln("testOut5");
  var x: R;
  acceptOutAndTwo(x, makeR(), makeR());
  writeln("end");
}
testOut5();

proc testOut6() {
  writeln("testOut6");
  var xx:R;
  acceptsOutAndReturnsNew(xx); // split inits xx
  writeln("end");
}
testOut6();

proc testRet1() {
  writeln("testRet1");
  var xx:R;
  xx = makeR(); // split-init
  writeln("end");
}
testRet1();

proc testRet2() {
  writeln("testRet2");
  var xx:R;
  xx; // preventing split-init
  xx = makeR();
  writeln("end");
}
testRet2();

proc testRet3() {
  writeln("testRet3");
  makeR();
  writeln("end");
}
testRet3();

proc testRet4() {
  writeln("testRet4");
  const ref reffy = makeR();
  writeln("end");
}
testRet4();

proc testRet5() {
  writeln("testRet5");
  acceptTwoAndReturnNew(acceptTwoAndReturnNew(makeR(), makeR()),
                        acceptTwoAndReturnNew(makeR(), makeR()));
  writeln("end");
}
testRet5();

proc testRet6() {
  writeln("testRet6");
  var x = acceptTwoAndReturnNew(acceptTwoAndReturnNew(makeR(), makeR()),
                                acceptTwoAndReturnNew(makeR(), makeR()));
  writeln("end");
}
testRet6();

proc testRet7() {
  writeln("testRet7");
  var x: R;
  x = acceptTwoAndReturnNew(makeR(), makeR());
  writeln("end");
}
testRet7();

proc testLit1() {
  writeln("testLit1");
  var x = acceptTwoAndReturnNew([new R()], [makeR(), makeR()]);
  writeln("end");
}
testLit1();

proc testLit2() {
  writeln("testLit2");
  acceptTwoAndReturnNew([new R()], [makeR(), makeR()]);
  writeln("end");
}
testLit2();

proc testLit3() {
  writeln("testLit3");
  var x = acceptTwoAndReturnNew((new R(),) , (makeR(), makeR()));
  writeln("end");
}
testLit3();

proc testLit4() {
  writeln("testLit4");
  acceptTwoAndReturnNew((new R(),), (makeR(), makeR()));
  writeln("end");
}
testLit4();

proc testLit5() {
  writeln("testLit5");
  var A:[1..2] R = [new R(), makeR()];
  writeln("end");
}
testLit5();

proc testLit6() {
  writeln("testLit6");
  var A:[1..2] R;
  A = [new R(), makeR()];
  writeln("end");
}
testLit6();

proc testLit7() {
  writeln("testLit7");
  var A:[1..2] R;
  A; // preventing split-init
  A = [new R(), makeR()];
  writeln("end");
}
testLit7();

// test global with out intent
writeln("testGlobalOut");
var globalR:R;
acceptsOut(globalR); // should split init globalR
writeln("end");
