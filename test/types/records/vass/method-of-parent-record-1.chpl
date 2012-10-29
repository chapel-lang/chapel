record A {
  proc test() { writeln("in A.test()"); }
}

record B: A { }

var a: A, b: B;

a.test();
b.test();
