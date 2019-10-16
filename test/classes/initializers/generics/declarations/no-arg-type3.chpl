// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with no arguments.  Basically, the type is not really
// generic.

class Foo {
  type t;

  proc init() {
    t = int;
  }
}

var foo: borrowed Foo(int)?; // We can create an instantiation with t = int
foo = new borrowed Foo();// and we can initialize it
writeln(foo.type:string);
