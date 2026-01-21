use Map;

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
  var l = new map(R, string);
  var empty = l.toArray();
  writeln("empty: ", empty);

  l.add(new R(1), "hi");

  var arr = l.toArray();
  writeln("arr: ", arr);
}

test(R1);
test(R2);
test(R3);

