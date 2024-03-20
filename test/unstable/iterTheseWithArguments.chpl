config param verbose = false;

proc myLog() do if verbose then writeln();
proc myLog(args...?k) do if verbose then writeln((...args));


//
// Records
//
record RecordTheseWithNoArg {
  const D = 0..10;
  const elms: [D] int = D*2;
  iter these() {
    myLog("serial");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind) where tag == iterKind.standalone {
    myLog("standalone");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind) where tag == iterKind.leader {
    myLog("leader");
    yield (D,);
  }
  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    myLog("follower ", followThis);
    const (followInds,) = followThis;
    for i in followInds {
      myLog("follower loop idx ", i);
      yield elms[i];
    }
  }
}
record RecordTheseWithArg {
  const D = 0..10;
  const elms: [D] int = D*2;
  iter these(n=1) {
    myLog("serial");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind, n) where tag == iterKind.standalone {
    myLog("standalone");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind, n) where tag == iterKind.leader {
    myLog("leader");
    yield (D,);
  }
  iter these(param tag: iterKind, n, followThis) where tag == iterKind.follower {
    myLog("follower ", followThis);
    const (followInds,) = followThis;
    for i in followInds {
      myLog("follower loop idx ", i);
      yield elms[i];
    }
  }
}
record RecordTheseWithDefaultArg {
  const D = 0..10;
  const elms: [D] int = D*2;
  iter these(n=1) {
    myLog("serial");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind, n=1) where tag == iterKind.standalone {
    myLog("standalone");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind, n=1) where tag == iterKind.leader {
    myLog("leader");
    yield (D,);
  }
  iter these(param tag: iterKind, n=1, followThis) where tag == iterKind.follower {
    myLog("follower ", followThis);
    const (followInds,) = followThis;
    for i in followInds {
      myLog("follower loop idx ", i);
      yield elms[i];
    }
  }
}

//
// Classes
//
class ClassTheseWithNoArg {
  const D = 0..10;
  const elms: [D] int = D*2;
  iter these() {
    myLog("serial");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind) where tag == iterKind.standalone {
    myLog("standalone");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind) where tag == iterKind.leader {
    myLog("leader");
    yield (D,);
  }
  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    myLog("follower ", followThis);
    const (followInds,) = followThis;
    for i in followInds {
      myLog("follower loop idx ", i);
      yield elms[i];
    }
  }
}
class ClassTheseWithArg {
  const D = 0..10;
  const elms: [D] int = D*2;
  iter these(n=1) {
    myLog("serial");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind, n) where tag == iterKind.standalone {
    myLog("standalone");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind, n) where tag == iterKind.leader {
    myLog("leader");
    yield (D,);
  }
  iter these(param tag: iterKind, n, followThis) where tag == iterKind.follower {
    myLog("follower ", followThis);
    const (followInds,) = followThis;
    for i in followInds {
      myLog("follower loop idx ", i);
      yield elms[i];
    }
  }
}
class ClassTheseWithDefaultArg {
  const D = 0..10;
  const elms: [D] int = D*2;
  iter these(n=1) {
    myLog("serial");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind, n=1) where tag == iterKind.standalone {
    myLog("standalone");
    for i in elms {
      yield i;
    }
  }
  iter these(param tag: iterKind, n=1) where tag == iterKind.leader {
    myLog("leader");
    yield (D,);
  }
  iter these(param tag: iterKind, n=1, followThis) where tag == iterKind.follower {
    myLog("follower ", followThis);
    const (followInds,) = followThis;
    for i in followInds {
      myLog("follower loop idx ", i);
      yield elms[i];
    }
  }
}


proc testNoArg(type T) {

  var r = new T();
  const D = r.D;

  myLog("for");
  for e in r {
    myLog(e);
  }
  myLog();

  myLog("forall");
  forall e in r {
    myLog(e);
  }
  myLog();

  myLog("forall zip first");
  forall (e,i) in zip(r,D) {
    myLog(e);
  }
  myLog();

  myLog("forall zip second");
  forall (i,e) in zip(D,r) {
    myLog(e);
  }

  myLog("for");
  for e in r.these() {
    myLog(e);
  }
  myLog();

  myLog("forall");
  forall e in r.these() {
    myLog(e);
  }
  myLog();

  myLog("forall zip first");
  forall (e,i) in zip(r.these(),D) {
    myLog(e);
  }
  myLog();

  myLog("forall zip second");
  forall (i,e) in zip(D,r.these()) {
    myLog(e);
  }
}

proc testArg(type T) {
  var r = new T();
  const D = r.D;

  // cannot invoke 'these' implicitly with arguments
  // myLog("for");
  // for e in r(17) {
  //   myLog(e);
  // }
  // myLog();

  // myLog("forall");
  // forall e in r(17) {
  //   myLog(e);
  // }
  // myLog();

  // myLog("forall zip first");
  // forall (e,i) in zip(r(17),D) {
  //   myLog(e);
  // }
  // myLog();

  // myLog("forall zip second");
  // forall (i,e) in zip(D,r(17)) {
  //   myLog(e);
  // }

  myLog("for");
  for e in r.these(17) {
    myLog(e);
  }
  myLog();

  myLog("forall");
  forall e in r.these(17) {
    myLog(e);
  }
  myLog();

  myLog("forall zip first");
  forall (e,i) in zip(r.these(17),D) {
    myLog(e);
  }
  myLog();

  myLog("forall zip second");
  forall (i,e) in zip(D,r.these(17)) {
    myLog(e);
  }
}

proc main() {

  testNoArg(RecordTheseWithNoArg);
  testArg(RecordTheseWithArg);

  testNoArg(RecordTheseWithDefaultArg);
  testArg(RecordTheseWithDefaultArg);



  testNoArg(ClassTheseWithNoArg);
  testArg(ClassTheseWithArg);

  testNoArg(ClassTheseWithDefaultArg);
  testArg(ClassTheseWithDefaultArg);
}

