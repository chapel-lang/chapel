//  lhs: unmanaged?  rhs: shared?  error: mm

class MyClass {
  var x: int;
}

var rhs: shared MyClass?;

var lhs: unmanaged MyClass? = rhs;
