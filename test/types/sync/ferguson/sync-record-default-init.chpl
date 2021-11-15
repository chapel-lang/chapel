class C { }
record R {
  var x: int = 0;
  var ptr: shared C = new shared C();
  proc init() {
    this.x = 0;
    writeln("init");
  }
  proc init(arg:int) {
    this.x = arg;
    writeln("init ", arg);
  }
  proc init=(const other: R) {
    this.x = other.x;
    writeln("init= ", other.x);
  }
  proc deinit() {
    writeln("deinit ", x);
  }
}
operator R.=(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}

proc testDefaultInitSync() {
  writeln("testDefaultInitSync");
  var x: sync R;
}
testDefaultInitSync();

proc testDefaultInitSyncReadXX() {
  writeln("testDefaultInitSyncReadXX");
  var x: sync R;
  var y = x.readXX();
  writeln(y);
}
testDefaultInitSyncReadXX();

proc testDefaultInitSyncWriteXF() {
  writeln("testDefaultInitSyncWriteXF");
  var x: sync R;
  x.writeXF(new R(1));
}
testDefaultInitSyncWriteXF();

proc testDefaultInitSyncWriteEF() {
  writeln("testDefaultInitSyncWriteEF");
  var x: sync R;
  x.writeEF(new R(1));
}
testDefaultInitSyncWriteEF();

proc testDefaultInitSyncWriteEFReadXX() {
  writeln("testDefaultInitSyncWriteEFReadXX");
  var x: sync R;
  x.writeEF(new R(1));
  var y = x.readXX();
  writeln(y);
}
testDefaultInitSyncWriteEFReadXX();

proc testDefaultInitSyncWriteEFReadFE() {
  writeln("testDefaultInitSyncWriteEFReadFE");
  var x: sync R;
  x.writeEF(new R(1));
  var y = x.readFE();
  writeln(y);
}
testDefaultInitSyncWriteEFReadFE();

proc testDefaultInitSyncWriteEFReadFF() {
  writeln("testDefaultInitSyncWriteEFReadFF");
  var x: sync R;
  x.writeEF(new R(1));
  var y = x.readFF();
  writeln(y);
}
testDefaultInitSyncWriteEFReadFF();

proc testDefaultInitSyncWriteEFWriteFF() {
  writeln("testDefaultInitSyncWriteEFWriteFF");
  var x: sync R;
  x.writeEF(new R(1));
  x.writeFF(new R(2));
}
testDefaultInitSyncWriteEFWriteFF();

proc testDefaultInitSyncWriteEFWriteXF() {
  writeln("testDefaultInitSyncWriteEFWriteXF");
  var x: sync R;
  x.writeEF(new R(1));
  x.writeXF(new R(2));
}
testDefaultInitSyncWriteEFWriteXF();

proc testDefaultInitSyncReset() {
  writeln("testDefaultInitSyncReset");
  var x: sync R;
  x.reset();
}
testDefaultInitSyncReset();

proc testDefaultInitSyncWriteEFReset() {
  writeln("testDefaultInitSyncWriteEFReset");
  var x: sync R;
  x.writeEF(new R(1));
  x.reset();
}
testDefaultInitSyncWriteEFReset();

proc testValInitSync() {
  writeln("testValInitSync");
  var x: sync R = new R(1);
}
testValInitSync();

proc testValInitSyncReadFE() {
  writeln("testValInitSyncReadFE");
  var x: sync R = new R(1);
  var y = x.readFE();
  writeln(y);
}
testValInitSyncReadFE();

proc testDefaultInitSingle() {
  writeln("testDefaultInitSingle");
  var x: single R;
}
testDefaultInitSingle();

proc testDefaultInitSingleReadXX() {
  writeln("testDefaultInitSingleReadXX");
  var x: single R;
  var y = x.readXX();
  writeln(y);
}
testDefaultInitSingleReadXX();

proc testDefaultInitSingleWriteEF() {
  writeln("testDefaultInitSingleWriteEF");
  var x: single R;
  x.writeEF(new R(1));
}
testDefaultInitSingleWriteEF();

proc testDefaultInitSingleWriteEFReadFF() {
  writeln("testDefaultInitSingleWriteEFReadFF");
  var x: single R;
  x.writeEF(new R(1));
  var y = x.readFF();
  writeln(y);
}
testDefaultInitSingleWriteEFReadFF();

proc testDefaultInitSingleWriteEFReadXX() {
  writeln("testDefaultInitSingleWriteEFReadXX");
  var x: single R;
  x.writeEF(new R(1));
  var y = x.readXX();
  writeln(y);
}
testDefaultInitSingleWriteEFReadXX();

proc testValInitSingle() {
  writeln("testValInitSingle");
  var x: single R = new R(1);
}
testValInitSingle();

proc testValInitSingleReadFF() {
  writeln("testValInitSingleReadFF");
  var x: single R = new R(1);
  var y = x.readFF();
  writeln(y);
}
testValInitSingleReadFF();
