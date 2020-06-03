//  lhs: shared!  rhs: borrowed?  errors: nlb, mm

class MyClass {
  var x: int;
}

var rhs: borrowed MyClass?;

var lhs: shared MyClass = rhs;

compilerError("done");
