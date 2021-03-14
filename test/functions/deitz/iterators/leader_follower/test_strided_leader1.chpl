iter foo(n: int) {
  for i in 1..n do
    yield i;
}

iter foo(param tag: iterKind, n: int) where tag == iterKind.leader {
  cobegin {
    on Locales(0) do yield (0..n-1 by 2,);
    on Locales(1) do yield (1..n-1 by 2,);
  }
}

iter foo(param tag: iterKind, followThis, n: int) where tag == iterKind.follower {
  for i in followThis(0)+1 do
    yield i;
}

config var n: int = 8;

var A: [1..n] int;

forall i in foo(n) do
  A(i) = here.id * 100 + i;

writeln(A);

use Random;

{
  var B: [1..n] real;

  var rs = new owned NPBRandomStream(real, seed=315);

  forall (i, r) in zip({1..n}, rs.iterate({1..n})) do
    B(i) = r;

  writeln(B);
}

{
  var B: [1..n] real;

  var rs = new owned NPBRandomStream(real, seed=315);

  forall (f, r) in zip(foo(n), rs.iterate({1..n})) do
    B(f) = r;

  writeln(B);
}
