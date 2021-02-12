//  lhs: owned!  rhs: unmanaged?  errors: nlb, mm

class MyClass {  var x: int;  }

var rhs: unmanaged MyClass?;

record MyRecord {
  var lhs: owned MyClass;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(rhs);
