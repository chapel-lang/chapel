record R {
  var s = "world";
  proc hello() { writeln("Hello ", s); return this; }

  proc init() { writeln("in init"); }
  proc deinit() { writeln("in deinit"); }
}

proc gimmeanr(): R throws {
  var ret: R;
  throw new owned Error();
  return ret;
}

proc testa() throws {
  var r = gimmeanr();
}
proc testb() throws {
  var r = gimmeanr().hello();
}
proc testc() throws {
  var r = gimmeanr().hello().hello();
}
proc testd() throws {
  var tmp: R;
  ref rtmp = tmp;
  var r = gimmeanr().hello().hello();
}

writeln("testa");
try {
  testa();
} catch { /* ignore errors for test */ }

writeln("testb");
try {
  testb();
} catch { /* ignore errors for test */ }

writeln("testc");
try {
  testc();
} catch { /* ignore errors for test */ }

writeln("testd");
try {
  testd();
} catch { /* ignore errors for test */ }
