
var global1 = 1;

proc test(x:int) ref       { writeln("ref1"); return global1; }
proc test(x:int)           { writeln("val1"); return 123; }
proc test(x:int) const ref { writeln("cr1");  return global1; }

proc test() ref       { writeln("ref2"); return global1; }
proc test() const ref { writeln("cr2");  return global1; }

var x1 = test(5);       // 3 "best candidates"
writeln(x1);

const ref x2 = test();  // 2 "best candidates"
writeln(x2);
