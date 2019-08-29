//  lhs: owned?  rhs: owned!  ok

class MyClass {  var x: int;  }

var lhs: owned MyClass?;
var rhs = new owned MyClass();

lhs = rhs;


