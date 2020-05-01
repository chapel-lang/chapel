//  lhs: shared?  rhs: owned!  ok

class MyClass {  var x: int;  }

{
  var lhs: shared MyClass? = nil;
  var rhs = new owned MyClass();

  lhs = rhs;
}
