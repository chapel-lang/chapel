use printHelp;

var id1: domain(int);
var id2: domain(int);
var A: [id1] real;
var B: [id2] real;

id1 += 1;
id1 += 2;
id1 += 3;
id2 += 1;
id2 += 2;
id2 += 4;
A[2] = 1.2;
A[3] = 3.4;
B[1] = 5.6;
writelnSortedByDom(A);
writelnSortedByDom(B);
B = A;
writelnSorted(B); // can't write sorted by domain because we can't anticipate
                  // the order that B's elements will be in
