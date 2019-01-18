record MyRecord {
  var a: string;
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
