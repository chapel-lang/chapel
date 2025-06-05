// Tests behavior when a serial iterator is declared unstable (but none of the
// others are)
var rangeToUse = 0..9;

var triggeredFollower: atomic bool = false;

proc main() {
  writeln("testing call of serial");
  for i in foo() {
    writeln(i);
  }

  writeln("testing call of standalone");
  var arr1: [rangeToUse] int;
  forall i in foo() {
    arr1[i] = i;
  }
  writeln(arr1);

  writeln("testing call of leader (and follower)");
  var arr2: [rangeToUse] int;
  forall (val, loc) in zip(foo(), arr2) {
    loc = val*2;
  }
  writeln(arr2);

  triggeredFollower.clear();

  writeln("testing call of just follower");
  var arr3: [rangeToUse] int;
  forall (loc, val) in zip(arr3, foo()) {
    loc = val*3;
  }
  writeln(arr3);
}

@unstable("The serial version of foo is unstable")
iter foo() {
  writeln("in the serial iterator");
  for i in rangeToUse {
    yield i;
  }
}

iter foo(param tag) where tag == iterKind.standalone {
  writeln("in the standalone iterator");
  for i in rangeToUse {
    yield i;
  }
}

// Stolen from the parallel iterators primer
proc computeChunk(r: range, myChunk, numChunks) {
  const numElems = r.size;
  const elemsperChunk = numElems/numChunks;
  const rem = numElems%numChunks;
  var mylow = r.low;
  if myChunk < rem {
    mylow += (elemsperChunk+1)*myChunk;
    return mylow..#(elemsperChunk + 1);
  } else {
    mylow += ((elemsperChunk+1)*rem + (elemsperChunk)*(myChunk-rem));
    return mylow..#elemsperChunk;
  }
}

// Partially stolen from the parallel iterators primer
iter foo(param tag) where tag == iterKind.leader {
  writeln("in the leader iterator");

  coforall tid in 0..#here.maxTaskPar {
    const myIters = computeChunk(rangeToUse, tid, here.maxTaskPar);
    const zeroBasedIters = myIters.translate(-rangeToUse.low);

    yield (zeroBasedIters,);
  }
}

// Also stolen from the parallel iterators primer
iter foo(param tag, followThis) where tag == iterKind.follower {
  if (triggeredFollower.testAndSet() == false) {
    writeln("in the follower iterator at least once");
  }
  const (followInds,) = followThis;
  const lowBasedIters = followInds.translate(rangeToUse.low);

  for i in lowBasedIters do
    yield i;
}
