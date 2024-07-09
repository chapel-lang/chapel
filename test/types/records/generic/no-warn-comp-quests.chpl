record R {
  type t;
  var x: t;
}

record R2 {
  type t;
}

record R3 {
  var x: R2(R(?));
}


var r = new R(int);
var r2 = new R2(t=R(int));
var r3 = new R3(x = new R2(R(int)));

writeln(r, ": ", r.type:string);
writeln(r2, ": ", r2.type:string);
writeln(r3, ": ", r3.type:string);