// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with an argument for the type field that won't just
// use the argument for the field directly but will instead do something
// different depending on what that argument is.
class Foo {
  type t;

  proc init(type tVal) where (isIntegralType(tVal)) { // true for int, uint
    t = int;
  }

  proc init(type tVal) where (!isIntegralType(tVal)) { // everything else
    t = tVal;
  }
}

var foo: borrowed Foo(uint)?;
var foo2: borrowed Foo(string)?;
var foo3: borrowed Foo(int)?;
writeln(foo3.type == foo.type);
writeln(foo.type:string);
writeln(foo2.type:string);
writeln(foo3.type:string);
