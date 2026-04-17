use List;

record R1 {
  var x: int;
  proc init(x: int) {
    this.x = x;
  }
}
record R2 {
  var x: int;
  proc init(x: int) {
    this.x = x;
  }
  proc init() {
    this.x = -1;
  }
}
record R3 {
  var arr: [1..10] int;
  proc init(x: int) {
    this.arr = x;
  }
}

proc test(type R) {
  var l = new list(R);
  var empty = l.toArray();
  writeln("empty: ", empty);

  l.pushBack(new R(1));
  l.pushBack(new R(2));
  l.pushBack(new R(3));
  on Locales.last {
    var arr = l.toArray();
    writeln("arr: ", arr);
  }
}

test(R1);
test(R2);
test(R3);

