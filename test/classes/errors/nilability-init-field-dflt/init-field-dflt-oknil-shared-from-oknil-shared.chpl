//  lhs: shared?  rhs: shared?  ok

class MyClass {  var x: int;  }

var rhs: shared MyClass?;

record MyRecord {
  var lhs: shared MyClass? = rhs;
}

var myr = new MyRecord();


