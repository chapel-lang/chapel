use driver;

config const n: int = 500;

for i in 1..n do
  foo(i);

proc foo(i: int) {
  const D: domain(2) dmapped Dist2D = {1..20, 1..20};
  var A: [D] int;

  A(2, 4) = 6;
  A(6, 8) = 10;

  const sum = + reduce A;

  if sum != 16 then
    halt("fail -- ", sum, " != 16");
}

writeln("success");
