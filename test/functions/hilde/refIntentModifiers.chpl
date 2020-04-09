// refIntentModifiers.chpl
//
// Part of a semantic future.
// We want to be able to specify that a ref arg is write-only, read-only or
// read-write.
// The last two should be covered by "const ref" and "ref" resp., so this
// proposal just adds "out ref".

proc foo(ref x:int) {
  x = 2;
}

proc bar(const ref x:int) {
  x = 3; // Should be flagged as an error.
}

proc baz1(out ref x:int) {
  x = 4;
}

proc baz2(out ref x:int) {
  // Should be flagged as an error.
  // Assignment to x is required.
}

var x = 1;
writeln(x); // 1
foo(x);
writeln(x); // 2
// bar(x);
// writeln(x); // 3
baz1(x);
writeln(x); // 4
// baz2(x);
// writeln(x); // 4
writeln("Done.");
