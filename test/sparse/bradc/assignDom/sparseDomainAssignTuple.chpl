config var n: int = 5;

const D = {0..n+1, 0..n+1};
var S: sparse subdomain(D);
S = ( (1,1), (2,2), (3,3), (4,4), (5,5) ); 

var A: [S] real;

for i in 1..n {
  A(i,i) = i;
}

for (i,j) in S {
  writeln("[", i, ", ", j, "]: ", A(i,j));
}
