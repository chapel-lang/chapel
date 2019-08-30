//  lhs: borrowed!  rhs: unmanaged?  error: nlb

class MyClass {  var x: int;  }

var lhs = new borrowed MyClass();
var rhs: unmanaged MyClass?;

lhs = rhs;

compilerError("done");

