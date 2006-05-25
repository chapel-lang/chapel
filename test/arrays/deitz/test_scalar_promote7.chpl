class C {
  fun foo(i : int, j : int)
    return i**2 + j**2;
}

var c = C();

writeln(c.foo((/1, 2, 3/), (/4, 5, 6/)));
