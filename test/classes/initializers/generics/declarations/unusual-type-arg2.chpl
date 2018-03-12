// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with an argument for the type field that won't just
// use the argument for the field directly but will instead do something
// different depending on what that argument is.

// Variant on unusual-type-arg.chpl, but instead of relying on the initializer
// being tied to the type constructor called, demonstrates the initializer call
// will yield the same type as certain declared versions.
class Foo {
  type t;

  proc init(type tVal) where (isIntegralType(tVal)) { // true for int, uint
    t = int;
  }

  proc init(type tVal) where (!isIntegralType(tVal)) { // everything else
    t = tVal;
  }
}

var foo = new Foo(uint);
var foo2: Foo(string);
var foo3: Foo(int);
writeln(foo3.type == foo.type);
writeln(foo.type:string);
writeln(foo2.type:string);
writeln(foo3.type:string);
delete foo;
