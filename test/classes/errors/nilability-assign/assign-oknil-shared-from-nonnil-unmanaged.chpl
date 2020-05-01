//  lhs: shared?  rhs: unmanaged!  error: mm

class MyClass {  var x: int;  }

var lhs: shared MyClass? = nil;
var rhs = new unmanaged MyClass();

lhs = rhs;

compilerError("done");
delete rhs;

