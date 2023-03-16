//  lhs: owned!  rhs: borrowed!  error: mm

class MyClass {
  var x: int;
}
var ownRhs = new owned MyClass();
var rhs = ownRhs.borrow();

var lhs: owned MyClass = rhs;
