//  lhs: borrowed?  rhs: owned!  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass? = nil;
var rhs = new owned MyClass();

lhs = rhs;


