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


writeln("setting");
getter() = 2;

writeln("by ref");
byRef(getter());

writeln("getting");
var x = getter();

writeln("by const ref");
byConstRef(getter());

writeln("by const in");
byConstIn(getter());

writeln("by in");
byIn(getter());
