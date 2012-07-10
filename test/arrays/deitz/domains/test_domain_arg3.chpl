proc foo(d: domain) {
  var a: [d] real;
  writeln(a);
}

foo({1..3, 1..3});
foo({1..9});
var d: domain(int);
d += 1;
d += 2;
foo(d);
