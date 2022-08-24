//  lhs: unmanaged!  rhs: borrowed!  error: mm

class MyClass {
  var x: int;
}

var rhs = (new owned MyClass()).borrow();

var lhs: unmanaged MyClass = rhs;
