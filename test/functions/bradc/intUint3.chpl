def foo(x: int, y: uint) {
  writeln("in int/uint foo");
}

def foo(x: uint, y: int) {
  writeln("in uint/int foo");
}

foo(1:uint, 2);
foo(1, 2:uint);
foo(1:uint, 2:uint);
