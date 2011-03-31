use BlockDist, Time;

type indexType = int(64),
  elemType = int(64);

config const n: indexType = 10;

const m = 2**n;

config const printArrays = false,
  printStats = false,
  trackStmStats = false;

const TableDist = new dmap(new Block(boundingBox=[0..m-1]));

const TableSpace: domain(1, indexType) dmapped TableDist = [0..m-1];

var T: [TableSpace] elemType;

proc main() {
  var i: indexType;

  [i in TableSpace] T(i) = i;

  if (printArrays) then writeln("Before swap T = \n", T);

  if (trackStmStats) then startStmStats();

  const startTime = getCurrentTime();

  for i in TableSpace {
    atomic {
      var x = T(i);
      T(i) = T((i+1)%m);
      T((i+1)%m) = x;
    }
  }

  for i in 0..m-1 by -1 {
    atomic {
      var x = T(i);
      T(i) = T((i+1)% m);
      T((i+1)% m) = x;
    }
  }

  const endTime = getCurrentTime() - startTime;

  if (trackStmStats) then stopStmStats();

  if (printArrays) then writeln("After swap T = \n", T);

  const numErrors = + reduce [i in TableSpace] (T(i) != i);
  writeln("Validation: ", if numErrors then "FAILURE" else "SUCCESS");
  if (printStats) then writeln("Time = ", endTime);
}
