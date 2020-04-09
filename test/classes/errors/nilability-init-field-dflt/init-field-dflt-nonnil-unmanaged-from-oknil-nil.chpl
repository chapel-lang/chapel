//  lhs: unmanaged!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: unmanaged MyClass = nil;
}

var myr = new MyRecord();

compilerError("done");
