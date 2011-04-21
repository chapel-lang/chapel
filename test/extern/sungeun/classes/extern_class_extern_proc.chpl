_extern class C { var x: int; }
_extern proc foo(c: C, x: int);
var myC = new C(5);

writeln(myC);

foo(myC, 3);

writeln(myC);

