// Exercises basic declarations of instantiations.  Here, the type creator has
// in their benevolence declared an initializer with an argument for the type
// field that is used to give the same type to the type field.
class Foo {
  type t;

  proc init(type tVal) {
    t = tVal;
  }
}

var foo: borrowed Foo(int)?;
writeln(foo.type:string);
