//  lhs: borrowed!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }

var lhs = new borrowed MyClass();


lhs = nil;

compilerError("done");

