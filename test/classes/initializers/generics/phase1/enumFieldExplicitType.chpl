enum Color {red, blue, green};

class Foo {
  var a;
  var b;

  proc init(val: Color) {
    a = val;
    b = Color.green;
    super.init();
  }
}

var foo = new Foo(Color.blue);
writeln(foo);
delete foo;
