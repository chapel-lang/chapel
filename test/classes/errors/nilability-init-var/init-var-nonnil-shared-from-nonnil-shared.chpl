//  lhs: shared!  rhs: shared!  ok

class MyClass {
  var x: int;
}

var rhs = new shared MyClass();

var lhs: shared MyClass = rhs;


