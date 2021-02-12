//  lhs: unmanaged?  rhs: shared!  error: mm

class MyClass {
  var x: int;
}

var rhs = new shared MyClass();

var lhs: unmanaged MyClass? = rhs;
