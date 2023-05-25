class C {
  var x: int;
  proc init(_x) {x = _x; writeln("init(",x,")"); }
  proc deinit() { writeln("deinit(",x,")"); }
}

proc foo() {
  writeln("entering foo");
  new owned C(1);
  writeln("exiting foo");
}

foo();
writeln("returned from foo()");
writeln();

proc bar() {
  writeln("entering bar");
  (new owned C(2)).borrow();
  writeln("exiting bar");
}

bar();
writeln("returned from bar()");
writeln();

proc baz() {
  writeln("entering baz");
  new shared C(3);
  writeln("exiting baz");
}

baz();
writeln("returned from baz()");
