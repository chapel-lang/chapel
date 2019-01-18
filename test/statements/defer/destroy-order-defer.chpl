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
  defer { writeln("defer post 1"); }
  var two = new R(2);
  defer { writeln("defer post 2"); }

  if check {
    defer { writeln("defer pre 3"); }
    var three = new R(3);
    defer { writeln("defer post 3"); }
    var four = new R(4);
    defer { writeln("defer post 4"); }
  }
}

foo();
