proc foo(x: int, y: int) {
  writeln("in int foo");
}

proc foo(x: uint, y: uint) {
  writeln("in uint foo");
}

foo(1,2);
foo(1:uint, 2);
foo(1:uint, 2:uint);
