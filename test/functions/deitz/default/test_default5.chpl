def foo(x : int = 4) : float return x / 2.0;

def foo(x : float) {
  writeln(x);
}

foo(foo(16));

foo(foo());
