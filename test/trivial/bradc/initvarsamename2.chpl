proc foo(x: int) {
  var x = x + 1;

  writeln("x is: ", x);
}

for i in 1..4 {
  foo(2);
}
