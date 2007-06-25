class Foo {
  type x_t;
  var  x;

  def Foo(type t, i:t, j:t) {
    x_t = t;
    x   = i+j;
  }
}

var bar = Foo(int, 5, 10);

writeln(bar.x);
