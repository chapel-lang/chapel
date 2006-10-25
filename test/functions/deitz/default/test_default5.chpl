def foo(x : int = 4) : real return x / 2.0;

def foo(x : real) {
  writeln(x);
}

foo(foo(16));

foo(foo());
