var global = 1;

proc getter() ref {
  writeln("IN REF");
  return global;
}
proc getter() const ref {
  writeln("IN CONST REF");
  return global;
}
proc getter() {
  writeln("IN VALUE");
  return global;
}


proc byRef(ref x) {
}
proc byConstRef(const ref x) {
}
proc byConstIn(in x) {
}
proc byIn(in x) {
}


proc retByRef() ref {
  return getter();
}

proc retByConstRef() const ref {
  return getter();
}

proc retByValue() {
  return getter();
}


writeln("setting");
getter() = 2;

writeln("by ref");
byRef(getter());

writeln("ret by ref");
retByRef();

writeln("by const ref");
byConstRef(getter());

writeln("ret by const ref");
retByConstRef();

writeln("getting");
var x = getter();

writeln("by const in");
byConstIn(getter());

writeln("by in");
byIn(getter());

writeln("ret by value");
retByValue();
