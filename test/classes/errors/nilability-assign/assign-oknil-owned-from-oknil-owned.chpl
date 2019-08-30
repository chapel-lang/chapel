//  lhs: owned?  rhs: owned?  ok

class MyClass {  var x: int;  }

var lhs: owned MyClass?;
var rhs: owned MyClass?;

lhs = rhs;


