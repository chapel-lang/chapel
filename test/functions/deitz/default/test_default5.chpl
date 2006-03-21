fun foo(x : int = 4) : float return x / 2.0;

fun foo(x : float) {
  writeln(x);
}

foo(foo(16));

foo(foo());
