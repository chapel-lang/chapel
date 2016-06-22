proc foo(r: range(?et, ?bt, ?sbl), s: et) {
  compilerWarning("Got into my foo!");
}

/*
proc foo(r: range(?et, ?bt, ?sbl), s: typeToSignedType(et)) where (et != typeToSignedType(et)) {
  compilerWarning("Got into my second foo!  ", typeToSignedType(et):string);
}
*/

proc foo(r: range(?), x) {
  compilerError("Didn't get into my foos!");
}

var r = 1..10;

var x: int;

foo(r, x);
