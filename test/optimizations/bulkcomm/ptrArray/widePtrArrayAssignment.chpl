use CommDiagnostics;

config const verboseDebug = false;
config const verboseComm = false;

config param mode = 0;

type srcType = if mode==0 then unmanaged C?
               else if mode==1 then owned C?
               else if mode==2 then shared C?
               else if mode==3 then R
               else unmanaged C?;

type dstType = if mode==0 then unmanaged C?
               else if mode==1 then owned C?
               else if mode==2 then shared C?
               else if mode==3 then R
               else unmanaged C?;

proc createObj(id) {
  if mode == 0 then return new unmanaged C(id);
  else if mode == 1 then return new owned C(id);
  else if mode == 2 then return new shared C(id);
  else if mode == 3 then return new R(id);
  else return new unmanaged C(id);
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

var a: [LocaleSpace] srcType;
for l in Locales do on l {
  a[here.id] = createObj(here.id);
}

on Locales[1] {
  var b: [LocaleSpace] dstType;

  if verboseComm then startVerboseCommHere();
  startCommDiagnosticsHere();
  b = a;
  stopCommDiagnosticsHere();
  if verboseComm then stopVerboseCommHere();
  /*writeln(getCommDiagnosticsHere());*/

  if mode==0 {
    if ptrArrEq(a, b) {
      writeln("Success");
    }
    else {
      writeln("Pointer arrays differ");
      writeln("Post assignment a");
      arrPrint(a);
      writeln("Post assignment b");
      arrPrint(b);
      writeln();
    }
  }
  // owned transfer can't be done in bulk
  // record arrays are transferred by value
  else {
    if ptrArrEq(a, b) {
      writeln("FAILED, pointer arrays should be different");
      writeln("Post assignment a");
      arrPrint(a);
      writeln("Post assignment b");
      arrPrint(b);
      writeln();
    }
    else {
      writeln("Success");
    }
  }
}

proc getWidePtrTup(p) {
  if !__primitive("is wide pointer", p) {
    writeln("You have to pass a wide pointer to this helper");
  }
  const locID = __primitive("_wide_get_locale", p);
  const nodeID = chpl_nodeFromLocaleID(locID);
  const sublocID = chpl_sublocFromLocaleID(locID);
  const addr = __primitive("_wide_get_addr", p);

  return (nodeID, sublocID, addr);
}

proc arrPrint(a) {
  for elem in a{
    const (nodeID, sublocID, addr) = getWidePtrTup(elem);
    writeln("nodeID: ", nodeID, " sublocID ", sublocID, " addr ", addr);
  }
}

proc ptrEq(p1, p2) {
  return getWidePtrTup(p1) == getWidePtrTup(p2);
}

proc ptrArrEq(a, b) {
  var eq = true;
  for (aa, bb) in zip(a, b) {
    if !ptrEq(aa, bb) then eq = false;
  }
  return eq;
}


