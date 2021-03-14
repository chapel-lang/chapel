pragma "safe"
module RO {

class MyClass {
  var x:int;
}

record MyRecord {
  var instance: owned MyClass?;
}

/*proc valueIdentity(arg) {
  return arg;
}*/

proc returnsCopyOfLocal() {
  var rec = new MyRecord(new owned MyClass(1));
  ref _ref = rec;
  return _ref;
}

proc test() {
  var v = returnsCopyOfLocal();
  writeln(v);
}
test();

}
