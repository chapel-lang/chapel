proc foo(X) {
  class Box {
    var val;
  }

  var b = new Box(X);
  writeln(b);
}

foo(10);
