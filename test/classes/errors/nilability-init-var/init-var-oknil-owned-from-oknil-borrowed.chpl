//  lhs: owned?  rhs: borrowed?  error: mm

class MyClass {
  var x: int;
}

var rhs: borrowed MyClass?;

var lhs: owned MyClass? = rhs;

compilerError("done");
