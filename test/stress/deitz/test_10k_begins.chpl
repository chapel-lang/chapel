use Time;

config const n = 10000;

var A: [1..n] int;

sync {
  for i in 1..n {
    begin {
      A(i) += 1;
    }
  }
}

for i in 1..n do
  if A(i) != 1 then
    halt("error detected");

writeln("success");
