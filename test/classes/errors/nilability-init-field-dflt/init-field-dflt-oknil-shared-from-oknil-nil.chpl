//  lhs: shared?  rhs: nil?  ok

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: shared MyClass? = nil;
}

var myr = new MyRecord();


