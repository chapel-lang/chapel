use IO;

// test functions end with "EOB" (a key variable expires end-of-block)
// or "LM" (a key variable expires at last mention)
// or neither (if it's more complex)

record R { proc deinit() { } }
class C { var x: int; }

proc setit(ref lhs, rhs) lifetime lhs < rhs {
  lhs = rhs;
}

proc ownedExample2EOB() {
  writeln("ownedExample2EOB");
  var b: borrowed C?;
  b; // no split init please
  b = (new owned C(1)).borrow();
  // point 1
  writeln(b);
  writeln();
  // point 2
}
ownedExample2EOB();

proc ownedExample3EOB() {
  writeln("ownedExample3EOB");
  var b: borrowed C?;
  setit(b, (new owned C(1)).borrow());
  // point 1
  writeln(b);
  writeln();
  // point 2
}
ownedExample3EOB();
