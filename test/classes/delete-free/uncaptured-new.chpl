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
writeln("returned from foo()\n");

proc bar() {
  writeln("entering bar");
  new borrowed C(2);
  writeln("exiting bar");
}

bar();
writeln("returned from bar()\n");

proc baz() {
  writeln("entering baz");
  new shared C(3);
  writeln("exiting baz");
}

baz();
writeln("returned from baz()\n");


proc qux() {
  writeln("entering qux");
  new C(4);
  writeln("exiting qux");
}

qux();
writeln("returned from qux()");
