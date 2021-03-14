proc foo(A: [] int) {
  for i in A.domain do
    writeln(A(i));
}

var D: domain(2) = {1..3, 1..4};
var A: [D] int;

var val = 0;
for i in D {
  A(i) = val;
  val += 1;
}

writeln(A);
foo(A);
