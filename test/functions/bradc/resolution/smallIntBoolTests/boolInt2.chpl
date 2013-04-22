//
// This shows that some int(8)/bool ops that used to be unambiguous when
// bools chose the smallest integer size are now ambiguous and demonstrates
// that this is consistent with what we've been doing with enums.
//
proc foo(x: int(?w), y: int(w)) {
  writeln("In int foo");
}

proc foo(x: uint(?w), y: uint(w)) {
  writeln("In uint foo");
}

proc foo(x: int(64), y: uint(64)) {
  writeln("In int/uint foo");
}

proc foo(x: uint(64), y: int(64)) {
  writeln("In uint/int foo");
}

enum mybool {
  myfalse=0, mytrue=1
}

var i: int(8);
var ui:uint(8);
var b: bool;
var myb: mybool;

foo(myb, i);
foo(myb, ui);
