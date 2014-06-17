config var n: int = 5;

const D = {1..n, 1..n};
var S: sparse subdomain(D);

for i in 1..n do
  S += (i,i);

var A: [S] real;

for i in 1..n do
  A(i,i) = i;

for i in 1..n {
  for j in 1..n {
    write(A(i,j), " ");
  }
  writeln();
}
