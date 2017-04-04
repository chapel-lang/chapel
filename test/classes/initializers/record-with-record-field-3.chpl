// The record MyRec has two fields of record type.
//   The 1st field has a default initializer that is a new expr
//   The 2nd field needs a copy constructor
// Ensure MyRec is initialized correctly

record MyRec {
  var x : int    = 10;
  var y : SubRec = new SubRec(20, 30);
  var z : SubRec = y;

  proc init() {
    // Note this is phase 2.  SubRec.init() runs first.
    writeln('MyRec.init()');
  }
}

record SubRec {
  var a : int = 1;
  var b : int = 2;

  proc init(_a : int, _b : int) {
    writeln('SubRec.init(', _a, ', ', _b, ')');
    a = _a;
    b = _b;
  }

  proc init(other : SubRec) {
    a = other.a;
    b = other.b;

    super.init();
  }
}

proc main() {
  var r : MyRec;

  writeln();
  writeln('r: ', r);
}