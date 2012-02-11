// An example of how to create task private variables.
//
// This example should work for forall loop iterating over default
// arrays/domains (you could drop the dmap), as well as Block, Cyclic,
// and probably BlockCyclic arrays/domains.
//
use BlockDist, PrivateDist;

record taskPrivateData {
  var inUse: bool = false;
  var x: int;
  var y: [0..#numLocales] real;
};

class localePrivateData {
  type myStuff;
  // assumes numCores is the same across all locales
  const numTasks = if dataParTasksPerLocale==0 then here.numCores
    else dataParTasksPerLocale;
  var tid: sync int = 0;
  var r = [0..#numTasks];
  var temps: [r] myStuff;
  proc gettid() {
    var mytid = tid;
    while temps[mytid%numTasks].inUse do mytid += 1;
    temps[mytid%numTasks].inUse = true;
    tid = mytid+1;
    return mytid%numTasks;
  }
  proc freetid(mytid) {
    tid; // just use as a lock
    temps[mytid].inUse = false;
    tid = mytid+1;
  }
}

var localePrivate: [PrivateSpace] localePrivateData(taskPrivateData);
forall l in localePrivate do l = new localePrivateData(taskPrivateData);

// an example use
config param nPerLocale = 100;
config const printTemps = false;
const D = [0..#nPerLocale*numLocales] dmapped Block(boundingBox=[0..#nPerLocale*numLocales]);
forall d in D {
  // my copy of the task private vars
  var lp = localePrivate[here.id];

  // int-fetch-add
  var tid = lp.gettid();

  lp.temps[tid].x += 1;
  lp.temps[tid].y += 1;

  lp.freetid(tid);
}

if printTemps then writeln(localePrivate.temps);

const numTasks = if dataParTasksPerLocale==0 then here.numCores
  else dataParTasksPerLocale;
for l in 0..#numLocales {
  var lp = localePrivate[l];
  var x = 0;
  var y: [D] real = 0.0;
  for tid in 0..#numTasks {
    x += lp.temps[tid].x;
    for i in 0..#numLocales do y[i] += lp.temps[tid].y[i];
  }
  if x != nPerLocale {
    for tid in 0..#numTasks do writeln(lp.temps[tid].x);
    halt("localePrivate[",l,"].temps[].x value is incorrect!");
  }
  for i in 0..#numLocales {
    if y[i] != nPerLocale {
      for tid in 0..#numTasks do writeln(lp.temps[tid].y);
      halt("localePrivate[",l,"].temps[].y values are incorrect!");
    }
  }
}
