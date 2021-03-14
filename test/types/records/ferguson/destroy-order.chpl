record R {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

config const check = true;

proc foo() {
  var one = new R(1);
  ref rone = one;
  var two = new R(2);
  ref rtwo = two;

  if check {
    var three = new R(3);
    ref rthree = three;
    var four = new R(4);
    ref rfour = four;
  }
}

foo();
