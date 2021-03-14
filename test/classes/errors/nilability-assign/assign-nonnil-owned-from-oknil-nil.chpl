//  lhs: owned!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }

var lhs = new owned MyClass();


lhs = nil;

compilerError("done");

