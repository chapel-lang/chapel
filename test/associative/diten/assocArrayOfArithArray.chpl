var D: domain(int);

for i in 1..2 {
  D += i;
}
var A: [D] [1..5] real;

[i in D] [j in 1..5] A(i)(j) = i + j/10.0;

for i in 1..2 {
  writeln(A(i));
}
