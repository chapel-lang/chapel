// Exercises basic declarations of instantiations.  Here, the type creator has
// declared an initializer with an argument for the generic var field that
// won't just use the argument for the field directly but will instead do
// something different depending on what that argument is.
class Foo {
  var v;

  proc init(vVal) where (isIntegralType(vVal.type)) {
    v = 10;
    super.init();
  }

  proc init(vVal) where (!isIntegralType(vVal.type)) {
    v = vVal;
    super.init();
  }
}

var foo: Foo(int);
var foo2: Foo(bool);
var foo3: Foo(uint);
writeln(foo3.type == foo.type);
writeln(foo.type:string);
writeln(foo2.type:string);
writeln(foo3.type:string);
