//  lhs: shared!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }

var lhs = new shared MyClass();


lhs = nil;

compilerError("done");

