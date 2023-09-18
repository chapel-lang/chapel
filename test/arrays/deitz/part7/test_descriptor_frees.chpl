use MemDiagnostics;

config const serially = true;

var D: domain(1) = {1..10};
var A: [D] real;

proc foo() {
  for i in D do
    A(1..i) = i..;
}

var mem = memoryUsed();

serial serially do foo();

mem = memoryUsed() - mem;

writeln(A);
writeln(mem);
