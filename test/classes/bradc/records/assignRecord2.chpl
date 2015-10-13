use List;

class myclass {
  var a: int;
  var b: real;
}

record myrecord {
  var a: list(int);
}

proc main() {
  var x: myrecord;
  var y: myrecord;

  x.a = makeList(1, 2, 3);
  y.a = makeList(4, 5, 6);

  writeln("x is: ", x);
  writeln("y is: ", y);

  x = y;
  y.a = makeList(7, 8, 9);

  writeln("x is: ", x);
  writeln("y is: ", y);
}
