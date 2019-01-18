record R {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

proc foo() {

  var count = 0;

  do {
    writeln("creating 1");
    defer { writeln("defer pre 1"); }
    var one = new R(1);
    defer { writeln("defer post 1"); }
    
    break;

    writeln("creating 2");
    var two = new R(2);
    defer { writeln("defer post 2"); }

    count += 1;

  } while count < 2;
}

foo();
