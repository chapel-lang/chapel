use MemDiagnostics;

class C {
  var x: int;
}

proc foo() {
  var c: [1..10] unmanaged C = [i in 1..10] new unmanaged C();
  for cc in c do delete cc;
}

serial {
  var m1 = memoryUsed();
  foo();
  var m2 = memoryUsed();
  writeln("Amount of leaked memory after deleting array c: ", m2-m1);
}
