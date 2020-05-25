//  lhs: owned?  rhs: owned!  ok

class MyClass {  var x: int;  }

var rhs = new owned MyClass();

record MyRecord {
  var lhs: owned MyClass? = rhs;
}

var myr = new MyRecord();

compilerError("done");
