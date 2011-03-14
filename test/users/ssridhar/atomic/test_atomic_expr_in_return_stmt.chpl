var x: int(64);

proc main {
  var y: int(64);
  coforall i in 1..10 do
    y = foo;
  writeln("x = ", x);
  writeln("y = ", y);
}

proc foo: int(64) {
  return atomic bar;
}

proc bar: int(64) {
  x += 1;
  return x;
}
