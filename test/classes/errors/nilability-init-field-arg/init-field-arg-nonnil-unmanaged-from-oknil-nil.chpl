//  lhs: unmanaged!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: unmanaged MyClass;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(nil);
