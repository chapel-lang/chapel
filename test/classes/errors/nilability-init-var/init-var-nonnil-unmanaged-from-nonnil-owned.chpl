//  lhs: unmanaged!  rhs: owned!  error: mm

class MyClass {
  var x: int;
}

var rhs = new owned MyClass();

var lhs: unmanaged MyClass = rhs;

compilerError("done");
