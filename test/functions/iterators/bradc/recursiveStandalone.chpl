iter myiter(r: range): int {
  halt("In the serial iterator!");
  for i in r do
    yield i;
}

iter myiter(r: range, param tag: iterKind): int 
       where tag == iterKind.standalone {
  if (r.size == 0) then return;
  for i in r do
    yield i;
  forall i in myiter(r.low..r.high-1) do
    yield i;
}

forall i in myiter(1..5) do
  write(i, " ");
