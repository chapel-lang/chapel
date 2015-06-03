config const n: int = 10;
config const ntasks: int = 2;

// from test/functions/iterators/diten/standaloneArgs.chpl

iter myiter(nn: int, nt: int) {
  writeln("Serial");
  for i in 0..#nt {
    for i in i*nn..#nn {
      yield i;
    }
  }
}

iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
  writeln("Standalone, with args");
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

// from test/functions/iterators/diten/standaloneParallelIter.chpl

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
  writeln("Standalone, no args");
  coforall i in 0..#ntasks {
    for i in i*n..#n {
      yield i;
    }
  }
}

proc checkNoIntent() {
  var xxx = 222;
  var sss$, qqq$: sync int;
  cobegin with (ref xxx) {
    {
      sss$;
      xxx = 333;
      writeln("  updated to: ", xxx);
      qqq$ = 1;
    }
    // 'xxx' is passed implicitly by blank intent
    forall iii in myiter(1,1) {
      sss$ = 1; // enable update to xxx
      qqq$;     // wait for the update to complete
      writeln("  within forall: ", xxx);
    }
  }
  writeln("  after forall: ", xxx);
}

proc checkRefIntent() {
  var xxx = 100;
  var sss$: sync int = 1;

  // 'xxx' is passed implicitly by blank intent
  forall iii in myiter() with (ref xxx) {
    sss$;
    xxx += 1;
    writeln(xxx);
    sss$ = 1;
  }
  writeln("After forall: ", xxx);
}


proc main {
  checkNoIntent();
  checkRefIntent();
}
