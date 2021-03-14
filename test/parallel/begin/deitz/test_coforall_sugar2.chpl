use Time;

config var n: int = 5;

var A: [1..n] int;

coforall i in 1..n {
  sleep(i:uint);
  A(i) = i;
}

writeln(A);
sleep(2);
writeln(A);
