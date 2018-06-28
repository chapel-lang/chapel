pragma "safe"
module RO {

pragma "use default init"
class MyClass {
  var x:int;
}

pragma "use default init"
record MyRecord {
  var instance: owned MyClass;
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
