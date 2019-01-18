/* Feature request: Allow type aliases to be used to instantiate objects */
class Foo {
  proc init() {
    writeln("Create a new Foo using the alias bar");
  }
}

type bar = Foo;

var myBar = new unmanaged bar();

delete myBar;
