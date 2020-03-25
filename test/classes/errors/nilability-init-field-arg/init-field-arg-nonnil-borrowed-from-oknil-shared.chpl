//  lhs: borrowed!  rhs: shared?  error: nlb

class MyClass {  var x: int;  }

var rhs: shared MyClass?;

record MyRecord {
  var lhs: borrowed MyClass;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(rhs);

compilerError("done");
