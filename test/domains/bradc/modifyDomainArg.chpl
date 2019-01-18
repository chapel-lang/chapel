config var n = 9;

var D = {1..n, 1..n};

var A: [D] real = [(i,j) in D]i + j/10.0;

writeln("A is:\n", A);

foo(D);

proc foo(ref dom) {
  dom = {1..n/2, 1..n/2};
  writeln("dom is: ", dom);
  writeln("D is: ", D);
  writeln("A is:\n", A);
}

