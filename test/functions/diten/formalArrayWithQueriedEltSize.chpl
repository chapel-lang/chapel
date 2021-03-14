proc f(x: [] real(?s)) {
  writeln(x.eltType:string);
}

var a: [1..2] real;
var b: [1..2] real(32);

f(a);
f(b);
