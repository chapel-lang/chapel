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
var ownFoo3 = new owned Foo();
var foo3 = ownFoo3.borrow();
writeln(foo.type == foo2.type);
writeln((foo.type :class) == foo3.type);
writeln(foo.type:string);
writeln(foo2.type: string);
writeln(foo3.type: string);
