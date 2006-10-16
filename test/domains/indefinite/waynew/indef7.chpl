param SIZE = 10;

var table: domain(int);
var odd: domain(int);
var A: [table] float;
writeln( A);
for d in 1..SIZE do 
  table.add( d);
writeln( A);
for d in 1..SIZE do A[d] = d + d:float/10.0;
writeln( A);

for d in 1..SIZE do 
  if d%2==1 then odd.add( d);
writeln( table);
table = odd;
writeln( table);
writeln( A);
