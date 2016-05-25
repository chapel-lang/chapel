use CommDiagnostics;
use Time;
use UserMapAssoc;

config const n = 10000;
config const checkMemberOnAdd = false;
//config const timing = false;

class MyMapper{
  proc indexToLocaleIndex(ind, targetLocs: [] locale) : int {
    const numlocs = targetLocs.size;
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }
}

proc process(r, ref d)
{
  var i:int;
  while( r.readln(i) ) {
    d += i;
  }
}

proc run() {
  
  var Dist = new UserMapAssoc(idxType=int, mapper = new MyMapper());
  var d:domain(int) dmapped new dmap(Dist);

  resetCommDiagnostics();
  startCommDiagnostics();

  var t: Timer;
  t.start();

  coforall loc in Locales {
    on loc {
      var f = opentmp();
      var w = f.writer();
      for i in 1..n {
        w.writeln(i);
      }
      w.close();

      var r = f.reader();
      process(r, d);
    }
  }

  t.stop();

  stopCommDiagnostics();

  writeln("Adding ", n, " elements took ", t.elapsed());

  writeln(getCommDiagnostics());

  writeln("d.size is ", d.size);

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

run();
