//  lhs: shared!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: shared MyClass = nil;
}

var myr = new MyRecord();

compilerError("done");
