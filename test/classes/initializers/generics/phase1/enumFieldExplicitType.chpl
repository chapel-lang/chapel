enum Color {red, blue, green};

class Foo {
  var a;
  var b;

  proc init(val: Color) {
    a = val;
    b = Color.green;
  }
}

var foo = new unmanaged Foo(Color.blue);
writeln(foo);
delete foo;
