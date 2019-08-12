//  lhs: unmanaged!  rhs: owned!  error: mm

class MyClass {  var x: int;  }

var rhs = new owned MyClass();

record MyRecord {
  var lhs: unmanaged MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
