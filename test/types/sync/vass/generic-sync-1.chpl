// Verify operation of by-ref passing a sync to a generic argument.

var d: sync int;
test(d);
test2(d);

proc test(s) {
  compilerWarning(typeToString(s.type), 0);  // see also ./sync-type-1.future
  s = 1;
  writeln("got lock");
}

proc test2(s) {
  compilerWarning(typeToString(s.type));
  writeln("isFull = ", s.isFull);
  s;
  writeln("released lock");
  writeln("isFull = ", s.isFull);
}
