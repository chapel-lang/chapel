extern class C { var x: int(64); }
extern proc foo(c: C, x: int(64));

var myC = new C(5);

writeln(myC);

foo(myC, 3);

writeln(myC);

delete myC;
