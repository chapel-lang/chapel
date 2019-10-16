//  lhs: owned!  rhs: owned?  error: nlb

class MyClass {  var x: int;  }

var rhs: owned MyClass?;

record MyRecord {
  var lhs: owned MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
