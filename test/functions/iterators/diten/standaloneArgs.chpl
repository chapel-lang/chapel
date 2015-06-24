config const n: int = 10;
config const ntasks: int = 2;

iter myiter(nn: int, nt: int) {
  writeln("Serial");
  for i in 0..#nt {
    for i in i*nn..#nn {
      yield i;
    }
  }
}

iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
  writeln("Standalone");
  coforall i in 0..#nt {
    for i in i*nn..#nn {
      yield i;
    }
  }
}

iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
  writeln("Leader");
  coforall i in 0..#nt {
    yield i*nn..#nn;
  }
}

iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
  writeln("Follower");
  for i in followThis {
    yield i;
  }
}

proc main {
  var A: [0..#ntasks*n] int;
  forall (i,j) in zip(myiter(n, ntasks), myiter(n, ntasks)) {
    A[i] = j;
  }

  forall i in myiter(n, ntasks) {
    A[i] += i;
  }

  for a in A do writeln(a);
}
