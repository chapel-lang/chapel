//  lhs: borrowed!  rhs: shared!  ok

class MyClass {
  var x: int;
}

var rhs = new shared MyClass();

var lhs: borrowed MyClass = rhs;


