//  lhs: shared?  rhs: unmanaged?  error: mm

class MyClass {
  var x: int;
}

var rhs: unmanaged MyClass?;

var lhs: shared MyClass? = rhs;

compilerError("done");
