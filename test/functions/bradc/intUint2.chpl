proc foo(x: int, y: uint) {
  writeln("in int/uint foo");
}

proc foo(x: uint, y: int) {
  writeln("in uint/int foo");
}

foo(1,2);
foo(1:uint, 2);
foo(1, 2:uint);
