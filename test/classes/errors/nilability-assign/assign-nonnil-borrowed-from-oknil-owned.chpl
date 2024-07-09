//  lhs: borrowed!  rhs: owned?  error: nlb

class MyClass {  var x: int;  }

var obj = new MyClass(); var lhs = obj.borrow();
var rhs: owned MyClass?;

lhs = rhs;

compilerError("done");

