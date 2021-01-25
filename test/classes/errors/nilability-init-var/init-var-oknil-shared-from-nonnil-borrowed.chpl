//  lhs: shared?  rhs: borrowed!  error: mm

class MyClass {
  var x: int;
}

var rhs = new borrowed MyClass();

var lhs: shared MyClass? = rhs;
