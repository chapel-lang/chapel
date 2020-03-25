//  lhs: unmanaged?  rhs: nil?  ok

class MyClass {  var x: int;  }



record MyRecord {
  var lhs: unmanaged MyClass? = nil;
}

var myr = new MyRecord();


