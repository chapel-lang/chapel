// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with no arguments.  Basically, the type is not really
// generic.

// This handles the case where we reuse the type instantiation
class Foo {
  type t;

  proc init() {
    t = int;
  }
}

var foo: borrowed Foo(int)?; // We can create an instantiation with t = int
var foo2: borrowed Foo(int)?;
var foo3 = new borrowed Foo();
writeln(foo.type == foo2.type);
writeln((foo.type :class) == foo3.type);
writeln(foo.type:string);
writeln(foo2.type: string);
writeln(foo3.type: string);
