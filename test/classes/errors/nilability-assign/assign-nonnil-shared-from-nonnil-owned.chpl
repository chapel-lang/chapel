//  lhs: shared!  rhs: owned!  ok

class MyClass {  var x: int;  }

{
  var lhs = new shared MyClass();
  var rhs = new owned MyClass();

  lhs = rhs;
}
