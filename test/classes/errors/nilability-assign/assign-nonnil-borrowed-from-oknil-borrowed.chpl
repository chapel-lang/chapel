//  lhs: borrowed!  rhs: borrowed?  error: nlb

class MyClass {  var x: int;  }

var obj = new owned MyClass(); var lhs = obj.borrow();
var rhs: borrowed MyClass?;

lhs = rhs;

compilerError("done");

