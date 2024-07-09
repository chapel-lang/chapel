class C     { proc dup(){ return 1; } }
class D : C { override proc dup(){ return 2; } proc dup(a:int){ return 3; } }

var ownC = new owned D();
var c:borrowed C = ownC.borrow();
writeln(c.dup());
