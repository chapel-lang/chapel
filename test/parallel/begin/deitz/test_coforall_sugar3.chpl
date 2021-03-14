use Time;

config var n = 5;

var A: [1..n+1] int;

A(n+1) = n+1;

coforall i in 1..n {
  while A(i+1) != i+1 do
    sleep(1);
  A(i) = i;
}

writeln(A);
