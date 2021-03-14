var A : [1..4, 1..4] real;
A(1,1) = 1.0;
for i in 2..4 do
  A(i,1) = A(i-1,1) + 1.0;
for i in 2..4 do
  A(1,i) = A(1,i-1) + 1.0;
for (i,j) in {2..4, 2..4} do
  A(i,j) = A(i-1,j-1) + 1.0;
writeln(A);

var B : [1..4] string;
B(1) = "X";
for i in 2..4 do
  B(i) = B(i-1) + "X";
writeln(B);

for a in A do
  writeln(a);

for b in B do
  writeln(b);
