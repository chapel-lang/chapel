iter myiter(r:range): int {
  if r.size == 0 then return;
  for i in r do yield i;
  for i in myiter(r.low..r.high-1) do yield i;
}

iter myiter(r:range, param tag: iterKind): int
  where tag == iterKind.leader {
  if r.size == 0 then return;
  for i in r do yield i;
  forall i in myiter(r.low..r.high-1) do yield i;
}

iter myiter(followThis, param tag: iterKind): int
  where tag == iterKind.follower {
  yield followThis;
}

forall i in myiter(1..5) do
  write(i, " ");
writeln();

