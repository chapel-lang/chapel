record Foo {
  type idxType;
  var D: domain(1, idxType);
  proc init(type idxType, space: range) {
    this.idxType = idxType;
    this.D = space;
  }
  proc init(type idxType, size: idxType) {
    this.init(idxType, 0..#size);
  }
}
var foo = new Foo(int, 10);
writeln(foo);
