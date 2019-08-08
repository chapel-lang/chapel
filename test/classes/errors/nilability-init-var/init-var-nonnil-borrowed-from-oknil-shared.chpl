//  lhs: borrowed!  rhs: shared?  error: nlb

class MyClass {
  var x: int;
}

var rhs: shared MyClass?;

var lhs: borrowed MyClass = rhs;

compilerError("done");
