enum Color {red, blue, green};
use Color;

class Foo {
  var a;
  var b;

  proc init(val: Color) {
    a = val;
    b = green;
  }
}

var foo = new unmanaged Foo(blue);
writeln(foo);
delete foo;
