//  lhs: owned!  rhs: unmanaged!  error: mm

class MyClass {  var x: int;  }

var rhs = new unmanaged MyClass();

record MyRecord {
  var lhs: owned MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
