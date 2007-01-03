class C {
  var a: [1..16] int(64);
}

var c = C();
var d = C();
var e = C();
var f = C();

_chpl_memtest_printMemTable(120);
