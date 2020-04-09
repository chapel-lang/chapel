//  lhs: unmanaged?  rhs: shared!  error: mm

class MyClass {  var x: int;  }

var rhs = new shared MyClass();

record MyRecord {
  var lhs: unmanaged MyClass?;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(rhs);

compilerError("done");
