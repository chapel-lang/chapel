//  lhs: shared?  rhs: owned?  ok

class MyClass {  var x: int;  }

var rhs: owned MyClass?;

record MyRecord {
  var lhs: shared MyClass? = rhs;
}

var myr = new MyRecord();


