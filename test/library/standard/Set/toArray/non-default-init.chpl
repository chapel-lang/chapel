use Set;

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

proc testList(type R) {
  var l = new set(R);
  var empty = l.toArray();
  writeln("empty: ", empty);

  l.add(new R(1));
  on Locales.last {
    var arr = l.toArray();
    writeln("arr: ", arr);
  }
}

testList(R1);
testList(R2);
testList(R3);

