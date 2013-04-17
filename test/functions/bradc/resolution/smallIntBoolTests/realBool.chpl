//
// This pair of tests shows that bool/real combos are treated the same
// as with enums
//
enum mybool {
  myfalse=0, mytrue=1
}

proc foo(x: int(?w), y: int(w)) {
  writeln("In int foo");
  return x|y;
}

proc foo(x: uint(?w), y: uint(w)) {
  writeln("In uint foo");
  return x|y;
}

proc foo(x: real(?w), y: real(w)) {
  writeln("In real foo");
  return x|y;
}

foo(1.1, 1:bool);
