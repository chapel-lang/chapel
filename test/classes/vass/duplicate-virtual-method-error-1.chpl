class C     { proc dup(){ return 1; } proc dup(){ return 4; } }
class D : C { proc dup(){ return 2; } }
var dObj = new D();
var c:borrowed C = dObj.borrow();
writeln(c.dup());
