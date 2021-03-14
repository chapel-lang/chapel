record R {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

proc foo() {

  var count = 0;

  while count < 2 {
    writeln("creating 1");
    defer { writeln("defer pre 1"); }
    var one = new R(1);
    ref rone = one;
    defer { writeln("defer post 1"); }
    
    return;

    writeln("creating 2");
    var two = new R(2);
    ref rtwo = two;
    defer { writeln("defer post 2"); }

    count += 1;

  }
}

foo();
