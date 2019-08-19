//  lhs: borrowed?  rhs: shared?  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass?;
var rhs: shared MyClass?;

lhs = rhs;


