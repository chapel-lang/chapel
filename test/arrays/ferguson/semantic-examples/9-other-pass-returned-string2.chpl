// This one is a sanity check
config const n = 1;

proc createArray() {
  var s:string;
  s = n:string;
  return s;
}
proc consumesArray(A) {
  writeln(A[1]);
}

proc doit() {
  consumesArray(createArray());
}

doit();
