// This test shows that the type of the 'out' actual
// does not need to match the type of its formal.

/////////// for classes ///////////

class C   { var x = 555; }
class D:C { var y = 666; }

proc testClasses(out i: borrowed D) { i = new D(); }

var c: borrowed C = new borrowed C();
testClasses(c);
writeln(c);

/////////// for integers ///////////

proc testInts(out iFormal: int(32)) { iFormal = 5; }

var i32: int(32);
testInts(i32);
writeln(i32);

var i64: int(64);
testInts(i64);
writeln(i64);
