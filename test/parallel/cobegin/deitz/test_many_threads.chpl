use Time;

config var n: int = 512;
config var s: uint = 4;

var A: [1..n] int;

proc sleepAndWrite(i: int) {
  sleep(s);
  A(i) = 1;
}

proc foo(i: int) {
  cobegin {
    sleepAndWrite(i);
    if i < n then
      foo(i+1);
  }
}

foo(1);

writeln(A);
