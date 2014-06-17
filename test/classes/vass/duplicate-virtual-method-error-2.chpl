class C     { proc dup(){ return 1; } }
class D : C { proc dup(){ return 2; } proc dup(){ return 3; } }

var c:C = new D();
writeln(c.dup());
