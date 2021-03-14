// Check that POD correctly accepts POD types and support promotion

proc checkPOD(arg: chpl_anyPOD) {
  writeln(arg.type:string, " is POD");
}

record R {
  var i: int;
  var r: real;
  var b: bool;
  var t: 2*bool;
}

var i: int;
var r: real;
var b: bool;
var tup: 2*int;
var rec: R;
checkPOD(i);
checkPOD(r);
checkPOD(b);
checkPOD(tup);
checkPOD(rec);

var Ai: [1..1] int;
var Ar: [1..1] real;
var Ab: [1..1] bool;
var Atup: [1..1] 2*int;
var Arec: [1..1] R;
checkPOD(Ai);
checkPOD(Ar);
checkPOD(Ab);
checkPOD(Atup);
checkPOD(Arec);
