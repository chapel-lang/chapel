// Exercises basic declarations of instantiations.  Here, the type creator has
// in their benevolence declared an initializer with an argument for the param
// field that is used to give the same value to the param field.  The
// initializer also has some side effects - we probably shouldn't see those,
// since the type isn't getting initialized, merely declared.
class Foo {
  param p;

  proc init(param pVal) {
    p = pVal;
    writeln(pVal);
  }
}

var foo: borrowed Foo(3)?;
writeln(foo.type :class :string);
