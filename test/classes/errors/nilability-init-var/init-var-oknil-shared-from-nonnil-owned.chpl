//  lhs: shared?  rhs: owned  deprecated.

class MyClass {
  var x: int;
}

{
  var rhs = new owned MyClass();
  var lhs: shared MyClass? = rhs;
}
