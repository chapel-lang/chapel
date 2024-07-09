//  lhs: borrowed?  rhs: borrowed!  ok

class MyClass {  var x: int;  }

var ownRhs = new owned MyClass();
var rhs = ownRhs.borrow();

record MyRecord {
  var lhs: borrowed MyClass? = rhs;
}

var myr = new MyRecord();


