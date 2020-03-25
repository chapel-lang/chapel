//  lhs: borrowed!  rhs: borrowed?  error: nlb

class MyClass {  var x: int;  }

var rhs: borrowed MyClass?;

record MyRecord {
  var lhs: borrowed MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
