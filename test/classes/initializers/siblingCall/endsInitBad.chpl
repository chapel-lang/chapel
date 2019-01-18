class Foo {
  var f1: int;
  var f2: bool;

  proc init(f1Val: int) {
    f2 = f1Val > 10; // field initialization not allowed with call to sibling
    this.init(f1Val, f2);
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
