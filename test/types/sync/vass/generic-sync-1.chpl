// Verify operation of by-ref passing a sync to a generic argument.

var d: sync int;
test(d);
test2(d);

proc test(s) {
  compilerWarning(s.type:string, 0);  // see also ./sync-type-1.future
  s.writeEF(1);
  writeln("got lock");
}

proc test2(s) {
  compilerWarning(s.type:string);
  writeln("isFull = ", s.isFull);
  s.readFE();
  writeln("released lock");
  writeln("isFull = ", s.isFull);
}
