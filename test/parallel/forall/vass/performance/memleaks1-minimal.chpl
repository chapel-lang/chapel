use MemDiagnostics;
const RELOC = Locales[0];

proc useMe(arg: int) { }

proc main() {
  var MY_VAR = 1;
  const m1 = memoryUsed();

  {
    forall idx in MYITER() do
      useMe(MY_VAR);
  }

  const m2 = memoryUsed();
  writeln("leaked: ", m2-m1);
}

iter MYITER() {
  yield 55555;
}

iter MYITER(param tag) where tag == iterKind.leader {
  for followThis in OTHER_ITER(tag) do  // redirect to another leader
    yield 88888;
}
iter MYITER(param tag, followThis) where tag == iterKind.follower {
  yield 66666;
}

iter OTHER_ITER(param tag) where tag == iterKind.leader {
  cobegin {
    on RELOC do
      yield 77777;
    var jjjjj: int;
  }
}
