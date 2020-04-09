proc f(in D: domain(1)) {
  writeln(D);
  D = {1..10};
}

var D = {1..5};
f(D);
writeln(D);
