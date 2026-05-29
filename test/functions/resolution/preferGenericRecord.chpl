record R {
  type t;
}

record S {
  type t;
}

proc foo(x: record) {
  writeln("In generic record version");
}

proc foo(x: R(?)) {
  writeln("In R version");
}

var myR: R(int);
var myS: S(int);

foo(myS);
foo(myR);
