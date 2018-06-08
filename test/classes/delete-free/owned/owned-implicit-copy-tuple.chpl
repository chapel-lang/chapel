use OwnedObject;

class Foo {
  var x: int;

  proc init(x: int) {
    this.x = x;
  }
}

var a = new Owned(new Foo(1));
var b = new Owned(new Foo(2));

var c = (a, b);

writeln(c);
