//  lhs: borrowed!  rhs: shared?  error: nlb

class MyClass {  var x: int;  }

var rhs: shared MyClass?;

record MyRecord {
  var lhs: borrowed MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
