//  lhs: unmanaged!  rhs: owned?  errors: nlb, mm

class MyClass {
  var x: int;
}

var rhs: owned MyClass?;

var lhs: unmanaged MyClass = rhs;

compilerError("done");
