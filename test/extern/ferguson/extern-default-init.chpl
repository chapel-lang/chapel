use IO;
import SysBasic.syserr;

extern record R {
  var a,b,c,d,e,f,g,h,i,j,k: int(64);
}

proc printR(r:R) {
  writeln(r.a, r.b, r.c, r.d, r.e, r.f, r.g, r.h, r.i, r.j, r.k);
}

proc testSyserr() {
  writeln("testSyserr");
  var x:syserr;
  writeln(x:int);
}
testSyserr();

proc testMystruct() {
  writeln("testMystruct");
  var st: R;
  printR(st);
}
testMystruct();

proc testSyserrArr() {
  writeln("testSyserrArr");
  var A:[1..3] syserr;
  for x in A {
    writeln(x:int);
  }
}
testSyserrArr();

proc testMystructArr() {
  writeln("testMystructArr");
  var A:[1..3] R;
  for x in A {
    printR(x);
  }
}
testMystructArr();

writeln("global syserr");
var globSysErr: syserr;
writeln(globSysErr:int);

writeln("global mystruct");
var g: R;
printR(g);
