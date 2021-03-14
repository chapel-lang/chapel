record testRecord {
  var dummy: int = 0;
  proc init(dummy: int=0) { this.dummy = dummy; }
}
proc <(lhs: testRecord, rhs: testRecord) {
  return lhs.dummy < rhs.dummy;
}

operator :(x:int, type t: testRecord) {
  return new testRecord(x);
}

class testClass {
  var dummy: int = 0;
  proc init(dummy: int=0) { this.dummy = dummy; }
}
proc <(lhs: testClass, rhs: testClass) {
  return lhs.dummy < rhs.dummy;
}
proc >(lhs: testClass, rhs: testClass) {
  return lhs.dummy > rhs.dummy;
}
