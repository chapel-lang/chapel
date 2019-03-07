// Exercises using a where clause that is redundant to the type declarations of
// the arguments
class Foo {
  var x: int;

  proc init(xVal:int) where (isInt(xVal)) {
    x = xVal;
  }

  proc init(realVal:real) where (isReal(realVal)) {
    x = floor(realVal):int;
  }
}

var f1 = new owned Foo(13);
var f2 = new owned Foo(3.4);
writeln(f1);
writeln(f2);
