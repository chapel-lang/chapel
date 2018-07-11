enum Color {red, blue, green};
use Color;

class Foo {
  var a: Color;
  var b: Color = green;
  var c = red;

  var d: Color; // omit
  var e: Color = green; // omit
  var f = red; // omit

  var g: Color;
  var h: Color = green;
  var i = red;

  proc init(val: Color) {
    a = val;
    b = val;
    c = val;

    g = blue;
    h = red;
    i = green;
  }
}

var foo = new borrowed Foo(blue);
writeln(foo);
