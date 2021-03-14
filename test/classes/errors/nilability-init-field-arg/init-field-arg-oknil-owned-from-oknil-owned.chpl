//  lhs: owned?  rhs: owned?  ok

class MyClass {  var x: int;  }

var rhs: owned MyClass?;

record MyRecord {
  var lhs: owned MyClass?;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(rhs);


