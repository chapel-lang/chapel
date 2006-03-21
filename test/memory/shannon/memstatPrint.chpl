class C {
  var u: int;
  var v: int;
  var w: int;
  var x: int;
  var y: int;
  var z: int;
}

fun foo() {
  var s: string = "1234567890";

  s = "";
}

var c = C();
foo();

_chpl_memtest_printMemStat();
