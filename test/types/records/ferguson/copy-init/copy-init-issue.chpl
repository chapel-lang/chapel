record MyRecord {
  var y:int;
  proc init(x:int) {
    y = x;
  }
  proc init=(from:MyRecord) {
    y = from.y;
    writeln("copy init");
  }
  proc deinit() {
    writeln("deinit ", y);
  }
}

proc foo(i:int, j = 4):MyRecord  {
  return new MyRecord(i);
}

proc test() {
  var x = foo(1);
  writeln(x);
  var y = foo(2);
  ref refY = y;
  writeln(y);
  var z = foo(3);
  writeln(z);
}

test();
