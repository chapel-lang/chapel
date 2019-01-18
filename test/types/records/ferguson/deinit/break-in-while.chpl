record R {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

proc foo() {

  var count = 0;

  while count < 1 {
    writeln("creating 1");
    var one = new R(1);
    
    break;

    writeln("creating 2");
    var two = new R(2);

    count += 1;

  }
}

foo();
