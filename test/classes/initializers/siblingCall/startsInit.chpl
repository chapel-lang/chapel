class Foo {
  var f1: int;
  var f2: bool;

  proc init(f1Val: int) {
    this.init(f1Val, f1Val > 10);
    if (!f2) {
      f1 += 2;
    }
  }

  proc init(f1Val: int, unmod: bool) {
    f1 = f1Val;
    f2 = unmod;
  }
}

proc main() {
  var c: owned Foo = new owned Foo(8);
  writeln(c);
}
