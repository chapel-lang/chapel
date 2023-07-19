//  lhs: unmanaged?  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs: unmanaged MyClass? = nil;
var obj = new MyClass(); var rhs = obj.borrow();

lhs = rhs;

compilerError("done");

