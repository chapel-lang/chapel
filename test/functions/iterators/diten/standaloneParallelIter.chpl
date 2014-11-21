config const n: int = 4;
config const ntasks: int = 4;

iter myiter() {
  writeln("Serial");
  for i in 0..#n*ntasks do yield i;
}

iter myiter(param tag:iterKind) where tag == iterKind.leader {
  writeln("Leader");
  coforall i in 0..#ntasks {
    const tmp = i*n..#n;
    yield tmp;
  }
}

iter myiter(param tag:iterKind, followThis: range) where tag == iterKind.follower {
  writeln("Follower");
  for i in followThis {
    yield i;
  }
}

iter myiter(param tag:iterKind) where tag == iterKind.standalone {
  writeln("Standalone");
  coforall i in 0..#ntasks {
    for i in i*n..#n {
      yield i;
    }
  }
}

proc main {
  use Sort;
  var count: atomic int;
  var A: [0..#ntasks*n] int;
  count.write(0);

  // standalone
  forall i in myiter() {
    const curr = count.fetchAdd(1);
    A[i] = curr;
  }

  for i in sorted(A) do
    writeln(i);

  // leader-follower zippered
  forall (i,j) in zip(myiter(), myiter()) {
    const curr = count.fetchAdd(1);
    A[i] = curr;
  }

  for i in sorted(A) do
    writeln(i);

  // serial
  for i in myiter() {
    const curr = count.fetchAdd(1);
    A[i] = curr;
  }

  for i in sorted(A) do
    writeln(i);
}

