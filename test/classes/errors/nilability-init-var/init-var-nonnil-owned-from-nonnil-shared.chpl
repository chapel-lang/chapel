//  lhs: owned!  rhs: shared!  error: mm

class MyClass {
  var x: int;
}

var rhs = new shared MyClass();

var lhs: owned MyClass = rhs;

compilerError("done");
