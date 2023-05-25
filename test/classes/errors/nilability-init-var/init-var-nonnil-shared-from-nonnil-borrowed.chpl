//  lhs: shared!  rhs: borrowed!  error: mm

class MyClass {
  var x: int;
}

var rhs = (new owned MyClass()).borrow();

var lhs: shared MyClass = rhs;
