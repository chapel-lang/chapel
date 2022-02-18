record foo {
  var x = 0;
  proc init() where false {}
  proc init(x: int) { this.x = x; }
}

proc test() {
  var D = {0..0};
  var A: [D] foo = [new foo(0)];
  writeln(A);

  manage D.unsafeAssign({0..1}, checks=true) as mgr do
    mgr.initialize(A, 1, new foo(1));

  writeln(A);
}
test();

