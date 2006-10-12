param SIZE = 10;

var idomain: domain(int);

for d in 1..SIZE do idomain.add( d);
var A: [idomain] float;
for d in 1..SIZE do A[d] = d + d:float/10.0;
writeln( A);


for d in 1..SIZE do 
  if (d%2 == 0) then idomain.remove( d);
writeln( idomain);
writeln( A);
