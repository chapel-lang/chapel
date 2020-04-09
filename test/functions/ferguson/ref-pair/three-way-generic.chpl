var global = 1;

proc getter(ref arg) ref {
  writeln("IN REF");
  return arg;
}
proc getter(const ref arg) const ref {
  writeln("IN CONST REF");
  return arg;
}
proc getter(const in arg) {
  writeln("IN VALUE");
  return arg;
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
getter(global) = 2;

writeln("by ref");
byRef(getter(global));

writeln("getting");
var x = getter(global);

writeln("by const ref");
byConstRef(getter(global));

writeln("by const in");
byConstIn(getter(global));

writeln("by in");
byIn(getter(global));
