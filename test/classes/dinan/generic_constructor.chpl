class Foo {
  type x_t;
  var  x;

  def Foo(type x_t, i:t, j:t) {
    x = i+j;
  }
}

var bar = new Foo(int, 5, 10);

writeln(bar.x);
