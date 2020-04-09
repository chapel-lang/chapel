// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with no arguments.  Basically, the type is not really
// generic.

class Foo {
  param p;

  proc init() {
    p = 4;
  }
}

var foo: Foo();
writeln(foo.type:string);
