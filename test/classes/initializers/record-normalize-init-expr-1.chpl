var a : int  = 50;
var b : real = 60.0;

record MyRec {
  var x  : int    = 10;
  var y  : real   = foo();
  var s0 : SubRec = new SubRec(a, b);
  var s1 : SubRec = new SubRec(x, y);

  proc init() {
    writeln('MyRec.init()');
  }
}

proc foo() : int {
  return 20;
}

record SubRec {
  var a : int  = 1;
  var b : real = 2.0;

  proc init(_a : int, _b : real) {
    writeln('SubRec.init(', _a, ', ', _b, ')');
    a = _a;
    b = _b;
  }
}

proc main() {
  var r : MyRec;

  writeln('r: ', r);
}