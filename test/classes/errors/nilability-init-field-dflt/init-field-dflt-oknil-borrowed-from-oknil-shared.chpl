//  lhs: borrowed?  rhs: shared?  ok

class MyClass {  var x: int;  }

var rhs: shared MyClass?;

record MyRecord {
  var lhs: borrowed MyClass? = rhs;
}

var myr = new MyRecord();


