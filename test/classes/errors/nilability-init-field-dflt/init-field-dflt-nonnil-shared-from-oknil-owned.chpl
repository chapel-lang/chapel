//  lhs: shared!  rhs: owned?  error: nlb

class MyClass {  var x: int;  }

var rhs: owned MyClass?;

record MyRecord {
  var lhs: shared MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
