config const n = 1000;
var D: domain(string);

for i in 1..n do
  if (i%3 == 0) || (i%7 == 0) then
    D += i:string;

var A: [D] int;

writeln("first forall");
forall i in D do
  A[i] = i:int;

writeln("second forall");
forall a in A do
  a += a;

for i in 1..n do
  if (i%3 == 0) || (i%7 == 0) then
    if A[i:string] != 2*i then
      writeln("incorrect value at position: ", i);
