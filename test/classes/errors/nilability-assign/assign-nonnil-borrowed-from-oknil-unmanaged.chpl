//  lhs: borrowed!  rhs: unmanaged?  error: nlb

class MyClass {  var x: int;  }

var obj = new MyClass(); var lhs = obj.borrow();
var rhs: unmanaged MyClass?;

lhs = rhs;

compilerError("done");

