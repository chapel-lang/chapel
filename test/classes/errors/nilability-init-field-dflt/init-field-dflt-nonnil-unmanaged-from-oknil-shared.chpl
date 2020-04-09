//  lhs: unmanaged!  rhs: shared?  errors: nlb, mm

class MyClass {  var x: int;  }

var rhs: shared MyClass?;

record MyRecord {
  var lhs: unmanaged MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
