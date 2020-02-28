var A: [3..6] real = [1.2, 3.4, 5.6, 7.8];
var t = (1.2, 3.4);
var s = "brad";
var b = b"bradford";
const D = {1..9, 1..9};
var SD: sparse subdomain(D) = [i in 1..9] (i,i);
var SA: [SD] real;

[(i,j) in SD] SA[i,j] = i + j / 10.0;

var M = ["brad" => 1, "ben" => 2, "bilbo" => 3];

for i in A.indices() do
  writeln(A[i]);

for i in t.indices() do
  writeln(t(i));

for i in s.indices() do
  writeln(s[i]);

for i in b.indices() do
  writeln(b[i]);

for i in SA.indices() do
  writeln(SA[i]);

/*
writeln((A.lowIdx, A.highIdx));
writeln(t.indices);
writeln((t2.lowIdx, t2.highIdx));
writeln((s.lowIdx, s.highIdx));
writeln((b.lowIdx, b.highIdx));
*/