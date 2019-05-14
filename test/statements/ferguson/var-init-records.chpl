record R {
  var x = 0;
  proc init() {
    writeln("R default init 0");
  }
  proc init(arg:int) {
    writeln("R int init ", arg);
    this.x = arg;
  }
  proc init=(arg:R) {
    writeln("R copy init from ", arg.x);
    this.x = arg.x;
  }
  proc deinit() {
    writeln("R deinit ", this.x);
  }
}

proc =(ref lhs:R, rhs:R) {
  writeln("R = lhs ", lhs.x, " rhs ", rhs.x);
  lhs.x = rhs.x;
}

proc makeR() {
  return new R(1);
}

proc testa() {
  writeln("a");
  var a = makeR();
}

proc testb() {
  writeln("b");
  var a = makeR();
  var b:R = a;
}

proc testc() {
  writeln("c");
  var b:R = makeR();
}

proc testd(type t) {
  writeln("d");
  var b:t = makeR();
}

proc teste() {
  writeln("e");
  var b:[1..1] R = makeR();
}

proc testf() {
  writeln("f");
  var a = makeR();
  var b:[1..1] R = a;
}

proc testg() {
  writeln("g");
  var b:[1..2] R = makeR();
}

proc testh() {
  writeln("h");
  var b:[1..1] R;
  b = makeR();
}

proc testi() {
  writeln("i");
  var a = makeR();
  var b:[1..1] R;
  b = a;
}




testa();
testb();
testc();
testd(R);
teste();
testf();
testg();
testh();
testi();
