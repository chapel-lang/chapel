module foo {
  proc foo(x: int) {
    return x * x;
  }

  var val = for i in 1..10 do foo(i);
  writeln(val);

  val = [ i in 1..10 ] foo(i);
  writeln(val);

  for i in 1..10 do writeln(foo(i));
}
