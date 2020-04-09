record R {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

config const doBreak = 0;

proc foo() {

  var count = 0;

  do {
    writeln("creating 1");
    defer { writeln("defer pre 1"); }
    var one = new R(1);
    ref rone = one;
    defer { writeln("defer post 1"); }
    
    if doBreak == 1 then
      break;

    writeln("creating 2");
    var two = new R(2);
    ref rtwo = two;
    defer { writeln("defer post 2"); }

    if doBreak == 2 then
      break;

    count += 1;

  } while count < 2;
}

foo();
