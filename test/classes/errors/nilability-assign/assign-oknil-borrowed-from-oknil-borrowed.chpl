//  lhs: borrowed?  rhs: borrowed?  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass?;
var rhs: borrowed MyClass?;

lhs = rhs;


