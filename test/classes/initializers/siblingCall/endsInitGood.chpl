class Foo {
  var f1: int;
  var f2: bool;

  proc init(f1Val: int) {
    var tmpf2 = f1Val > 10;
    this.init(f1Val, tmpf2);
  }

  proc init(f1Val: int, unmod: bool) {
    f1 = f1Val;
    f2 = unmod;
  }
}

proc main() {
  var c: shared Foo = new shared Foo(8);
  writeln(c);
}
