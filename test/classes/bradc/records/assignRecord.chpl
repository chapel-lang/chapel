class myclass {
  var a: integer;
  var b: float;
}

record myrecord {
  var c: myclass;
}

function main() {
  var x: myrecord;
  var y: myrecord;

  x.c = myclass();
  x.c.a = 1;
  x.c.b = 2.3;

  y.c = myclass();
  y.c.a = 4;
  y.c.b = 5.6;

  writeln("x is: ", x);
  writeln("y is: ", y);

  x = y;
  x.c.a = 7;
  x.c.b = 8.9;

  writeln("x is: ", x);
  writeln("y is: ", y);
}
