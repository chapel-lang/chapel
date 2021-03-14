// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with no arguments.  Basically, the type is not really
// generic.

class Foo {
  type t;

  proc init() {
    t = int;
  }
}

var foo: Foo();
writeln(foo.type:string);
