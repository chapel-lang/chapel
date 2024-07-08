//  lhs: shared?  rhs: owned!  error

class MyClass {  var x: int;  }

{
  var lhs: shared MyClass? = nil;
  var rhs = new owned MyClass();

  lhs = rhs;
}
