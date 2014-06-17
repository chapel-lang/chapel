config const n = 10;
const D = {1..n, 1..n};
var DSps: sparse subdomain(D);
var A: [DSps] int;
A.IRV = -1;

writeln("A[D] is:");
for i in 1..n {
  write(A(i,1));
  for j in 2..n {
    write(" ", A(i,j));
  }
  writeln();
}

DSps = ((1,1), (3,3), (5,5), (7,7), (9,9));

writeln("A[D] is:");
for i in 1..n {
  write(A(i,1));
  for j in 2..n {
    write(" ", A(i,j));
  }
  writeln();
}
