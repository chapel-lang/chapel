record foo {
  var x = 0;
  proc init() where false {}
  proc init(x: int) { this.x = x; }
}

proc test() {
  var D = {0..0};
  var A: [D] foo = [new foo(0)];
  writeln(A);

  // Intentially leave new element of 'A' uninitialized, this should
  // trigger a halt from the manager because A is NDI.
  manage D.unsafeAssign({0..1}, checks=true) as mgr do;

  writeln(A);
}
test();

