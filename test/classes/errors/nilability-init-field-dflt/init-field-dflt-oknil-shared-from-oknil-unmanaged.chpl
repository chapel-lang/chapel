//  lhs: shared?  rhs: unmanaged?  error: mm

class MyClass {  var x: int;  }

var rhs: unmanaged MyClass?;

record MyRecord {
  var lhs: shared MyClass? = rhs;
}

var myr = new MyRecord();

compilerError("done");
