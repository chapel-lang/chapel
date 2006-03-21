fun foo(i : int) : float {
  return i * 3.0;
}

fun bar() {
  writeln(foo(1));
  writeln(foo(2));
  writeln(foo(3));

  var d : domain(1) = [1..3];
  var foo : [d] float;

  foo(1) = 4.0;
  foo(2) = 2.0;
  foo(3) = 9.0;

  writeln(foo(1));
  writeln(foo(2));
  writeln(foo(3));
}

bar();
