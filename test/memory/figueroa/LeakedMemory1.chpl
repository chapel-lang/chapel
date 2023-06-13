use MemDiagnostics;

class C1 {
  var x: int;
}

class C2: C1 {
}

class C3: C2 {
  var y: int;
}

var m1 = memoryUsed();
var c1: unmanaged C1 = new unmanaged C1();
delete c1;
var m2 = memoryUsed();
writeln("Amount of leaked memory after deleting c1: ", m2-m1);

m1 = memoryUsed();
var c2 = new unmanaged C2();
delete c2;
m2 = memoryUsed();
writeln("Amount of leaked memory after deleting c2: ", m2-m1);

m1 = memoryUsed();
var c3 = new unmanaged C3();
delete c3;
m2 = memoryUsed();
writeln("Amount of leaked memory after deleting c3: ", m2-m1);
