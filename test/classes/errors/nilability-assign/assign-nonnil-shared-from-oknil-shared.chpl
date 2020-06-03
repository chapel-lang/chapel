//  lhs: shared!  rhs: shared?  error: nlb

class MyClass {  var x: int;  }

var lhs = new shared MyClass();
var rhs: shared MyClass?;

lhs = rhs;

compilerError("done");

