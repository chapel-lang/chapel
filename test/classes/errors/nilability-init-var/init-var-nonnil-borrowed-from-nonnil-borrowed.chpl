//  lhs: borrowed!  rhs: borrowed!  ok

class MyClass {
  var x: int;
}

var rhs = new borrowed MyClass();

var lhs: borrowed MyClass = rhs;


