def foo(x: int, y: int) {
  writeln("in int foo");
}

def foo(x: uint, y: uint) {
  writeln("in uint foo");
}

foo(1,2);
foo(1u, 2);
foo(1u, 2u);
