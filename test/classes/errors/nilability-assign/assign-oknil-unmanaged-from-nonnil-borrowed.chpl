//  lhs: unmanaged?  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs: unmanaged MyClass? = nil;
var rhs = new borrowed MyClass();

lhs = rhs;

compilerError("done");

