// Check that POD does not trigger for some non POD types

proc checkPOD(arg: chpl_anyPOD) {
  writeln("POD");
}

proc checkPOD(arg) {
  writeln("Not POD");
}

record RD {
  var i: int;
  proc deinit() { }
}

record RA {
  var A: [1..1] int;
}

var t: 2*RA;
var rd: RD;
var ra: RA;
var a: atomic int;
checkPOD(t);
checkPOD(rd);
checkPOD(ra);
checkPOD(a);
