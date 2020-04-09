enum Color {red, blue, green};
// use Color; // Uncommenting this line allows blue to be accessed without
// the `Color.` prefix.

class Foo {
  var a;

  proc init() {
    a = blue;
  }
}

var foo = new unmanaged Foo();
writeln(foo);
delete foo;
