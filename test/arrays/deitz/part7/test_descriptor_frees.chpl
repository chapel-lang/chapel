use Memory;

var D: domain(1) = [1..10];
var A: [D] real;

def foo() {
  for i in D do
    A(1..i) = i..;
}

var mem = memoryUsed();

foo();

mem = memoryUsed() - mem;

writeln(A);
writeln(mem);
