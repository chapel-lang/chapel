record Foo {
  var x;

  proc init() {
    x = 10;
  }
}

var x: Foo(int) = new Foo();
writeln(x);
