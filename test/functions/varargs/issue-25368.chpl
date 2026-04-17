// original bug made the fully generic overload get called instead of the myRecord(?) overload
// this has been fixed on `main`
record myRecord {
  type t = int;
}

var r1 = new myRecord(int);
var r2 = new myRecord(string);

proc foo(args:myRecord(?) ...) {
  writeln("myRecord(?) overload");
}

proc foo(args ...) {
  writeln("fully generic overload");
}

foo(r1, r2);
