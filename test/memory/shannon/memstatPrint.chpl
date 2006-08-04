config var print: bool = false;

class C {
  var a: int(64);
  var b: int(64);
  var c: int(64);
  var d: int(64);
  var e: int(64);
  var f: int(64);
  var g: int(64);
}

_chpl_memtest_printMemStat();
var c = C();
_chpl_memtest_printMemStat();
var d = C();
_chpl_memtest_printMemStat();
var e = C();
_chpl_memtest_printMemStat();
var f = C();
_chpl_memtest_printMemStat();

if print {
  writeln(c);
  writeln(d);
  writeln(e);
  writeln(f);
}
