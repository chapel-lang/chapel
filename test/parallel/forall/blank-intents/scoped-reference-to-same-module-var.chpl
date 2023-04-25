module foo {
  var x: int;

  writeln(foo.x);

  var A = for i in 1..10 do foo.x;
  writeln(A);
}
