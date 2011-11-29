class Foo {
  var x: int;

  proc Foo(y: int) {
    x = 2*y;
  }
}

var bar = new Foo(5);

writeln(bar.x);
