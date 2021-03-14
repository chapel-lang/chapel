//  lhs: unmanaged!  rhs: owned?  errors: nlb, mm

class MyClass {  var x: int;  }

var rhs: owned MyClass?;

record MyRecord {
  var lhs: unmanaged MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
