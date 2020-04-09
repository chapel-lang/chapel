// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with no arguments.  Basically, the type is not really
// generic.

class Foo {
  param p;

  proc init() {
    p = 4;
  }
}

var foo: borrowed Foo(4)?; // We can create an instantiation with p = 4
foo = new borrowed Foo();// and we can initialize it
writeln(foo.type:string);
