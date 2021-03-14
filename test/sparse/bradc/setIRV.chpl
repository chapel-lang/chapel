config const n = 10;
const D = {1..n, 1..n};
const DSps: sparse subdomain(D) = [i in 1..n] (i,i);
var A: [DSps] int = [(i,j) in DSps] i*10 + j;
A.IRV = -1;

writeln("A[D] is:");
for i in 1..n {
  write(A(i,1));
  for j in 2..n {
    write(" ", A(i,j));
  }
  writeln();
}
