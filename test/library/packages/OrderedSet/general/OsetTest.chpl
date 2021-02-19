record testRecord {
  var dummy: int = 0;
  proc init(dummy: int=0) { this.dummy = dummy; }
}
operator testRecord.<(lhs: testRecord, rhs: testRecord) {
  return lhs.dummy < rhs.dummy;
}

operator :(x:int, type t: testRecord) {
  return new testRecord(x);
}

class testClass {
  var dummy: int = 0;
  proc init(dummy: int=0) { this.dummy = dummy; }
}
operator testRecord.<(lhs: testClass, rhs: testClass) {
  return lhs.dummy < rhs.dummy;
}
operator testRecord.>(lhs: testClass, rhs: testClass) {
  return lhs.dummy > rhs.dummy;
}
