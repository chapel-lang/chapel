var globalArray : [1..4] int;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}

proc writeit(const ref x) {
  writeln(x);
}

proc writeArray(x) {
  writeit(x);
}

proc getElement(x) {
  writeln(x[1]);
}

proc getArray(x) {
  var y = x;
  writeln(y);
}

proc setit(ref x) {
  x = 1;
}

proc setX(x) {
  setit(x);
}

proc setElement(x) {
  x[1] = 1;
}

proc setArray(x) {
  x = 1;
}


writeln("writeArray: ");
writeArray(getter(globalArray));
writeArray(getter(getter(globalArray)));

writeln("getElement: ");
getElement(getter(globalArray));
getElement(getter(getter(globalArray)));

writeln("getArray: ");
getArray(getter(globalArray));
getArray(getter(getter(globalArray)));

writeln("setX: ");
setX(getter(globalArray));
setX(getter(getter(globalArray)));

writeln("setElement: ");
setElement(getter(globalArray));
setElement(getter(getter(globalArray)));

writeln("setArray: ");
setArray(getter(globalArray));
setArray(getter(getter(globalArray)));

