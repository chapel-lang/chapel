//  lhs: shared!  rhs: owned?  error: nlb

class MyClass {
  var x: int;
}

var rhs: owned MyClass?;

var lhs: shared MyClass = rhs;

compilerError("done");
