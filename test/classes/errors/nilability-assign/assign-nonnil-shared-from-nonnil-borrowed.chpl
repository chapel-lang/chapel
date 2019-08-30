//  lhs: shared!  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs = new shared MyClass();
var rhs = new borrowed MyClass();

lhs = rhs;

compilerError("done");

