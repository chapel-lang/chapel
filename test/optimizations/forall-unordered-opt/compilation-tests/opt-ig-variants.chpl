use CyclicDist;
use BlockDist;
use Random;
use Time;
use UnorderedCopy;

const numTasksPerLocale = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                       else here.maxTaskPar;
const numTasks = numLocales * numTasksPerLocale;
config const N = 1000000;
config const M = 10000;

const numUpdates = N * numTasks;
const tableSize = M * numTasks;

const Mspace = {0..tableSize-1};
const D = Mspace dmapped Cyclic(startIdx=Mspace.low);
var A: [D] int = 0..tableSize-1;

const Nspace = {0..numUpdates-1};
const D2 = Nspace dmapped Block(Nspace);
var rindex: [D2] int;

fillRandom(rindex, 1);
rindex = mod(rindex, tableSize);

var tmp: [D2] int;

proc test1l() {
  var A: [0..#M] int = 0..#M;
  var rindex: [0..#N] int;
  var tmp: [0..#N] int;

  forall i in 0..#N {
    tmp.localAccess[i] = A[rindex.localAccess[i]];
  }
}
test1l();

proc test1() {
  forall i in D2 do
    tmp.localAccess[i] = A[rindex.localAccess[i]];
}
test1();

proc test2l() {
  var A: [0..#M] int = 0..#M;
  var rindex: [0..#N] int;
  var tmp: [0..#N] int;

  forall i in 0..#N {
    tmp.localAccess[i] = A[rindex[i]];
  }
}
test2l();

proc test2() {
  forall i in D2 do
    tmp.localAccess[i] = A[rindex[i]];
}
test2();


proc test3l() {
  var A: [0..#M] int = 0..#M;
  var rindex: [0..#N] int;
  var tmp: [0..#N] int;

  forall (t, r) in zip(tmp, rindex) do
    t = A[r];
}
test3l();

proc test3() {
  forall (t, r) in zip(tmp, rindex) do
    t = A[r];
}
test3();


proc test4l() {
  var A: [0..#M] int = 0..#M;
  var rindex: [0..#N] int;
  var tmp: [0..#N] int;

  tmp = A[rindex];
}
test4l();

proc test4() {
  tmp = A[rindex];
}
test4();
