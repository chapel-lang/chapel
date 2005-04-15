class myclass {
  var a: integer;
  var b: float;
}

record myrecord {
  var a: seq(integer);
}

function main() {
  var x: myrecord;
  var y: myrecord;

  x.a = (/1, 2, 3/);
  y.a = (/4, 5, 6/);

  writeln("x is: ", x);
  writeln("y is: ", y);

  x = y;
  y.a = (/7, 8, 9/);

  writeln("x is: ", x);
  writeln("y is: ", y);
}
