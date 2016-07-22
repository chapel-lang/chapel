// Modified from
// test/classes/dinan/generic_constructor.chpl
class Foo {
  type x_t;
  var  x;

  proc init(type x_t, i:x_t, j:x_t) {
    x = i+j;
  }
}

var bar = new Foo(int, 5, 10);

writeln(bar.x);
