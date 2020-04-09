const globalConst:[1..4] int;

proc getter(ref arg) ref {
  return arg;
}

proc getter(const ref arg) const ref {
  return arg;
}

proc setElement(x) {
  x[1] = 1;
}

proc callSetElement(x) {
  setElement(x);
}

proc callSetElementWithGetter(x) {
  setElement(getter(x));
}

proc writeit(x) {
  writeln(x[1]);
}

proc writeit_in(in x) {
  writeln(x[1]);
}

proc f(const ref array) {
  setElement(array); // error
  callSetElement(array); // error
  callSetElementWithGetter(array); // error
}


proc g() {
  const localConst:[1..4] int;
  f(localConst);
  setElement(localConst); // error
  callSetElement(localConst); // error
  callSetElementWithGetter(localConst); // error
}

f(globalConst);
g();
setElement(globalConst); // error
callSetElement(globalConst); // error
callSetElementWithGetter(globalConst); // error
writeit(globalConst);
writeit_in(globalConst);
writeln(globalConst);


