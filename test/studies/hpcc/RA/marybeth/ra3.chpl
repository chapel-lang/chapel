/*  This is the Chapel version of Random Access HPC benchmark.
 *
 *  This is the Chapel version of the MTA version + inner vector loop
 */  
// param used below still gives errors
param POLY:uint(64) = 7;

config const verify = true;
config const errorTolerance = 0.01;
config const doIO = true;
config const showTiming = false;

config const totalMemSize:int = 100000;
const logTableSize:int  = log2(totalMemSize/2);
const tableSize:uint(64) = (1 << logTableSize):uint(64);

const numUpdates:uint(64) = 4*tableSize;
config const numStreams:int = 1 << 9;
const bigStep:int = (numUpdates:int)/numStreams;
config const littleStep = 64;

const TableDomain = {0:uint(64)..#tableSize};
var Table: [TableDomain] uint(64);

const RandStepsDomain = {0..#64};
var RandomSteps: [RandStepsDomain] uint(64);

const StreamDomain = {0..#numStreams};
const BigStepDomain = {0..#bigStep};
const LittleStepDomain = {0..#littleStep};
const UpdateDomain: domain(2) = {0..#numStreams,0..#bigStep};

var realTime:real;
var GUPs:real;

proc main() {

  use Time;
  var t:Timer;

  if doIO then writeRAdata();

  initRandomSteps();

  t.clear();
  t.start();

  randomAccessUpdate();

  t.stop();
  realTime = t.elapsed();

  GUPs = (if (realTime > 0.0) then (1.0 / realTime) else -1.0);
  GUPs *= 1.0e-9*numUpdates;

  if (doIO && showTiming) then writeRAresults();

  if (verify) then verifyResults();

}

proc randomAccessUpdate() {
 
  [i in TableDomain] Table(i) = i;
  var lock: sync bool = true;

  forall j in StreamDomain {
    var ranvec: [LittleStepDomain] uint(64);
    [k in LittleStepDomain] ranvec(k) = randomStart(bigStep*j+(bigStep/littleStep)*k);
    for i in BigStepDomain by littleStep{
      for k in LittleStepDomain{
        ranvec(k) = (ranvec(k) << 1) ^ (if (ranvec(k):int(64) < 0) then POLY else 0);
        lock;
        atomic {Table(ranvec(k) & (tableSize-1)) ^= ranvec(k);}
        lock = true;
      }
    }
  }
}

proc randomStart(step0:int):uint(64) {

  var i:int;
  var ran:uint(64) = 2;

  if (step0 ==0) then 
    return 1;
  else
    i = log2(step0);
  while (i > 0) do {
    var temp:uint(64) = 0;
    for j in RandStepsDomain {
      if (( ran >> j) & 1) then temp ^= RandomSteps(j);
    }
    ran = temp;
    i -= 1;
    if (( step0 >> i) & 1) then
      ran = (ran << 1) ^ (if (ran:int(64) < 0) then POLY else 0);
  }
  return ran;
}

proc initRandomSteps() {
  
  var temp:uint(64) = 1;

  for i in RandStepsDomain {
    RandomSteps(i) = temp;
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0);
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0);
  }
}

proc verifyResults() {

  var temp: uint(64) = 1;  
  for i in UpdateDomain {
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0);
    Table(temp & (tableSize-1)) ^= temp;  
  }

  var numErrors = 0;
  for i in TableDomain {
    if (Table(i) != i) then
      numErrors += 1;
  }

  if doIO {
    write("Found ", numErrors, " errors in ", tableSize, " locations ");
    if (numErrors <= errorTolerance*tableSize) {
      writeln("(passed)");
    } else {
      writeln("(failed)");
    }
  }
}

proc writeRAdata() {

  writeln("Main table size = 2^",logTableSize," = ",tableSize," words");
  writeln("Number of updates = ",numUpdates);
  writeln("Number of random streams = ",numStreams);
  writeln("Length of each random stream = ",bigStep);
  writeln("Inner loop length = ",littleStep);
  writeln();
}

proc writeRAresults() {

  writeln("Real time used = ", realTime, " seconds");
  writeln(GUPs, " Billion(10^9) Updates    per second [GUP/s]");
  writeln();
}
