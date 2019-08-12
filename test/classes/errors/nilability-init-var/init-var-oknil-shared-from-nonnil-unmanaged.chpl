//  lhs: shared?  rhs: unmanaged!  error: mm

class MyClass {
  var x: int;
}

var rhs = new unmanaged MyClass();

var lhs: shared MyClass? = rhs;

compilerError("done");
