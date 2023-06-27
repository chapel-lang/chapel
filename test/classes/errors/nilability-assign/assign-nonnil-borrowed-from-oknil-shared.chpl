//  lhs: borrowed!  rhs: shared?  error: nlb

class MyClass {  var x: int;  }

var obj = new MyClass(); var lhs = obj.borrow();
var rhs: shared MyClass?;

lhs = rhs;

compilerError("done");

