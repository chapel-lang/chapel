//  lhs: unmanaged?  rhs: unmanaged?  ok

class MyClass {
  var x: int;
}

var rhs: unmanaged MyClass?;

var lhs: unmanaged MyClass? = rhs;


