use printHelp;

// Test that domain assignment resizes+copies arrays correctly.

param N = 40;

var id1: domain(int);
var id2: domain(int);
var A: [id1] real;
var B: [id2] real;

for i in 1..(3*N)/4 {
  id1 += i;
  A[i] = i + 0.1;
}

for i in N/4..N {
  id2 += i;
  B[i] = i + 0.2;
}

// writeln( id1);
// writeln( A);
// writeln( id2);
// writeln( B);

id2 = id1;

// writeln( B);

B[N/2] = 1234.5;

for i in N/4..(3*N)/8 {
  id2 -= i;
}

for i in N..2*N {
  id2 += i;
  B[i] = i + 0.2;
}

writelnSortedByDom( B);
writeln( "B[", N/2, "]=", B[N/2]);



