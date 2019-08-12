//  lhs: borrowed?  rhs: nil?  ok

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: borrowed MyClass? = nil;
}

var myr = new MyRecord();


