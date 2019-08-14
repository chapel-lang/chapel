//  lhs: borrowed?  rhs: borrowed!  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass?;
var rhs = new borrowed MyClass();

lhs = rhs;


