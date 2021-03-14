//  lhs: unmanaged!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }

var lhs = new unmanaged MyClass();


lhs = nil;

compilerError("done");
delete lhs;

