//  lhs: shared?  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs: shared MyClass?;
var rhs = new borrowed MyClass();

lhs = rhs;

compilerError("done");

