

class Foo {
  var x: int;

  proc init(x: int) {
    this.x = x;
  }
}

const a = new owned Foo(1);
const b = new owned Foo(2);

var c = (a, b);
