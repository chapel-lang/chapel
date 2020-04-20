//  lhs: owned?  rhs: owned!  ok

class MyClass {  var x: int;  }

record MyRecord {
  var lhs: owned MyClass?;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

{
  var rhs = new owned MyClass();
  var myr = new MyRecord(rhs);
}
