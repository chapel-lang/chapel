use CommDiagnostics;

config const verboseDebug = false;
config const verboseComm = false;

config param mode = 0;

type srcType = if mode==0 then unmanaged C?
               else if mode==1 then owned C?
               else if mode==2 then shared C?
               else if mode==3 then R
               else owned C?;

type dstType = if mode==0 then unmanaged C?
               else if mode==1 then owned C?
               else if mode==2 then shared C?
               else if mode==3 then R
               else owned C?;

proc createObj(id) {
  if mode == 0 then return new unmanaged C(id);
  else if mode == 1 then return new owned C(id);
  else if mode == 2 then return new shared C(id);
  else if mode == 3 then return new R(id);
  else return new owned C(id);
}

proc cleanup(array) {
  if mode == 0 then delete array;
}

class C {
  var x: int;

  proc init(n) {
    this.x = n;
  }
}

record R {
  var x: int;

  proc init() { }
  proc init(n) {
    this.x = n;
  }
}

config const elemPerLocale = 10;
const elemRange = 0..#(numLocales*elemPerLocale);

var a: [elemRange] srcType;
for l in Locales do on l {
  for i in 0..#elemPerLocale {
    a[here.id*elemPerLocale+i] = createObj(here.id);
  }
}

on Locales[1] {
  var b: [elemRange] dstType;

  if verboseComm then startVerboseCommHere();
  startCommDiagnosticsHere();

  b = a;

  stopCommDiagnosticsHere();
  if verboseComm then stopVerboseCommHere();
  writeln(getCommDiagnosticsHere());
}

cleanup(a);
