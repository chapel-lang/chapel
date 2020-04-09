config const n = 1000;

var D: sparse subdomain({1..n});

for i in 1..n do
  if (i%3 == 0) || (i%7 == 0) then
    D += i;

var A: [D] int;

writeln("first forall");
forall i in D do
  A[i] = i;

writeln("second forall");
forall a in A do
  a += a;

for i in 1..n do
  if (i%3 == 0) || (i%7 == 0) then
    if A[i] != 2*i then
      writeln("incorrect value at position: ", i);
