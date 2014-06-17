use printHelp;

// Test the resizing via add (_double) and domain assignment (_double and
// _half).

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

writelnSorted( id1);
writelnSortedByDom( A);
writelnSorted( id2);
writelnSortedByDom( B);

id2 = id1;

writelnSortedByDom( B);
