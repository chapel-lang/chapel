//  lhs: owned?  rhs: unmanaged?  error: mm

class MyClass {
  var x: int;
}

var rhs: unmanaged MyClass?;

var lhs: owned MyClass? = rhs;

compilerError("done");
