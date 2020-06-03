// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with an argument for the var field that is used to
// give the same value to the var field.
class Foo {
  var v;

  proc init(vVal) {
    v = vVal;
  }
}

var foo: borrowed Foo(int)?;
writeln(foo.type:string);
