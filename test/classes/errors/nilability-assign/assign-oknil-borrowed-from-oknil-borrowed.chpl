//  lhs: borrowed?  rhs: borrowed?  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass? = nil;
var rhs: borrowed MyClass?;

lhs = rhs;


