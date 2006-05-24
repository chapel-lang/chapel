fun foo(i : int) {
  return i**2;
}

var s : seq of int = (/ 1, 2, 3, 4 /);

writeln(foo(s));
