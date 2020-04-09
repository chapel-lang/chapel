proc foo(i : int) : int {
  return i * 2;
}

writeln(foo(foo(foo(foo(foo(2))))));
