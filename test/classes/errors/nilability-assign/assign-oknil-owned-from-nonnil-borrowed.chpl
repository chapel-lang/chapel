//  lhs: owned?  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs: owned MyClass? = nil;
var obj = new MyClass(); var rhs = obj.borrow();

lhs = rhs;

compilerError("done");

