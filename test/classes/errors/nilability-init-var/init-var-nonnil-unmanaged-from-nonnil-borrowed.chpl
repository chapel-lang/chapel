//  lhs: unmanaged!  rhs: borrowed!  error: mm

class MyClass {
  var x: int;
}

var obj = new MyClass(); var rhs = obj.borrow();

var lhs: unmanaged MyClass = rhs;
