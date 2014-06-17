var A, B: [1..100] bool;

for i in 1..100 by 3 do
  A(i) = true;

for param i in 1..100 by 3 do
  B(i) = true;

for i in 1..100 do
  if A(i) != B(i) then
    writeln("FAIL: i=", i, " A(i)=", A(i), " B(i)=", B(i));
