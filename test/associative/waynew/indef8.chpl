use printHelp;

var idomain: domain(int);
var A: [idomain] real;
var B: [idomain] real;

idomain += 1;
idomain += 2;
idomain += 3;
idomain += 4;
A[3] = 1.2;
A[4] = 3.4;
B[1] = 5.6;
writelnSortedByDom(A);
writelnSortedByDom(B);
B = A;
writelnSortedByDom(B);
