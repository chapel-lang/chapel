record MyRec {
  var x : int    = 10;
  var y : SubRec = foo(20, 30);

  proc init() {

  }
}


// Note: This does not have an initializer
record SubRec {
  var a : int;
  var b : int;

  proc SubRec(_a : int, _b : int) {
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
