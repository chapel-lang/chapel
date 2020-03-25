//  lhs: unmanaged!  rhs: unmanaged!  ok

class MyClass {  var x: int;  }

var rhs = new unmanaged MyClass();

record MyRecord {
  var lhs: unmanaged MyClass;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(rhs);

delete rhs;
