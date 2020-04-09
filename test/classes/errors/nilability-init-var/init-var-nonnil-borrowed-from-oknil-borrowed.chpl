//  lhs: borrowed!  rhs: borrowed?  error: nlb

class MyClass {
  var x: int;
}

var rhs: borrowed MyClass?;

var lhs: borrowed MyClass = rhs;

compilerError("done");
