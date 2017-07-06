record R {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

config const check = true;

proc foo() {
  var one = new R(1);
  var two = new R(2);

  if check {
    var three = new R(3);
    var four = new R(4);
  }
}

foo();
