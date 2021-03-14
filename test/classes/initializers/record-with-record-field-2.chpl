// The record MyRec has a field of record type.
// That field has a default initializer that is a new expr
// Ensure MyRec is initialized correctly

record MyRec {
  var x : int    = 10;
  var y : SubRec = new SubRec(20, 30);

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
}

proc main() {
  var r : MyRec;

  writeln();
  writeln('r: ', r);
}