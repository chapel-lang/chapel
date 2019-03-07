class Foo {
  type x_t;
  var  x;

  proc init(type x_t, i:x_t, j:x_t) {
    this.x_t = x_t;
    x = i+j;
  }
}

var bar = new unmanaged Foo(int, 5, 10);

writeln(bar.x);
delete bar;
