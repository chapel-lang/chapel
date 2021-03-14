var globalInt = 1;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}

proc vgetter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc vgetter(const ref arg) {
  writeln("VALUE");
  return arg;
}


proc readX(x) {
  writeln(x);
}

proc readConstX(const ref x) {
  writeln(x);
}

proc readInX(in x) {
  writeln(x);
}

proc setX(ref x) {
  x = 1;
}

writeln("readX: ");
readX(getter(globalInt));
readX(getter(getter(globalInt)));
readX(vgetter(globalInt));
readX(vgetter(vgetter(globalInt)));
writeln("readConstX: ");
readConstX(getter(globalInt));
readConstX(getter(getter(globalInt)));
readConstX(vgetter(globalInt));
readConstX(vgetter(vgetter(globalInt)));
writeln("readInX: ");
readInX(getter(globalInt));
readInX(getter(getter(globalInt)));
readInX(vgetter(globalInt));
readInX(vgetter(vgetter(globalInt)));
writeln("setX: ");
setX(getter(globalInt));
setX(getter(getter(globalInt)));
setX(vgetter(globalInt));
setX(vgetter(vgetter(globalInt)));

