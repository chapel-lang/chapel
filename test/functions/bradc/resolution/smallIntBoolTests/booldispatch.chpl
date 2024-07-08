proc foo(x: int(8)) {
  writeln("In 8-bit version of foo");
}

proc foo(x: int(64)) {
  writeln("In 64-bit version of foo");
}

proc bar(x: int(8)) {
  writeln("In 8-bit version of bar");
}

proc bar(x: int(32)) {
  writeln("In 32-bit version of bar");
}

var b: bool = true;

//
// Since foo() supports a 64-bit (default) int version, bool should choose to dispatch to that
// first, regardless of the bool's size
//
foo(b);

//
// When there is no 64-bit int to dispatch to, bools will dispatch to the smallest int size
// available.  This mimics what happens with enums.
//
bar(b);
