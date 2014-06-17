use Time;

const n = 5;

var a: [1..n] int;

for i in 1..n do begin {
  a[i] = foo(i);
}

for i in 1..n {
  while a[i] == 0 do
    sleep(1);
  writeln(a[i]);
}

proc foo(i: int) return i**2;
