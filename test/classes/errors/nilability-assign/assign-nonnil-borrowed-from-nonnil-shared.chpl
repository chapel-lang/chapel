//  lhs: borrowed!  rhs: shared!  ok

class MyClass {  var x: int;  }

var lhs = new borrowed MyClass();
var rhs = new shared MyClass();

lhs = rhs;


