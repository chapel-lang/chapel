//  lhs: borrowed!  rhs: owned!  ok

class MyClass {  var x: int;  }

var lhs = (new owned MyClass()).borrow();
var rhs = new owned MyClass();

lhs = rhs;


