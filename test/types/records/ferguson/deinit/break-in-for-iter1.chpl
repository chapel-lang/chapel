record R {
  var x: int;
  proc init(x:int) {
    this.x = x;
    writeln("Creating ", x);
  }
  proc deinit() {
    writeln("Destroying ", x);
  }
}

iter myIter() {
  var r = new R(0);

  for i in 1..10 {
    var r1 = new R(i);
    var r2 = new R(10*i);
    ref refr = r2;
    yield i;
  }
}

config const breakOnIter = 2;

proc foo() {

  for i in myIter() {
    writeln(i);
    if i == breakOnIter then
      break;
  }
}

foo();
