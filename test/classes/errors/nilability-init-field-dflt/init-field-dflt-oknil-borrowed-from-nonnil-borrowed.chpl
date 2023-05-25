//  lhs: borrowed?  rhs: borrowed!  ok

class MyClass {  var x: int;  }

var rhs = (new owned MyClass()).borrow();

record MyRecord {
  var lhs: borrowed MyClass? = rhs;
}

var myr = new MyRecord();


