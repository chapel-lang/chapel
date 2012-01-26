// An example of how to create task private variables.
//
// This example should work for forall loop iterating over default
// arrays/domains (you could drop the dmap), as well as Block, Cyclic,
// and probably BlockCyclic arrays/domains.
//
use BlockDist;

record taskPrivateData {
  var x: int;
  var y: [0..#numLocales] real;
};

class localePrivateData {
  type myStuff;
  var tid: sync int = 0;
  var r: domain(1);
  var temps: [r] myStuff;
}

const myD = [LocaleSpace] dmapped Block(boundingBox=[LocaleSpace]);
var localePrivate: [myD] localePrivateData(taskPrivateData);

forall l in localePrivate do l = new localePrivateData(taskPrivateData);

// assumes numCores is the same across all locales (prolly should use max)
var numTasks = if dataParTasksPerLocale==0 then here.numCores
  else dataParTasksPerLocale;
localePrivate.r = [0..#numTasks];


// an example use
config param nPerLocale = 8;
config const printTemps = false;
const D = [0..#nPerLocale*numLocales] dmapped Block(boundingBox=[0..#nPerLocale*numLocales]);
forall d in D {
  // my copy of the task private vars
  var lp = localePrivate[here.id];

  // int-fetch-add
  var tid = lp.tid;
  lp.tid = tid+1;

  tid = tid % numTasks;

  lp.temps[tid].x += tid+1;
  lp.temps[tid].y[here.id] += 1;
}

if printTemps then writeln(localePrivate.temps);

for l in 0..#numLocales {
  var lp = localePrivate[l];
  var y =  0.0;
  for tid in 0..#numTasks {
    y += + reduce lp.temps[tid].y;
    for l2 in 0..#numLocales {
      if l != l2 {
        if lp.temps[tid].y[l2] != 0.0 then
          halt("localePrivate[",l,"].temps[",tid,"].y[", l2, "] is not 0.0!");
      }
    }
  }
  if y != nPerLocale then
    halt("localePrivate[",l,"].temps[].y value is incorrect!");
}
