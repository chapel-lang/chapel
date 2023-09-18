//  lhs: shared!  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var obj = new MyClass(); var rhs = obj.borrow();

record MyRecord {
  var lhs: shared MyClass = rhs;
}

var myr = new MyRecord();

compilerError("done");
