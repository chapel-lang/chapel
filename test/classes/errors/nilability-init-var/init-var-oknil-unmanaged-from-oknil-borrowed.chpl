//  lhs: unmanaged?  rhs: borrowed?  error: mm

class MyClass {
  var x: int;
}

var rhs: borrowed MyClass?;

var lhs: unmanaged MyClass? = rhs;

compilerError("done");
