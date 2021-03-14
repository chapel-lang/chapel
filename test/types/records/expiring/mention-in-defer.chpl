record R {
  var x: int;
  proc init(arg: int) { this.x = arg; writeln("init ", arg); }
  proc init() { this.x = 1; writeln("init 1"); }
  proc deinit() { writeln("deinit ", this.x); }
}

proc test1() {
  writeln("test1");
  var rec = new R();
  defer { writeln("defer setting 0"); rec.x = 0; }
  writeln("created rec ", rec);
  writeln("end");
}
test1();

proc test2() {
  writeln("test2");
  var rec = new R();
  defer { writeln("unrelated defer"); }
  writeln("created rec ", rec);
  writeln("end");
}
test2();

proc test3() {
  writeln("test3");
  var a = new R(1);
  defer { writeln("defer setting 0"); a.x = 0; }
  var b = new R(2);
  writeln("created recs ", a, " ", b);
  writeln("end");
}
test3();
