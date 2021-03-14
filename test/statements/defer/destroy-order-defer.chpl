record R {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

config const check = true;

proc foo() {
  defer { writeln("defer pre 1"); }
  var one = new R(1);
  ref rone = one;
  defer { writeln("defer post 1"); }
  var two = new R(2);
  ref rtwo = two;
  defer { writeln("defer post 2"); }

  if check {
    defer { writeln("defer pre 3"); }
    var three = new R(3);
    ref rthree = three;
    defer { writeln("defer post 3"); }
    var four = new R(4);
    ref rfour = four;
    defer { writeln("defer post 4"); }
  }
}

foo();
