union U {
  var x: int;
  var y: real;
}
require "union-type-constraint.h";
extern union UU {
  var x: int;
  var y: real;
}

proc doit(x: union) {
  writeln(x.x);
}

record myRec {
  var myU: union;
}

var u: U;
u.x = 10;
doit(u);
var r = new myRec(u);
writeln(r.type:string, " ", r);

var uu: UU;
uu.x = 3;
doit(uu);
var rr = new myRec(uu);
writeln(rr.type:string, " ", rr.myU.x);
