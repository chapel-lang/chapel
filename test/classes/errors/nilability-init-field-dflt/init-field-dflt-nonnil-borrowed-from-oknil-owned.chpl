//  lhs: borrowed!  rhs: owned?  error: nlb

class MyClass {  var x: int;  }

var rhs: owned MyClass?;

record MyRecord {
  var lhs: borrowed MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
