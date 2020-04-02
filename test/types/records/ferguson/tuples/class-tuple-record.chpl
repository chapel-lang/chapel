use myrecord;

record OtherRecord {
  var r:R = new R(1);
}

class MyClass {
  var tup:(OtherRecord,) = (new OtherRecord(),);
}

proc run() {
  var myC = new unmanaged MyClass();
  myC.tup(0).r.verify();
  assert(myC.tup(0).r.x == 1);
  delete myC;
}

run();
verify();

