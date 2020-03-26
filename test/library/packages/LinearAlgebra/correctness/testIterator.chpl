use LinearAlgebra;

config const n=5;

var A = Matrix(n);
for (i,j) in {1..n,1..n} {
    A[i,j] = (i-1)*n + j;
}
writeln(A);
writeln();

for a in trilIter(A) {
    write(a, ' ');
}
writeln();
for a in trilIter(A, -1) {
    write(a, ' ');
}
writeln();
for a in triuIter(A, 1) {
    write(a, ' ');
}
writeln();
