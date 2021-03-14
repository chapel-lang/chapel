//  lhs: borrowed?  rhs: unmanaged?  ok

class MyClass {
  var x: int;
}

var rhs: unmanaged MyClass?;

var lhs: borrowed MyClass? = rhs;


