class Foo {
  var x: int;

  def Foo(y: int) {
    x = 2*y;
  }
}

var bar = new Foo(5);

writeln(bar.x);
