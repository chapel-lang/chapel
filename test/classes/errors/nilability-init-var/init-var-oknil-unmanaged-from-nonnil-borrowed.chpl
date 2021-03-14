//  lhs: unmanaged?  rhs: borrowed!  error: mm

class MyClass {
  var x: int;
}

var rhs = new borrowed MyClass();

var lhs: unmanaged MyClass? = rhs;
