proc f(i: int) {
  writeln("In foo(:int)");
}

proc f(i) {
  writeln("In foo(:any)");
}

var D = {1..4};
var A: [D] int = [i in D] i;

f(2);
f(A);
