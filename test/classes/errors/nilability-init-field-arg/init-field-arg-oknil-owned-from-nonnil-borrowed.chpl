//  lhs: owned?  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var obj = new MyClass(); var rhs = obj.borrow();

record MyRecord {
  var lhs: owned MyClass?;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(rhs);
