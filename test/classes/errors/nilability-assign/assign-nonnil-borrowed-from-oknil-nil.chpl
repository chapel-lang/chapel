//  lhs: borrowed!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }

var lhs = (new owned MyClass()).borrow();


lhs = nil;

compilerError("done");

