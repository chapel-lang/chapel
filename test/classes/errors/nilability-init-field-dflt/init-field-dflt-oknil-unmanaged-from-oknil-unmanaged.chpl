//  lhs: unmanaged?  rhs: unmanaged?  ok

class MyClass {  var x: int;  }

var rhs: unmanaged MyClass?;

record MyRecord {
  var lhs: unmanaged MyClass? = rhs;
}

var myr = new MyRecord();


