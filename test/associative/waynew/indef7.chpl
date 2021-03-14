use printHelp;

param SIZE = 10;

var table: domain(int);
var odd: domain(int);
var A: [table] real;
writelnSortedByDom(A);
for d in 1..SIZE do 
  table.add( d);
writelnSortedByDom(A);
for d in 1..SIZE do A[d] = d + d:real/10.0;
writelnSortedByDom(A);

for d in 1..SIZE do 
  if d%2==1 then odd.add( d);
writelnSorted(table);
table = odd;
writelnSorted(table);
writelnSortedByDom(A);
