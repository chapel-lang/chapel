use Heap;

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
  operator <(x: R3, y: R3) do return x.arr[1] < y.arr[1];
  operator >(x: R3, y: R3) do return x.arr[1] > y.arr[1];
}

proc testList(type R) {
  var l = new heap(R);
  var empty = l.toArray();
  writeln("empty: ", empty);

  l.push(new R(1));
  l.push(new R(2));
  l.push(new R(3));
  on Locales.last {
    var arr = l.toArray();
    writeln("arr: ", arr);
  }
}

testList(R1);
testList(R2);
testList(R3);

