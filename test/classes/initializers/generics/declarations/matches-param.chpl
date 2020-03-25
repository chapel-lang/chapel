// Exercises basic declarations of instantiations.  Here, the type creator has
// in their benevolence declared an initializer with an argument for the param
// field that is used to give the same value to the param field.
class Foo {
  param p;

  proc init(param pVal) {
    p = pVal;
  }
}

var foo: borrowed Foo(3)?;
writeln(foo.type:string);
