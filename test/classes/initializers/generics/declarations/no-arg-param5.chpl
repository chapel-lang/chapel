// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with no arguments.  Basically, the type is not really
// generic.

// This handles the case where we reuse the type instantiation, but reverses the
// order of calls
class Foo {
  param p;

  proc init() {
    p = 4;
  }
}

var foo3 = new borrowed Foo();
var foo: borrowed Foo(4)?; // We can create an instantiation with p = 4
var foo2: borrowed Foo(4)?;
writeln(foo.type == foo2.type);
writeln(foo.type == foo3.type?);
writeln(foo.type:string);
writeln(foo2.type: string);
writeln(foo3.type: string);
