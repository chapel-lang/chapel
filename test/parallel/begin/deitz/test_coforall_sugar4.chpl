config const n: int = 12;

var A: [1..n] int = 1000;

for e in A do
  if e != 1000 then
    halt("fail: ", A);

coforall i in 1..n {
  A(i) = 2000+i;
}

for (e,i) in zip(A,2001..) do
  if e != i then
    halt("fail: ", A);

var count$: sync int = 0,
    flag$: sync bool = true;

for i in 1..n {
  const count = count$.readFE();
  if count == 0 then
    flag$.readFE();
  count$.writeEF(count + 1);
  begin {
    A(i) = 3000+i;
    const count = count$.readFE();
    if count == 1 then
      flag$.writeEF(true);
    count$.writeEF(count - 1);
  }
}

flag$.readFE();

for (e,i) in zip(A,3001..) do
  if e != i then
    halt("fail: ", A);

writeln("success: ", A);
