// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with an argument for the generic var field that
// won't just use the argument for the field directly but will instead do
// something different depending on what that argument is.

// Variant on unusual-var-arg.chpl, but instead of relying on the initializer
// being tied to the type constructor called, demonstrates the initializer call
// will yield the same type as certain declared versions.
class Foo {
  var v;

  proc init(vVal) where (isIntegralType(vVal.type)) {
    v = 10;
  }

  proc init(vVal) where (!isIntegralType(vVal.type)) {
    v = vVal;
  }
}

var foo: borrowed Foo(int)?;
var foo2: borrowed Foo(bool)?;
var val: uint = 1;
var foo3 = new borrowed Foo(val);
writeln(foo3.type? == foo.type);
writeln(foo.type:string);
writeln(foo2.type:string);
writeln(foo3.type:string);
writeln(foo3);
