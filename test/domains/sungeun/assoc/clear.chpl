config const n = 10;

var D: domain(int);
var A: [D] int;

for i in 1..n do D += i;
A = 1;
for i in D.sorted() do write((i, A[i]));
writeln();

for i in 1..n by 2 do D -= i;
for i in D.sorted() do write((i, A[i]));
writeln();

for i in 1..n do D += i;
A += 1;
for i in D.sorted() do write((i, A[i]));
writeln();

D.clear();
for i in 1..n do D += i;
for i in D.sorted() do write((i, A[i]));
writeln();
