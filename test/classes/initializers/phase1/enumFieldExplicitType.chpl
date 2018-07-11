enum Color {red, blue, green};

class Foo {
  var a: Color;
  var b: Color = Color.green;
  var c = Color.red;

  var d: Color; // omit
  var e: Color = Color.green; // omit
  var f = Color.red; // omit

  var g: Color;
  var h: Color = Color.green;
  var i = Color.red;

  proc init(val: Color) {
    a = val;
    b = val;
    c = val;

    g = Color.blue;
    h = Color.red;
    i = Color.green;
  }
}

var foo = new borrowed Foo(Color.blue);
writeln(foo);
