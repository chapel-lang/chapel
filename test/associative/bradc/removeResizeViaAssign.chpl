config const n = 64;

use printHelp;

var D, D2: domain(int);

var A: [D] real;

for i in 1..n {
  D += i;
  A(i) = i;
}

for i in 1..n by 16 do
  D2 += i;

writelnSorted(D);
writelnSortedByDom(A);
writelnSorted(D2);

D = D2;

writelnSorted(D);
writelnSortedByDom(A);
