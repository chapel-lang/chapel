proc foo(X: [?XD] int) {
  writeln("X is: ", X);
}

const D = {1..6};

var A: [D] int;

for i in D do
  A(i) = i;

foo(A[1..3]);       // this doesn't work

foo(A[D by 2]);     // this does!?

foo(A[2..6 by 2]);  // this doesn't

const D2 = {2..6 by 2};
foo(A[D2]);          // this does

const D3 = {1..3};
foo(A[D3]);          // as does this
