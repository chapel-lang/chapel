//  lhs: borrowed!  rhs: unmanaged?  error: nlb

class MyClass {  var x: int;  }

var rhs: unmanaged MyClass?;

record MyRecord {
  var lhs: borrowed MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
