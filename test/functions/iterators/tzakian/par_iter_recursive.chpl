iter countdown(i:int):int {
  if (i -1 > 0) {
    var tmp = i -1;
    for j in countdown(tmp) do
      yield j;
  }
  yield i;
}

iter countdown(param tag:iterKind, i:int):int where tag == iterKind.leader {
  if (i -1 > 0) {
    var tmp = i -1;
    forall j in countdown(tmp) do
      yield j;
  }
  yield i;
}

iter countdown(param tag:iterKind, i:int, followThis):int where tag == iterKind.follower { 
  yield followThis;
}

forall i in countdown(10) do
  writeln(1);
