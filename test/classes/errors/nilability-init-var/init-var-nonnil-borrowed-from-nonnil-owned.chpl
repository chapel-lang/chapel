//  lhs: borrowed!  rhs: owned!  ok

class MyClass {
  var x: int;
}

var rhs = new owned MyClass();

var lhs: borrowed MyClass = rhs;


