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

  for i in 1..3 {
    var r1 = new R(i);
    var r2 = new R(10*i);
    ref refR = r2;
    yield i;
  }
}

proc foo() {

  for i in myIter() {
    writeln(i);
  }
}

foo();
