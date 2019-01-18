// This test exercises using a type alias as the type of a field we omit the
// initialization of.
type tAlias = bool;

class Foo {
  var x: tAlias;

  proc init() {
  }
}

var foo = new unmanaged Foo();
writeln(foo);
delete foo;
