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

var b8: bool(8) = true;
var b16: bool(16) = true;
var b32: bool(32) = true;
var b64: bool(64) = true;
var b: bool = true;

//
// Since foo() supports a 64-bit (default) int version, bool should choose to dispatch to that
// first, regardless of the bool's size
//
foo(b8);
foo(b16);
foo(b32);
foo(b64);
foo(b);

//
// When there is no 64-bit int to dispatch to, bools will dispatch to the smallest int size
// available.  This mimics what happens with enums.
//
bar(b8);
bar(b16);
bar(b32);
bar(b64);
bar(b);
