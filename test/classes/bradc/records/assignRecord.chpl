class myclass {
  var a: int;
  var b: real;
}

record myrecord {
  var c: borrowed myclass?;
}

proc main() {
  var x: myrecord;
  var y: myrecord;

  var class1 = new myclass();
  var class2 = new myclass();

  x.c    = class1.borrow();
  x.c!.a = 1;
  x.c!.b = 2.3;

  y.c    = class2.borrow();
  y.c!.a = 4;
  y.c!.b = 5.6;

  writeln("x is: ", x);
  writeln("y is: ", y);

  x      = y;
  x.c!.a = 7;
  x.c!.b = 8.9;

  writeln("x is: ", x);
  writeln("y is: ", y);
}
