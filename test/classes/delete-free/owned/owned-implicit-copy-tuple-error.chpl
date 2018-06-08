use OwnedObject;

class Foo {
  var x: int;

  proc init(x: int) {
    this.x = x;
  }
}

const a = new Owned(new Foo(1));
const b = new Owned(new Foo(2));

var c = (a, b);
