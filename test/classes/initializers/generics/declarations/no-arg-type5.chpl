// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with no arguments.  Basically, the type is not really
// generic.

// This handles the case where we reuse the type instantiation, but reverses the
// order of calls
class Foo {
  type t;

  proc init() {
    t = int;
  }
}

var foo3 = new unmanaged Foo();
var foo: unmanaged Foo(int)?; // We can create an instantiation with t = int
var foo2: unmanaged Foo(int)?;
var bb = foo3.borrow();
writeln(foo.type == foo2.type);
writeln((foo.type: borrowed class) == bb.type);
writeln(foo.type:string);
writeln(foo2.type: string);
writeln(foo3.type: string);
delete foo3;
