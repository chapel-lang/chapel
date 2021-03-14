
var DOM = {1..4};
var ARR = [555, 222, 222, 333];

proc writeme(arg1:int, arg2:int) { writeln(arg1, " w ", arg2); }

iter iterator() {
  for elm in ARR {
    yield elm;
  }
}

proc main {
  // simple loop, 1 index var
  writeln("AAA");
  [ idx in zip(iterator(),DOM) ] {
    writeln(idx);
  }

  // simple loop, 2 index vars
  writeln("BBB");
  [ (idx1,idx2) in zip(iterator(),DOM) ] {
    writeln(idx1,",",idx2);
  }

  // Ben's original code.
  writeln("CCC");
  const (bestVal, bestIndex) = minloc reduce zip(iterator(), DOM);
  writeln(bestVal, "@", bestIndex);

  writeln("DDD");
  // Rewritten as a forall loop.
  var bestResult = max(2*int);
  [idx in zip(iterator(),DOM) with (minloc reduce bestResult)] {
    writeln(idx);
    bestResult reduce= idx;
  }

  // Write the result.
  writeln("EEE");
  writeln(bestResult);

  // How about promotion?
  writeln("FFF");
  writeme(iterator(), DOM);

  // How about loop **expressions** ?
  writeln("GGG");
  [ idx1 in [(idx2,idx3) in zip(iterator(),DOM)] idx2+idx3 ]
    writeln(idx1);
}
