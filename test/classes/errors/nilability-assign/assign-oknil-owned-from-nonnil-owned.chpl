//  lhs: owned?  rhs: owned!  ok

class MyClass {  var x: int;  }

{
  var lhs: owned MyClass? = nil;
  var rhs = new owned MyClass();

  lhs = rhs;
}
