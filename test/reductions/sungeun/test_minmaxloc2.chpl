
config const n=2;
config const m=2;

var A: [1..m, 1..n] real;

for (i,j) in {1..m, 1..n} do A(i,j) = i*0.2381+j*.0001;

writeln(A);
writeln(min reduce (A));
writeln(minloc reduce zip(A, A.domain));

for (i,j) in {1..m, 1..n} do A(i,j) = -i*0.2381-j*.0001;
writeln(A);
writeln(max reduce (A));
writeln(maxloc reduce zip(A, A.domain));
