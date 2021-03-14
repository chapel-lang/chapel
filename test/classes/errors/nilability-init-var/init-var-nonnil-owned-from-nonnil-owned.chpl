//  lhs: owned!  rhs: owned!  ok

class MyClass {
  var x: int;
}

{
  var rhs = new owned MyClass();
  var lhs: owned MyClass = rhs;
}
