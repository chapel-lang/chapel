record MyRecord {
  var a;
  proc init=(other:MyRecord) {
    this.a = other.a;
  }
  proc init(arg) {
    this.a = arg;
  }
}

proc test1() {
  var r = new MyRecord("Hello"*2);
  writeln(r);
}
test1();

proc test2() {
  var r:MyRecord = new MyRecord("Hello"*3);
  writeln(r);
}
test2();

proc test3() {
  var r:MyRecord(string) = new MyRecord("Hello"*4);
  writeln(r);
}
test3();
