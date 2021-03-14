//  lhs: owned!  rhs: nil?  error: nlb

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: owned MyClass = nil;
}

var myr = new MyRecord();

compilerError("done");
