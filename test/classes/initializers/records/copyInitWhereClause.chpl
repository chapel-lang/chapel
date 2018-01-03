record Foo {
  var x: int;

  proc init(xVal) where (xVal.type != Foo) {
    x = xVal;
    super.init();
  }
}

var foo1 = new Foo(13);
writeln(foo1);
