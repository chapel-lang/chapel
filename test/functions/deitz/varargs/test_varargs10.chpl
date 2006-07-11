fun foo(x : float, ys : string ...2) {
  writeln(x);
  writeln((...ys));
}

foo(2.0, "hello", "world!");
