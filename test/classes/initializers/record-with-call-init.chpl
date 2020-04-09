record MyRec {
  var x : int    = 10;
  var y : SubRec = foo(20, 30);

  proc init() {

  }
}


record SubRec {
  var a : int;
  var b : int;

  proc init(_a : int, _b : int) {
    a = _a;
    b = _b;
  }
}

proc foo(x : int, y : int) {
  var r = new SubRec(x, y);

  return r;
}

proc main() {
  var r : MyRec;

  writeln('r: ', r);
}
