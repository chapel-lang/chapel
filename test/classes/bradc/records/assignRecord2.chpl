class myclass {
  var a: int;
  var b: real;
}

record myrecord {
  var a: seq(int);
}

def main() {
  var x: myrecord;
  var y: myrecord;

  x.a = _seq(1, 2, 3);
  y.a = _seq(4, 5, 6);

  writeln("x is: ", x);
  writeln("y is: ", y);

  x = y;
  y.a = _seq(7, 8, 9);

  writeln("x is: ", x);
  writeln("y is: ", y);
}
