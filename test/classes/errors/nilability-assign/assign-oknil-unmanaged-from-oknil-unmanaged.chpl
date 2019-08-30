//  lhs: unmanaged?  rhs: unmanaged?  ok

class MyClass {  var x: int;  }

var lhs: unmanaged MyClass?;
var rhs: unmanaged MyClass?;

lhs = rhs;


