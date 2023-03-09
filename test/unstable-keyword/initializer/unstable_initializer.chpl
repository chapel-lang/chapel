record Foo {
  var v1 = 3;

  @unstable("this initializer is unstable") proc init() {
    this.init(7);
  }

  proc init(x: int) {
    v1 = x;
    this.complete();
  }
}

proc main() {
  var f1 = new Foo(5);
  writeln(f1);
  var f2 = new Foo();
  writeln(f2);
}