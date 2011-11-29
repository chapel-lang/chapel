proc foo(i : int) {
  return i**2;
}

var s: [1..4] int = (1, 2, 3, 4);

writeln(foo(s));
