var A: [1..4] int;

for i in 2..4 do
  A(i) = new A(i-1);
