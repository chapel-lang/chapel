//  lhs: borrowed?  rhs: shared!  ok

class MyClass {  var x: int;  }

var rhs = new shared MyClass();

record MyRecord {
  var lhs: borrowed MyClass?;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(rhs);


