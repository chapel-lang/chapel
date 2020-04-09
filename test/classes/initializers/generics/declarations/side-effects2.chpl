// Exercises basic declarations of instantiations.  Here, the type creator has
// in their benevolence declared an initializer with an argument for the param
// field that is used to give the same value to the param field.  The
// initializer also has some side effects - we probably shouldn't see those,
// since the type isn't getting initialized, merely declared.

// Uses an explicit type function
class Foo {
  param p;

  proc init(param pVal) {
    p = pVal;
    writeln(pVal);
  }
}

proc FooX(param p) type {
  var localvar = new borrowed Foo(p);
  return localvar.type;
}

var foo: FooX(3)?;
writeln(foo.type :class :string);
