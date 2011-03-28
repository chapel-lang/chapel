use Memory;

class C { var a: int; }

var c: C; 

var m1 = memoryUsed();

atomic {
  c = new C(1);
}

var m2 = memoryUsed();

writeln(c.a);
writeln("leaked memory = ", m2-m1);
