proc foo(X: [?XD] int) {
  X *= -1;
}

const D = {1..6};

var A: [D] int;

for i in D do
  A(i) = i;

writeln("A is: ", A);

foo(A[1..3]);       // this doesn't work

writeln("A is: ", A);

foo(A[D by 2]);     // this does!?

writeln("A is: ", A);

foo(A[2..6 by 2]);  // this doesn't

writeln("A is: ", A);

const D2 = {2..6 by 2};
foo(A[D2]);          // this does

writeln("A is: ", A);

const D3 = {1..3};
foo(A[D3]);          // as does this

writeln("A is: ", A);


const DD = {1..3};

var B: [DD] int;

for i in DD do
  B(i) = i;

writeln("B is: ", B);

foo(B);

writeln("B is: ", B);
