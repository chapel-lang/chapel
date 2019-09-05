//  lhs: shared?  rhs: borrowed?  error: mm

class MyClass {  var x: int;  }

var rhs: borrowed MyClass?;

record MyRecord {
  var lhs: shared MyClass? = rhs;
}

var myr = new MyRecord();

compilerError("done");
