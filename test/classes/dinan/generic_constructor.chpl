class Foo {
  type x_t;
  var  x;

  proc Foo(type x_t, i:x_t, j:x_t) {
    x = i+j;
  }
}

var bar = new owned Foo(int, 5, 10);

writeln(bar.x);
