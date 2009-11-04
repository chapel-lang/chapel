use Time;

def bar(n: int, i: int) {
  for i in i..n do
    yield i;
}

def bar(param tag: iterator, n: int, i: int): int where tag == iterator.leader {
  if i == 1 then
    coforall j in 2..n do
      on Locales((j-1)%numLocales) do
        for k in bar(tag, n, j) do
          yield k;
  yield i;
}

def bar(param tag: iterator, follower, n: int, i: int) where tag == iterator.follower {
  yield follower;
}

config const n: int = 8;

var A: [1..n] int;

forall i in bar(n, 1) do
  A(i) = i;

writeln(A);
