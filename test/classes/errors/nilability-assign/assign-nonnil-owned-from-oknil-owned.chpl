//  lhs: owned!  rhs: owned?  error: nlb

class MyClass {  var x: int;  }

var lhs = new owned MyClass();
var rhs: owned MyClass?;

lhs = rhs;

compilerError("done");

