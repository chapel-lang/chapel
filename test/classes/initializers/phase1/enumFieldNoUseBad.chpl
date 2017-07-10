enum Color {red, blue, green};
// use Color; // Uncommenting this line allows blue to be accessed without
// the `Color.` prefix.

class Foo {
  var a: Color;

  proc init() {
    a = blue;
    super.init();
  }
}

var foo = new Foo();
writeln(foo);
delete foo;
