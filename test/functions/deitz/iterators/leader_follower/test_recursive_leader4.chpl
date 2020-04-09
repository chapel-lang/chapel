use Time;

iter bar(n: int, i: int) {
  for i in i..n do
    yield i;
}

iter bar(param tag: iterKind, n: int, i: int): int where tag == iterKind.leader {
  if i == 1 then
    coforall j in 2..n do
      on Locales((j-1)%numLocales) do
        for k in bar(tag, n, j) do
          yield k;
  yield i;
}

iter bar(param tag: iterKind, followThis, n: int, i: int) where tag == iterKind.follower {
  yield followThis;
}

config const n: int = 8;

var A: [1..n] int;

forall i in bar(n, 1) do
  A(i) = i;

writeln(A);
