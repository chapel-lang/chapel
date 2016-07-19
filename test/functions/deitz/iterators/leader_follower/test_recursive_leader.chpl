iter foo(n: int) {
  for i in 1..n do
    yield i;
}

extern proc printf(fmt:c_string);
iter foo(param tag: iterKind, n: int):range where tag == iterKind.leader {
  printf(c"In leader\n");
  if n == 1 then
    yield 1..1;
  else {
    for i in foo(iterKind.leader, n/2) do
      yield i;
    yield n/2..n;
  }
}

iter foo(param tag: iterKind, followThis, n: int) where tag == iterKind.follower {
  printf(c"In follower\n");
  for i in followThis do
    yield i;
}

config var n: int = 8;

var A: [1..n] int;

for i in foo(n) {
  A(i) = 1;
}

writeln(A);

forall i in foo(n) {
  A(i) = 2;
}

writeln(A);
