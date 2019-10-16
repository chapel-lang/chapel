//  lhs: owned!  rhs: shared?  errors: nlb, mm

class MyClass {
  var x: int;
}

var rhs: shared MyClass?;

var lhs: owned MyClass = rhs;

compilerError("done");
