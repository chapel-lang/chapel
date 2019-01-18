use Time;

var A: [1..0] int, a: int;

while stdin.read(a) do
  A.push_back(a);

coforall a in A {
  sleep(a);
  writeln(a);
}
