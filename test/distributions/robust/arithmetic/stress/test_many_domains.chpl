use driver;

config const n: int = 500;

for i in 1..n do
  foo(i);

proc foo(i: int) {
  const modi = i % 20;

  const D: domain(2) dmapped Dist2D = {1..modi, 1..modi};

  const sum = + reduce [(i,j) in D] i;

  const expectedSum = modi * + reduce [j in 1..modi] j;

  if sum != expectedSum then
    halt("fail -- ", sum, " != ", expectedSum);
}

writeln("success");
