//  lhs: owned!  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs = new owned MyClass();
var obj = new MyClass(); var rhs = obj.borrow();

lhs = rhs;

compilerError("done");

