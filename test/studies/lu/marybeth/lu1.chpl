//  Simple LU algorithm with pivoting.
//  No attempt to shorten code with array indexing features.
//  This code will serve as baseline test for other Chapel versions.

param n = 3;

const AD = {1..n,1..n};
const Vec = {1..n};
var A: [AD] real;
var piv: [Vec] int;
var colmax: real;
var temp: real;
var ind, itemp: int;

initA(A);
[i in Vec] piv(i) = i;

writeln("Unfactored Matrix:");
writeln(A);

for k in 1..n-1 {
  colmax = abs(A(k,k));
  for i in k+1..n {
    if (abs(A(i,k)) > colmax) {
      colmax = abs(A(i,k));
      ind = i;
    }
  }
  if (ind != k) {
    itemp = piv(k);
    piv(k) = piv(ind);
    piv(ind) = itemp;
    for i in 1..n {
      temp = A(k,i);
      A(k,i) = A(ind,i);
      A(ind,i) = temp;
    }
  }
  if (A(k,k) != 0.0) { 
    for i in k+1..n {
      A(i,k) = A(i,k)/A(k,k);
    }
  }
  for i in k+1..n {
    for j in k+1..n {
      A(i,j) = A(i,j) - A(i,k)*A(k,j);
    }
  }
}
writeln();
writeln("Factored Matrix:");
writeln(A);
writeln();
writeln("Pivot Vector:");
writeln(piv);


proc initA(A){

A(1,1) = 3.0;
A(1,2) = 17.0;
A(1,3) = 10.0;
A(2,1) = 2.0;
A(2,2) = 4.0;
A(2,3) = -2.0;
A(3,1) = 6.0;
A(3,2) = 18.0;
A(3,3) = -12.0;

}
