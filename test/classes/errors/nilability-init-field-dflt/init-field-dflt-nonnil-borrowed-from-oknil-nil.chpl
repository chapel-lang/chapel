//  lhs: borrowed!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: borrowed MyClass = nil;
}

var myr = new MyRecord();

compilerError("done");
