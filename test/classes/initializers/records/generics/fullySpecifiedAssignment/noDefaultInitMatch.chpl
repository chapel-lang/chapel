record Foo {
  var x;

  proc init() {
    x = 10;
    super.init();
  }
}

var x: Foo(int) = new Foo();
writeln(x);
delete x;
