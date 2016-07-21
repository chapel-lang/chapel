config param tryIllegal = true;

for i in 1..5 do
  writeln("hello #", i);
writeln();

var i: int;
for i in 1..5 do
  writeln(i);
writeln("i is: ", i);
writeln();

if tryIllegal then
  for i in 1..5 do
    i += 1;        // Illegal: i is 'const' for ranges

var A: [1..5] real;
for a in A do
  a = 1.2;
writeln(A);
writeln();

writeln("Printing five messages:");
for 1..5 do
  writeln("hi!");
writeln();

for i in 1..5 {
  writeln("Computing on A[", i, "]");
  A[i] += i;
}
writeln(A);
writeln();

for i in 1..5 do {
  writeln("Computing on A[", i, "]");
  A[i] += (i / 10.0);
}
writeln(A);
writeln();
