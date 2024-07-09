//  lhs: borrowed!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }

var obj = new MyClass(); var lhs = obj.borrow();


lhs = nil;

compilerError("done");

