//  lhs: borrowed?  rhs: borrowed!  ok

class MyClass {  var x: int;  }

var rhs = new borrowed MyClass();

record MyRecord {
  var lhs: borrowed MyClass? = rhs;
}

var myr = new MyRecord();


