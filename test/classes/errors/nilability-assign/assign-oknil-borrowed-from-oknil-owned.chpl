//  lhs: borrowed?  rhs: owned?  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass?;
var rhs: owned MyClass?;

lhs = rhs;


