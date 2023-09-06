var D: domain(int);

for i in 1..2 {
  D += i;
}
var A: [D] [1..5] real;

[i in D with (ref A)] [j in 1..5 with (ref A)] A(i)(j) = i + j/10.0;

for i in 1..2 {
  writeln(A(i));
}
