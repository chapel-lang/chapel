class C     { proc dup(){ return 1; } proc dup(){ return 4; } }
class D : C { proc dup(){ return 2; } }

var c:C = new D();
writeln(c.dup());
