use List;

var A: [3..6] real = [1.2, 3.4, 5.6, 7.8];
var t = (1.2, 3.4);
var s = "brad";
var b = b"bradford";
const D = {1..3, 1..3};
var SD: sparse subdomain(D) = [i in 1..3] (i,i);
var SA: [SD] real;
var l: list(int);
l.append(1);
l.append(2);
l.append(3);


[(i,j) in SD] SA[i,j] = i + j / 10.0;

var M = ["brad" => 1, "ben" => 2, "bilbo" => 3];

for i in A.indices do
  writeln(A[i]);

for i in t.indices do
  writeln(t(i));

for i in s.indices do
  writeln(s[i]);

for i in b.indices do
  writeln(b[i]);

for i in SA.indices do
  writeln(SA[i]);

for i in l.indices do
  writeln(l[i]);

writeln(A.indices);
writeln(t.indices);
writeln(s.indices);
writeln(b.indices);
writeln(SA.indices);
writeln(l.indices);
