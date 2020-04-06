use myrecord;

record OtherRecord {
  var arr:[1..1] R = [new R(1)];
}

class MyClass {
  var tup:(OtherRecord,) = (new OtherRecord(),);
}

proc run() {
  var myC = new unmanaged MyClass();
  myC.tup(0).arr[1].verify();
  assert(myC.tup(0).arr[1].x == 1);
  delete myC;
}

run();
verify();

