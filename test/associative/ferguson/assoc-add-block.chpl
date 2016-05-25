use CommDiagnostics;
use Time;
use UserMapAssoc;
use BlockDist;

config const n = 10000;
config const checkMemberOnAdd = false;
config const timing = false;
config const verify = true;

class MyMapper{
  proc indexToLocaleIndex(ind, targetLocs: [] locale) : int {
    const numlocs = targetLocs.size;
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }
}


proc run() {
  
  var BlockSpace = {1..n} dmapped Block(boundingBox={1..n});
  var numbers:[BlockSpace] int;

  var Dist = new UserMapAssoc(idxType=int, mapper = new MyMapper());
  var d:domain(int) dmapped new dmap(Dist);

  forall (i, n) in zip(BlockSpace, numbers) {
    n = i;
  }

  resetCommDiagnostics();
  startCommDiagnostics();

  var t: Timer;
  t.start();

  forall n in numbers with (ref d) {
    d += n;
  }

  t.stop();

  stopCommDiagnostics();

  if timing then
    writeln("Adding ", n, " elements took ", t.elapsed());

  writeln(getCommDiagnostics());

  writeln("d.size is ", d.size);

  if verify {
    for i in 1..n {
      if !d.member(i) then
        writeln("Error: d does not contain ", i);
    }

    var i = 1;
    for j in d.sorted() {
      assert(j == i);
      i += 1;
    }
  }
}

run();
