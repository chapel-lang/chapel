//  lhs: owned?  rhs: nil?  ok

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: owned MyClass? = nil;
}

var myr = new MyRecord();


