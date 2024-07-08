//  lhs: shared!  rhs: owned!  error

class MyClass {  var x: int;  }

{
  var lhs = new shared MyClass();
  var rhs = new owned MyClass();

  lhs = rhs;
}
