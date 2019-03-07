

var AR1 = [20,50,30];
var AR2 = [12,15,13 ];

proc writeme(arg1:int, arg2:int) { writeln(arg1, " w ", arg2); }

iter onlyS() {
  writeln("onlyS serial");
  for s in AR1 do yield s;
}

iter haveLF() {
  writeln("haveLF serial");
  for s in AR2 do yield s;
}

iter haveLF(param tag) where tag == iterKind.leader {
  writeln("haveLF leader");
  yield 777;
}

iter haveLF(param tag, followThis) where tag == iterKind.follower {
  writeln("haveLF follower");
  for s in AR2 do yield s;
}

// like zippered-serial.chpl
proc testSerPar() {
  // simple loop, 1 index var
  writeln("AAA");
  [ idx in zip(onlyS(),haveLF()) ] {
    writeln(idx);
  }

  // simple loop, 2 index vars
  writeln("BBB");
  [ (idx1,idx2) in zip(onlyS(),haveLF()) ] {
    writeln(idx1,",",idx2);
  }

  // Ben's original code.
  writeln("CCC");
  const (bestVal, bestIndex) = minloc reduce zip(onlyS(),haveLF());
  writeln(bestVal, "@", bestIndex);

  writeln("DDD");
  // Rewritten as a forall loop.
  var bestResult = max(2*int);
  [idx in zip(onlyS(),haveLF()) with (minloc reduce bestResult)] {
    writeln(idx);
    bestResult reduce= idx;
  }

  // Write the result.
  writeln("EEE");
  writeln(bestResult);

  // How about promotion?
  writeln("FFF");
  writeme(onlyS(),haveLF());
}

// like zippered-par-serial.chpl
proc testParSer() {
  // simple loop, 1 index var
  writeln("AAA");
  [ idx in zip(haveLF(),onlyS()) ] {
    writeln(idx);
  }

  // simple loop, 2 index vars
  writeln("BBB");
  [ (idx1,idx2) in zip(haveLF(),onlyS()) ] {
    writeln(idx1,",",idx2);
  }

  // Ben's original code.
  writeln("CCC");
  const (bestVal, bestIndex) = minloc reduce zip(haveLF(),onlyS());
  writeln(bestVal, "@", bestIndex);

  writeln("DDD");
  // Rewritten as a forall loop.
  var bestResult = max(2*int);
  [idx in zip(haveLF(),onlyS()) with (minloc reduce bestResult)] {
    writeln(idx);
    bestResult reduce= idx;
  }

  // Write the result.
  writeln("EEE");
  writeln(bestResult);

  // How about promotion?
  writeln("FFF");
  writeme(haveLF(),onlyS());
}

testSerPar();
testParSer();
