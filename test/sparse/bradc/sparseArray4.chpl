config var n: int = 5;

const D = {0..n+1, 0..n+1};
var S: sparse subdomain(D);

var A: [S] real;

for i in 1..n by -1 {
  S += (i,i);
  A(i,i) = i;
}

for i in 0..n+1 {
  for j in 0..n+1 {
    write(A(i,j), " ");
  }
  writeln();
}
