config var n = 9;

var D = [1..n, 1..n];

var A: [(i,j) in D] real = i + j/10.0;

writeln("A is:\n", A);

foo(D);

def foo(dom) {
  dom = [1..n/2, 1..n/2];
  writeln("dom is: ", dom);
  writeln("D is: ", D);
  writeln("A is:\n", A);
}

