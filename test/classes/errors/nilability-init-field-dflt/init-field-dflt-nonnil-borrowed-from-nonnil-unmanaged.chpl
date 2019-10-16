//  lhs: borrowed!  rhs: unmanaged!  ok

class MyClass {  var x: int;  }

var rhs = new unmanaged MyClass();

record MyRecord {
  var lhs: borrowed MyClass = rhs;
}

var myr = new MyRecord();

delete rhs;
