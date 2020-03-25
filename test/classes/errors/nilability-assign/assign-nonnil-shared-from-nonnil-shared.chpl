//  lhs: shared!  rhs: shared!  ok

class MyClass {  var x: int;  }

var lhs = new shared MyClass();
var rhs = new shared MyClass();

lhs = rhs;


